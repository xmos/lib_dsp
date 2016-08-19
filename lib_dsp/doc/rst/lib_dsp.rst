.. include:: ../../../README.rst

Overview
--------

Introduction
............

This API reference manual describes the XMOS xCORE-200 fixed-point digital signal processing
firmware library.  The library implements of a suite of common signal processing functions
for use on XMOS xCORE-200 multicore microcontrollers.

Library Organization
....................

The library is divided into function collections with each collection covering a specific
digital signal processing algorithm category.
The API and implementation for each category are provided by a single 'C' header file and
implementation file.

.. table:: DSP library organization

  +-------------+----------------+---------------------------------------------------------------+
  | Category    | Source Files   | Functions                                                     |
  +=============+================+===============================================================+
  | Fixed point | dsp_qformat    | Q8 through Q31 formats, fixed and floating point conversions  |
  +-------------+----------------+---------------------------------------------------------------+
  | Filters     | dsp_filters    | FIR, biquad, cascaded biquad, and convolution                 |
  +-------------+----------------+---------------------------------------------------------------+
  | Adaptive    | dsp_adaptive   | LMS and NLMS Adaptive filters                                 |
  +-------------+----------------+---------------------------------------------------------------+
  | Scalar math | dsp_math       | Multiply, divide, square root, exponential, natural logarithm |
  |             |                | trigonometric, hyperbolic                                     |
  +-------------+----------------+---------------------------------------------------------------+
  | Vector math | dsp_vector     | Scalar/vector add/subtract/multiply, dot product              |
  +-------------+----------------+---------------------------------------------------------------+
  | Matrix math | dsp_matrix     | Scalar/matrix add/subtract/multiply, inverse and transpose    |
  +-------------+----------------+---------------------------------------------------------------+
  | Statistics  | dsp_statistics | Vector mean, sum-of-squares, root-mean-square, variance       |
  +-------------+----------------+---------------------------------------------------------------+
  | Design      | dsp_design     | Biquad coefficient generation for various filter types        |
  +-------------+----------------+---------------------------------------------------------------+
  | FFT         | dsp_fft        | Forward and inverse Fast Fourier Transforms.                  |
  +-------------+----------------+---------------------------------------------------------------+
  | Sample Rate | dsp_ds3,       | Downsample by factor of 3 (e.g. 48KHz to 16KHz Audio SRC).    |
  | Conversion  | dsp_os3        | Oversample by factor of 3 (e.g. 16KHz to 48KHz Audio SRC).    |
  +-------------+----------------+---------------------------------------------------------------+


Fixed-Point Format
------------------

Q Format Introduction
.....................

The library functions support 32 bit input and output data, with internal 64 bit accumulator.
The output data can be scaled to any of the supported Q Formats (Q8 through Q31), for all functions except for
sample rate conversion, which uses a fixed Q31 format.

Further details about Q Format numbers is available `here <https://en.wikipedia.org/wiki/Q_(number_format)>`_.

The 'q_format' Parameter
........................

All XMOS DSP library functions that incorporate a multiply operation accept a parameter called
q_format. This parameter can naively be used to specify the fixed point format for all
operands and results (if applicable) where the formats are the same for all parameters. For
example::

  result_q28 = dsp_math_multiply( input1_q28, input2_q28, 28 );
  
The 'q_format' parameter, being used after one or more sequences of multiply and/or
multiply-accumulate, is used to right-shift the 64-bit accumulator before truncating the value
back to a 32-bit integer (i.e. the 32-bit fixed-point result).  Therefore the 'q_format'
parameter can be used to perform the proper fixed-point adjustment for any combination of input
operand fixed-point format and desired fixed-point result format.

The output fixed-point fraction
bit count is equal to the sum of the two input fraction bit counts minus the desired result fraction
bit count::

  q_format = input1 fraction bit count +  input2 fraction bit count - result fraction bit count
  
For example::

  // q_format_parameter = 31 = 30 + 29 - 28
  result_q28 = dsp_math_multiply( input1_q30, input2_q29, 31 );

  // q_format_parameter = 27 = 28 + 29 - 30
  result_q30 = dsp_math_multiply( input1_q28, input2_q29, 27 );
  
Filter Functions: Finite Impulse Response (FIR) Filter
------------------------------------------------------

.. doxygenfunction:: dsp_filters_fir

Filter Functions: Interpolating FIR Filter
------------------------------------------

.. doxygenfunction:: dsp_filters_interpolate

Filter Functions: Decimating FIR Filter
---------------------------------------

.. doxygenfunction:: dsp_filters_decimate

Filter Functions: Bi-Quadratic (BiQuad) IIR Filter
--------------------------------------------------

.. doxygenfunction:: dsp_filters_biquad

Filter Functions: Cascaded BiQuad Filter
----------------------------------------

