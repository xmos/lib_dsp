#ifndef XMOS_DSP_VECTOR
#define XMOS_DSP_VECTOR

// Vector minimum/maximum
//
// 'input_vector':  Pointer to source data array
// 'vector_length': Length of the output and input arrays
//
// return value:     Array index where first minimum or maximum value occurs

int xmos_dsp_vector_minimum
(
    const int* input_vector,
    int        vector_length
);

int xmos_dsp_vector_maximum
(
    const int* input_vector,
    int        vector_length
);

// Vector negation (R = -X)
//
// 'result_vector_R': Pointer to the resulting data array
// 'input_vector_X':  Pointer/reference to source data
// 'vector_length':   Length of the input and output vectors

void xmos_dsp_vector_negate
(
    int*       result_vector_R,
    const int* input_vector_X,
    int        vector_length
);

// Vector / scalar addition (R = X + a)
//
// 'result_vector_R': Pointer to the resulting data array
// 'input_vector_X':  Pointer/reference to source data
// 'scalar_value_A':  Scalar value to add to each 'input' element
// 'vector_length':   Length of the input and output vectors

void xmos_dsp_vector_adds
(
    int*       result_vector_R,
    const int* input_vector_X,
    int        scalar_value_A,
    int        vector_length
);

// Vector / scalar multiplication (R = X * a)
//
// 'result_vector_R': Pointer to the resulting data array
// 'input_vector_X':  Pointer/reference to source data X
// 'scalar_value_A':  Scalar value to multiply each 'input' element by
// 'vector_length':   Length of the input and output vectors
// 'q_format':        Fixed point format, the number of bits making up fractional part

void xmos_dsp_vector_muls
(
    int*       result_vector_R,
    const int* input_vector_X,
    int        scalar_value_A,
    int        vector_length,
    int        q_format
);

// Vector / vector addition (R = X + Y)
//
// 'result_vector_R': Pointer to the resulting data array
// 'input_vector_X':  Pointer to source data array X
// 'input_vector_Y':  Pointer to source data array Y
// 'vector_length':   Length of the input and output vectors

void xmos_dsp_vector_addv
(
    int*       result_vector_R,
    const int* input_vector_X,
    const int* input_vector_Y,
    int        vector_length
);

// Vector / vector subtraction (R = X - Y)
//
// 'result_vector_R': Pointer to the resulting data array
// 'input_vector_X':  Pointer to source data array X
// 'input_vector_Y':  Pointer to source data array Y
// 'vector_length':   Length of the input and output vectors

void xmos_dsp_vector_subv
(
    int*       result_vector_R,
    const int* input_vector_X,
    const int* input_vector_Y,
    int        vector_length
);

// Vector / vector multiplication (R = X * Y)
//
// 'result_vector_R': Pointer to the resulting data array
// 'input_vector_X':  Pointer to source data array X
// 'input_vector_Y':  Pointer to source data array Y
// 'vector_length':   Length of the input and output vectors
// 'q_format':        Fixed point format, the number of bits making up fractional part

void xmos_dsp_vector_mulv
(
    int*       result_vector_R,
    const int* input_vector_X,
    const int* input_vector_Y,
    int        vector_length,
    int        q_format
);

// Vector multiplication and scalar addition (R = X * Y + a)
//
// 'result_vector_R': Pointer to the resulting data array
// 'input_vector_X':  Pointer to source data array X
// 'input_vector_Y':  Pointer to source data array Y
// 'scalar_value_A':  Scalar value to add to each X*Y result
// 'vector_length':   Length of the input and output vectors
// 'q_format':        Fixed point format, the number of bits making up fractional part

void xmos_dsp_vector_mulv_adds
(
    int*       result_vector_R,
    const int* input_vector_X,
    const int* input_vector_Y,
    int        input_scalar_A,
    int        vector_length,
    int        q_format
);

// Scalar multiplication and vector addition (R = a * X + Y)
//
// 'result_vector_R': Pointer to the resulting data array
// 'input_vector_X':  Pointer to source data array X
// 'input_scalar_A':  Scalar value to multiply each 'input' element by
// 'input_vector_Y':  Pointer to source data array Y
// 'vector_length':   Length of the input and output vectors
// 'q_format':        Fixed point format, the number of bits making up fractional part

void xmos_dsp_vector_muls_addv
(
    int*       result_vector_R,
    const int* input_vector_X,
    int        input_scalar_A,
    const int* input_vector_Y,
    int        vector_length,
    int        q_format
);

// Scalar multiplication and vector subtraction (R = a * X - Y)
//
// 'result_vector_R': Pointer to the resulting data array
// 'input_scalar_A':  Scalar value to multiply each 'input' element by
// 'input_vector_X':  Pointer to source data array X
// 'input_vector_Y':  Pointer to source data array Y
// 'vector_length':   Length of the input and output vectors
// 'q_format':        Fixed point format, the number of bits making up fractional part

void xmos_dsp_vector_muls_subv
(
    int*       result_vector_R,
    const int* input_vector_X,
    int        input_scalar_A,
    const int* input_vector_Y,
    int        vector_length,
    int        q_format
);

// Vector multiplication and vector addition (R = X * Y + Z)
//
// 'result_vector_R': Pointer to the resulting data array
// 'input_vector_X':  Pointer to source data array X
// 'input_vector_Y':  Pointer to source data array Y
// 'input_vector_Z':  Pointer to source data array Z
// 'vector_length':   Length of the input and output vectors
// 'q_format':        Fixed point format, the number of bits making up fractional part

void xmos_dsp_vector_mulv_addv
(
    int*       result_vector_R,
    const int* input_vector_X,
    const int* input_vector_Y,
    const int* input_vector_Z,
    int        vector_length,
    int        q_format
);

// Vector multiplication and vector addition (R = X * Y - Z)
//
// 'result_vector_R': Pointer to the resulting data array
// 'input_vector_X':  Pointer to source data array X
// 'input_vector_Y':  Pointer to source data array Y
// 'input_vector_Z':  Pointer to source data array Z
// 'vector_length':   Length of the input and output vectors
// 'q_format':        Fixed point format, the number of bits making up fractional part

void xmos_dsp_vector_mulv_subv
(
    int*       result_vector_R,
    const int* input_vector_X,
    const int* input_vector_Y,
    const int* input_vector_Z,
    int        vector_length,
    int        q_format
);

#endif
