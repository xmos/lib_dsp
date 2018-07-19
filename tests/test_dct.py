import math
import xmostest
import numpy as np

Q_FORMAT = (1,31)
ERROR_THRESHOLD = 1e-5

class DCTTester(xmostest.Tester):
    def __init__(self, dcts, saturate=True):
        super(DCTTester, self).__init__()
        self._dcts = []
        self._saturate = saturate
        self.register_dct_tests(dcts)

    def register_dct_tests(self, dcts):
        self._dcts += dcts
        for N in dcts:
            self.register_test("lib_dsp", "simple_tests", "DCT%d" % N, {})

    def set_dct_test_result(self, dct, result):
        if not dct in self._dcts:
            raise Exception("Test for DCT%d not registered!" % dct)
        xmostest.set_test_result("lib_dsp", "simple_tests", "DCT%d" % dct, {},
                                result, {})

    # Uses DCT-II following the algorithm on wikipedia
    def dct(self, x, q=(1,31), dtype=np.int32):
        N = len(x)
        X = np.zeros((N,), dtype=dtype)
        for k, _ in enumerate(x):
            cos_arr = np.cos((np.arange(N) + 0.5) * (np.pi / N) * k,
                             dtype=np.float32)
            val = np.dot(x, cos_arr)
            type_info = np.iinfo(dtype)
            if self._saturate:
                if val > type_info.max:
                    val = type_info.max
                elif val < type_info.min:
                    val = type_info.min
            X[k] = np.round(val)
        return X

    def verify_dct(self, input, output):
        print "\nDCT %d: " % len(input),
        reference_output = np.array(self.dct(input, Q_FORMAT))
        no_overflow_output = np.array(self.dct(input, Q_FORMAT, np.int64))
        overflow = np.any(reference_output != no_overflow_output)
        error = np.abs(reference_output - output)
        max_error_i = np.argmax(error)
        real_error = float(error[max_error_i]) / (1<<Q_FORMAT[1])
        if real_error > ERROR_THRESHOLD:
            print "\nError of %f greater than threshold %f!"\
                    % (real_error, ERROR_THRESHOLD)
            print "Input | Output | Correct"
            for i, in_val in enumerate(input):
                print in_val, output[i], reference_output[i],
                print "<----" if i == max_error_i else ""
            return False
        print "Success",
        if overflow:
            print "(with " + ("saturation)" if self._saturate else "overflow)")
        else:
            print ""
        return True

    def run(self, output):
        result = "PASS"
        i = 0
        while i < len(output):
            line = output[i]
            if "BEGIN" in line:
                for j, line2 in enumerate(output[i:]):
                    if "END" in line2:
                        end = i + j
                        break
            else:
                i += 1
                continue
            test_strings = output[i+1:end]
            N = len(test_strings)
            test_tuples = np.zeros((N, 2), dtype=np.int32)
            for i, string in enumerate(test_strings):
                string_pair = string.split()
                test_tuples[i,:] = [int(string_pair[0]), int(string_pair[1])]
            # Verify output
            result_bool = self.verify_dct(test_tuples[:,0], test_tuples[:,1])
            result = "PASS" if result_bool else "FAIL"
            self.set_dct_test_result(N, result)
            i = end + 1

def runtest():
    resources = xmostest.request_resource("xsim")

    tester = DCTTester(dcts=[48, 32, 24, 16, 12, 8, 6, 4, 3, 2, 1])

    xmostest.run_on_simulator(resources['xsim'],
                              '../AN00209_xCORE-200_DSP_Library/app_dct/bin/app_dct.xe',
                              tester=tester, timeout=1200)