.. doxygenfunction:: dsp_filters_biquads

Adaptive Filter Functions: LMS Adaptive Filter
----------------------------------------------

.. doxygenfunction:: dsp_adaptive_lms

Adaptive Filter Functions: Normalized LMS Filter
------------------------------------------------

.. doxygenfunction:: dsp_adaptive_nlms

Scalar Math Functions: Multiply
-------------------------------

.. doxygenfunction:: dsp_math_multiply

Scalar Math Functions: Multiply Saturated
-----------------------------------------

.. doxygenfunction:: dsp_math_multiply_sat

Scalar Math Functions: Square Root
----------------------------------

.. doxygenfunction:: dsp_math_sqrt

Scalar Math Functions: Sine
---------------------------

.. doxygenfunction:: dsp_math_sin

Scalar Math Functions: Cosine
-----------------------------

.. doxygenfunction:: dsp_math_cos

Scalar Math Functions: Arctangent
----------------------------------

.. doxygenfunction:: dsp_math_atan

Scalar Math Functions: Exponential
----------------------------------

.. doxygenfunction:: dsp_math_exp

Scalar Math Functions: Natural Logarithm
----------------------------------------

.. doxygenfunction:: dsp_math_log

Scalar Math Functions: Hyperbolic Sine 
--------------------------------------

.. doxygenfunction:: dsp_math_sinh

Scalar Math Functions: Hyperbolic Cosine 
----------------------------------------

.. doxygenfunction:: dsp_math_cosh

Vector Math Functions: Minimum Value
------------------------------------

.. doxygenfunction:: dsp_vector_minimum

Vector Math Functions: Maximum Value
------------------------------------

.. doxygenfunction:: dsp_vector_maximum

Vector Math Functions: Element Negation
---------------------------------------

.. doxygenfunction:: dsp_vector_negate

Vector Math Functions: Element Absolute Value
---------------------------------------------

.. doxygenfunction:: dsp_vector_abs

Vector Math Functions: Scalar Addition
--------------------------------------

.. doxygenfunction:: dsp_vector_adds

Vector Math Functions: Scalar Multiplication
--------------------------------------------

.. doxygenfunction:: dsp_vector_muls

Vector Math Functions: Vector Addition
--------------------------------------

.. doxygenfunction:: dsp_vector_addv

Vector Math Functions: Vector Subtraction
-----------------------------------------

.. doxygenfunction:: dsp_vector_subv

Vector Math Functions: Vector Multiplication
--------------------------------------------

.. doxygenfunction:: dsp_vector_mulv

Vector Math Functions: Vector multiplication and scalar addition
----------------------------------------------------------------

.. doxygenfunction:: dsp_vector_mulv_adds

Vector Math Functions: Scalar multiplication and vector addition
----------------------------------------------------------------

.. doxygenfunction:: dsp_vector_muls_addv

Vector Math Functions: Scalar multiplication and vector subtraction
-------------------------------------------------------------------

.. doxygenfunction:: dsp_vector_muls_subv

Vector Math Functions: Vector multiplication and vector addition
----------------------------------------------------------------

.. doxygenfunction:: dsp_vector_mulv_addv

Vector Math Functions: Vector multiplication and vector subtraction
-------------------------------------------------------------------

.. doxygenfunction:: dsp_vector_mulv_subv

Vector Math Functions: Complex vector multiplication
----------------------------------------------------

.. doxygenfunction:: dsp_vector_mulv_complex

Matrix Math Functions: Element Negation
---------------------------------------

.. doxygenfunction:: dsp_matrix_negate

Matrix Math Functions: Scalar Addition
--------------------------------------

.. doxygenfunction:: dsp_matrix_adds

Matrix Math Functions: Scalar Multiplication
--------------------------------------------

.. doxygenfunction:: dsp_matrix_muls

Matrix Math Functions: Matrix Addition
--------------------------------------

.. doxygenfunction:: dsp_matrix_addm

Matrix Math Functions: Matrix Subtraction
-----------------------------------------

.. doxygenfunction:: dsp_matrix_subm

Matrix Math Functions: Matrix Multiplication
--------------------------------------------

.. doxygenfunction:: dsp_matrix_mulm

Statistics Functions: Vector Absolute Sum
-----------------------------------------

.. doxygenfunction:: dsp_vector_abs_sum
  
Statistics Functions: Vector Mean
---------------------------------

.. doxygenfunction:: dsp_vector_mean

Statistics Functions: Vector Power (Sum-of-Squares)
---------------------------------------------------

.. doxygenfunction:: dsp_vector_power

Statistics Functions: Root Mean Square (RMS)
--------------------------------------------

.. doxygenfunction:: dsp_vector_rms

Statistics Functions: Dot Product
---------------------------------

