#!/usr/bin/env python
# Copyright (c) 2018, XMOS Ltd, All rights reserved
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

class FFTTester(xmostest.Tester):
    def __init__(self, fft_lengths, test_name, seed):
        super(FFTTester, self).__init__()
        for length in fft_lengths:
            self.register_test("lib_dsp", "fft_tests", test_name,
                               {'FFT Length': length, 'Seed': seed})
        self._fft_lengths = fft_lengths
        self._poly = 0xEB31D82E
        self._test_name = test_name
        self._seed = seed
        self._data = seed

    def _get_random(self):
        self._data = crc32(self._data, -1, self._poly)
        return self._data

    def set_fft_test_result(self, length, result):
        xmostest.set_test_result("lib_dsp", "fft_tests", self._test_name,
                           {'FFT Length': length, 'Seed': self._seed},
                           result, {})

    def generate(self, fft_length, fft_input=None):
        fft_input = None
        fft_count = 180*1024 / (8*fft_length)

        fft_output = np.zeros((fft_length,2), dtype=np.int32)
        for t in range(fft_count):
            if not fft_input:
                fft_input = []
                for i in range(fft_length):
                    data = self._get_random()
                    re = ctypes.c_int32(data).value >> 1
                    data = self._get_random()
                    im = ctypes.c_int32(data).value >> 1
                    fft_input.append(complex(re, im))
            sp = np.fft.fft(fft_input)
            for i in range(fft_length):
                re = int(sp[i].real/fft_length)
                im = int(sp[i].imag/fft_length)
                fft_output[i,:] = (re, im)
        return fft_output

    def verify_fft(self, fft_length, output, fft_input):
        print "\nFFT %d: " % fft_length
        reference_output = self.generate(fft_length, fft_input)
        #print "FFT out: ", output
        #print "Ref out: ", reference_output
        re_error = np.abs(reference_output[:, 0] - output[:, 0])
        im_error = np.abs(reference_output[:, 1] - output[:, 1])
        max_re_error_i = np.argmax(re_error)
        max_im_error_i = np.argmax(im_error)
        re_error_float = float(re_error[max_re_error_i]) / (1<<Q_FORMAT[1])
        im_error_float = float(im_error[max_im_error_i]) / (1<<Q_FORMAT[1])
        if re_error_float > ERROR_THRESHOLD:
            print "\nError of %f greater than threshold %f!"\
                    % (re_error_float, ERROR_THRESHOLD)
            #print "Input | Output | Correct"
            #for i, in_val in enumerate(input):
            #    print in_val, output[i], reference_output[i],
            #    print "<----" if i == max_error_i else ""
            return False
        print "Success",
        return True

    def parse_line(self, line, length):
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
            if line[:4] == "SEED":
                seed = int(line[5:])
                if self._seed != seed:
                    print "Error! Seed of .xc does not equal given seed: %d != %d"\
                        % (seed, self._seed)
                    return
                else:
                    print "Seed correct!"
            if line[:3] == "FFT":
                fft_length = int(line.strip()[3:-1])
            else:
                continue
            fft_output_line = output[i+1]
            fft_output = self.parse_line(fft_output_line, fft_length)
            if self.verify_fft(fft_length, fft_output, None):
                result = "PASS"
            else:
                result = "FAIL"
            self.set_fft_test_result(fft_length, result)

def runtest():
    seed = get_seed()

    resources = xmostest.request_resource("axe")

    tester = FFTTester(fft_lengths=[8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192],
            test_name="forward_random", seed=seed)

    xmostest.run_on_simulator(resources['axe'],
                              'test_fft_forward_random/bin/test.xe',
                              tester=tester, timeout=1200)

if __name__ == "__main__":
    runtest()
