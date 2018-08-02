# Copyright (c) 2015-2018, XMOS Ltd, All rights reserved
import xmostest

def runtest():
    resources = xmostest.request_resource("axe")
     
    tester = xmostest.ComparisonTester(open('hann_test.expect'),
                                       'lib_dsp', 'simple_tests',
                                       'app_window_post_fft', {})
     
    xmostest.run_on_simulator(resources['axe'],
                              '../AN00209_xCORE-200_DSP_Library/app_window_post_fft/bin/app_window_post_fft.xe',
                              tester=tester, timeout=1200)
