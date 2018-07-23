#!/usr/bin/env python
# Copyright (c) 2015-2017, XMOS Ltd, All rights reserved
import xmostest

if __name__ == "__main__":
   xmostest.init()
    
   xmostest.register_group("lib_dsp",
                           "simple_tests",
                           "Simple functionality tests",
                           "The tests for this library currently contain basic tests for the major functionality of the library.")
    
    
   xmostest.register_group("lib_dsp",
                           "fft_tests",
                           "Test FFT correctness and functionality",
                           "Test FFT correctness and functionality for all supported FFT lengths.")
    
   xmostest.runtests()
   xmostest.finish()
