// Copyright (c) 2015-2016, XMOS Ltd, All rights reserved

#ifndef DSP_VECTOR_H_
#define DSP_VECTOR_H_

#include <stdint.h>

/** Vector Minimum
 * 
 *  Locate the vector's first occurring minimum value, returning the index of
 *  the first occurring minimum value.
 * 
 *  Example:
 * 
 *  \code 
 *  int32_t samples[256];
 *  int32_t index = dsp_vector_minimum( samples, 256 );
 *  \endcode 
 * 
 *  \param  input_vector   Pointer to source data array.
 *  \param  vector_length  Length of the output and input arrays.
 *  \returns               Array index where first minimum value occurs.
 */

int32_t dsp_vector_minimum
(
    const int32_t input_vector[],
    const int32_t vector_length
);

/** Vector Minimum
 * 
 *  Locate the vector's first occurring maximum value, returning the index of
 *  the first occurring maximum value.
 * 
 *  Example:
 * 
 *  \code 
 *  int32_t samples[256];
 *  int32_t index = dsp_vector_maximum( samples, 256 );
 *  \endcode 
 * 
 *  \param  input_vector   Pointer to source data array.
 *  \param  vector_length  Length of the output and input arrays.
 *  \returns               Array index where first maximum value occurs.
 */

int32_t dsp_vector_maximum
(
    const int32_t input_vector[],
    const int32_t vector_length
);

/** Vector negation: ``R[i] = -X[i]``
 *
 *  This function sets each result element to the negative value of the
 *  corresponding input element.
 *
 *  Each negated element is computed by twos-compliment negation therefore the
 *  minimum negative fixed-point value can not be negated to generate its
 *  corresponding maximum positive fixed-point value.  For example: -Q28(-8.0)
 *  will not result in a fixed-point value representing +8.0.
 * 
 *  Example:
 *
 *  \code 
 *  int32_t samples[256] = { 0, 1, 2, 3, ... not shown for brevity };
 *  int32_t result[256];
 *  dsp_vector_negate( samples, result, 256 );
 *  \endcode 
 * 
 *  \param  input_vector_X   Pointer/reference to source data.
 *  \param  result_vector_R  Pointer to the resulting data array.
 *  \param  vector_length    Length of the input and output vectors.
 */

void dsp_vector_negate
(
    const int32_t input_vector_X[],
    int32_t       result_vector_R[],
    const int32_t vector_length
);

/** Vector absolute value: ``R[i] = |X[i]|``
 * 
 *  This function sets each element of the result vector to the absolute value of the
 *  corresponding input vector element.
 *
 *  Example:
 * 
 *  \code 
 *  int32_t samples[256] = { 0, 1, 2, 3, ... not shown for brevity };
 *  int32_t result[256];
 *  dsp_vector_abs( samples, result, 256 );
 *  \endcode 
 * 
 *  If an element is less than zero it is negated to compute its absolute
 *  value. Negation is computed via twos-compliment negation therefore the
 *  minimum negative fixed-point value can not be negated to generate its
 *  corresponding maximum positive fixed-point value. For example: -Q28(-8.0)
 *  will not result in a fixed-point value representing +8.0.
 * 
 *  \param  input_vector_X    Pointer/reference to source data.
 *  \param  result_vector_R   Pointer to the resulting data array.
 *  \param  vector_length     Length of the input and output vectors.
 */

void dsp_vector_abs
(
    const int32_t input_vector_X[],
    int32_t       result_vector_R[],
    const int32_t vector_length
);

/** Vector / scalar addition: ``R[i] = X[i] + A``
 * 
 *  This function adds a scalar value to each vector element.
 *
 *  32-bit addition is used to compute the scalar plus vector element result.
 *  Therefore fixed-point value overflow conditions should be observed.  The
 *  resulting values are not saturated.
 * 
 *  Example:
 * 
 *  \code 
 *  int32_t input_vector_X[256] = { 0, 1, 2, 3, ... not shown for brevity };
 *  int32_t input_scalar_A = Q28( 0.333 );  
 *  int32_t result_vector_R[256];
 *  dsp_vector_adds( input_vector_X, scalar_value_A, result_vector_R, 256 );
 *  \endcode 
 * 
 *  \param  input_vector_X   Pointer/reference to source data array X
 *  \param  input_scalar_A   Scalar value to add to each input element
 *  \param  result_vector_R  Pointer to the resulting data array
 *  \param  vector_length    Length of the input and output vectors
 */

void dsp_vector_adds
(
    const int32_t input_vector_X[],
    int32_t       input_scalar_A,
    int32_t       result_vector_R[],
    const int32_t vector_length
);

