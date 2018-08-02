# Copyright (c) 2015-2018, XMOS Ltd, All rights reserved
import xmostest

def runtest():
    resources = xmostest.request_resource("axe")
     
    tester = xmostest.ComparisonTester(open('matrix_test.expect'),
                                       'lib_dsp', 'simple_tests',
                                       'app_matrix', {})
     
    xmostest.run_on_simulator(resources['axe'],
                              '../AN00209_xCORE-200_DSP_Library/app_matrix/bin/app_matrix.xe',
                              tester=tester)
