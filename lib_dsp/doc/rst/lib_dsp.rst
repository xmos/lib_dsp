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
    :class: narrow

  +-------------+----------------------+---------------------------------------------------------------+
  | Category    | Source Files         | Functions                                                     |
  +=============+======================+===============================================================+
  | Fixed point | lib_dsp_qformat      | Q16 through Q31 formats, fixed and floating point conversions |
  +-------------+----------------------+---------------------------------------------------------------+
  | Filters     | lib_dsp_filters      | FIR, biquad, cascaded biquad, and convolution                 |
  +-------------+----------------------+---------------------------------------------------------------+
  | Adaptive    | lib_dsp_adaptive     | LMS and NLMS Adaptive filters                                 |
  +-------------+----------------------+---------------------------------------------------------------+
  | Scalar math | lib_dsp_math         | Multiply, square root, reciprocal, inverse square root        |
  +-------------+----------------------+---------------------------------------------------------------+
  | Vector math | lib_dsp_vector       | Scalar/vector add/subtract/multiply, dot product              |
  +-------------+----------------------+---------------------------------------------------------------+
  | Matrix math | lib_dsp_matrix       | Scalar/matrix add/subtract/multiply, inverse and transpose    |
  +-------------+----------------------+---------------------------------------------------------------+
  | Statistics  | lib_dsp_statistics   | Vector mean, sum-of-squares, root-mean-square, variance       |
  +-------------+----------------------+---------------------------------------------------------------+
  | Design      | lib_dsp_design       | Biquad coefficient generation for various filter types        |
  +-------------+----------------------+---------------------------------------------------------------+
  | FFT         | lib_dsp_fft          | Forward and inverse Fast Fourier Transforms.                  |
  +-------------+----------------------+---------------------------------------------------------------+

Fixed-Point Format
------------------

Q Format Introduction
.....................

The library functions support 32 bit input and output data, with internal 64 bit accumulator.
The output data can be scaled to any of the supported Q Formats (Q8 through Q31).
Further details about Q Format numbers is available here : https://en.wikipedia.org/wiki/Q_(number_format).

The 'q_format' Parameter
........................

All XMOS DSP library functions that incorporate a multiply operation accept a parameter called
q_format. This parameter can naively be used to specify the fixed point format for all
operands and results (if applicable) where the formats are the same for all parameters. For
example::

  result_q28 = lib_dsp_math_multiply( input1_q28, input2_q28, 28 );
  
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
  result_q28 = lib_dsp_math_multiply( input1_q30, input2_q29, 31 );

  // q_format_parameter = 27 = 28 + 29 - 30
  result_q30 = lib_dsp_math_multiply( input1_q28, input2_q29, 27 );
  
Filter Functions: Finite Impulse Response (FIR) Filter
------------------------------------------------------

.. doxygenfunction:: lib_dsp_filters_fir

Filter Functions: Interpolating FIR Filter
------------------------------------------

.. doxygenfunction:: lib_dsp_filters_interpolate

Filter Functions: Decimating FIR Filter
---------------------------------------

.. doxygenfunction:: lib_dsp_filters_decimate

Filter Functions: Bi-Quadratic (BiQuad) IIR Filter
--------------------------------------------------

.. doxygenfunction:: lib_dsp_filters_biquad

Filter Functions: Cascaded BiQuad Filter
----------------------------------------

.. doxygenfunction:: lib_dsp_filters_biquads

Adaptive Filter Functions: LMS Adaptive Filter
----------------------------------------------

.. doxygenfunction:: lib_dsp_adaptive_lms

Adaptive Filter Functions: Normalized LMS Filter
------------------------------------------------

.. doxygenfunction:: lib_dsp_adaptive_nlms

Scalar Math Functions: Multiply
-------------------------------

.. doxygenfunction:: lib_dsp_math_multiply

Scalar Math Functions: Square Root
----------------------------------

.. doxygenfunction:: lib_dsp_math_squareroot

Vector Math Functions: Minimum Value
------------------------------------

.. doxygenfunction:: lib_dsp_vector_minimum

Vector Math Functions: Maximum Value
------------------------------------

.. doxygenfunction:: lib_dsp_vector_maximum

Vector Math Functions: Element Negation
---------------------------------------

.. doxygenfunction:: lib_dsp_vector_negate

Vector Math Functions: Element Absolute Value
---------------------------------------------

.. doxygenfunction:: lib_dsp_vector_abs

