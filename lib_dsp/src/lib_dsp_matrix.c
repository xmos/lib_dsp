// ============================================================================
// Copyright (c) 2015, XMOS Ltd, All rights reserved

#include <platform.h>
#include "lib_dsp_qformat.h"
#include "lib_dsp_vector.h"
#include "lib_dsp_matrix.h"

// ============================================================================

// Matrix negation: R[i] = -X[i]
//
// 'input_matrix_X':  Pointer to source data array.
// 'result_matrix_R': Pointer to the resulting 2-dimensional data array.
// 'row_count':       Number of rows in input matrix.
// 'column_count':    Number of columns in input matrix.

void lib_dsp_matrix_negate
(
    const int* input_matrix_X,
    int*       result_matrix_R,
    int        row_count,
    int        column_count
) {
    lib_dsp_vector_negate( input_matrix_X, result_matrix_R, row_count * column_count );
}

// ============================================================================

// Matrix / scalar addition: R[i] = X[i] + A
//
// 'input_matrix_X':  Pointer to source data array.
// 'scalar_value_A':  Scalar value to add to each 'input' element.
// 'result_matrix_R': Pointer to the resulting 2-dimensional data array.
// 'row_count':       Number of rows in input and output matrices.
// 'column_count':    Number of columns in input and output matrices.

void lib_dsp_matrix_adds
(
    const int* input_matrix_X,
    int        scalar_value_A,
    int*       result_matrix_R,
    int        row_count,
    int        column_count
) {
    lib_dsp_vector_adds
    (
        input_matrix_X,
        scalar_value_A,
        result_matrix_R,
        row_count * column_count
    );
}

// ============================================================================

// Matrix / scalar multiplication: R[i] = X[i] * A
//
// 'input_matrix_X':  Pointer to source data array X.
// 'scalar_value_A':  Scalar value to multiply each 'input' element by.
// 'result_matrix_R': Pointer to the resulting 2-dimensional data array.
// 'row_count':       Number of rows in input and output matrices.
// 'column_count':    Number of columns in input and output matrices.
// 'q_format':        Fixed point format, the number of bits making up fractional part.

void lib_dsp_matrix_muls
(
    const int* input_matrix_X,
    int        scalar_value_A,
    int*       result_matrix_R,
    int        row_count,
    int        column_count,
    int        q_format
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

// ============================================================================

// Matrix / matrix addition: R[i] = X[i] + Y[i]
//
// 'input_matrix_X':  Pointer to source data array X.
// 'input_matrix_Y':  Pointer to source data array Y.
// 'result_matrix_R': Pointer to the resulting 2-dimensional data array.
// 'row_count':       Number of rows in input and output matrices.
// 'column_count':    Number of columns in input and output matrices.

void lib_dsp_matrix_addm
(
    const int* input_matrix_X,
    const int* input_matrix_Y,
    int*       result_matrix_R,
    int        row_count,
    int        column_count
) {
    lib_dsp_vector_addv
    (
        input_matrix_X,
        input_matrix_Y,
        result_matrix_R,
        row_count * column_count
    );
}

// ============================================================================

// Matrix / matrix subtraction: R[i] = X[i] - Y[i]
//
// 'input_matrix_X':  Pointer to source data array X.
// 'input_matrix_Y':  Pointer to source data array Y.
// 'result_matrix_R': Pointer to the resulting 2-dimensional data array.
// 'row_count':       Number of rows in input and output matrices.
// 'column_count':    Number of columns in input and output matrices.

void lib_dsp_matrix_subm
(
    const int* input_matrix_X,
    const int* input_matrix_Y,
    int*       result_matrix_R,
    int        row_count,
    int        column_count
) {
    lib_dsp_vector_subv
    (
        input_matrix_X,
        input_matrix_Y,
        result_matrix_R,
        row_count * column_count
    );
}

// ============================================================================

// Matrix / matrix multiplication: R[i] = X[i] * Y[i]
//
// 'input_matrix_X':  Pointer to source data array X.
// 'input_matrix_Y':  Pointer to source data array Y.
// 'result_matrix_R': Pointer to the resulting 2-dimensional data array.
// 'row_count':       Number of rows in input and output matrices.
// 'column_count':    Number of columns in input and output matrices.
// 'q_format':        Fixed point format, the number of bits making up fractional part.

// <FIXME> - assumes 'row_count' == 'column_count'
// <TODO> - optimize using double-word load/store
// <TODO> - optimize for 2x2, 3x3 and 4x4 matrices

void lib_dsp_matrix_mulm
(
    const int* input_matrix_X,
    const int* input_matrix_Y,
    int*       result_matrix_R,
    int        row_count,
    int        column_count,
    int        q_format
) {
    int ah; unsigned al;
    int x, y;
    // <TODO>: Optimize
    for( int r = 0; r < row_count; ++r )
    {
        ah = 0; al = 1 << (q_format-1);
        for( int c = 0; c < column_count; ++c )
        {
            for( int i = 0, j = 0; i < row_count && j < column_count; ++i, ++j )
            {
                x = input_matrix_X[r * column_count + i];
                y = input_matrix_Y[j * column_count + c];
                asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x),"r"(y),"0"(ah),"1"(al));
            }
            asm("lextract %0,%1,%2,%3,32": "=r"(ah):"r"(ah),"r"(al),"r"(q_format));
            result_matrix_R[r * column_count + c] = ah;
        }
    }
}

// ============================================================================

// Matrix transposition
//
// 'input_matrix_X':  Pointer/reference to source data.
// 'result_matrix_R': Pointer/reference to the resulting data.
// 'row_count':       Number of rows in input and output matrices.
// 'column_count':    Number of columns in input and output matrices.
// 'q_format':        Fixed point format, the number of bits making up fractional part.

void lib_dsp_matrix_transpose
(
    const int* input_matrix_X,
    int*       result_matrix_R,
    int        row_count,
    int        column_count,
    int        q_format
) {
    // <TODO>: Optimize 
    for( int r = 0; r < row_count; ++r )
    {
        for( int c = 0; c < column_count; ++c )
        {
            result_matrix_R[c * column_count + r] = input_matrix_X[r * column_count + c];
        }
    }
}

// ============================================================================
