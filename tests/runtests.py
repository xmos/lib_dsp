#!/usr/bin/env python
import xmostest

if __name__ == "__main__":
   xmostest.init()
    
   xmostest.register_group("lib_dsp",
                           "simple_tests",
                           "Simple functionality tests",
                           "The tests for this library currently contain basic tests for the major functionality of the library.")
    
    
   xmostest.register_group("lib_dsp",
                           "fft_tests_fast",
                           "Test FFT correctness and functionality",
                           "Test FFT correctness and functionality for all supported FFT lengths.")
    
   xmostest.runtests()
   xmostest.finish()
