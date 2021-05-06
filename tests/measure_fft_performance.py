#!/usr/bin/env python
# Copyright 2016-2021 XMOS LIMITED.
# This Software is subject to the terms of the XMOS Public Licence: Version 1.
import xmostest
import sys
sys.path.insert(0, 'test_fft_correctness/src')
import gen_test
import shutil
import os.path
import random
import tempfile
       
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

#    tester = DeletyComparisonTester(open(test_dir_name + '.expect'),
#                                       'lib_dsp',
#                                       'fft_performance_tests',
#                                       test_name, directory_name,
#                                       {'FFT length':(1<<length_log2), 'Seed':seed})
#                                       
#    tester.set_min_testlevel(testlevel)

    xmostest.run_on_simulator(resources['xsim'], binary,
                              simargs=[],
                              tester = 0)

#     shutil.rmtree(directory_name)  

def measure_fft_performance():
  try:
   for r in range(3, 14):
       do_fft_test(r, "smoke", 'test_fft_performance', "fft_performance")
  except:
   print "Test Exception"
   #clean everything up
   for file in os.listdir("."):
     if file.startswith("temp_fft_dir_"):
       shutil.rmtree(file)

#Todo: Find a way to delete the temp directories.
#Note: The following code deletes the test directory before the xsim run.
#      Causes this error: xsim: error: C59076: problem loading file: ./temp_fft_dir_P3PQWz/bin/test.xe: Could not open input file
#  #clean everything up
#  for file in os.listdir("."):
#    if file.startswith("temp_fft_dir_"):
#      shutil.rmtree(file)      
  

measure_fft_performance()
