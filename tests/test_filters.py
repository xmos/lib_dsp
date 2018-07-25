# Copyright (c) 2015-2018, XMOS Ltd, All rights reserved
import xmostest

def runtest():
    resources = xmostest.request_resource("xsim")
     
    tester = xmostest.ComparisonTester(open('filters_test.expect'),
                                       'lib_dsp', 'simple_tests',
                                       'app_filters', {})
     
    xmostest.run_on_simulator(resources['xsim'],
                              '../AN00209_xCORE-200_DSP_Library/app_filters/bin/app_filters.xe',
                              tester=tester)
