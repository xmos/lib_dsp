import xmostest

def runtest():
    resources = xmostest.request_resource("xsim")
     
    tester = xmostest.ComparisonTester(open('hann.expect'),
                                       'lib_dsp', 'simple_tests',
                                       'app_window_post_fft', {})
     
    xmostest.run_on_simulator(resources['xsim'],
                              '../AN00209_xCORE-200_DSP_Library/app_window_post_fft/bin/app_window_post_fft.xe',
                              tester=tester, timeout=1200)
