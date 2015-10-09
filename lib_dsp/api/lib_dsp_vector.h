// Copyright (c) 2015, XMOS Ltd, All rights reserved

#ifndef LIB_DSP_VECTOR
#define LIB_DSP_VECTOR

/** Vector Minimum
 * 
 *  Locate the vector's first occurring minimum value, returning the index of
 *  the first occurring minimum value.
 * 
 *  Example:
 * 
 *  \code 
 *  int samples[256];
 *  int result = lib_dsp_vector_minimum( samples, 256 );
 *  \endcode 
 * 
 *  \param  input_vector   Pointer to source data array.
 *  \param  vector_length  Length of the output and input arrays.
 *  \returns               Array index where first minimum value occurs.
 */

int lib_dsp_vector_minimum
(
    const int input_vector[],
    int       vector_length
);

/** Vector Minimum
 * 
 *  Locate the vector's first occurring maximum value, returning the index of
 *  the first occurring maximum value.
 * 
 *  Example:
 * 
 *  \code 
 *  int samples[256];
 *  int result = lib_dsp_vector_maximum( samples, 256 );
 *  \endcode 
 * 
 *  \param  input_vector   Pointer to source data array.
 *  \param  vector_length  Length of the output and input arrays.
 *  \returns               Array index where first maximum value occurs.
 */

int lib_dsp_vector_maximum
(
    const int input_vector[],
    int       vector_length
);

/** Vector negation: ``R[i] = -X[i]``
 *
 *  This function computes the negative value for each input element and sets
 *  the corresponding result element to its negative value.
 *
 *  Each negated element is computed by twos-compliment negation therefore the
 *  minimum negative fixed-point value can not be negated to generate its
 *  corresponding maximum positive fixed-point value.  For example: -Q28(-8.0)
 *  will not result in a fixed-point value representing +8.0.
 * 
 *  Example:
 *
 *  \code 
 *  int samples[256];
 *  int result[256];
 *  lib_dsp_vector_negate( samples, result, 256 );
 *  \endcode 
 * 
 *  \param  input_vector_X   Pointer/reference to source data.
 *  \param  result_vector_R  Pointer to the resulting data array.
 *  \param  vector_length    Length of the input and output vectors.
 */

void lib_dsp_vector_negate
(
    const int input_vector_X[],
    int       result_vector_R[],
    int       vector_length
);

/** Vector absolute value: ``R[i] = |X[i]|``
 * 
 *  Set each element of the result vector to the absolute value of the
 *  corresponding input vector element.
 *
 *  Example:
 * 
 *  \code 
 *  int samples[256];
 *  int result[256];
 *  lib_dsp_vector_abs( samples, result, 256 );
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

void lib_dsp_vector_abs
(
    const int input_vector_X[],
    int       result_vector_R[],
    int       vector_length
);

/** Vector / scalar addition: ``R[i] = X[i] + A``
 * 
 *  This function adds a scalar value to each vector element.
 *
 *  32-bit addition is used to compute the scaler plus vector element result.
 *  Therefore fixed-point value overflow conditions should be observed.  The
 *  resulting values are not saturated.
 * 
 *  Example:
 * 
 *  \code 
 *  int input_vector_X[256];
 *  int input_scalar_A = Q28( 0.333 );  
 *  int result_vector_R[256];
 *  lib_dsp_vector_adds( input_vector_X, scalar_value_A, result_vector_R, 256 );
 *  \endcode 
 * 
 *  \param  input_vector_X   Pointer/reference to source data array X
 *  \param  input_scalar_A   Scalar value to add to each input element
 *  \param  result_vector_R  Pointer to the resulting data array
 *  \param  vector_length    Length of the input and output vectors
 */

void lib_dsp_vector_adds
(
    const int input_vector_X[],
    int       input_scalar_A,
    int       result_vector_R[],
    int       vector_length
);