/** Vector / scalar multiplication: ``R[i] = X[i] * A``
 *
 *  The elements in vector X are multiplied with the scalar A and stored in result vector R.
 *  Each multiplication produces a 64-bit result. 
 *  If overflow occurs it is saturated at the minimum/maximum value given the fixed-point format
 *  and finally shifted right by ``q_format`` bits.
 * 
 *  Example:
 *
 *  \code 
 *  int32_t input_vector_X[256] = { 0, 1, 2, 3, ... not shown for brevity };
 *  int32_t input_scalar_A = Q28( 0.333 );  
 *  int32_t result_vector_R[256];
 *  dsp_vector_muls( input_vector_X, scalar_value_A, result_vector_R, 256 );
 *  \endcode 
 * 
 *  \param  input_vector_X    Pointer/reference to source data array X.
 *  \param  input_scalar_A    Scalar value to multiply each element by.
 *  \param  result_vector_R   Pointer to the resulting data array.
 *  \param  vector_length     Length of the input and output vectors.
 *  \param  q_format          Fixed point format, the number of bits making up fractional part.
 */

void dsp_vector_muls
(
    const int32_t input_vector_X[],
    int32_t       input_scalar_A,
    int32_t       result_vector_R[],
    const int32_t vector_length,
    const int32_t q_format
);

 
/** Vector / vector addition: ``R[i] = X[i] + Y[i]``
 * 
 *  32 bit addition is used to add Vector X to Vector Y. 
 *  The results are stored in result vector R.
 *  They are not saturated so overflow may occur.
 *
 *  Example:
 *
 *  \code 
 *  int32_t input_vector_X[256] = { 0, 1, 2, 3, ... not shown for brevity };
 *  int32_t input_vector_Y[256] = { 0, -1, -2, -3, ... not shown for brevity };
 *  int32_t result_vector_R[256];  
 *  dsp_vector_addv( input_vector_X, input_vector_Y, result_vector_R, 256 );
 *  \endcode 
 * 
 *  \param  input_vector_X    Pointer to source data array X.
 *  \param  input_vector_Y    Pointer to source data array Y.
 *  \param  result_vector_R   Pointer to the resulting data array.
 *  \param  vector_length     Length of the input and output vectors.
 */

void dsp_vector_addv
(
    const int32_t input_vector_X[],
    const int32_t input_vector_Y[],
    int32_t       result_vector_R[],
    const int32_t vector_length
);

/** Vector / vector subtraction: ``R[i] = X[i] - Y[i]``
 * 
 *  32 bit subtraction is used to subtract Vector Y from Vector X. 
 *  The results are stored in result vector R.
 *  They are not saturated so overflow may occur.
 * 
 *  Example:
 *
 *  \code 
 *  int32_t input_vector_X[256] = { 0, 1, 2, 3, ... not shown for brevity };
 *  int32_t input_vector_Y[256] = { 0, -1, -2, -3, ... not shown for brevity };
 *  int32_t result_vector_R[256];  
 *  dsp_vector_subv( input_vector_X, input_vector_Y, result_vector_R, 256 );
 *  \endcode 
 * 
 *  \param  input_vector_X    Pointer to source data array X.
 *  \param  input_vector_Y    Pointer to source data array Y.
 *  \param  result_vector_R   Pointer to the resulting data array.
 *  \param  vector_length     Length of the input and output vectors.
 */

void dsp_vector_subv
(
    const int32_t input_vector_X[],
    const int32_t input_vector_Y[],
    int32_t       result_vector_R[],
    const int32_t vector_length
);

/** Vector / vector multiplication: ``R[i] = X[i] * Y[i]``
 * 
 *  Each multiplication produces a 64-bit result. 
 *  If overflow occurs it is saturated at the minimum/maximum value given the fixed-point format
 *  and finally shifted right by ``q_format`` bits.
 * 
 *  Example:
 *
 *  \code 
 *  int32_t input_vector_X[256] = { 0, 1, 2, 3, ... not shown for brevity };
 *  int32_t input_vector_Y[256] = { 0, 1, 2, 3, ... not shown for brevity };
 *  int32_t result_vector_R[256];  
 *  dsp_vector_mulv( input_vector_X, input_vector_Y, result_vector_R, 256, 28 );
 *  \endcode 
 * 
 *  \param  input_vector_X    Pointer to source data array X.
 *  \param  input_vector_Y    Pointer to source data array Y.
 *  \param  result_vector_R   Pointer to the resulting data array.
 *  \param  vector_length     Length of the input and output vectors.
 *  \param  q_format          Fixed point format (i.e. number of fractional bits).
 */

void dsp_vector_mulv
(
    const int32_t input_vector_X[],
    const int32_t input_vector_Y[],
    int32_t       result_vector_R[],
    const int32_t vector_length,
    const int32_t q_format
);

