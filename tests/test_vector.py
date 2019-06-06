# Copyright (c) 2015-2019, XMOS Ltd, All rights reserved
#!/usr/bin/env python
import time
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

class VectorTester(xmostest.Tester):
    def __init__(self, seed, verbose=True, print_all=False):
        super(VectorTester, self).__init__()
        self._poly = 0xEB31D82E
        self._seed = seed
        self._data = seed
        self._verbose = verbose
        self._print_all = print_all
        self._results = [] # [(name, config, result)]

    def _get_random(self):
        self._data = crc32(self._data, -1, self._poly)
        return self._data

    def _set_vector_test_result(self, test_name, length, result, qformat=None):
        test_config = {'Vector Length': length, 'Seed': self._seed}
        if qformat:
            test_config['Q Format'] = qformat
        self._results.append((test_name, test_config, result))

    def _push_test_results(self):
        for result in self._results:
            test_name = result[0]
            test_config = result[1]
            test_result = result[2]
            self.register_test("lib_dsp", "vector_tests", test_name, test_config)
            xmostest.set_test_result("lib_dsp", "vector_tests", test_name,
                                     test_config, test_result, {})

    def _generate_vector(self, length):
        vec = np.zeros((length,), dtype=np.int32)
        for i in range(length):
            vec[i] = self._get_random()
        return vec

    def _parse_line(self, line, length, dtype=np.int32):
        strings = line.split(',')
        output = np.zeros((length,), dtype=dtype)
        for j, string in enumerate(strings):
            if not len(string.strip()):
                continue
            output[j] = int(string)
        return output

    def _verify_test(self, test_name, input, output_vec, reference_vec, length,
                     dtype=np.int32):
        output_vec = output_vec.astype(dtype)
        reference_vec = reference_vec.astype(dtype)
        eq_vec = np.isclose(output_vec, reference_vec, atol=1)
        result = "PASS" if np.all(eq_vec) else "FAIL"
        if result == "FAIL":
            print "Test \"%s\" failed!" % test_name,
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
        self._set_vector_test_result(test_name, length, result)

    def run(self, output):
        vec1 = np.array([])
        vec2 = np.array([])
        vec3 = np.array([])
        vec1_re = np.array([])
        vec1_im = np.array([])
        vec2_re = np.array([])
        vec2_im = np.array([])
        rshift = 0
        max_length = -1
        ii32 = np.iinfo(np.int32)
        xc_i = 0
        for i, line in enumerate(output):
            line = line.strip()
            if i == len(output)-1: break
            next_line = output[i+1].strip()
            if line[:4] == "SEED":
                seed = int(line[5:])
                cur_seed = ctypes.c_int32(self._data).value
                if cur_seed != seed:
                    print "Error! Seed of .xc does not equal given seed: %d != %d"\
                        % (seed, cur_seed)
                    return
                else:
                    print "Seed correct!"
                    self._seed = seed
            if "MAX_VECTOR_LENGTH" in line:
                eq_index = line.index("=")
                max_length = int(line[eq_index + 1:])
            if "RSHIFT" in line:
                eq_index = line.index("=")
                rshift = int(line[eq_index + 1:])
            if line == "GENERATE":
                length = self._get_random() % max_length
                if length == 0:
                    length = 1
                if xc_i < 10:
                    length = xc_i + 1
                scalar = ctypes.c_int32(self._get_random()).value >> rshift
                vec1 = self._generate_vector(length) >> rshift
                vec2 = self._generate_vector(length) >> rshift
                vec3 = self._generate_vector(length) >> rshift
                vec1_re = self._generate_vector(length) >> rshift
                vec1_im = self._generate_vector(length) >> rshift
                vec2_re = self._generate_vector(length) >> rshift
                vec2_im = self._generate_vector(length) >> rshift
                xc_i += 1
            if "SET_VECTOR_LENGTH" in line:
                eq_index = line.index("=")
                length = int(line[eq_index + 1:])
                scalar = self._get_random() >> rshift
                vec1 = self._generate_vector(length) >> rshift
                vec2 = self._generate_vector(length) >> rshift
                vec3 = self._generate_vector(length) >> rshift
                vec1_re = self._generate_vector(length) >> rshift
                vec1_im = self._generate_vector(length) >> rshift
                vec2_re = self._generate_vector(length) >> rshift
                vec2_im = self._generate_vector(length) >> rshift
            if line == "min":
                reference_vec = np.array([np.argmin(vec1)])
                output_vec = self._parse_line(next_line, 1)
                input_list = [["vec1"]]
                self._verify_test("min", input_list, output_vec, reference_vec, length)
            if line == "max":
                reference_vec = np.array([np.argmax(vec1)])
                output_vec = self._parse_line(next_line, 1)
                input_list = [["vec1"]]
                self._verify_test("min", input_list, output_vec, reference_vec, length)
            if line == "negate":
                reference_vec = -vec1
                output_vec = self._parse_line(next_line, length)
                input_list = [["vec1"]]
                self._verify_test("negate", input_list, output_vec, reference_vec, length)
            if line == "abs":
                reference_vec = np.abs(vec1)
                output_vec = self._parse_line(next_line, length)
                input_list = [vec1]
                self._verify_test("abs", input_list, output_vec, reference_vec, length)
            if line == "adds":
                reference_vec = vec1 + scalar
                output_vec = self._parse_line(next_line, length)
                input_list = [vec1, [scalar]]
                self._verify_test("adds", input_list, output_vec, reference_vec, length)
            if line == "muls":
                reference_vec = (vec1.astype(np.int64) * scalar) >> 24
                reference_vec = np.clip(reference_vec, ii32.min, ii32.max)
                output_vec = self._parse_line(next_line, length)
                input_list = [vec1, [scalar]]
                self._verify_test("muls", input_list, output_vec, reference_vec, length)
            if line == "addv":
                reference_vec = vec1 + vec2
                output_vec = self._parse_line(next_line, length)
                input_list = [vec1, vec2]
                self._verify_test("addv", input_list, output_vec, reference_vec, length)
            if line == "minv":
                reference_vec = np.minimum(vec1.astype(np.uint32),
                                           vec2.astype(np.uint32)).astype(np.int32)
                output_vec = self._parse_line(next_line, length)
                input_list = [vec1, vec2]
                self._verify_test("minv", input_list, output_vec, reference_vec, length)
            if line == "subv":
                reference_vec = vec1 - vec2
                output_vec = self._parse_line(next_line, length)
                input_list = [vec1, vec2]
                self._verify_test("subv", input_list, output_vec, reference_vec, length)
            if line == "mulv":
                reference_vec = np.float_(vec1.astype(np.int64) * vec2.astype(np.int64))\
                                / (1 << 24)
                reference_vec = np.clip(reference_vec, ii32.min, ii32.max)
                output_vec = self._parse_line(next_line, length)
                input_list = [vec1, vec2]
                self._verify_test("mulv", input_list, output_vec, reference_vec, length)
            if line == "mulv_adds":
                reference_vec = (vec1.astype(np.int64) * vec2.astype(np.int64)) >> 24
                reference_vec = np.clip(reference_vec, ii32.min, ii32.max)
                reference_vec += scalar
                input_list = [vec1, vec2, [scalar]]
                output_vec = self._parse_line(next_line, length)
                self._verify_test("mulv_adds", input_list, output_vec, reference_vec, length)
            if line == "muls_addv":
                reference_vec = (vec1.astype(np.int64) * scalar)  >> 24
                reference_vec = np.clip(reference_vec, ii32.min, ii32.max)
                reference_vec += vec2.astype(np.int64)
                input_list = [vec1, [scalar], vec2]
                output_vec = self._parse_line(next_line, length)
                self._verify_test("muls_addv", input_list, output_vec, reference_vec, length)
            if line == "muls_subv":
                reference_vec = (vec1.astype(np.int64) * scalar)  >> 24
                reference_vec = np.clip(reference_vec, ii32.min, ii32.max)
                reference_vec -= vec2.astype(np.int64)
                output_vec = self._parse_line(next_line, length)
                input_list = [vec1, [scalar], vec2]
                self._verify_test("muls_subv", input_list, output_vec, reference_vec, length)
            if line == "mulv_addv":
                reference_vec = (vec1.astype(np.int64) * vec2.astype(np.int64)) >> 24
                reference_vec = np.clip(reference_vec, ii32.min, ii32.max)
                reference_vec += vec3.astype(np.int64)
                output_vec = self._parse_line(next_line, length)
                input_list = [vec1, vec2, vec3]
                self._verify_test("mulv_addv", input_list, output_vec, reference_vec, length)
            if line == "mulv_subv":
                reference_vec = (vec1.astype(np.int64) * vec2.astype(np.int64)) >> 24
                reference_vec = np.clip(reference_vec, ii32.min, ii32.max)
                reference_vec -= vec3.astype(np.int64)
                output_vec = self._parse_line(next_line, length)
                input_list = [vec1, vec2, vec3]
                self._verify_test("mulv_subv", input_list, output_vec, reference_vec, length)
            if line == "mulv_complex":
                ref_re = ((vec1_re.astype(np.int64) * vec2_re.astype(np.int64)) >> 24)\
                       - ((vec1_im.astype(np.int64) * vec2_im.astype(np.int64)) >> 24)
                ref_im = ((vec1_re.astype(np.int64) * vec2_im.astype(np.int64)) >> 24)\
                       + ((vec2_re.astype(np.int64) * vec1_im.astype(np.int64)) >> 24)
                ref_re = np.clip(ref_re, ii32.min, ii32.max)
                ref_im = np.clip(ref_im, ii32.min, ii32.max)
                out_re = self._parse_line(next_line, length)
                out_im = self._parse_line(output[i+2].strip(), length)
                input_list = [vec1_re, vec2_re]
                self._verify_test("mulv_complex_re", input_list, out_re, ref_re, length)
                input_list = [vec1_im, vec2_im]
                self._verify_test("mulv_complex_im", input_list, out_im, ref_im, length)
        print "Tests complete..."
        time.sleep(1)
        self._push_test_results()


def runtest():
    seed = get_seed()

    resources = xmostest.request_resource("axe")

    tester = VectorTester(seed, verbose=True, print_all=True)

    xmostest.run_on_simulator(resources['axe'],
                              'test_vector/bin/test.xe',
                              tester=tester, timeout=1200)

if __name__ == "__main__":
    runtest()
