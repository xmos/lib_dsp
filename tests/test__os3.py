import xmostest

def runtest():
    resources = xmostest.request_resource("xsim")
     
    tester = xmostest.ComparisonTester(open('os3_test.expect'),
                                       'lib_dsp', 'simple_tests',
                                       'app_os3', {})
     
    xmostest.run_on_simulator(resources['xsim'],
                              '../AN00209_xCORE-200_DSP_Library/app_os3/bin/app_os3.xe',
                              tester=tester)
