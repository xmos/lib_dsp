import xmostest


def runtest():
    resources = xmostest.request_resource("xsim")
     
    tester = xmostest.ComparisonTester(open('transforms_test.expect'),
                                       'lib_dsp', 'simple_tests',
                                       'app_transforms', {})
     
    xmostest.run_on_simulator(resources['xsim'],
                              '../AN00209_xCORE-200_DSP_Library/app_transforms/bin/app_transforms.xe',
                              tester=tester)
