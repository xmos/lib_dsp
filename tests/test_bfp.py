import xmostest

def runtest():
    resources = xmostest.request_resource("xsim")
     
    tester = xmostest.ComparisonTester(open('bfp_test.expect'),
                                       'lib_dsp', 'simple_tests',
                                       'test_bfp', {})
     
    xmostest.run_on_simulator(resources['xsim'],
                              '../tests/test_bfp/bin/test.xe',
                              tester=tester)
