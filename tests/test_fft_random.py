#!/usr/bin/env python
import xmostest
import sys
sys.path.insert(0, 'test_fft_correctness/src')
import gen_test
import shutil
import os
import os.path
import random
import tempfile
import ctypes
import time
import math
import numpy as np

Q_FORMAT = (1, 31)
ERROR_THRESHOLD = 1e-5

def get_seed():
    script_location = os.path.dirname(os.path.realpath(__file__))
    stdout, stderr = xmostest.call_get_output(['git', 'rev-parse', 'HEAD'],
                                              cwd=script_location)
    if not stderr:
        seed = int(stdout[0].strip()[:7], 16)
    else:
        seed = 255
    return seed

def crc32(d, x, poly):
	crc = d
	data = x
	for bit in range(32):
		xorBit = (crc & 1)
		crc  = ((crc >> 1) & 0x7fffffff) | ((data & 1) << 31)
		data = (data >> 1) & 0x7fffffff
		if (xorBit != 0):
	  		crc = crc ^ poly
	return crc

def bitrev(x):
    x = np.array([x], dtype=np.uint32)[0]
    return int('{:032b}'.format(x)[::-1], 2)

def clz(x):
    leading_zeros = '{:032b}'.format(x).index('1')
    if leading_zeros < 0:
        return 32
    return leading_zeros

def index_bitrev(x, fft_length):
    x = np.array([x], dtype=np.uint32)[0]
    x_string = '{:032b}'.format(x)
    mask = fft_length - 1
    mask_length = int(math.log(fft_length, 2))
    reverse_val = bitrev(mask & x) >> (clz(fft_length) + 1)
    x_string = x_string[:-mask_length] + ('{:0%db}' % mask_length).format(reverse_val)
    return int(x_string, 2)

