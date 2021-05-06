#!/usr/bin/env python2
# Copyright 2015-2021 XMOS LIMITED.
# This Software is subject to the terms of the XMOS Public Licence: Version 1.
import xmostest

if __name__ == "__main__":
   xmostest.init()
    
   xmostest.register_group("lib_dsp",
                           "simple_tests",
                           "Simple functionality tests",
                           "The tests for this library currently contain basic tests for the major functionality of the library.")
    
    
   xmostest.runtests()
   xmostest.finish()