/** Vector multiplication and scalar addition: ``R[i] = X[i] * Y[i] + A``
 * 
 *  Each multiplication produces a 64-bit result. 
 *  If overflow occurs it is saturated at the minimum/maximum value given the fixed-point format
 *  and finally shifted right by ``q_format`` bits.
 *  Then the scalar is added. Overflow may occur after the addition.
 * 
 *  Example:
 *
 *  \code 
 *  int32_t input_vector_X[256] = { 0, 1, 2, 3, ... not shown for brevity };
 *  int32_t input_vector_Y[256] = { 0, 1, 2, 3, ... not shown for brevity };
 *  int32_t input_scalar_A = Q28( 0.333 );
 *  int32_t result_vector_R[256];  
 *  dsp_vector_mulv_adds( input_vector_X, input_vector_Y, scalar_value_A, result_vector_R, 256, 28 );
 *  \endcode 
 * 
 *  \param  input_vector_X    Pointer to source data array X.
 *  \param  input_vector_Y    Pointer to source data array Y.
 *  \param  input_scalar_A    Scalar value to add to each X*Y result.
 *  \param  result_vector_R   Pointer to the resulting data array.
 *  \param  vector_length     Length of the input and output vectors.
 *  \param  q_format          Fixed point format (i.e. number of fractional bits).
 */

void dsp_vector_mulv_adds
(
    const int32_t input_vector_X[],
    const int32_t input_vector_Y[],
    int32_t       input_scalar_A,
    int32_t       result_vector_R[],
    const int32_t vector_length,
    const int32_t q_format
);

/** Scalar multiplication and vector addition: ``R[i] = X[i] * A + Y[i]``
 * 
 *  The elements in vector X are multiplied with the scalar A 
 *  and then added to the corresponding element in Y.
 * 
 *  Each multiplication produces a 64-bit result. 
 *  If overflow occurs it is saturated at the minimum/maximum value given the fixed-point format
 *  and finally shifted right by ``q_format`` bits.
 *  Overflow may occur after the addition Y[i].
 * 
 *  Example:
 *
 *  \code 
 *  int32_t input_vector_X[256] = { 0, 1, 2, 3, ... not shown for brevity };
 *  int32_t input_scalar_A = Q28( 0.333 );
 *  int32_t input_vector_Y[256] = { 0, 1, 2, 3, ... not shown for brevity };
 *  int32_t result_vector_R[256];
 *  dsp_vector_muls_addv( input_vector_X, input_scalar_A, input_vector_Y, result_vector_R, 256, 28 );
 *  \endcode 
 * 
 *  \param  input_vector_X    Pointer to source data array X.
 *  \param  input_scalar_A    Scalar value to multiply each element by.
 *  \param  input_vector_Y    Pointer to source data array Y
 *  \param  result_vector_R   Pointer to the resulting data array.
 *  \param  vector_length     Length of the input and output vectors
 *  \param  q_format          Fixed point format (i.e. number of fractional bits).
 */

void dsp_vector_muls_addv
(
    const int32_t input_vector_X[],
    int32_t       input_scalar_A,
    const int32_t input_vector_Y[],
    int32_t       result_vector_R[],
    const int32_t vector_length,
    const int32_t q_format
);

/** Scalar multiplication and vector subtraction: ``R[i] = X[i] * A - Y[i]``
 * 
 *  The elements in vector X are multiplied with the scalar A.
 *  The corresponding element in Y is subtracted from that.
 * 
 *  Each multiplication produces a 64-bit result. 
 *  If overflow occurs it is saturated at the minimum/maximum value given the fixed-point format
 *  and finally shifted right by ``q_format`` bits.
 *  Overflow may occur after the subtraction of Y[i].
 * 
 *  Example:
 *
 *  \code 
 *  int32_t input_vector_X[256];
 *  int32_t input_scalar_A = Q28( 0.333 );
 *  int32_t input_vector_Y[256];
 *  int32_t result_vector_R[256];
 *  dsp_vector_muls_subv( input_vector_X, input_scalar_A, input_vector_Y, result_vector_R, 256, 28 );
 *  \endcode 
 * 
 *  \param  input_scalar_A    Scalar value to multiply each element by.
 *  \param  input_vector_X    Pointer to source data array X.
 *  \param  input_vector_Y    Pointer to source data array Y.
 *  \param  result_vector_R   Pointer to the resulting data array.
 *  \param  vector_length     Length of the input and output vectors.
 *  \param  q_format          Fixed point format (i.e. number of fractional bits).
 */

void dsp_vector_muls_subv
(
    const int32_t input_vector_X[],
    int32_t       input_scalar_A,
    const int32_t input_vector_Y[],
    int32_t       result_vector_R[],
    const int32_t vector_length,
    const int32_t q_format
);