.. doxygenfunction:: dsp_vector_dotprod

Filter Design Functions: Notch Filter
-------------------------------------

.. doxygenfunction:: dsp_design_biquad_notch

Filter Design Functions: Low-pass Filter
----------------------------------------

.. doxygenfunction:: dsp_design_biquad_lowpass

Filter Design Functions: High-pass Filter
-----------------------------------------

.. doxygenfunction:: dsp_design_biquad_highpass

Filter Design Functions: All-pass Filter
----------------------------------------

.. doxygenfunction:: dsp_design_biquad_allpass

Filter Design Functions: Band-pass Filter
-----------------------------------------

.. doxygenfunction:: dsp_design_biquad_bandpass

Filter Design Functions: Peaking Filter
---------------------------------------

.. doxygenfunction:: dsp_design_biquad_peaking

Filter Design Functions: Bass Shelving Filter
---------------------------------------------

.. doxygenfunction:: dsp_design_biquad_lowshelf

Filter Design Functions: Treble Shelving Filter
-----------------------------------------------

.. doxygenfunction:: dsp_design_biquad_highshelf

FFT functions
-------------

**Note:** The method for processing two real signals with a single complex FFT was improved.
It now requires only half the memory. 
The function dsp_fft_split_spectrum is used to split the combined N point output of dsp_fft_forward into two half-spectra of size N/2. One for each of the two real input signals.
dsp_fft_merge_spectra is used to merge the two half-spectra into a combined spectrum that can be processed by dsp_fft_inverse.

.. doxygenfunction:: dsp_fft_split_spectrum
.. doxygenfunction:: dsp_fft_merge_spectra
.. doxygenfunction:: dsp_fft_short_to_long
.. doxygenfunction:: dsp_fft_long_to_short
.. doxygenfunction:: dsp_fft_bit_reverse
.. doxygenfunction:: dsp_fft_forward
.. doxygenfunction:: dsp_fft_inverse

Audio Sample Rate Conversion
----------------------------

The DSP library includes downsample (decimate) and oversample (upsample or interpolate) by factor of three functions called DS3 and OS3. In each case, the DSP processing is carried out each time a single output sample is required. In the case of the decimator, three input samples passed to filter with a resulting one sample output on calling the processing function. The interpolator produces an output sample each time the processing function is called but will require a single sample to be pushed into the filter every third cycle. All samples use Q31 format (left justified signed 32b integer).

Both sample rate converters are based on a 144 tap FIR filter with two sets of coefficients available, depending on application requirements:

 * firos3_b_144.dat / firds3_b_144.dat - These filters have 20dB of attenuation at the nyquist frequency and a higher cutoff frequency
 * firos3_144.dat / firds3_144.dat - These filters have 60dB of attenuation at the nyquist frequency but trade this off with a lower cutoff frequency

The filter coefficients may be selected by adjusting the line::

  #define   FIROS3_COEFS_FILE

and::

  #define   FIRDS3_COEFS_FILE

in the files ``dsp_os3.h`` and ``dsp_ds3.h`` respectively.

The OS3 processing takes up to 157 core cycles to compute a sample which translates to 1.57us at 100MHz or 2.512us at 62.5MHz core speed. This permits up to 8 channels of 16KHz -> 48KHz sample rate conversion in a single 62.5MHz core.

The DS3 processing takes up to 402 core cycles to compute a sample which translates to 4.02us at 100MHz or 6.432us at 62.5MHz core speed. This permits up to 9 channels of 48KHz -> 16KHz sample rate conversion in a single 62.5MHz core. 

Both downsample and oversample functions return ``ERROR`` or  ``NOERROR`` status codes as defined in return codes enums listed below.

The down sampling functions return the following error codes ::

  FIRDS3_NO_ERROR
  FIRDS3_ERROR

The up sampling functions return the following error codes ::

  FIROS3_NO_ERROR
  FIROS3_ERROR

For details on synchronous audio sample rate conversion by factors of two, or asynchronous audio sample rate conversion please see the XMOS Sample Rate Conversion Library [#]_.

.. [#] http://www.xmos.com/published/lib_src-userguide


DS3 Function API
................

.. doxygentypedef:: FIRDS3ReturnCodes_t

.. doxygenfunction:: FIRDS3_init

.. doxygenfunction:: FIRDS3_sync

.. doxygenfunction:: FIRDS3_proc

OS3 Function API
................

.. doxygentypedef:: FIROS3ReturnCodes_t

.. doxygenfunction:: FIROS3_init

.. doxygenfunction:: FIROS3_sync

.. doxygenfunction:: FIROS3_input

.. doxygenfunction:: FIRDS3_proc

|appendix|

Known Issues
------------

There are no known issues.

.. include:: ../../../CHANGELOG.rst
