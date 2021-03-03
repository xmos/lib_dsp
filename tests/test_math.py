# Copyright (c) 2015-2021, XMOS Ltd, All rights reserved
# This software is available under the terms provided in LICENSE.txt.
import xmostest

def runtest():
    resources = xmostest.request_resource("xsim")
     
    tester = xmostest.ComparisonTester(open('math_test.expect'),
                                       'lib_dsp', 'simple_tests',
                                       'app_math', {})
     
    xmostest.run_on_simulator(resources['xsim'],
                              '../AN00209_xCORE-200_DSP_Library/app_math/bin/app_math.xe',
                              tester=tester, timeout=1200)
