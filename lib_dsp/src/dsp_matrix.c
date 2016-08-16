// Copyright (c) 2015-2016, XMOS Ltd, All rights reserved

#include <platform.h>
#include "dsp_qformat.h"
#include "dsp_vector.h"
#include "dsp_matrix.h"



void dsp_matrix_negate
(
    const int32_t* input_matrix_X,
    int32_t*       result_matrix_R,
    int32_t        row_count,
    int32_t        column_count
)
{
    dsp_vector_negate( input_matrix_X, result_matrix_R, row_count * column_count );
}



void dsp_matrix_adds
(
    const int32_t* input_matrix_X,
    int32_t        scalar_value_A,
    int32_t*       result_matrix_R,
    int32_t        row_count,
    int32_t        column_count
) {
    dsp_vector_adds
    (
        input_matrix_X,
        scalar_value_A,
        result_matrix_R,
        row_count * column_count
    );
}



void dsp_matrix_muls
(
    const int32_t* input_matrix_X,
    int32_t        scalar_value_A,
    int32_t*       result_matrix_R,
    int32_t        row_count,
    int32_t        column_count,
    const int32_t q_format
) {
    dsp_vector_muls
    (
        input_matrix_X,
        scalar_value_A,
        result_matrix_R,
        row_count * column_count,
        q_format
    );
}



void dsp_matrix_addm
(
    const int32_t* input_matrix_X,
    const int32_t* input_matrix_Y,
    int32_t*       result_matrix_R,
    int32_t        row_count,
    int32_t        column_count
) {
    dsp_vector_addv
    (
        input_matrix_X,
        input_matrix_Y,
        result_matrix_R,
        row_count * column_count
    );
}



void dsp_matrix_subm
(
    const int32_t* input_matrix_X,
    const int32_t* input_matrix_Y,
    int32_t*       result_matrix_R,
    int32_t        row_count,
    int32_t        column_count
) {
    dsp_vector_subv
    (
        input_matrix_X,
        input_matrix_Y,
        result_matrix_R,
        row_count * column_count
    );
}

#define MATRIX_X_IN_EXTERNAL_RAM 0
#define MATRIX_Y_IN_EXTERNAL_RAM 0

void dsp_matrix_mulm
(
    const int32_t* input_matrix_X,
    const int32_t* input_matrix_Y,
    int32_t*       result_matrix_R,
    const int32_t  rows_X,
    const int32_t  cols_Y,
    const int32_t  cols_X_rows_Y,
    const int32_t q_format
) {
    int32_t ah; uint32_t al;

    for( int32_t rx = 0; rx < rows_X; ++rx )
    {
#if MATRIX_X_IN_EXTERNAL_RAM
        const int32_t* X_row_ptr = interface.get_array_ptr(0, rx); // matrix index and row vector index
#else
        const int32_t* X_row_ptr = &input_matrix_X[rx * cols_X_rows_Y];
#endif

        // column in X
        for( int32_t cy = 0; cy < cols_Y; cy++ )
        {
            // TODO: for large matrixes. provide the following arrays through shared memory

#if MATRIX_Y_IN_EXTERNAL_RAM
            const int32_t* Y_column_ptr = interface.get_array_ptr(1, cy); // matrix index and column vector index
#else
            const int32_t* Y_column_ptr = &input_matrix_Y[cy * cols_X_rows_Y];
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



void dsp_matrix_transpose
(
    const int32_t* input_matrix_X,
    int32_t*       result_matrix_R,
    int32_t        row_count,
    int32_t        column_count,
    const int32_t q_format
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
