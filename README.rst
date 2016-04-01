xCORE-200 DSP Library
=====================

Summary
-------

This API reference manual describes the XMOS fixed-point digital signal processing
software library.  The library implements of a suite of common signal processing functions
for use on XMOS xCORE-200 multicore microcontrollers.

Required tools and libraries
............................

* xTIMEcomposer Tools Version 14.1.2 or later

Required hardware
.................

Only XMOS xCORE-200 based multicore microcontrollers are supported with this library.
The previous generation XS1 based multicore microntrollers are not supported.

The xCORE-200 has a single cycle 32x32->64 bit multiply/accumulate unit,
single cycle double-word load and store, dual issue instruction execution,
and other instruction set enhancements.
These features make xCORE-200 an efficient platform for executing
digital signal processing algorithms.

Prerequisites
.............

This document assumes familiarity with the XMOS xCORE architecture,
the XMOS tool chain, the 'C' programming language,
and digital signal processing concepts.

Software version and dependencies
.................................

.. libdeps::

Related application notes
.........................

The following application notes use this library:

  * AN00209 - xCORE-200 DSP Library
