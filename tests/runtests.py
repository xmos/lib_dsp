#!/usr/bin/env python2
# Copyright (c) 2015-2021, XMOS Ltd, All rights reserved
# This software is available under the terms provided in LICENSE.txt.
import xmostest

if __name__ == "__main__":
   xmostest.init()
    
   xmostest.register_group("lib_dsp",
                           "simple_tests",
                           "Simple functionality tests",
                           "The tests for this library currently contain basic tests for the major functionality of the library.")
    
    
   xmostest.runtests()
   xmostest.finish()
