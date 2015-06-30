#ifndef XMOS_DSP_MATH
#define XMOS_DSP_MATH

// Scalar fixed-point multiply
//
// 'input1_value': Multiply operand #1.
// 'input2_value': Multiply operand #2.
// 'q_format':     Fixed point format, the number of bits making up fractional part.
//
// return value:   input1_value * input2_value.

int xmos_dsp_math_multiply
(
    int input1_value,
    int input2_value,
    int q_format
);

// Scalar reciprocal
//
// 'input_value': Input value for computation.
// 'q_format':    Fixed point format, the number of bits making up fractional part.
//
// return value:  Reciporcal of input value.

int xmos_dsp_math_reciprocal( int input_value, int q_format );

// Scalar inverse square root
//
// 'input_value': Input value for computation.
// 'q_format':    Fixed point format, the number of bits making up fractional part.
//
// return value:  Inverse square root of input value.

int xmos_dsp_math_invsqrroot( int input_value, int q_format );

// Scalar square root
//
// 'input_value': Input value for computation.
// 'q_format':    Fixed point format, the number of bits making up fractional part.
//
// return value:  Square root of input value.

int xmos_dsp_math_squareroot( int input_value, int q_format );

#endif
