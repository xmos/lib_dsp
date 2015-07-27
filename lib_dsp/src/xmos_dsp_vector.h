// ================================================================================================

#ifndef XMOS_DSP_VECTOR
#define XMOS_DSP_VECTOR

// ================================================================================================

// Vector Minimum
//
// Locate the vector's first occurring minimum value, returning the index of the first occuring
// minimum value.
//
// Parameters:
//
// 'input_vector':  Pointer to source data array.
// 'vector_length': Length of the output and input arrays.
//
// Return value:    Array index where first minimum value occurs.
//
// Algorithm:
//
//  index = -1, value = maximum 32 bit signed integer
//  for i = 0 to (vector_length - 1):
//    if input_vector_X[i] < result:
//      value = input_vector_X[i]
//      index = i
//  return index
//
// Example:
//
//  int samples[256];
//  int result = xmos_dsp_vector_minimum( samples, 256 );

int xmos_dsp_vector_minimum
(
    const int input_vector[],
    int       vector_length
);

// ================================================================================================

// Vector Minimum
//
// Locate the vector's first occurring maximum value, returning the index of the first occuring
// maximum value.
//
// Parameters:
//
// 'input_vector':  Pointer to source data array.
// 'vector_length': Length of the output and input arrays.
//
// Return value:    Array index where first maximum value occurs.
//
// Algorithm:
//
//  index = -1, value = minimum 32 bit signed integer
//  for i = 0 to (vector_length - 1):
//    if input_vector_X[i] > result:
//      value = input_vector_X[i]
//      index = i
//  return index
//
// Example:
//
//  int samples[256];
//  int result = xmos_dsp_vector_maximum( samples, 256 );

int xmos_dsp_vector_maximum
(
    const int input_vector[],
    int       vector_length
);

// ================================================================================================

// Vector negation: R[i] = -X[i]
//
// This function computes the negative value for each input element and sets the corresponding
// result element to it's negative value.
//
// Parameters:
//
// 'input_vector_X':  Pointer/reference to source data.
// 'result_vector_R': Pointer to the resulting data array.
// 'vector_length':   Length of the input and output vectors.
//
// Algorithm:
//
//  result = 0
//  for i = 0 to (vector_length - 1): input_vector_X[i] = -input_vector_X[i]
//
// Behavior:
//
// Each negated element is computed by twos-compliment negation therefore the minimum negative
// fixed-point value can not be negated to generate it's corresponding maximum positive fixed-point
// value.
//
// For example: -Q28(-8.0) will not result in a fixed-point value representing +8.0.
//
// Example:
//
//  int samples[256];
//  int result[256];
//  xmos_dsp_vector_negate( samples, result, 256 );

void xmos_dsp_vector_negate
(
    const int input_vector_X[],
    int       result_vector_R[],
    int       vector_length
);

// ================================================================================================

// Vector absolute value: R[i] = |X[i]|
//
// Set each element of the result vector to the absolute value of the corresponding input vector
// element.
//
// Parameters:
//
// 'input_vector_X':  Pointer/reference to source data.
// 'result_vector_R': Pointer to the resulting data array.
// 'vector_length':   Length of the input and output vectors.
//
// Algorithm:
//
//  for i = 0 to (vector_length - 1): result_vector_R[i] = abs( input_vector_X[i] )
//
// Behavior:
//
// If an element is less than zero it is negated to compute it's absolute value.  Negation is
// computed via twos-compliment negation therefore the minimum negative fixed-point value can not be
// negated to generate it's corresponding maximum positive fixed-point value.
//
// For example: -Q28(-8.0) will not result in a fixed-point value representing +8.0.
//
// Example:
//
//  int samples[256];
//  int result[256];
//  xmos_dsp_vector_abs( samples, result, 256 );

void xmos_dsp_vector_abs
(
    const int input_vector_X[],
    int       result_vector_R[],
    int       vector_length
);

// ================================================================================================

// Vector / scalar addition: R[i] = X[i] + A
//
// This function adds a scalar value to each vector element.
//
// Parameters:
//
// 'input_vector_X':  Pointer/reference to source data array X
// 'scalar_value_A':  Scalar value to add to each 'input' element
// 'result_vector_R': Pointer to the resulting data array
// 'vector_length':   Length of the input and output vectors
//
// Algorithm:
//
//  for i = 0 to (vector_length - 1):
//    result_vector_R[i] = input_vector_X[i] + scalar_value_A
//
// Behavior:
//
// 32-bit addition is used to compute the scaler plus vector element result. Therefore fixed-point
// value overflow conditions should be observed.  The resulting values are not saturated.
//
// Example:
//
//  int input_vector_X[256];
//  int scalar_value_A = Q28( 0.333 );  
//  int result_vector_R[256];
//  xmos_dsp_vector_adds( input_vector_X, scalar_value_A, result_vector_R, 256 );

