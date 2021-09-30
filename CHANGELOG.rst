xCORE-200 DSP library change log
================================

6.2.1
-----

  * CHANGED: Jenkinsfile used for CI

6.2.0
-----

  * CHANGED: Use XMOS Public Licence Version 1
  * CHANGED: dsp_vector_abs function to always return positive values

6.1.0
-----

  * ADDED: Support for XS3 architecture

6.0.2
-----

  * CHANGED: use XS2 version of platform-specific functions on XS3
  * CHANGED: Pin Python package versions
  * REMOVED: not necessary cpanfile

6.0.1
-----

  * CHANGED: Updated generate_window.py to work with Python 3
  * CHANGED: Updated to use Jenkins shared library 0.14.1

6.0.0
-----

  * ADDED: vector add, sub and mul functions for int8, int16 and int32 in both
    fixed and block floating point.
  * CHANGED: Functions re-implemented in C, requires the use of unsafe pointers
    if called from XC

  * Changes to dependencies:

    - lib_logging: Added dependency 3.0.0

5.0.0
-----

  * CHANGED: Build files updated to support new "xcommon" behaviour in xwaf.

4.2.0
-----

  * Added dsp_ch_pair_t struct to represent channel pairs
  * Added floating point biquads
  * Changed implementation of biquads to be faster and smaller
  * Added use of pipenv to set up python environment

4.1.0
-----

  * Added post-FFT Hanning windowing
  * Added function to combine real and imaginary arrays into complex array
  * Added function to split complex array into real and imaginary arrays
  * Added 48-point DCT
  * Added dsp filter FIR add sample
  * Added softplus
  * Added integer sqrt
  * Documentation updates

4.0.0
-----

  * Removed synchronous sample rate conversion functions - now maintained in
    lib_src
  * Fixed bug in dsp_vector_mulv_addv()
  * Faster bit reverse and inverse FFT
  * Added real FFT
  * Added real reverse FFT
  * Added FFT with top half blanked
  * Logistics functions
  * Block floating point functions
  * Complex FIR
  * Complex vector arithmetic, with optional scaling
  * Added in-place complex vector scaling
  * Added complex vector magnitude
  * Added complex vector scaling with arithmetic shift
  * Added complex negative multiply and accumulate

3.1.0
-----

  * Deprecated synchronous sample rate conversion functions - now maintained in
    lib_src
  * Added functions to compute a fast fixed point atan2 and hypotenuse
  * Added Q8 versions of the arc sine and arc cosine functions
  * Added 16384 point sine table
  * Improved performance of the forwards FFT function, with small reduction in
    memory footprint

3.0.0
-----

  * Added exponential and natural logarithm functions
  * Added Hyperbolic sine and cosine
  * Fixed Matrix Multiplication and improved performance
  * Changed API prefix from ``lib_dsp_`` to ``dsp_``.
  * Changed lib_dsp_fft_complex_t to dsp_complex_t and
    lib_dsp_fft_complex_short_t to dsp_complex_short_t
  * Various fixes in API documentation
  * Added complex vector multiplication
  * Added synchronous sample rate conversion (downsample or upsample by factor
    3)

2.0.0
-----

  * FFT interface update. Consolidated interface and improved testing.
  * Halved the memory for processing two real signals with a single complex FFT.
  * Renamed \*_transforms to \*_fft to improve naming consistency
  * Improved performance and accuracy of dsp_math_sqrt. Error is <= 1. Worst
    case performance is 96 cycles.
  * int32_t and uint32_t now used more consistently.

1.0.4
-----

  * Added fixed point sine and cosine functions. Performance: 62 cycles for
    dsp_math_sin, 64 cycles for dsp_math_cos.
  * Brute force testing of all input values proved accuracy to within one LSB
    (error is <= 1)
  * Added short int complex and tworeals FFT and iFFT
  * Improved Macros for converting from double to int and int to double.
  * Added optimised fixed point atan function dsp_math_atan
  * Most tests in math_app.xc are now self-checking. Improved error reporting.
  * Option for performance measurements in 10ns cycles.

1.0.3
-----

  * Update to source code license and copyright

1.0.2
-----

  * FFT and inverse FFT for two complex short int signals

1.0.1
-----

  * FFT and inverse FFT for complex signals or two real signals.

1.0.0
-----

  * Initial version

