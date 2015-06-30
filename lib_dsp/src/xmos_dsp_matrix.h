#ifndef XMOS_DSP_MATRIX
#define XMOS_DSP_MATRIX

// Matrix negation: R = -X
//
// 'result_matrix_R': Pointer to the resulting 2-dimensional data array.
// 'input_matrix_X':  Pointer/reference to source data.
// 'row_count':       Number of rows in input matrix.
// 'column_count':    Number of columns in input matrix.

void xmos_dsp_matrix_negate
(
    int       result_matrix_R[],
    const int input_matrix_X[],
    int       row_count,
    int       column_count
);

// Matrix / scalar addition: R = X + a
//
// 'result_matrix_R': Pointer to the resulting 2-dimensional data array.
// 'input_matrix_X':  Pointer/reference to source data.
// 'scalar_value_A':  Scalar value to add to each 'input' element.
// 'row_count':       Number of rows in input and output matrices.
// 'column_count':    Number of columns in input and output matrices.

void xmos_dsp_matrix_adds
(
    int       result_matrix_R[],
    const int input_matrix_X[],
    int       scalar_value_A,
    int       row_count,
    int       column_count
);

// Matrix / scalar multiplication: R = X * a
//
// 'result_matrix_R': Pointer to the resulting 2-dimensional data array.
// 'input_matrix_X':  Pointer/reference to source data X.
// 'scalar_value_A':  Scalar value to multiply each 'input' element by.
// 'row_count':       Number of rows in input and output matrices.
// 'column_count':    Number of columns in input and output matrices.
// 'q_format':        Fixed point format, the number of bits making up fractional part.

void xmos_dsp_matrix_muls
(
    int       result_matrix_R[],
    const int input_matrix_X[],
    int       scalar_value_A,
    int       row_count,
    int       column_count,
    int       q_format
);

// Matrix / matrix addition: R = X + Y
//
// 'result_matrix_R': Pointer to the resulting 2-dimensional data array.
// 'input_matrix_X':  Pointer to source data array X.
// 'input_matrix_Y':  Pointer to source data array Y.
// 'row_count':       Number of rows in input and output matrices.
// 'column_count':    Number of columns in input and output matrices.

void xmos_dsp_matrix_addm
(
    int       result_matrix_R[],
    const int input_matrix_X[],
    const int input_matrix_Y[],
    int       row_count,
    int       column_count
);

// Matrix / matrix subtraction: R = X - Y
//
// 'result_matrix_R': Pointer to the resulting 2-dimensional data array.
// 'input_matrix_X':  Pointer to source data array X.
// 'input_matrix_Y':  Pointer to source data array Y.
// 'row_count':       Number of rows in input and output matrices.
// 'column_count':    Number of columns in input and output matrices.

void xmos_dsp_matrix_subm
(
    int       result_matrix_R[],
    const int input_matrix_X[],
    const int input_matrix_Y[],
    int       row_count,
    int       column_count
);

// Matrix / matrix multiplication: R = X * Y
//
// 'result_matrix_R': Pointer to the resulting 2-dimensional data array.
// 'input_matrix_X':  Pointer to source data array X.
// 'input_matrix_Y':  Pointer to source data array Y.
// 'row_count':       Number of rows in input and output matrices.
// 'column_count':    Number of columns in input and output matrices.
// 'q_format':        Fixed point format, the number of bits making up fractional part.

void xmos_dsp_matrix_mulm
(
    int       result_matrix_R[],
    const int input_matrix_X[],
    const int input_matrix_Y[],
    int       row_count,
    int       column_count,
    int       q_format
);

// Matrix inversion
//
// 'result_matrix_R': Pointer/reference to the resulting data.
// 'input_matrix_X':  Pointer/reference to source data.
// 'row_count':       Number of rows in input and output matrices.
// 'column_count':    Number of columns in input and output matrices.
// 'q_format':        Fixed point format, the number of bits making up fractional part.

void __TODO__xmos_dsp_matrix_invert
(
    int       result_matrix_R[],
    const int input_matrix_X[],
    int       row_count,
    int       column_count,
    int       q_format
);

// Matrix transposition
//
// 'result_matrix_R': Pointer/reference to the resulting data.
// 'input_matrix_X':  Pointer/reference to source data.
// 'row_count':       Number of rows in input and output matrices.
// 'column_count':    Number of columns in input and output matrices.
// 'q_format':        Fixed point format, the number of bits making up fractional part.

void xmos_dsp_matrix_transpose
(
    int       result_matrix_R[],
    const int input_matrix_X[],
    int       row_count,
    int       column_count,
    int       q_format
);

#endif
