// Copyright (c) 2015-2016, XMOS Ltd, All rights reserved

#ifndef DSP_MATRIX_H_
#define DSP_MATRIX_H_

#include "stdint.h"



/** Matrix negation: ``R[i][j] = -X[i][j]``
 * 
 *  Each negated element is computed by twos-compliment negation therefore
 *  the minimum negative fixed-point value can not be negated to generate its
 *  corresponding maximum positive fixed-point value. For example: -Q28(-8.0)
 *  will not result in a fixed-point value representing +8.0.
 * 
 *  Example:
 * 
 *  \code
 *  int32_t samples[8][32];
 *  int32_t result[8][32];  
 *  dsp_matrix_negate( samples, result, 8, 32 );
 *  \endcode
 * 
 *  \param  input_matrix_X   Pointer/reference to source data.
 *  \param  result_matrix_R  Pointer to the resulting 2-dimensional data array.
 *  \param  row_count        Number of rows in input and output matrices.
 *  \param  column_count     Number of columns in input and output matrices.
 */

void dsp_matrix_negate
(
    const int32_t input_matrix_X[],
    int32_t       result_matrix_R[],
    const int32_t row_count,
    const int32_t column_count
);

/** Matrix / scalar addition: ``R[i][j] = X[i][j] + A``
 * 
 *  32-bit addition is used to compute the scaler plus matrix element result.
 *  Therefore fixed-point value overflow conditions should be observed.  The
 *  resulting values are not saturated.
 * 
 *  Example:
 * 
 *  \code
 *  int32_t input_matrix_X[8][32];
 *  int32_t input_scalar_A = Q28( 0.333 );
 *  int32_t result_vector_R[8][32];  
 *  dsp_matrix_adds( input_matrix_X, scalar_matrix_A, result_matrix_R, 8, 32 );
 *  \endcode
 * 
 *  \param  input_matrix_X   Pointer/reference to source data.
 *  \param  input_scalar_A   Scalar value to add to each input element.
 *  \param  result_matrix_R  Pointer to the resulting 2-dimensional data array.
 *  \param  row_count        Number of rows in input and output matrices.
 *  \param  column_count     Number of columns in input and output matrices.
 */

void dsp_matrix_adds
(
    const int32_t input_matrix_X[],
    int32_t       input_scalar_A,
    int32_t       result_matrix_R[],
    const int32_t row_count,
    const int32_t column_count
);

/** Matrix / scalar multiplication: ``R[i][j] = X[i][j] * A``
 * 
 *  Each element of the input matrix is multiplied by a scalar value using a
 *  32bit multiply 64-bit accumulate function therefore fixed-point
 *  multiplication and q-format adjustment overflow behavior must be considered
 *  (see behavior for the function ``dsp_math_multiply``).
 * 
 *  Example:
 * 
 *  \code
 *  int32_t input_matrix_X[8][32];
 *  int32_t input_scalar_A = Q28( 0.333 );
 *  int32_t result_vector_R[8][32];  
 *  dsp_matrix_muls( input_matrix_X, scalar_value_A, result_matrix_R, 256, 8, 32, 28 );
 *  \endcode
 * 
 *  \param  input_matrix_X   Pointer/reference to source data X.
 *  \param  input_scalar_A   Scalar value to multiply each element by.
 *  \param  result_matrix_R  Pointer to the resulting 2-dimensional data array.
 *  \param  row_count        Number of rows in input and output matrices.
 *  \param  column_count     Number of columns in input and output matrices.
 *  \param  q_format         Fixed point format (i.e. number of fractional bits).
 */

void dsp_matrix_muls
(
    const int32_t input_matrix_X[],
    int32_t       input_scalar_A,
    int32_t       result_matrix_R[],
    const int32_t row_count,
    const int32_t column_count,
    const int32_t q_format
);

/** Matrix / matrix addition: ``R[i][j] = X[i][j] + Y[i][j]``
 * 
 *  32-bit addition is used to compute the result for each element.
 *  Therefore fixed-point value overflow conditions should be observed.
 *  The resulting values are not saturated.
 * 
 *  Example:
 * 
 *  \code
 *  int32_t input_matrix_X [256] = { 0, 1, 2, 3, ... not shown for brevity };
 *  int32_t input_matrix_Y [256] = { 0, 1, 2, 3, ... not shown for brevity };
 *  int32_t result_matrix_R[256];  
 *  dsp_matrix_addm( input_matrix_X, input_matrix_Y, result_matrix_R, 8, 32 );
 *  \endcode
 * 
 *  \param  input_matrix_X   Pointer to source data array X.
 *  \param  input_matrix_Y   Pointer to source data array Y.
 *  \param  result_matrix_R  Pointer to the resulting 2-dimensional data array.
 *  \param  row_count        Number of rows in input and output matrices.
 *  \param  column_count     Number of columns in input and output matrices.
 */