/** Vector / scalar multiplication: ``R[i] = X[i] * A``
 *
 *  32-bit addition is used to compute the scaler plus vector element result.
 *  Therefore fixed-point value overflow conditions should be observed. The
 *  resulting values are not saturated.
 * 
 *  Example:
 *
 *  \code 
 *  int input_vector_X[256];
 *  int input_scalar_A = Q28( 0.333 );  
 *  int result_vector_R[256];
 *  lib_dsp_vector_adds( input_vector_X, scalar_value_A, result_vector_R, 256 );
 *  \endcode 
 * 
 *  \param  input_vector_X    Pointer/reference to source data array X.
 *  \param  input_scalar_A    Scalar value to multiply each element by.
 *  \param  result_vector_R   Pointer to the resulting data array.
 *  \param  vector_length     Length of the input and output vectors.
 *  \param  q_format          Fixed point format, the number of bits making up fractional part.
 */

void lib_dsp_vector_muls
(
    const int input_vector_X[],
    int       input_scalar_A,
    int       result_vector_R[],
    int       vector_length,
    int       q_format
);

 
/** Vector / vector addition: ``R[i] = X[i] + Y[i]``
 * 
 *  32-bit addition is used to compute the scaler plus vector element result.
 *  Therefore fixed-point value overflow conditions should be observed. The
 *  resulting values are not saturated.
 *
 *  Example:
 *
 *  \code 
 *  int input_vector_X[256];
 *  int input_vector_Y[256];
 *  int result_vector_R[256];  
 *  lib_dsp_vector_addv( input_vector_X, input_vector_Y, result_vector_R, 256 );
 *  \endcode 
 * 
 *  \param  input_vector_X    Pointer to source data array X.
 *  \param  input_vector_Y    Pointer to source data array Y.
 *  \param  result_vector_R   Pointer to the resulting data array.
 *  \param  vector_length     Length of the input and output vectors.
 */

void lib_dsp_vector_addv
(
    const int input_vector_X[],
    const int input_vector_Y[],
    int       result_vector_R[],
    int       vector_length
);

/** Vector / vector subtraction: ``R[i] = X[i] - Y[i]``
 * 
 *  32-bit subtraction is used to compute the scaler plus vector element result.
 *  Therefore fixed-point value overflow conditions should be observed. The
 *  resulting values are not saturated.
 * 
 *  Example:
 *
 *  \code 
 *  int input_vector_X[256];
 *  int input_vector_Y[256];
 *  int result_vector_R[256];  
 *  lib_dsp_vector_subv( input_vector_X, input_vector_Y, result_vector_R, 256 );
 *  \endcode 
 * 
 *  \param  input_vector_X    Pointer to source data array X.
 *  \param  input_vector_Y    Pointer to source data array Y.
 *  \param  result_vector_R   Pointer to the resulting data array.
 *  \param  vector_length     Length of the input and output vectors.
 */

void lib_dsp_vector_subv
(
    const int input_vector_X[],
    const int input_vector_Y[],
    int       result_vector_R[],
    int       vector_length
);

/** Vector / vector multiplication: ``R[i] = X[i] * Y[i]``
 * 
 *  Elements in each of the input vectors are multiplied together using a
 *  32-bit multiply 64-bit accumulate function therefore fixed-point
 *  multiplication and q-format adjustment overflow behavior must be
 *  considered (see behavior for the function ``lib_dsp_math_multiply``).
 * 
 *  Example:
 *
 *  \code 
 *  int input_vector_X[256];
 *  int input_vector_Y[256];
 *  int result_vector_R[256];  
 *  lib_dsp_vector_mulv( input_vector_X, input_vector_Y, result_vector_R, 256, 28 );
 *  \endcode 
 * 
 *  \param  input_vector_X    Pointer to source data array X.
 *  \param  input_vector_Y    Pointer to source data array Y.
 *  \param  result_vector_R   Pointer to the resulting data array.
 *  \param  vector_length     Length of the input and output vectors.
 *  \param  q_format          Fixed point format (i.e. number of fractional bits).
 */

