# Copyright 2015-2021 XMOS LIMITED.
# This Software is subject to the terms of the XMOS Public Licence: Version 1.
import xmostest

def runtest():
    resources = xmostest.request_resource("xsim")
     
    tester = xmostest.ComparisonTester(open('complex_fir_test.expect'),
                                       'lib_dsp', 'simple_tests',
                                       'app_complex_fir', {})
     
    xmostest.run_on_simulator(resources['xsim'],
                              '../AN00209_xCORE-200_DSP_Library/app_complex_fir/bin/app_complex_fir.xe',
                              tester=tester, timeout=1200)
