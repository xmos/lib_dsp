# Copyright (c) 2015, XMOS Ltd, All rights reserved
import xmostest

def runtest():
    resources = xmostest.request_resource("xsim")
     
    tester = xmostest.ComparisonTester(open('design_test.expect'),
                                       'lib_dsp', 'simple_tests',
                                       'app_design', {})
     
    xmostest.run_on_simulator(resources['xsim'],
                              '../AN00209_xCORE-200_DSP_Library/app_design/bin/app_design.xe',
                              tester=tester)