void lib_dsp_vector_mulv
(
    const int input_vector_X[],
    const int input_vector_Y[],
    int       result_vector_R[],
    int       vector_length,
    int       q_format
);

/** Vector multiplication and scalar addition: ``R[i] = X[i] * Y[i] + A``
 * 
 *  Elements in each of the input vectors are multiplied together using a
 *  32-bit multiply 64-bit accumulate function therefore fixed-point
 *  multiplication and q-format adjustment overflow behavior must be
 *  considered (see behavior for the function ``lib_dsp_math_multiply``).
 *  32-bit addition is used to compute the vector element plus scalar value
 *  result. Therefore fixed-point value overflow conditions should be observed. 
 *  The resulting values are not saturated.
 * 
 *  Example:
 *
 *  \code 
 *  int input_vector_X[256];
 *  int input_vector_Y[256];
 *  int input_scalar_A = Q28( 0.333 );
 *  int result_vector_R[256];  
 *  lib_dsp_vector_mulv_adds( input_vector_X, input_vector_Y, scalar_value_A, result_vector_R, 256, 28 );
 *  \endcode 
 * 
 *  \param  input_vector_X    Pointer to source data array X.
 *  \param  input_vector_Y    Pointer to source data array Y.
 *  \param  input_scalar_A    Scalar value to add to each X*Y result.
 *  \param  result_vector_R   Pointer to the resulting data array.
 *  \param  vector_length     Length of the input and output vectors.
 *  \param  q_format          Fixed point format (i.e. number of fractional bits).
 */

void lib_dsp_vector_mulv_adds
(
    const int input_vector_X[],
    const int input_vector_Y[],
    int       input_scalar_A,
    int       result_vector_R[],
    int       vector_length,
    int       q_format
);

/** Scalar multiplication and vector addition: ``R[i] = X[i] * A + Y[i]``
 * 
 *  Each element in the input vectors is multiplied by a scalar using a
 *  32bit multiply 64-bit accumulate function therefore fixed-point
 *  multiplication and q-format adjustment overflow behavior must be considered
 *  (see behavior for the function ``lib_dsp_math_multiply``). 32-bit addition is
 *  used to compute the vector element minus vector element result. Therefore
 *  fixed-point value overflow conditions should be observed.  The resulting
 *  values are not saturated.
 * 
 *  Example:
 *
 *  \code 
 *  int input_vector_X[256];
 *  int input_scalar_A = Q28( 0.333 );
 *  int input_vector_Y[256];
 *  int result_vector_R[256];
 *  lib_dsp_vector_muls_addv( input_vector_X, input_scalar_A, input_vector_Y, result_vector_R, 256, 28 );
 *  \endcode 
 * 
 *  \param  input_vector_X    Pointer to source data array X.
 *  \param  input_scalar_A    Scalar value to multiply each element by.
 *  \param  input_vector_Y    Pointer to source data array Y
 *  \param  result_vector_R   Pointer to the resulting data array.
 *  \param  vector_length     Length of the input and output vectors
 *  \param  q_format          Fixed point format (i.e. number of fractional bits).
 */

void lib_dsp_vector_muls_addv
(
    const int input_vector_X[],
    int       input_scalar_A,
    const int input_vector_Y[],
    int       result_vector_R[],
    int       vector_length,
    int       q_format
);

/** Scalar multiplication and vector subtraction: ``R[i] = X[i] * A - Y[i]``
 * 
 *  Each element in the input vectors is multiplied by a scalar using a
 *  32bit multiply 64-bit accumulate function therefore fixed-point
 *  multiplication and q-format adjustment overflow behavior must be considered
 *  (see behavior for the function ``lib_dsp_math_multiply``). 32-bit subtraction
 *  is used to compute the vector element minus vector element result. Therefore
 *  fixed-point value overflow conditions should be observed.  The resulting
 *  values are not saturated.
 * 
 *  Example:
 *
 *  \code 
 *  int input_vector_X[256];
 *  int input_scalar_A = Q28( 0.333 );
 *  int input_vector_Y[256];
 *  int result_vector_R[256];
 *  lib_dsp_vector_muls_subv( input_vector_X, input_scalar_A, input_vector_Y, result_vector_R, 256, 28 );
 *  \endcode 
 * 
 *  \param  input_scalar_A    Scalar value to multiply each element by.
 *  \param  input_vector_X    Pointer to source data array X.
 *  \param  input_vector_Y    Pointer to source data array Y.
 *  \param  result_vector_R   Pointer to the resulting data array.
 *  \param  vector_length     Length of the input and output vectors.
 *  \param  q_format          Fixed point format (i.e. number of fractional bits).
 */

