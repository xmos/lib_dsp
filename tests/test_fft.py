#!/usr/bin/env python
import xmostest
import sys
sys.path.insert(0, 'test_fft_correctness/src')
import gen_test
import shutil
import os.path
import random
import tempfile

class DeletyComparisonTester(xmostest.ComparisonTester):
    def __init__(self, golden, product, group, test, dir_to_delete, config = {}, env = {},
                 regexp = False, ignore=[]):
        super(DeletyComparisonTester, self).__init__( golden, product, group, test,  config, env,
                 regexp, ignore)
        self.d = dir_to_delete


    def run(self, output):
        super(DeletyComparisonTester, self).run(output)
        shutil.rmtree(self.d)


def do_fft_test(length_log2, testlevel, test_dir_name, test_name):

    seed  = random.randrange(4294967294);

    directory_name = tempfile.mkdtemp(prefix='temp_fft_dir_', dir='.')

    source_directory =  os.path.join(directory_name, 'src')

    os.mkdir(source_directory)

    gen_test.generate(180, length_log2, 1, seed, source_directory)
    shutil.copy(os.path.join(test_dir_name,'src','test.xc'), source_directory)
    shutil.copy(os.path.join(test_dir_name,'Makefile'), directory_name)

    resources = xmostest.request_resource("xsim")

    binary = os.path.join(directory_name, 'bin', 'test.xe')

    tester = DeletyComparisonTester(open(test_dir_name + '.expect'),
                                       'lib_dsp',
                                       'fft_tests',
                                       test_name, directory_name,
                                       {'FFT length':(1<<length_log2), 'Seed':seed})

    tester.set_min_testlevel(testlevel)

    xmostest.run_on_simulator(resources['xsim'], binary,
                              simargs=[],
                              tester = tester)

def runtest():
    try:
        for r in range(3, 14):
            do_fft_test(r, "smoke", 'test_fft_forward', "forward_fft")
            do_fft_test(r, "smoke", 'test_fft_inverse', "inverse_fft")
            do_fft_test(r, "smoke", 'test_fft_index_bit_reverse', "index_bit_reversal")
            do_fft_test(r, "smoke", 'test_fft_split_and_merge', "fft_split_and_merge")
            do_fft_test(r, "smoke", 'test_fft_short_long', "short_and_long_conversion ")
    except:
        #clean everything up
        for file in os.listdir("."):
            if file.startswith("temp_fft_dir_"):
                shutil.rmtree(file)
