import xmostest

def runtest():
    resources = xmostest.request_resource("xsim")
     
    tester = xmostest.ComparisonTester(open('statistics_test.expect'),
                                       'lib_dsp', 'simple_tests',
                                       'app_statistics', {})
     
    xmostest.run_on_simulator(resources['xsim'],
                              '../examples/AN00209_xCORE-200_DSP_Library/app_statistics/bin/app_statistics.xe',
                              tester=tester)