Vector Math Functions: Scalar Addition
--------------------------------------

.. doxygenfunction:: lib_dsp_vector_adds

Vector Math Functions: Scalar Multiplication
--------------------------------------------

.. doxygenfunction:: lib_dsp_vector_muls

Vector Math Functions: Vector Addition
--------------------------------------

.. doxygenfunction:: lib_dsp_vector_addv

Vector Math Functions: Vector Subtraction
-----------------------------------------

.. doxygenfunction:: lib_dsp_vector_subv

Vector Math Functions: Vector Multiplication
--------------------------------------------

.. doxygenfunction:: lib_dsp_vector_mulv

Vector Math Functions: Vector multiplication and scalar addition
----------------------------------------------------------------

.. doxygenfunction:: lib_dsp_vector_mulv_adds

Vector Math Functions: Scalar multiplication and vector addition
----------------------------------------------------------------

.. doxygenfunction:: lib_dsp_vector_muls_addv

Vector Math Functions: Scalar multiplication and vector subtraction
-------------------------------------------------------------------

.. doxygenfunction:: lib_dsp_vector_muls_subv

Vector Math Functions: Vector multiplication and vector addition
----------------------------------------------------------------

.. doxygenfunction:: lib_dsp_vector_mulv_addv

Vector Math Functions: Vector multiplication and vector subtraction
-------------------------------------------------------------------

.. doxygenfunction:: lib_dsp_vector_mulv_subv

Matrix Math Functions: Element Negation
---------------------------------------

.. doxygenfunction:: lib_dsp_matrix_negate

Matrix Math Functions: Scalar Addition
--------------------------------------

.. doxygenfunction:: lib_dsp_matrix_adds

Matrix Math Functions: Scalar Multiplication
--------------------------------------------

.. doxygenfunction:: lib_dsp_matrix_muls

Matrix Math Functions: Matrix Addition
--------------------------------------

.. doxygenfunction:: lib_dsp_matrix_addm

Matrix Math Functions: Matrix Subtraction
-----------------------------------------

.. doxygenfunction:: lib_dsp_matrix_subm

Matrix Math Functions: Matrix Multiplication
--------------------------------------------

.. doxygenfunction:: lib_dsp_matrix_mulm
  
Statistics Functions: Vector Mean
---------------------------------

.. doxygenfunction:: lib_dsp_vector_mean

Statistics Functions: Vector Power (Sum-of-Squares)
---------------------------------------------------

.. doxygenfunction:: lib_dsp_vector_power

Statistics Functions: Root Mean Square (RMS)
--------------------------------------------

.. doxygenfunction:: lib_dsp_vector_rms

Statistics Functions: Dot Product
---------------------------------

.. doxygenfunction:: lib_dsp_vector_dotprod


Filter Design Functions: Notch Filter
-------------------------------------

.. doxygenfunction:: lib_dsp_design_biquad_notch

Filter Design Functions: Low-pass Filter
----------------------------------------

.. doxygenfunction:: lib_dsp_design_biquad_lowpass

Filter Design Functions: High-pass Filter
-----------------------------------------

.. doxygenfunction:: lib_dsp_design_biquad_highpass

Filter Design Functions: All-pass Filter
----------------------------------------

.. doxygenfunction:: lib_dsp_design_biquad_allpass

Filter Design Functions: Band-pass Filter
-----------------------------------------

.. doxygenfunction:: lib_dsp_design_biquad_bandpass

Filter Design Functions: Peaking Filter
---------------------------------------

.. doxygenfunction:: lib_dsp_design_biquad_peaking

Filter Design Functions: Base Shelving Filter
---------------------------------------------

.. doxygenfunction:: lib_dsp_design_biquad_lowshelf

Filter Design Functions: Treble Shelving Filter
-----------------------------------------------

.. doxygenfunction:: lib_dsp_design_biquad_highshelf

FFT functions
-------------

.. doxygenfunction:: lib_dsp_fft_split_spectrum
.. doxygenfunction:: lib_dsp_fft_merge_spectra
.. doxygenfunction:: lib_dsp_fft_short_to_long
.. doxygenfunction:: lib_dsp_fft_long_to_short
.. doxygenfunction:: lib_dsp_fft_bit_reverse
.. doxygenfunction:: lib_dsp_fft_forward
.. doxygenfunction:: lib_dsp_fft_inverse


|appendix|

Known Issues
------------

There are no known issues.

.. include:: ../../../CHANGELOG.rst
