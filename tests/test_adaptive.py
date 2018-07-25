# Copyright (c) 2015-2018, XMOS Ltd, All rights reserved
import xmostest

def runtest():
    resources = xmostest.request_resource("xsim")
     
    tester = xmostest.ComparisonTester(open('adaptive_test.expect'),
                                       'lib_dsp', 'simple_tests',
                                       'app_adaptive', {})
     
    xmostest.run_on_simulator(resources['xsim'],
                              '../AN00209_xCORE-200_DSP_Library/app_adaptive/bin/app_adaptive.xe',
                              tester=tester)
