// Copyright (c) 2015, XMOS Ltd, All rights reserved
// ================================================================================================
#ifndef XMOS_DSP_MATRIX
#define XMOS_DSP_MATRIX

// ================================================================================================

// Matrix negation: R = -X
//
// Parameters:
//
// 'input_matrix_X':  Pointer/reference to source data.
// 'result_matrix_R': Pointer to the resulting 2-dimensional data array.
// 'row_count':       Number of rows in input matrix.
// 'column_count':    Number of columns in input matrix.
//
// Algorithm:
//
//  result = 0
//  for i = 0 to (row_count - 1):
//    for j = 0 to (column_count - 1):
//      k = i * column_count + j
//      input_vector_X[k] = -input_vector_X[k]
//
// Behavior:
//
// Each negated element is computed by two's-compliment negation therefore the minimum negative
// fixed-point value can not be negated to generate it's corresponding maximum positive fixed-point
// value.
//
// For example: -Q28(-8.0) will not result in a fixed-point value representing +8.0.
//
// Example:
//
//  int samples[8][32];
//  int result[8][32];  
//  xmos_dsp_matrix_negate( samples, result, 8, 32 );

void xmos_dsp_matrix_negate
(
    const int input_matrix_X[],
    int       result_matrix_R[],
    int       row_count,
    int       column_count
);

// ================================================================================================

// Matrix / scalar addition: R = X + a
//
// Parameters:
//
// 'input_matrix_X':  Pointer/reference to source data.
// 'scalar_value_A':  Scalar value to add to each 'input' element.
// 'result_matrix_R': Pointer to the resulting 2-dimensional data array.
// 'row_count':       Number of rows in input and output matrices.
// 'column_count':    Number of columns in input and output matrices.
//
// Algorithm:
//
//  result = 0
//  for i = 0 to (row_count - 1):
//    for j = 0 to (column_count - 1):
//      k = i * column_count + j
//      result_matrix_R[k] = input_matrix_X[k] * scalar_value_A
//
// Behavior:
//
// 32-bit addition is used to compute the scaler plus matrix element result. Therefore fixed-point
// value overflow conditions should be observed.  The resulting values are not saturated.
//
// Example:
//
//  int input_matrix_X[8][32];
//  int scalar_value_A = Q28( 0.333 );
//  int result_vector_R[8][32];  
//  xmos_dsp_matrix_adds( input_matrix_X, scalar_matrix_A, result_matrix_R, 8, 32 );

void xmos_dsp_matrix_adds
(
    const int input_matrix_X[],
    int       scalar_value_A,
    int       result_matrix_R[],
    int       row_count,
    int       column_count
);

// ================================================================================================

// Matrix / scalar multiplication: R = X * a
//
// Parameters:
//
// 'input_matrix_X':  Pointer/reference to source data X.
// 'scalar_value_A':  Scalar value to multiply each 'input' element by.
// 'result_matrix_R': Pointer to the resulting 2-dimensional data array.
// 'row_count':       Number of rows in input and output matrices.
// 'column_count':    Number of columns in input and output matrices.
// 'q_format':        Fixed point format, the number of bits making up fractional part.
//
// Algorithm:
//
//  result = 0
//  for i = 0 to (row_count - 1):
//    for j = 0 to (column_count - 1):
//      k = i * column_count + j
//      result_matrix_R[k] = input_matrix_X[k] * scalar_value_A
//
// Behavior:
//
// Each element of the input matrix is multiplied by a scalar value using a 32bit multiply 64-bit
// accumulate function therefore fixed-point multiplication and q-format adjustment overflow
// behavior must be considered (see behavior for the function 'xmos_dsp_math_multiply').
//
// Example:
//
//  int input_matrix_X[8][32];
//  int scalar_value_A = Q28( 0.333 );
//  int result_vector_R[8][32];  
//  xmos_dsp_matrix_muls( input_matrix_X, scalar_value_A, result_matrix_R, 256, 8, 32, 28 );

void xmos_dsp_matrix_muls
(
    const int input_matrix_X[],
    int       scalar_value_A,
    int       result_matrix_R[],
    int       row_count,
    int       column_count,
    int       q_format
);

// ================================================================================================

