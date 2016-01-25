import xmostest

def runtest():
    # Test app_transforms: 'tworeals' configuration
    resources = xmostest.request_resource("xsim")

    tester = xmostest.ComparisonTester(open('tworeal_fft_test.expect'),
                                       'lib_dsp', 'simple_tests',
                                       'app_transforms', {'config':'tworeals'})

    xmostest.run_on_simulator(resources['xsim'],
                              '../AN00209_xCORE-200_DSP_Library/app_transforms/bin/tworeals_fft/app_transforms_tworeals_fft.xe',
                              tester=tester)

    # Test app_transforms: 'complex' configuration
    resources = xmostest.request_resource("xsim")

    tester = xmostest.ComparisonTester(open('complex_fft_test.expect'),
                                       'lib_dsp', 'simple_tests',
                                       'app_transforms', {'config':'complex'})

    xmostest.run_on_simulator(resources['xsim'],
                              '../AN00209_xCORE-200_DSP_Library/app_transforms/bin/complex_fft/app_transforms_complex_fft.xe',
                              tester=tester)

    # Test app_fft_short_int: 'two_complex' configuration
    resources = xmostest.request_resource("xsim")

    tester = xmostest.ComparisonTester(open('app_fft_short_int_two_complex.xe.expect'),
                                       'lib_dsp', 'simple_tests',
                                       'app_fft_short_int', {'config':'two_complex'})

    xmostest.run_on_simulator(resources['xsim'],
                              '../AN00209_xCORE-200_DSP_Library/app_fft_short_int/bin/two_complex/app_fft_short_int_two_complex.xe',
                              tester=tester)

    # Test app_fft_short_int: 'complex' configuration
    resources = xmostest.request_resource("xsim")

    tester = xmostest.ComparisonTester(open('app_fft_short_int_complex.xe.expect'),
                                       'lib_dsp', 'simple_tests',
                                       'app_fft_short_int', {'config':'complex'})

    xmostest.run_on_simulator(resources['xsim'],
                              '../AN00209_xCORE-200_DSP_Library/app_fft_short_int/bin/complex/app_fft_short_int_complex.xe',
                              tester=tester)

    # Test app_fft_short_int: 'tworeals' configuration
    resources = xmostest.request_resource("xsim")

    tester = xmostest.ComparisonTester(open('app_fft_short_int_tworeals.xe.expect'),
                                       'lib_dsp', 'simple_tests',
                                       'app_fft_short_int', {'config':'tworeals'})

    xmostest.run_on_simulator(resources['xsim'],
                              '../AN00209_xCORE-200_DSP_Library/app_fft_short_int/bin/tworeals/app_fft_short_int_tworeals.xe',
                              tester=tester)