class FFTTester(xmostest.Tester):
    def __init__(self, test_name, seed, verbose=False, print_all=False):
        super(FFTTester, self).__init__()
        self._poly = 0xEB31D82E
        self._test_name = test_name
        self._seed = seed
        self._data = seed
        self._results = []
        self._verbose = verbose
        self._print_all = print_all

    def _get_random(self):
        self._data = crc32(self._data, -1, self._poly)
        return self._data

    def _set_fft_test_result(self, test_name, length, result):
        test_config = {'FFT Length': length, 'Seed': self._seed}
        self._results.append((test_name, test_config, result))

    def _push_test_results(self):
        for result in self._results:
            test_name = result[0]
            test_config = result[1]
            test_result = result[2]
            self.register_test("lib_dsp", "fft_tests_fast", test_name, test_config)
            xmostest.set_test_result("lib_dsp", "fft_tests_fast", test_name,
                                     test_config, test_result, {})

    def _generate_fft_input(self, fft_length):
        fft_input = []
        for i in range(fft_length):
            data = self._get_random()
            re = ctypes.c_int32(data).value >> 1
            data = self._get_random()
            im = ctypes.c_int32(data).value >> 1
            fft_input.append(complex(re, im))
        return fft_input

    def _tuple_array_to_complex(self, tuples):
        complex_arr = []
        for (i, j) in tuples:
            complex_arr.append(complex(i, j))
        return complex_arr

    def _complex_array_to_tuple(self, complex_arr):
        tuples = []
        for z in complex_arr:
            tuples.append((int(z.real), int(z.imag)))
        return tuples

    def _calculate_fft(self, fft_length, fft_input=None):
        fft_count = 180*1024 / (8*fft_length)

        fft_output = np.zeros((fft_length,2), dtype=np.int32)
        for t in range(fft_count):
            if fft_input is None:
                fft_input = self._generate_fft_input(fft_length)
            sp = np.fft.fft(fft_input)
            for i in range(fft_length):
                re = int(sp[i].real/fft_length)
                im = int(sp[i].imag/fft_length)
                fft_output[i,:] = (re, im)
        return fft_output

    def _verify_forward_fft(self, fft_length, output, fft_input, reference_fft):
        print "\nFFT %d: " % fft_length
        #print "FFT out: ", output
        #print "Ref out: ", reference_fft
        re_error = np.abs(reference_fft[:, 0] - output[:, 0])
        im_error = np.abs(reference_fft[:, 1] - output[:, 1])
        max_re_error_i = np.argmax(re_error)
        max_im_error_i = np.argmax(im_error)
        re_error_float = float(re_error[max_re_error_i]) / (1<<Q_FORMAT[1])
        im_error_float = float(im_error[max_im_error_i]) / (1<<Q_FORMAT[1])
        if re_error_float > ERROR_THRESHOLD:
            print "\nError of %f greater than threshold %f!"\
                    % (re_error_float, ERROR_THRESHOLD)
            #print "Input | Output | Correct"
            #for i, in_val in enumerate(input):
            #    print in_val, output[i], reference_fft[i],
            #    print "<----" if i == max_error_i else ""
            return False
        print "Success",
        return True

    def _verify_inverse_fft(self, fft_length, output, fft_input):
        print "\nInverse FFT %d: " % fft_length
        fft_input = np.array(self._complex_array_to_tuple(fft_input))
        #print "FFT out: ", output
        #print "Ref out: ", reference_fft
        re_error = np.abs(output[:, 0] - fft_input[:, 0])
        im_error = np.abs(output[:, 1] - fft_input[:, 1])
        max_re_error_i = np.argmax(re_error)
        max_im_error_i = np.argmax(im_error)
        re_error_float = float(re_error[max_re_error_i]) / (1<<Q_FORMAT[1])
        im_error_float = float(im_error[max_im_error_i]) / (1<<Q_FORMAT[1])
        if re_error_float > ERROR_THRESHOLD:
            print "\nError of %f greater than threshold %f!"\
                    % (re_error_float, ERROR_THRESHOLD)
            eq_vec = (np.array(re_error, dtype=np.float_) / (1<<Q_FORMAT[1])) < ERROR_THRESHOLD
            self._print_diff([reference_fft], output, fft_input, eq_vec, fft_length)
            #print "Input | Output | Correct"
            #for i, in_val in enumerate(input):
            #    print in_val, output[i], reference_fft[i],
            #    print "<----" if i == max_error_i else ""
            return False
        print "Success",
        return True

    def _verify_bit_reverse(self, fft_length, output, fft_input):
        print "\nBit Reverse %d: " % fft_length
        fft_input = np.array(self._complex_array_to_tuple(fft_input))
        reference_vec = []
        for (i, j) in fft_input:
            i = i % fft_length
            j = j % fft_length
            reference_vec.append((index_bitrev(i, fft_length), index_bitrev(j, fft_length)));
        #print "FFT out: ", output
        #print "Ref out: ", reference_fft
        output = np.array(output)
        reference_vec = np.array(reference_vec, dtype=np.int32)
        re_error = np.abs(output[:, 0] - reference_vec[:, 0])
        im_error = np.abs(output[:, 1] - reference_vec[:, 1])
        max_re_error_i = np.argmax(re_error)
        max_im_error_i = np.argmax(im_error)
        re_error_float = float(re_error[max_re_error_i]) / (1<<Q_FORMAT[1])
        im_error_float = float(im_error[max_im_error_i]) / (1<<Q_FORMAT[1])
        if re_error_float > ERROR_THRESHOLD:
            print "\nError of %f greater than threshold %f!"\
                    % (re_error_float, ERROR_THRESHOLD)
            eq_vec = (np.array(re_error, dtype=np.float_) / (1<<Q_FORMAT[1])) < ERROR_THRESHOLD
            self._print_diff([fft_input], output, reference_vec, eq_vec, fft_length)
            #print "Input | Output | Correct"
            #for i, in_val in enumerate(input):
            #    print in_val, output[i], reference_fft[i],
            #    print "<----" if i == max_error_i else ""
            return False
        print "Success",
        return True

    def _verify_split_spectrum(self, fft_length, output, fft_input, reference_fft):
        print "\nBit Reverse %d: " % fft_length
        fft_input = np.array(self._complex_array_to_tuple(fft_input))
        #print "FFT out: ", output
        #print "Ref out: ", reference_fft
        output = np.array(output)
        reference_vec = reference_fft
        reference_vec = np.array(reference_vec, dtype=np.int32)
        reference_vec[0, 1] = reference_vec[fft_length/2, 0]
        reference_vec[fft_length/2:,:] = 0
        re_error = np.abs(output[:, 0] - reference_vec[:, 0])
        im_error = np.abs(output[:, 1] - reference_vec[:, 1])
        max_re_error_i = np.argmax(re_error)
        max_im_error_i = np.argmax(im_error)
        re_error_float = float(re_error[max_re_error_i]) / (1<<Q_FORMAT[1])
        im_error_float = float(im_error[max_im_error_i]) / (1<<Q_FORMAT[1])
        if re_error_float > ERROR_THRESHOLD:
            print "In: " + ";".join("%d,%d" % (a[0], a[1]) for a in fft_input)
            print "Ref: " + ";".join("%d,%d" % (a[0], a[1]) for a in reference_fft)
            print "\nError of %f greater than threshold %f!"\
                    % (re_error_float, ERROR_THRESHOLD)
            eq_vec = (np.array(re_error, dtype=np.float_) / (1<<Q_FORMAT[1])) < ERROR_THRESHOLD
            self._print_diff([fft_input], output, reference_vec, eq_vec, fft_length)
            #print "Input | Output | Correct"
            #for i, in_val in enumerate(input):
            #    print in_val, output[i], reference_fft[i],
            #    print "<----" if i == max_error_i else ""
            return False
        print "Success",
        return True

    def _print_diff(self, input, output_vec, reference_vec, eq_vec, length):
        print "Vector length = %d" % length
        if self._verbose:
            print "index: [input] output != reference"
            last_correct = -1
            for i, eq in enumerate(eq_vec):
                if eq and not self._print_all:
                    if last_correct == -1:
                        last_correct = i
                else:
                    if last_correct != -1:
                        print "Correct: %d .. %d" % (last_correct, i-1)
                        last_correct = -1
                    input_strs = []
                    for in_val in input:
                        if len(in_val) == length:
                            input_strs.append(str(in_val[i]))
                        else:
                            input_strs.append(str(in_val[0]))
                    input_str = "[" + ", ".join(input_strs) + "]"
                    eq_str = "==" if eq else "!="
                    if eq:
                        print "Correct:",
                    print "%d:"%i, input_str, output_vec[i], eq_str, reference_vec[i]
            if last_correct != -1:
                print "Correct: %d .. %d" % (last_correct, length-1)

    def _parse_line(self, line, length):
        strings = line.split(';')
        output = np.zeros((length, 2), dtype=np.int32)
        for j, string in enumerate(strings):
            if not len(string.strip()):
                continue
            string_split = string.split(',')
            re = int(string_split[0])
            im = int(string_split[1])
            output[j, :] = (re, im)
        return output

    def run(self, output):
        for i, line in enumerate(output):
            line = line.strip()
            if i+1 < len(output):
                next_line = output[i+1].strip()

            if line[:6] == "LENGTH":
                fft_length = int(line[6:])
            if line == "GENERATE":
                fft_input = self._generate_fft_input(fft_length)
                reference_fft = self._calculate_fft(fft_length, fft_input)
                fft_input_re = [(z.real+0j) for z in fft_input]
                reference_fft_re = self._calculate_fft(fft_length, fft_input_re)
            if line[:4] == "SEED":
                seed = int(line[5:])
                if self._seed != seed:
                    print "Error! Seed of .xc does not equal given seed: %d != %d"\
                        % (seed, self._seed)
                    return
                else:
                    print "Seed correct!"
            if line == "FFT":
                # Forward FFT
                fft_output_line = next_line
                fft_output = self._parse_line(fft_output_line, fft_length)
                if self._verify_forward_fft(fft_length, fft_output, fft_input, reference_fft):
                    result = "PASS"
                else:
                    result = "FAIL"
                self._set_fft_test_result("fft_forward", fft_length, result)
            if line == "IFFT":
                # Inverse FFT
                fft_output_line = next_line
                fft_output = self._parse_line(fft_output_line, fft_length)
                if self._verify_inverse_fft(fft_length, fft_output, fft_input):
                    result = "PASS"
                else:
                    result = "FAIL"
                self._set_fft_test_result("fft_inverse", fft_length, result)
            if line == "BIT_REVERSE":
                bit_reverse_line = next_line
                bit_reverse_output = self._parse_line(bit_reverse_line, fft_length)
                if self._verify_bit_reverse(fft_length, bit_reverse_output, fft_input):
                    result = "PASS"
                else:
                    result = "FAIL"
                self._set_fft_test_result("fft_bit_reverse", fft_length, result)
            if line == "SPLIT_SPECTRUM":
                split_spectrum_line = next_line
                split_spectrum_output = self._parse_line(split_spectrum_line, fft_length)
                if self._verify_split_spectrum(fft_length, split_spectrum_output, fft_input_re, reference_fft_re):
                    result = "PASS"
                else:
                    result = "FAIL"
                self._set_fft_test_result("fft_split_spectrum", fft_length, result)
        print "\nTests complete."
        time.sleep(1)
        self._push_test_results()

def runtest():
    seed = get_seed()

    resources = xmostest.request_resource("axe")

    tester = FFTTester(test_name="fft_random", seed=seed, verbose=True, print_all=True)

    xmostest.run_on_simulator(resources['axe'],
                              'test_fft_random/bin/test.xe',
                              tester=tester, timeout=1200)

if __name__ == "__main__":
    runtest()