// Matrix / matrix addition: R[i] = X[i] + Y[i]
//
// Parameters:
//
// 'input_matrix_X':  Pointer to source data array X.
// 'input_matrix_Y':  Pointer to source data array Y.
// 'result_matrix_R': Pointer to the resulting 2-dimensional data array.
// 'row_count':       Number of rows in input and output matrices.
// 'column_count':    Number of columns in input and output matrices.
//
// Algorithm:
//
//  for i = 0 to (vector_length - 1):
//    result_matrix_R[i] = input_matrix_X[i] + input_matrix_Y[i]
//
// Behavior:
//
// 32-bit addition is used to compute the result for each element. Therefore fixed-point value
// overflow conditions should be observed.  The resulting values are not saturated.
//
// Example:
//
//  int input_matrix_X [256];
//  int input_matrix_Y [256];
//  int result_matrix_R[256];  
//  xmos_dsp_matrix_addv( input_matrix_X, input_matrix_Y, result_matrix_R, 8, 32 );

void xmos_dsp_matrix_addm
(
    const int input_matrix_X[],
    const int input_matrix_Y[],
    int       result_matrix_R[],
    int       row_count,
    int       column_count
);

// ================================================================================================

// Matrix / matrix subtraction: R[i] = X[i] - Y[i]
//
// Parameters:
//
// 'input_matrix_X':  Pointer to source data array X.
// 'input_matrix_Y':  Pointer to source data array Y.
// 'result_matrix_R': Pointer to the resulting 2-dimensional data array.
// 'row_count':       Number of rows in input and output matrices.
// 'column_count':    Number of columns in input and output matrices.
//
// Algorithm:
//
//  for i = 0 to (vector_length - 1):
//    result_matrix_R[i] = input_matrix_X[i] - input_matrix_Y[i]
//
// Behavior:
//
// 32-bit subtraction is used to compute the result for each element. Therefore fixed-point value
// overflow conditions should be observed.  The resulting values are not saturated.
//
// Example:
//
//  int input_matrix_X [256];
//  int input_matrix_Y [256];
//  int result_matrix_R[256];  
//  xmos_dsp_matrix_addv( input_matrix_X, input_matrix_Y, result_matrix_R, 8, 32 );

void xmos_dsp_matrix_subm
(
    const int input_matrix_X[],
    const int input_matrix_Y[],
    int       result_matrix_R[],
    int       row_count,
    int       column_count
);

// ================================================================================================

// Matrix / matrix multiplication: R[i] = X[i] * Y[i]
//
// Parameters:
//
// 'input_matrix_X':  Pointer to source data array X.
// 'input_matrix_Y':  Pointer to source data array Y.
// 'result_matrix_R': Pointer to the resulting 2-dimensional data array.
// 'row_count':       Number of rows in input and output matrices.
// 'column_count':    Number of columns in input and output matrices.
// 'q_format':        Fixed point format, the number of bits making up fractional part.
//
// Algorithm:
//
//  result = 0
//  for i = 0 to (row_count - 1):
//    for j = 0 to (column_count - 1):
//      k = i * column_count + j
//      result_matrix_R[k] = input_matrix_X[k] * input_matrix_Y[k]
//
// Behavior:
//
// Elements in each of the input matrices are multiplied together using a 32bit multiply 64-bit
// accumulate function therefore fixed-point multiplication and q-format adjustment overflow
// behavior must be considered (see behavior for the function 'xmos_dsp_math_multiply').
//
// Example:
//
//  int input_matrix_X[8][32];
//  int input_matrix_Y[8][32];
//  int result_vector_R[8][32];  
//  xmos_dsp_matrix_mulm( input_matrix_X, input_matrix_Y, result_matrix_R, 256, 8, 32, 28 );

void xmos_dsp_matrix_mulm
(
    const int input_matrix_X[],
    const int input_matrix_Y[],
    int       result_matrix_R[],
    int       row_count,
    int       column_count,
    int       q_format
);

// ================================================================================================

// Matrix transposition
//
// Parameters:
//
// 'input_matrix_X':  Pointer/reference to source data.
// 'result_matrix_R': Pointer/reference to the resulting data.
// 'row_count':       Number of rows in input and output matrices.
// 'column_count':    Number of columns in input and output matrices.
// 'q_format':        Fixed point format, the number of bits making up fractional part.
//
// Algorithm:
//
// Behavior:
//
// Example:

void xmos_dsp_matrix_transpose
(
    const int input_matrix_X[],
    int       result_matrix_R[],
    int       row_count,
    int       column_count,
    int       q_format
);

// ================================================================================================

#endif