/** Vector multiplication and vector addition: ``R[i] = X[i] * Y[i] + Z[i]``
 * 
 *  Each multiplication produces a 64-bit result. 
 *  If overflow occurs it is saturated at the minimum/maximum value given the fixed-point format
 *  and finally shifted right by ``q_format`` bits.
 *  Overflow may occur after the addition of Z[i].
 * 
 *  Example:
 *
 *  \code 
 *  int32_t input_vector_X[256];
 *  int32_t input_vector_Y[256];
 *  int32_t input_vector_Z[256];
 *  int32_t result_vector_R[256];
 *  dsp_vector_mulv_addv( input_vector_X, input_vector_Y, input_vector_Z, result_vector_R, 256, 28 );
 *  \endcode 
 * 
 *  \param  input_vector_X    Pointer to source data array X.
 *  \param  input_vector_Y    Pointer to source data array Y.
 *  \param  input_vector_Z    Pointer to source data array Z.
 *  \param  result_vector_R   Pointer to the resulting data array.
 *  \param  vector_length     Length of the input and output vectors.
 *  \param  q_format          Fixed point format (i.e. number of fractional bits).
 */

void dsp_vector_mulv_addv
(
    const int32_t input_vector_X[],
    const int32_t input_vector_Y[],
    const int32_t input_vector_Z[],
    int32_t       result_vector_R[],
    const int32_t vector_length,
    const int32_t q_format
);

/** Vector multiplication and vector subtraction: ``R[i] = X[i] * Y[i] - Z[i]``
 * 
 *  Each multiplication produces a 64-bit result. 
 *  If overflow occurs it is saturated at the minimum/maximum value given the fixed-point format
 *  and finally shifted right by ``q_format`` bits.
 *  Overflow may occur after the subtraction of Z[i].
 * 
 *  Example:
 *
 *  \code 
 *  int32_t input_vector_X[256];
 *  int32_t input_vector_Y[256];
 *  int32_t input_vector_Z[256];
 *  int32_t result_vector_R[256];
 *  dsp_vector_mulv_subv( input_vector_X, input_vector_Y, input_vector_Z, result_vector_R, 256, 28 );
 *  \endcode 
 * 
 *  \param  input_vector_X    Pointer to source data array X.
 *  \param  input_vector_Y    Pointer to source data array Y.
 *  \param  input_vector_Z    Pointer to source data array Z.
 *  \param  result_vector_R   Pointer to the resulting data array.
 *  \param  vector_length     Length of the input and output vectors.
 *  \param  q_format          Fixed point format (i.e. number of fractional bits).
 */

void dsp_vector_mulv_subv
(
    const int32_t input_vector_X[],
    const int32_t input_vector_Y[],
    const int32_t input_vector_Z[],
    int32_t       result_vector_R[],
    const int32_t vector_length,
    const int32_t q_format
);

/** Complex vector / vector multiplication: ``R[i] = X[i] * Y[i]``
 * 
 *  Vectors X[i] and Y[i] are complex, with separate arrays for the real and imaginary components.
 * 
 *  Each multiplication produces a 64-bit result. 
 *  If overflow occurs it is saturated at the minimum/maximum value given the fixed-point format
 *  and finally shifted right by ``q_format`` bits.
 * 
 *  Example:
 *
 *  \code 
 *  int32_t input_vector_X_re[256] = { 0, 1, 2, 3, ... not shown for brevity };
 *  int32_t input_vector_X_im[256] = { 0, 1, 2, 3, ... not shown for brevity };
 *  int32_t input_vector_Y_re[256] = { 0, 1, 2, 3, ... not shown for brevity };
 *  int32_t input_vector_Y_im[256] = { 0, 1, 2, 3, ... not shown for brevity };
 *  int32_t result_vector_R_re[256];  
 *  int32_t result_vector_R_im[256];  
 *  dsp_vector_mulv_complex( input_vector_X_re, input_vector_X_im, input_vector_Y_re, input_vector_Y_im, result_vector_R_re, result_vector_R_im, 256, 28 );
 *  \endcode 
 * 
 *  \param  input_vector_X_re   Pointer to real source data array X.
 *  \param  input_vector_X_im   Pointer to imaginary source data array X.
 *  \param  input_vector_Y_re   Pointer to real source data array Y.
 *  \param  input_vector_Y_im   Pointer to imaginary source data array Y.
 *  \param  result_vector_R_re  Pointer to the resulting real data array.
 *  \param  result_vector_R_im  Pointer to the resulting imaginary data array.
 *  \param  vector_length       Length of the input and output vectors.
 *  \param  q_format            Fixed point format (i.e. number of fractional bits).
 */

void dsp_vector_mulv_complex (const int32_t input_vector_X_re[],
    const int32_t input_vector_X_im[],
    const int32_t input_vector_Y_re[],
    const int32_t input_vector_Y_im[],
    int32_t       result_vector_R_re[],
    int32_t       result_vector_R_im[],
    const int32_t vector_length,
    const int32_t q_format
);

#endif