void xmos_dsp_vector_adds
(
    const int input_vector_X[],
    int       scalar_value_A,
    int       result_vector_R[],
    int       vector_length
);

// ================================================================================================

// Vector / scalar multiplication: R[i] = X[i] * A
//
// Parameters:
//
// 'input_vector_X':  Pointer/reference to source data array X.
// 'scalar_value_A':  Scalar value to multiply each 'input' element by.
// 'result_vector_R': Pointer to the resulting data array.
// 'vector_length':   Length of the input and output vectors.
// 'q_format':        Fixed point format, the number of bits making up fractional part.
//
// Algorithm:
//
//  for i = 0 to (vector_length - 1):
//    result_vector_R[i] = input_vector_X[i] * scalar_value_A
//
// Behavior:
//
// 32-bit addition is used to compute the scaler plus vector element result. Therefore fixed-point
// value overflow conditions should be observed.  The resulting values are not saturated.
//
// Example:
//
//  int input_vector_X[256];
//  int scalar_value_A = Q28( 0.333 );  
//  int result_vector_R[256];
//  xmos_dsp_vector_adds( input_vector_X, scalar_value_A, result_vector_R, 256 );

void xmos_dsp_vector_muls
(
    const int input_vector_X[],
    int       scalar_value_A,
    int       result_vector_R[],
    int       vector_length,
    int       q_format
);

// ================================================================================================

// Vector / vector addition: R[i] = X[i] + Y[i]
//
// Parameters:
//
// 'input_vector_X':  Pointer to source data array X.
// 'input_vector_Y':  Pointer to source data array Y.
// 'result_vector_R': Pointer to the resulting data array.
// 'vector_length':   Length of the input and output vectors.
//
// Algorithm:
//
//  for i = 0 to (vector_length - 1):
//    result_vector_R[i] = input_vector_X[i] + input_vector_Y[i]
//
// Behavior:
//
// 32-bit addition is used to compute the scaler plus vector element result. Therefore fixed-point
// value overflow conditions should be observed.  The resulting values are not saturated.
//
// Example:
//
//  int input_vector_X[256];
//  int input_vector_Y[256];
//  int result_vector_R[256];  
//  xmos_dsp_vector_addv( input_vector_X, input_vector_Y, result_vector_R, 256 );

void xmos_dsp_vector_addv
(
    const int input_vector_X[],
    const int input_vector_Y[],
    int       result_vector_R[],
    int       vector_length
);

// ================================================================================================

// Vector / vector subtraction: R[i] = X[i] - Y[i]
//
// Parameters:
//
// 'input_vector_X':  Pointer to source data array X.
// 'input_vector_Y':  Pointer to source data array Y.
// 'result_vector_R': Pointer to the resulting data array.
// 'vector_length':   Length of the input and output vectors.
//
// Algorithm:
//
//  for i = 0 to (vector_length - 1):
//    result_vector_R[i] = input_vector_X[i] - input_vector_Y[i]
//
// Behavior:
//
// 32-bit addition is used to compute the scaler plus vector element result. Therefore fixed-point
// value overflow conditions should be observed.  The resulting values are not saturated.
//
// Example:
//
//  int input_vector_X[256];
//  int input_vector_Y[256];
//  int result_vector_R[256];  
//  xmos_dsp_vector_subv( input_vector_X, input_vector_Y, result_vector_R, 256 );

void xmos_dsp_vector_subv
(
    const int input_vector_X[],
    const int input_vector_Y[],
    int       result_vector_R[],
    int       vector_length
);

// ================================================================================================

// Vector / vector multiplication: R[i] = X[i] * Y[i]
//
// Parameters:
//
// 'input_vector_X':  Pointer to source data array X.
// 'input_vector_Y':  Pointer to source data array Y.
// 'result_vector_R': Pointer to the resulting data array.
// 'vector_length':   Length of the input and output vectors.
// 'q_format':        Fixed point format, the number of bits making up fractional part.
//
// Algorithm:
//
//  for i = 0 to (vector_length - 1):
//    result_vector_R[i] = input_vector_X[i] * input_vector_Y[i]
//
// Behavior:
//
// Elements in each of the input vectors are multiplied together using a 32bit multiply 64-bit
// accumulate function therefore fixed-point multiplication and q-format adjustment overflow
// behavior must be considered (see behavior for the function 'xmos_dsp_math_multiply').
//
// Example:
//
//  int input_vector_X[256];
//  int input_vector_Y[256];
//  int result_vector_R[256];  
//  xmos_dsp_vector_mulv( input_vector_X, input_vector_Y, result_vector_R, 256, 28 );

