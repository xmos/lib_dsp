// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <platform.h>
#include "lib_dsp_qformat.h"
#include "lib_dsp_vector.h"
#include "lib_dsp_matrix.h"

/** Matrix negation: ``R[i][j] = -X[i][j]``
 * 
 *  Each negated element is computed by two's-compliment negation therefore
 *  the minimum negative fixed-point value can not be negated to generate it's
 *  corresponding maximum positive fixed-point value. For example: -Q28(-8.0)
 *  will not result in a fixed-point value representing +8.0.
 * 
 *  Example:
 * 
 *  \code
 *  int32_t samples[8][32];
 *  int32_t result[8][32];  
 *  lib_dsp_matrix_negate( samples, result, 8, 32 );
 *  \endcode
 * 
 *  \param  input_matrix_X   Pointer/reference to source data.
 *  \param  result_matrix_R  Pointer to the resulting 2-dimensional data array.
 *  \param  row_count        Number of rows in input matrix.
 *  \param  column_count     Number of columns in input matrix.
 */

void lib_dsp_matrix_negate
(
    const int32_t* input_matrix_X,
    int32_t*       result_matrix_R,
    int32_t        row_count,
    int32_t        column_count
)
{
    lib_dsp_vector_negate( input_matrix_X, result_matrix_R, row_count * column_count );
}

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
 *  lib_dsp_matrix_adds( input_matrix_X, scalar_matrix_A, result_matrix_R, 8, 32 );
 *  \endcode
 * 
 *  \param  input_matrix_X   Pointer/reference to source data.
 *  \param  input_scalar_A   Scalar value to add to each input element.
 *  \param  result_matrix_R  Pointer to the resulting 2-dimensional data array.
 *  \param  row_count        Number of rows in input and output matrices.
 *  \param  column_count     Number of columns in input and output matrices.
 */

void lib_dsp_matrix_adds
(
    const int32_t* input_matrix_X,
    int32_t        scalar_value_A,
    int32_t*       result_matrix_R,
    int32_t        row_count,
    int32_t        column_count
) {
    lib_dsp_vector_adds
    (
        input_matrix_X,
        scalar_value_A,
        result_matrix_R,
        row_count * column_count
    );
}

/** Matrix / scalar multiplication: ``R[i][j] = X[i][j] * A``
 * 
 *  Each element of the input matrix is multiplied by a scalar value using a
 *  32bit multiply 64-bit accumulate function therefore fixed-point
 *  multiplication and q-format adjustment overflow behavior must be considered
 *  (see behavior for the function ``lib_dsp_math_multiply``).
 * 
 *  Example:
 * 
 *  \code
 *  int32_t input_matrix_X[8][32];
 *  int32_t input_scalar_A = Q28( 0.333 );
 *  int32_t result_vector_R[8][32];  
 *  lib_dsp_matrix_muls( input_matrix_X, scalar_value_A, result_matrix_R, 256, 8, 32, 28 );
 *  \endcode
 * 
 *  \param  input_matrix_X   Pointer/reference to source data X.
 *  \param  input_scalar_A   Scalar value to multiply each element by.
 *  \param  result_matrix_R  Pointer to the resulting 2-dimensional data array.
 *  \param  row_count        Number of rows in input and output matrices.
 *  \param  column_count     Number of columns in input and output matrices.
 *  \param  q_format         Fixed point format (i.e. number of fractional bits).
 */

void lib_dsp_matrix_muls
(
    const int32_t* input_matrix_X,
    int32_t        scalar_value_A,
    int32_t*       result_matrix_R,
    int32_t        row_count,
    int32_t        column_count,
    int32_t        q_format
) {
    lib_dsp_vector_muls
    (
        input_matrix_X,
        scalar_value_A,
        result_matrix_R,
        row_count * column_count,
        q_format
    );
}

/** Matrix / matrix addition: ``R[i][j] = X[i][j] + Y[i][j]``
 * 
 *  32-bit addition is used to compute the result for each element.
 *  Therefore fixed-point value overflow conditions should be observed.
 *  The resulting values are not saturated.
 * 
 *  Example:
 * 
 *  \code
 *  int32_t input_matrix_X [256];
 *  int32_t input_matrix_Y [256];
 *  int32_t result_matrix_R[256];  
 *  lib_dsp_matrix_addv( input_matrix_X, input_matrix_Y, result_matrix_R, 8, 32 );
 *  \endcode
 * 
 *  \param  input_matrix_X   Pointer to source data array X.
 *  \param  input_matrix_Y   Pointer to source data array Y.
 *  \param  result_matrix_R  Pointer to the resulting 2-dimensional data array.
 *  \param  row_count        Number of rows in input and output matrices.
 *  \param  column_count     Number of columns in input and output matrices.
 */

