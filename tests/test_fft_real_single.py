# Copyright (c) 2015-2018, XMOS Ltd, All rights reserved
import xmostest

def runtest():
    resources = xmostest.request_resource("xsim")
     
    tester = xmostest.ComparisonTester(open('fft_real_single_test.expect'),
                                       'lib_dsp', 'simple_tests',
                                       'app_fft_real_single', {})
     
    xmostest.run_on_simulator(resources['xsim'],
                              '../AN00209_xCORE-200_DSP_Library/app_fft_real_single/bin/app_fft_real_single.xe',
                              tester=tester, timeout=1200)
