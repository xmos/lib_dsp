#!/usr/bin/env python
import xmostest

if __name__ == "__main__":
   xmostest.init()
    
   xmostest.register_group("lib_dsp",
                           "simple_tests",
                           "Simple functionality tests",
                           "The tests for this library currently contain basic tests for the major functionality of the library.")
    
   xmostest.runtests()
   xmostest.finish()