void lib_dsp_matrix_addm
(
    const int32_t* input_matrix_X,
    const int32_t* input_matrix_Y,
    int32_t*       result_matrix_R,
    int32_t        row_count,
    int32_t        column_count
) {
    lib_dsp_vector_addv
    (
        input_matrix_X,
        input_matrix_Y,
        result_matrix_R,
        row_count * column_count
    );
}

/** Matrix / matrix subtraction: ``R[i][j] = X[i][j] - Y[i][j]``
 * 
 *  32-bit subtraction is used to compute the result for each element.
 *  Therefore fixed-point value overflow conditions should be observed.
 *  The resulting values are not saturated.
 * 
 *  Example:
 * 
 *  \code
 *  int32_t input_matrix_X [256];
 *  int32_t input_matrix_Y [256];
 *  int32_t result_matrix_R[256];  
 *  lib_dsp_matrix_addv( input_matrix_X, input_matrix_Y, result_matrix_R, 8, 32 );
 *  \endcode
 * 
 *  \param  input_matrix_X   Pointer to source data array X.
 *  \param  input_matrix_Y   Pointer to source data array Y.
 *  \param  result_matrix_R  Pointer to the resulting 2-dimensional data array.
 *  \param  row_count        Number of rows in input and output matrices.
 *  \param  column_count     Number of columns in input and output matrices.
 */

void lib_dsp_matrix_subm
(
    const int32_t* input_matrix_X,
    const int32_t* input_matrix_Y,
    int32_t*       result_matrix_R,
    int32_t        row_count,
    int32_t        column_count
) {
    lib_dsp_vector_subv
    (
        input_matrix_X,
        input_matrix_Y,
        result_matrix_R,
        row_count * column_count
    );
}

void lib_dsp_matrix_mulm
(
    const int32_t* input_matrix_X,
    const int32_t* input_matrix_Y,
    int32_t*       result_matrix_R,
    const int32_t        rows_X,
    const int32_t        cols_Y,
    const int32_t        cols_X_rows_Y,
    int32_t        q_format
) {
    int32_t ah; uint32_t al;
    int32_t x, y;

    for( int32_t rx = 0; rx < rows_X; ++rx )
    {
#if MATRIX_X_IN_EXTERNAL_RAM
        int32_t* X_row_ptr = interface.get_array_ptr(0, rx); // matrix index and row vector index
#else
        int32_t* X_row_ptr = &input_matrix_X[rx * cols_X_rows_Y];
#endif

        // column in X
        for( int32_t cy = 0; cy < cols_Y; cy++ )
        {
            // TODO: for large matrixes. provide the following arrays through shared memory

#if MATRIX_Y_IN_EXTERNAL_RAM
            int32_t* Y_column_ptr = interface.get_array_ptr(1, cy); // matrix index and column vector index
#else
            int32_t* Y_column_ptr = &input_matrix_Y[cy * cols_X_rows_Y];
#endif
            //
            int32_t x1, x0;
            int32_t y1, y0;
            ah = 0; al = 1 << (q_format-1);
            for(int32_t i = 0; i < (cols_X_rows_Y>>1); i++)
            {
                // load two items from row rx in X and two from column cy in Y
                asm("ldd %0,%1,%2[%3]":"=r"(x1),"=r"(x0):"r"(X_row_ptr),"r"(i));
                asm("ldd %0,%1,%2[%3]":"=r"(y1),"=r"(y0):"r"(Y_column_ptr),"r"(i));
                // mutliply two x and two y
                asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(y0),"0"(ah),"1"(al));
                asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(y1),"0"(ah),"1"(al));
            }
            asm("lextract %0,%1,%2,%3,32": "=r"(ah):"r"(ah),"r"(al),"r"(q_format));
            result_matrix_R[rx * cols_Y + cy] = ah;
        }
    }
}

/** Matrix transposition
 * 
 *  \param  input_matrix_X   Pointer/reference to source data.
 *  \param  result_matrix_R  Pointer/reference to the resulting data.
 *  \param  row_count        Number of rows in input and output matrices.
 *  \param  column_count     Number of columns in input and output matrices.
 *  \param  q_format         Fixed point format (i.e. number of fractional bits).
 */

void lib_dsp_matrix_transpose
(
    const int32_t* input_matrix_X,
    int32_t*       result_matrix_R,
    int32_t        row_count,
    int32_t        column_count,
    int32_t        q_format
) {
    // <TODO>: Optimize 
    for( int32_t r = 0; r < row_count; ++r )
    {
        for( int32_t c = 0; c < column_count; ++c )
        {
            result_matrix_R[c * column_count + r] = input_matrix_X[r * column_count + c];
        }
    }
}