void xmos_dsp_vector_mulv
(
    const int input_vector_X[],
    const int input_vector_Y[],
    int       result_vector_R[],
    int       vector_length,
    int       q_format
);

// ================================================================================================

// Vector multiplication and scalar addition: R[i] = X[i] * Y[i] + A
//
// Parameters:
//
// 'input_vector_X':  Pointer to source data array X.
// 'input_vector_Y':  Pointer to source data array Y.
// 'scalar_value_A':  Scalar value to add to each X*Y result.
// 'result_vector_R': Pointer to the resulting data array.
// 'vector_length':   Length of the input and output vectors.
// 'q_format':        Fixed point format, the number of bits making up fractional part.
//
// Algorithm:
//
//  for i = 0 to (vector_length - 1):
//    result_vector_R[i] = input_vector_X[i] * input_vector_Y[i] + input_scalar_A
//
// Behavior:
//
// Elements in each of the input vectors are multiplied together using a 32bit multiply 64-bit
// accumulate function therefore fixed-point multiplication and q-format adjustment overflow
// behavior must be considered (see behavior for the function 'xmos_dsp_math_multiply').
// 
// 32-bit addition is used to compute the vector element plus scalar value result. Therefore
// fixed-point value overflow conditions should be observed.  The resulting values are not saturated.
//
// Example:
//
//  int input_vector_X[256];
//  int input_vector_Y[256];
//  int scalar_value_A = Q28( 0.333 );
//  int result_vector_R[256];  
//  xmos_dsp_vector_mulv_adds( input_vector_X, input_vector_Y, scalar_value_A, result_vector_R, 256, 28 );

void xmos_dsp_vector_mulv_adds
(
    const int input_vector_X[],
    const int input_vector_Y[],
    int       input_scalar_A,
    int       result_vector_R[],
    int       vector_length,
    int       q_format
);

// ================================================================================================

// Scalar multiplication and vector addition: R[i] = X[i] * A + Y[i]
//
// Parameters:
//
// 'input_vector_X':  Pointer to source data array X.
// 'input_scalar_A':  Scalar value to multiply each 'input' element by.
// 'input_vector_Y':  Pointer to source data array Y
// 'result_vector_R': Pointer to the resulting data array.
// 'vector_length':   Length of the input and output vectors
// 'q_format':        Fixed point format, the number of bits making up fractional part
//
// Algorithm:
//
//  for i = 0 to (vector_length - 1):
//    result_vector_R[i] = input_scalar_A * input_vector_X[i] + input_vector_Y[i]
//
// Behavior:
//
// Each element in the input vectors is multiplied by a scalar using a 32bit multiply 64-bit
// accumulate function therefore fixed-point multiplication and q-format adjustment overflow
// behavior must be considered (see behavior for the function 'xmos_dsp_math_multiply').
//
// 32-bit addition is used to compute the vector element minus vector element result. Therefore
// fixed-point value overflow conditions should be observed.  The resulting values are not saturated.
//
// Example:
//
//  int input_vector_X[256];
//  int scalar_value_A = Q28( 0.333 );
//  int input_vector_Y[256];
//  int result_vector_R[256];
//  xmos_dsp_vector_muls_addv( input_vector_X, input_scalar_A, input_vector_Y, result_vector_R, 256, 28 );

void xmos_dsp_vector_muls_addv
(
    const int input_vector_X[],
    int       input_scalar_A,
    const int input_vector_Y[],
    int       result_vector_R[],
    int       vector_length,
    int       q_format
);

// ================================================================================================