void lib_dsp_vector_muls_subv
(
    const int input_vector_X[],
    int       input_scalar_A,
    const int input_vector_Y[],
    int       result_vector_R[],
    int       vector_length,
    int       q_format
);

/** Vector multiplication and vector addition: ``R[i] = X[i] * Y[i] + Z[i]``
 * 
 *  The elements in the input vectors are multiplied before being summed
 *  therefore fixed-point multiplication behavior must be considered (see
 *  behavior for the function ``lib_dsp_math_multiply``). Due to successive
 *  32-bit additions being accumulated using 64-bit arithmetic overflow during
 *  the summation process is unlikely. The final value, being effectively the
 *  result of a left-shift by ``q_format`` bits will potentially overflow the
 *  final fixed-point value depending on the resulting summed value and the
 *  chosen Q-format.
 * 
 *  Example:
 *
 *  \code 
 *  int input_vector_X[256];
 *  int input_vector_Y[256];
 *  int input_vector_Z[256];
 *  int result_vector_R[256];
 *  lib_dsp_vector_mulv_subv( input_vector_X, input_vector_Y, input_vector_Z, result_vector_R, 256 );
 *  \endcode 
 * 
 *  \param  input_vector_X    Pointer to source data array X.
 *  \param  input_vector_Y    Pointer to source data array Y.
 *  \param  input_vector_Z    Pointer to source data array Z.
 *  \param  result_vector_R   Pointer to the resulting data array.
 *  \param  vector_length     Length of the input and output vectors.
 *  \param  q_format          Fixed point format (i.e. number of fractional bits).
 */

void lib_dsp_vector_mulv_addv
(
    const int input_vector_X[],
    const int input_vector_Y[],
    const int input_vector_Z[],
    int       result_vector_R[],
    int       vector_length,
    int       q_format
);

/** Vector multiplication and vector addition: ``R[i] = X[i] * Y[i] - Z[i]``
 * 
 *  The elements in the input vectors are multiplied before being subtracted
 *  therefore fixed-point multiplication behavior must be considered (see
 *  behavior for the function ``lib_dsp_math_multiply``). Due to successive
 *  32-bit subtractions being accumulated using 64-bit arithmetic overflow during
 *  the summation process is unlikely. The final value, being effectively the
 *  result of a left-shift by ``q_format`` bits will potentially overflow the
 *  final fixed-point value depending on the resulting summed value and the
 *  chosen Q-format.
 * 
 *  Example:
 *
 *  \code 
 *  int input_vector_X[256];
 *  int input_vector_Y[256];
 *  int input_vector_Z[256];
 *  int result_vector_R[256];
 *  lib_dsp_vector_mulv_subv( input_vector_X, input_vector_Y, input_vector_Z, result_vector_R, 256 );
 *  \endcode 
 * 
 *  \param  input_vector_X    Pointer to source data array X.
 *  \param  input_vector_Y    Pointer to source data array Y.
 *  \param  input_vector_Z    Pointer to source data array Z.
 *  \param  result_vector_R   Pointer to the resulting data array.
 *  \param  vector_length     Length of the input and output vectors.
 *  \param  q_format          Fixed point format (i.e. number of fractional bits).
 */

void lib_dsp_vector_mulv_subv
(
    const int input_vector_X[],
    const int input_vector_Y[],
    const int input_vector_Z[],
    int       result_vector_R[],
    int       vector_length,
    int       q_format
);

#endif
