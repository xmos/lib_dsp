xCORE-200 DSP library change log
================================

2.0.0
-----

  * FFT interface update. Consolidated interface and improved testing.
  * Halved the memory for processing two real signals with a single complex FFT.
  * Renamed \*_transforms to \*_fft to improve naming consistency
  * Improved performance and accuracy of lib_dsp_math_squareroot. Error is <= 1.
    Worst case performance is 96 cycles.
  * int32_t and uint32_t now used more consistently.

1.0.4
-----

  * Added fixed point sine and cosine functions. Performance: 62 cycles for
    lib_dsp_math_sin, 64 cycles for lib_dsp_math_cos.
  * Brute force testing of all input values proved accuracy to within one LSB
    (error is <= 1)
  * Added short int complex and tworeals FFT and iFFT
  * Improved Macros for converting from double to int and int to double.
  * Added optimised fixed point atan function lib_dsp_math_atan
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

