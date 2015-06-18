#include <platform.h>
#include "xmos_dsp_qformat.h"
#include "xmos_dsp_vector.h"
#include "xmos_dsp_matrix.h"

// Matrix negation (R = -X)
//
// 'result_matrix_R': Pointer to the resulting 2-dimensional data array
// 'input_matrix_X':  Pointer/reference to source data
// 'row_count':       Number of rows in input matrix
// 'column_count':    Number of columns in input matrix

void xmos_dsp_matrix_negate
(
    int*       result_matrix_R,
    const int* input_matrix_X,
    int        row_count,
    int        column_count
)
{
    xmos_dsp_vector_negate( result_matrix_R, input_matrix_X, row_count * column_count );
}

// Matrix / scalar addition (R = X + a)
//
// 'result_matrix_R': Pointer to the resulting 2-dimensional data array
// 'input_matrix_X':  Pointer/reference to source data
// 'scalar_value_A':  Scalar value to add to each 'input' element
// 'row_count':       Number of rows in input and output matrices
// 'column_count':    Number of columns in input and output matrices

void xmos_dsp_matrix_adds
(
    int*       result_matrix_R,
    const int* input_matrix_X,
    int        scalar_value_A,
    int        row_count,
    int        column_count
)
{
    xmos_dsp_vector_adds
    (
        result_matrix_R,
        input_matrix_X,
        scalar_value_A,
        row_count * column_count
    );
}

// Matrix / scalar multiplication (R = X * a)
//
// 'result_matrix_R': Pointer to the resulting 2-dimensional data array
// 'input_matrix_X':  Pointer/reference to source data X
// 'scalar_value_A':  Scalar value to multiply each 'input' element by
// 'row_count':       Number of rows in input and output matrices
// 'column_count':    Number of columns in input and output matrices
// 'q_format':        Fixed point format, the number of bits making up fractional part

void xmos_dsp_matrix_muls
(
    int*       result_matrix_R,
    const int* input_matrix_X,
    int        scalar_value_A,
    int        row_count,
    int        column_count,
    int        q_format
)
{
    xmos_dsp_vector_muls
    (
        result_matrix_R,
        input_matrix_X,
        scalar_value_A,
        row_count * column_count,
        q_format
    );
}

// Matrix / matrix addition (R = X + Y)
//
// 'result_matrix_R': Pointer to the resulting 2-dimensional data array
// 'input_matrix_X':  Pointer to source data array X
// 'input_matrix_Y':  Pointer to source data array Y
// 'row_count':       Number of rows in input and output matrices
// 'column_count':    Number of columns in input and output matrices

void xmos_dsp_matrix_addm
(
    int*       result_matrix_R,
    const int* input_matrix_X,
    const int* input_matrix_Y,
    int        row_count,
    int        column_count
)
{
    xmos_dsp_vector_addv
    (
        result_matrix_R,
        input_matrix_X,
        input_matrix_Y,
        row_count * column_count
    );
}

// Matrix / matrix subtraction (R = X - Y)
//
// 'result_matrix_R': Pointer to the resulting 2-dimensional data array
// 'input_matrix_X':  Pointer to source data array X
// 'input_matrix_Y':  Pointer to source data array Y
// 'row_count':       Number of rows in input and output matrices
// 'column_count':    Number of columns in input and output matrices

void xmos_dsp_matrix_subm
(
    int*       result_matrix_R,
    const int* input_matrix_X,
    const int* input_matrix_Y,
    int        row_count,
    int         column_count
)
{
    xmos_dsp_vector_subv
    (
        result_matrix_R,
        input_matrix_X,
        input_matrix_Y,
        row_count * column_count
    );
}

// Matrix / matrix multiplication (R = X * Y)
//
// 'result_matrix_R': Pointer to the resulting 2-dimensional data array
// 'input_matrix_X':  Pointer to source data array X
// 'input_matrix_Y':  Pointer to source data array Y
// 'row_count':       Number of rows in input and output matrices
// 'column_count':    Number of columns in input and output matrices
// 'q_format':        Fixed point format, the number of bits making up fractional part

// <FIXME> - assumes 'row_count' == 'column_count'
// <TODO> - optimize using double-word load/store
// <TODO> - optimize for 2x2, 3x3 and 4x4 matrices

void xmos_dsp_matrix_mulm
(
    int*       result_matrix_R,
    const int* input_matrix_X,
    const int* input_matrix_Y,
    int        row_count,
    int        column_count,
    int        q_format
)
{
    int ah; unsigned al;
    int x, y;
    for( int r = 0; r < row_count; ++r )
    {
        ah = 0; al = 1 << (q_format-1);
        for( int c = 0; c < column_count; ++c )
        {
            for( int i = 0, j = 0; i < row_count && j < column_count; ++i, ++j )
            {
                x = input_matrix_X[r * column_count + i];
                y = input_matrix_Y[j * column_count + c];
                //XS2_MACCS( ah, al, x, y, ah, al );
            }
            asm("lextract %0,%1,%2,%3,32": "=r"(ah):"r"(ah),"r"(al),"r"(q_format));
            result_matrix_R[r * column_count + c] = ah;
        }
    }
}

// Matrix inversion
//
// 'result_matrix_R': Pointer/reference to the resulting data
// 'input_matrix_X':  Pointer/reference to source data
// 'row_count':       Number of rows in input and output matrices
// 'column_count':    Number of columns in input and output matrices
// 'q_format':        Fixed point format, the number of bits making up fractional part

void __TODO__xmos_dsp_matrix_invert
(
    int*       result_matrix_R,
    const int* input_matrix_X,
    int        row_count,
    int        column_count,
    int        q_format
)
{
}

// Matrix transposition
//
// 'result_matrix_R': Pointer/reference to the resulting data
// 'input_matrix_X':  Pointer/reference to source data
// 'row_count':       Number of rows in input and output matrices
// 'column_count':    Number of columns in input and output matrices
// 'q_format':        Fixed point format, the number of bits making up fractional part

void xmos_dsp_matrix_transpose
(
    int*       result_matrix_R,
    const int* input_matrix_X,
    int        row_count,
    int        column_count,
    int        q_format
)
{
    for( int r = 0; r < row_count; ++r )
    {
        //ah = 0; al = 1 << (q_format-1);
        for( int c = 0; c < column_count; ++c )
        {
            result_matrix_R[c * column_count + r] = input_matrix_X[r * column_count + c];
        }
    }
}
