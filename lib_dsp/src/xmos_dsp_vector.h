#ifndef XMOS_DSP_VECTOR
#define XMOS_DSP_VECTOR

// Locate the vector's first occurring minimum value
//
// 'input_vector':  Pointer to source data array.
// 'vector_length': Length of the output and input arrays.
//
// return value:     Array index where first minimum value occurs.

int xmos_dsp_vector_minimum
(
    const int input_vector[],
    int       vector_length
);

// Locate the vector's first occurring maximum value
//
// 'input_vector':  Pointer to source data array.
// 'vector_length': Length of the output and input arrays.
//
// return value:     Array index where first maximum value occurs.

int xmos_dsp_vector_maximum
(
    const int input_vector[],
    int       vector_length
);

// Vector negation: R = -X
//
// 'input_vector_X':  Pointer/reference to source data.
// 'result_vector_R': Pointer to the resulting data array.
// 'vector_length':   Length of the input and output vectors.

void xmos_dsp_vector_negate
(
    const int input_vector_X[],
    int       result_vector_R[],
    int       vector_length
);

// Vector absolute value: R = |X|
//
// 'input_vector_X':  Pointer/reference to source data.
// 'result_vector_R': Pointer to the resulting data array.
// 'vector_length':   Length of the input and output vectors.

void xmos_dsp_vector_abs
(
    const int input_vector_X[],
    int       result_vector_R[],
    int       vector_length
);

// Vector / scalar addition: R = X + a
//
// 'input_vector_X':  Pointer/reference to source data array X
// 'scalar_value_A':  Scalar value to add to each 'input' element
// 'result_vector_R': Pointer to the resulting data array
// 'vector_length':   Length of the input and output vectors

void xmos_dsp_vector_adds
(
    const int input_vector_X[],
    int       scalar_value_A,
    int       result_vector_R[],
    int       vector_length
);

// Vector / scalar multiplication: R = X * a
//
// 'input_vector_X':  Pointer/reference to source data array X.
// 'scalar_value_A':  Scalar value to multiply each 'input' element by.
// 'result_vector_R': Pointer to the resulting data array.
// 'vector_length':   Length of the input and output vectors.
// 'q_format':        Fixed point format, the number of bits making up fractional part.

void xmos_dsp_vector_muls
(
    const int input_vector_X[],
    int       scalar_value_A,
    int       result_vector_R[],
    int       vector_length,
    int       q_format
);

// Vector / vector addition: R = X + Y
//
// 'input_vector_X':  Pointer to source data array X.
// 'input_vector_Y':  Pointer to source data array Y.
// 'result_vector_R': Pointer to the resulting data array.
// 'vector_length':   Length of the input and output vectors.

void xmos_dsp_vector_addv
(
    const int input_vector_X[],
    const int input_vector_Y[],
    int       result_vector_R[],
    int       vector_length
);

// Vector / vector subtraction: R = X - Y
//
// 'input_vector_X':  Pointer to source data array X.
// 'input_vector_Y':  Pointer to source data array Y.
// 'result_vector_R': Pointer to the resulting data array.
// 'vector_length':   Length of the input and output vectors.

void xmos_dsp_vector_subv
(
    const int input_vector_X[],
    const int input_vector_Y[],
    int       result_vector_R[],
    int       vector_length
);

// Vector / vector multiplication: R = X * Y
//
// 'input_vector_X':  Pointer to source data array X.
// 'input_vector_Y':  Pointer to source data array Y.
// 'result_vector_R': Pointer to the resulting data array.
// 'vector_length':   Length of the input and output vectors.
// 'q_format':        Fixed point format, the number of bits making up fractional part.

void xmos_dsp_vector_mulv
(
    const int input_vector_X[],
    const int input_vector_Y[],
    int       result_vector_R[],
    int       vector_length,
    int       q_format
);

// Vector multiplication and scalar addition (R = X * Y + a)
//
// 'input_vector_X':  Pointer to source data array X.
// 'input_vector_Y':  Pointer to source data array Y.
// 'scalar_value_A':  Scalar value to add to each X*Y result.
// 'result_vector_R': Pointer to the resulting data array.
// 'vector_length':   Length of the input and output vectors.
// 'q_format':        Fixed point format, the number of bits making up fractional part.

void xmos_dsp_vector_mulv_adds
(
    const int input_vector_X[],
    const int input_vector_Y[],
    int       input_scalar_A,
    int       result_vector_R[],
    int       vector_length,
    int       q_format
);

// Scalar multiplication and vector addition: R = a * X + Y
//
// 'input_vector_X':  Pointer to source data array X.
// 'input_scalar_A':  Scalar value to multiply each 'input' element by.
// 'input_vector_Y':  Pointer to source data array Y
// 'result_vector_R': Pointer to the resulting data array.
// 'vector_length':   Length of the input and output vectors
// 'q_format':        Fixed point format, the number of bits making up fractional part

void xmos_dsp_vector_muls_addv
(
    const int input_vector_X[],
    int       input_scalar_A,
    const int input_vector_Y[],
    int       result_vector_R[],
    int       vector_length,
    int       q_format
);

// Scalar multiplication and vector subtraction: R = a * X - Y
//
// 'input_scalar_A':  Scalar value to multiply each 'input' element by.
// 'input_vector_X':  Pointer to source data array X.
// 'input_vector_Y':  Pointer to source data array Y.
// 'result_vector_R': Pointer to the resulting data array.
// 'vector_length':   Length of the input and output vectors.
// 'q_format':        Fixed point format, the number of bits making up fractional part.

void xmos_dsp_vector_muls_subv
(
    const int input_vector_X[],
    int       input_scalar_A,
    const int input_vector_Y[],
    int       result_vector_R[],
    int       vector_length,
    int       q_format
);

// Vector multiplication and vector addition: R = X * Y + Z
//
// 'input_vector_X':  Pointer to source data array X.
// 'input_vector_Y':  Pointer to source data array Y.
// 'input_vector_Z':  Pointer to source data array Z.
// 'result_vector_R': Pointer to the resulting data array.
// 'vector_length':   Length of the input and output vectors.
// 'q_format':        Fixed point format, the number of bits making up fractional part.

void xmos_dsp_vector_mulv_addv
(
    const int input_vector_X[],
    const int input_vector_Y[],
    const int input_vector_Z[],
    int       result_vector_R[],
    int       vector_length,
    int       q_format
);

// Vector multiplication and vector addition: R = X * Y - Z
//
// 'input_vector_X':  Pointer to source data array X.
// 'input_vector_Y':  Pointer to source data array Y.
// 'input_vector_Z':  Pointer to source data array Z.
// 'result_vector_R': Pointer to the resulting data array.
// 'vector_length':   Length of the input and output vectors.
// 'q_format':        Fixed point format, the number of bits making up fractional part.

void xmos_dsp_vector_mulv_subv
(
    const int input_vector_X[],
    const int input_vector_Y[],
    const int input_vector_Z[],
    int       result_vector_R[],
    int       vector_length,
    int       q_format
);

#endif