void dsp_matrix_addm
(
    const int32_t input_matrix_X[],
    const int32_t input_matrix_Y[],
    int32_t       result_matrix_R[],
    const int32_t row_count,
    const int32_t column_count
);

/** Matrix / matrix subtraction: ``R[i][j] = X[i][j] - Y[i][j]``
 * 
 *  32-bit subtraction is used to compute the result for each element.
 *  Therefore fixed-point value overflow conditions should be observed.
 *  The resulting values are not saturated.
 * 
 *  Example:
 * 
 *  \code
 *  int32_t input_matrix_X [256] = { 0, 1, 2, 3, ... not shown for brevity };
 *  int32_t input_matrix_Y [256] = { 0, 1, 2, 3, ... not shown for brevity };
 *  int32_t result_matrix_R[256];  
 *  dsp_matrix_subm( input_matrix_X, input_matrix_Y, result_matrix_R, 8, 32 );
 *  \endcode
 * 
 *  \param  input_matrix_X   Pointer to source data array X.
 *  \param  input_matrix_Y   Pointer to source data array Y.
 *  \param  result_matrix_R  Pointer to the resulting 2-dimensional data array.
 *  \param  row_count        Number of rows in input and output matrices.
 *  \param  column_count     Number of columns in input and output matrices.
 */

void dsp_matrix_subm
(
    const int32_t input_matrix_X[],
    const int32_t input_matrix_Y[],
    int32_t       result_matrix_R[],
    const int32_t row_count,
    const int32_t column_count
);

/** Matrix / matrix multiplication: ``R[i][j] = X[i][j] * Y[i][j]``
 * 
 *  Elements in each of the input matrices are multiplied together using a
 *  32bit multiply 64-bit accumulate function therefore fixed-point
 *  multiplication and q-format adjustment overflow behavior must be considered
 *  (see behavior for the function ``dsp_math_multiply``). 
 *  The algorithm is optimised for performance using double word load and store instructions.
 *  As a result the matrices must have an even number of rows and columns.
 * 
 *  Example:
 *  MxN * NxP = MxP
 *  \code
 *  int32_t input_matrix_X[rows_X][N];
 *  int32_t input_matrix_Y[columns_Y][N]; // transposed for better memory alighment !!
 *  int32_t result_matrix_R[rows_x][columns_Y];  
 *  dsp_matrix_mulm( input_matrix_X, input_matrix_Y, result_matrix_R, 256, 8, 32, 28 );
 *  \endcode
 * 
 *  \param  input_matrix_X   Pointer to source data array X.
 *  \param  input_matrix_Y   Pointer to source data array Y. 
 *  \param  result_matrix_R  Pointer to the resulting 2-dimensional data array.
 *  \param  rows_X           Number of rows in input matrix X. Must be even or will trap.
 *  \param  cols_Y           Number of columns input matrix Y. Must be even or will trap.
 *  \param  cols_X_rows_Y    Number of columns in input matrix X == rows in input matrix Y. Must be even or will trap.
 *  \param  q_format         Fixed point format (i.e. number of fractional bits).
 */
// N == columns_X == rows_Y
void dsp_matrix_mulm
(
    const int32_t input_matrix_X[],
    const int32_t input_matrix_Y[],
    int32_t       result_matrix_R[],
    const int32_t rows_X,
    const int32_t cols_Y,
    const int32_t cols_X_rows_Y,
    const int32_t q_format
);

/** Matrix transposition
 * 
 *  \param  input_matrix_X   Pointer/reference to source data.
 *  \param  result_matrix_R  Pointer/reference to the resulting data.
 *  \param  row_count        Number of rows in input and output matrices.
 *  \param  column_count     Number of columns in input and output matrices.
 *  \param  q_format         Fixed point format (i.e. number of fractional bits).
 */

void dsp_matrix_transpose
(
    const int32_t input_matrix_X[],
    int32_t       result_matrix_R[],
    const int32_t row_count,
    const int32_t column_count,
    const int32_t q_format
);

#endif
