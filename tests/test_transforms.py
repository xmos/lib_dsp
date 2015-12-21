import xmostest


def runtest():
    resources = xmostest.request_resource("xsim")
     
    tester = xmostest.ComparisonTester(open('tworeal_fft_test.expect'),
                                       'lib_dsp', 'simple_tests',
                                       'app_transforms', {})
     
    xmostest.run_on_simulator(resources['xsim'],
                              '../AN00209_xCORE-200_DSP_Library/app_transforms/bin/tworeal_fft/app_transforms_tworeal_fft.xe',
                              tester=tester)


    resources = xmostest.request_resource("xsim")
     
    tester = xmostest.ComparisonTester(open('complex_fft_test.expect'),
                                       'lib_dsp', 'simple_tests',
                                       'app_transforms', {})
     
    xmostest.run_on_simulator(resources['xsim'],
                              '../AN00209_xCORE-200_DSP_Library/app_transforms/bin/complex_fft/app_transforms_complex_fft.xe',
                              tester=tester)


    resources = xmostest.request_resource("xsim")
     
    tester = xmostest.ComparisonTester(open('complex_short_int_fft_test.expect'),
                                       'lib_dsp', 'simple_tests',
                                       'app_fft_short_int', {})
     
    xmostest.run_on_simulator(resources['xsim'],
                              '../AN00209_xCORE-200_DSP_Library/app_fft_short_int/bin/app_fft_short_int.xe',
                              tester=tester)


