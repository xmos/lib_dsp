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

  +--------------+----------------+---------------------------------------------------------------+
  | Category     | Source Files   | Functions                                                     |
  +==============+================+===============================================================+
  | Fixed point  | dsp_qformat    | Q8 through Q31 formats, fixed and floating point conversions  |
  +--------------+----------------+---------------------------------------------------------------+
  | Filters      | dsp_filters    | FIR, biquad, cascaded biquad, and convolution                 |
  +--------------+----------------+---------------------------------------------------------------+
  | Adaptive     | dsp_adaptive   | LMS and NLMS Adaptive filters                                 |
  +--------------+----------------+---------------------------------------------------------------+
  | Scalar math  | dsp_math       | Multiply, divide, square root, exponential, natural logarithm |
  |              |                | trigonometric, hyperbolic                                     |
  +--------------+----------------+---------------------------------------------------------------+
  | Integer math | dsp_math       | Square root                                                   |
  +--------------+----------------+---------------------------------------------------------------+
  | Complex math | dsp_complex    | Complex math functions plus real, imaginary combine/split     |
  +--------------+----------------+---------------------------------------------------------------+
  | Vector math  | dsp_vector     | Scalar/vector add/subtract/multiply, dot product              |
  +--------------+----------------+---------------------------------------------------------------+
  | Matrix math  | dsp_matrix     | Scalar/matrix add/subtract/multiply, inverse and transpose    |
  +--------------+----------------+---------------------------------------------------------------+
  | Statistics   | dsp_statistics | Vector mean, sum-of-squares, root-mean-square, variance       |
  +--------------+----------------+---------------------------------------------------------------+
  | Design       | dsp_design     | Biquad coefficient generation for various filter types        |
  +--------------+----------------+---------------------------------------------------------------+
  | FFT          | dsp_fft        | Forward and inverse Fast Fourier Transforms.                  |
  +--------------+----------------+---------------------------------------------------------------+
  | DCT          | dsp_dct        | Forward and inverse Discrete Cosine Transforms.               |
  +--------------+----------------+---------------------------------------------------------------+


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

Filter Functions: Finite Impulse Response (FIR) Filter Add Sample
-----------------------------------------------------------------

.. doxygenfunction:: dsp_filters_fir_add_sample

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

Scalar Math Functions: Signed Division
--------------------------------------

.. doxygenfunction:: dsp_math_divide

Scalar Math Functions: Unsigned Division
----------------------------------------

.. doxygenfunction:: dsp_math_divide_unsigned

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

Scalar Math Functions: Softplus
----------------------------------------

.. doxygenfunction:: dsp_math_softplus

Integer Math Functions: Square Root
-----------------------------------

.. doxygenfunction:: dsp_math_int_sqrt

Integer Math Functions: 64-bit Square Root
------------------------------------------

.. doxygenfunction:: dsp_math_int_sqrt64

Complex Math Functions: Add
---------------------------

.. doxygenfunction:: dsp_complex_add

Complex Math Functions: Subtract
--------------------------------

.. doxygenfunction:: dsp_complex_sub

Complex Math Functions: Multiplication
--------------------------------------

.. doxygenfunction:: dsp_complex_mul

Complex Math Functions: Inner Product
-------------------------------------

.. doxygenfunction:: dsp_complex_mul_conjugate

Complex Math Functions: Complex FIR Filter
------------------------------------------

.. doxygenfunction:: dsp_complex_fir

Complex Math Functions: Element By Element Multiplication
---------------------------------------------------------

.. doxygenfunction:: dsp_complex_mul_vector

Complex Math Functions: Element By Element Inner Product
--------------------------------------------------------

.. doxygenfunction:: dsp_complex_mul_conjugate_vector

Complex Math Functions: Element By Element Vector Product
---------------------------------------------------------

.. doxygenfunction:: dsp_complex_mul_conjugate_vector3

Complex Math Functions: Element By Element Vector Add
-----------------------------------------------------

.. doxygenfunction:: dsp_complex_add_vector

Complex Math Functions: Element By Element Add With Shift Left Scaling
----------------------------------------------------------------------

.. doxygenfunction:: dsp_complex_add_vector_shl

Complex Math Functions: Element By Element Add With Scaling
-----------------------------------------------------------

.. doxygenfunction:: dsp_complex_add_vector_scale

Complex Math Functions: Element By Element Subtraction
------------------------------------------------------

.. doxygenfunction:: dsp_complex_sub_vector

Complex Math Functions: Element By Element Addition Into A Third Vector
-----------------------------------------------------------------------

.. doxygenfunction:: dsp_complex_add_vector3

Complex Math Functions: Element By Element Subtraction Into A Third Vector
--------------------------------------------------------------------------

.. doxygenfunction:: dsp_complex_sub_vector3

Complex Math Functions: Element By Element Multiply Accumulate
--------------------------------------------------------------

.. doxygenfunction:: dsp_complex_macc_vector

Complex Math Functions: Element By Element Multiply Subtract
------------------------------------------------------------

.. doxygenfunction:: dsp_complex_nmacc_vector

Complex Math Functions: Multiply Scalar And Shift Down
------------------------------------------------------

.. doxygenfunction:: dsp_complex_scalar_vector3

Complex Math Functions: Return Magnitude With Selectable Precision
------------------------------------------------------------------

.. doxygenfunction:: dsp_complex_magnitude_vector

Complex Math Functions: Scale By A Fraction
-------------------------------------------

.. doxygenfunction:: dsp_complex_scale_vector

Complex Math Functions: Combine Separate Real And Imaginary Arrays Into Interleaved Complex
-------------------------------------------------------------------------------------------

.. doxygenfunction:: dsp_complex_combine

Complex Math Functions: Split Interleaved Complex Array Into Separate Real And Imaginary
----------------------------------------------------------------------------------------

.. doxygenfunction:: dsp_complex_split

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

DCT functions
-------------

.. doxygenfunction:: dsp_dct_forward48
.. doxygenfunction:: dsp_dct_forward32
.. doxygenfunction:: dsp_dct_forward24
.. doxygenfunction:: dsp_dct_forward16
.. doxygenfunction:: dsp_dct_forward12
.. doxygenfunction:: dsp_dct_forward8
.. doxygenfunction:: dsp_dct_forward6
.. doxygenfunction:: dsp_dct_forward4
.. doxygenfunction:: dsp_dct_forward3
.. doxygenfunction:: dsp_dct_forward2
.. doxygenfunction:: dsp_dct_forward1
.. doxygenfunction:: dsp_dct_inverse4
.. doxygenfunction:: dsp_dct_inverse3
.. doxygenfunction:: dsp_dct_inverse2
.. doxygenfunction:: dsp_dct_inverse1

|appendix|

Known Issues
------------

There are no known issues.

.. include:: ../../../CHANGELOG.rst
