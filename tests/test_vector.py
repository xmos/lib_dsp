import xmostest

def runtest():
    resources = xmostest.request_resource("xsim")
     
    tester = xmostest.ComparisonTester(open('vector_test.expect'),
                                       'lib_dsp', 'simple_tests',
                                       'app_vector', {})
     
    xmostest.run_on_simulator(resources['xsim'],
                              '../examples/AN00209_xCORE-200_DSP_Library/app_vector/bin/app_vector.xe',
                              tester=tester)
