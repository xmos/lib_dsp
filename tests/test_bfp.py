# Copyright (c) 2015-2018, XMOS Ltd, All rights reserved
import xmostest

def runtest():
    resources = xmostest.request_resource("axe")
     
    tester = xmostest.ComparisonTester(open('bfp_test.expect'),
                                       'lib_dsp', 'simple_tests',
                                       'app_bfp', {})
     
    xmostest.run_on_simulator(resources['axe'],
                              '../AN00209_xCORE-200_DSP_Library/app_bfp/bin/app_bfp.xe',
                              tester=tester, timeout=1200)