// Scalar multiplication and vector subtraction: R[i] = X[i] * A - Y[i]
//
// Parameters:
//
// 'input_scalar_A':  Scalar value to multiply each 'input' element by.
// 'input_vector_X':  Pointer to source data array X.
// 'input_vector_Y':  Pointer to source data array Y.
// 'result_vector_R': Pointer to the resulting data array.
// 'vector_length':   Length of the input and output vectors.
// 'q_format':        Fixed point format, the number of bits making up fractional part.
//
// Algorithm:
//
//  for i = 0 to (vector_length - 1):
//    result_vector_R[i] = input_scalar_A * input_vector_X[i] - input_vector_Y[i]
//
// Behavior:
//
// Each element in the input vectors is multiplied by a scalar using a 32bit multiply 64-bit
// accumulate function therefore fixed-point multiplication and q-format adjustment overflow
// behavior must be considered (see behavior for the function 'xmos_dsp_math_multiply').
//
// 32-bit addition is used to compute the vector element minus vector element result. Therefore
// fixed-point value overflow conditions should be observed.  The resulting values are not saturated.
//
// Example:
//
//  int input_vector_X[256];
//  int scalar_value_A = Q28( 0.333 );
//  int input_vector_Y[256];
//  int result_vector_R[256];
//  xmos_dsp_vector_muls_subv( input_vector_X, input_scalar_A, input_vector_Y, result_vector_R, 256, 28 );

void xmos_dsp_vector_muls_subv
(
    const int input_vector_X[],
    int       input_scalar_A,
    const int input_vector_Y[],
    int       result_vector_R[],
    int       vector_length,
    int       q_format
);

// ================================================================================================

// Vector multiplication and vector addition: R[i] = X[i] * Y[i] + Z[i]
//
// Parameters:
//
// 'input_vector_X':  Pointer to source data array X.
// 'input_vector_Y':  Pointer to source data array Y.
// 'input_vector_Z':  Pointer to source data array Z.
// 'result_vector_R': Pointer to the resulting data array.
// 'vector_length':   Length of the input and output vectors.
// 'q_format':        Fixed point format, the number of bits making up fractional part.
//
// Algorithm:
//
//  for i = 0 to (vector_length - 1):
//    result_vector_R[i] = input_vector_X[i] * input_vector_Y[i] + input_vector_Z[i]
//
// Behavior:
//
// The elements in the input vectors are multiplied before being summed therefore fixed-point
// multiplication behavior must be considered (see behavior for the function
// 'xmos_dsp_math_multiply').
//
// Due to successive 32-bit additions being accumulated using 64-bit arithmetic overflow during the
// summation process is unlikely. The final value, being effectively the result of a left-shift by
// 'q_format' bits will potentially overflow the final fixed-point value depending on the resulting
// summed value and the chosen Q-format.
//
// Example:
//
//  int input_vector_X[256];
//  int input_vector_Y[256];
//  int input_vector_Z[256];
//  int result_vector_R[256];
//  xmos_dsp_vector_mulv_subv( input_vector_X, input_vector_Y, input_vector_Z, result_vector_R, 256 );

void xmos_dsp_vector_mulv_addv
(
    const int input_vector_X[],
    const int input_vector_Y[],
    const int input_vector_Z[],
    int       result_vector_R[],
    int       vector_length,
    int       q_format
);

// ================================================================================================

// Vector multiplication and vector addition: R[i] = X[i] * Y[i] - Z[i]
//
// Parameters:
//
// 'input_vector_X':  Pointer to source data array X.
// 'input_vector_Y':  Pointer to source data array Y.
// 'input_vector_Z':  Pointer to source data array Z.
// 'result_vector_R': Pointer to the resulting data array.
// 'vector_length':   Length of the input and output vectors.
// 'q_format':        Fixed point format, the number of bits making up fractional part.
//
// Algorithm:
//
//  for i = 0 to (vector_length - 1):
//    result_vector_R[i] = input_vector_X[i] * input_vector_Y[i] - input_vector_Z[i]
//
// Behavior:
//
// The elements in the input vectors are multiplied before being summed therefore fixed-point
// multiplication behavior must be considered (see behavior for the function
// 'xmos_dsp_math_multiply').
//
// Due to successive 32-bit additions being accumulated using 64-bit arithmetic overflow during the
// summation process is unlikely. The final value, being effectively the result of a left-shift by
// 'q_format' bits will potentially overflow the final fixed-point value depending on the resulting
// summed value and the chosen Q-format.
//
// Example:
//
//  int input_vector_X[256];
//  int input_vector_Y[256];
//  int input_vector_Z[256];
//  int result_vector_R[256];
//  xmos_dsp_vector_mulv_subv( input_vector_X, input_vector_Y, input_vector_Z, result_vector_R, 256 );

void xmos_dsp_vector_mulv_subv
(
    const int input_vector_X[],
    const int input_vector_Y[],
    const int input_vector_Z[],
    int       result_vector_R[],
    int       vector_length,
    int       q_format
);

// ================================================================================================

#endif
