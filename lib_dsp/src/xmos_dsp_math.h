// ================================================================================================

#ifndef XMOS_DSP_MATH
#define XMOS_DSP_MATH

// ================================================================================================

// Scalar multiply
//
// This function multiplies two scalar values and produces a result according to fixed-point format
// specified by the 'q_format' parameter.
//
// Parameters:
//
// 'input1_value': Multiply operand #1.
// 'input2_value': Multiply operand #2.
// 'q_format':     Fixed point format, the number of bits making up fractional part.
//
// Return value:   input1_value * input2_value.
//
// Algorithm:
//
// The two operands are multiplied to produce a 64-bit result which is tested for overflow,
// clamped at the minimum/maximum value given the fixed-point format if overflow occurs,
// and finally shifted right by 'q_format' bits. 
//
// 1) Y = X1 * X2
// 2) Y = min( max( Q_FORMAT_MIN, Y ), Q_FORMAT_MAX, Y )
// 3) Y = Y >> q_format
//
// Behavior:
//
// While saturation is employed after multiplication an overflow condition when preparing the final
// result must still be considered when specifying a Q-format whose fixed-point numerical range do
// not accomodate the final result of multiplication and saturation (if applied).
//
// Example:
//
// int result = xmos_dsp_math_multiply( Q28(-0.33), sample, 28 );

int xmos_dsp_math_multiply
(
    int input1_value,
    int input2_value,
    int q_format
);

// ================================================================================================

// Scalar reciprocal
//
// This function computes the reciprocal of the input value using an iterative approximation method.
//
// Parameters:
//
// 'input_value': Input value for computation.
// 'q_format':    Fixed point format, the number of bits making up fractional part.
//
// Return value:  Reciporcal of input value.
//
// Algorithm:
//
// 1) result = 1.0
// 2) result = result + result * (1 − input_value * result)
// 3) Repeat step #2 until desired precision is achieved
//
// Behavior:
//
// Example:
//
// int result = xmos_dsp_math_reciprocal( sample, 28 );

int xmos_dsp_math_reciprocal( int input_value, int q_format );

// ================================================================================================

// Scalar inverse square root
//
// This function computes the reciprocal of the square root of the input value using an iterative
// approximation method.
//
// Parameters:
//
// 'input_value': Input value for computation.
// 'q_format':    Fixed point format, the number of bits making up fractional part.
//
// Return value:  Inverse square root of input value.
//
// Algorithm:
//
// 1) result = 1.0
// 2) result = result + result * (1 - input * result^2) / 2
// 3) Repeat step #2 until desired precision is achieved
//
// Behavior:
//
// Example:

int xmos_dsp_math_invsqrroot( int input_value, int q_format );

// ================================================================================================

// Scalar square root
//
// This function computes the square root of the input value.
//
// Parameters:
//
// 'input_value': Input value for computation.
// 'q_format':    Fixed point format, the number of bits making up fractional part.
//
// Return value:  Square root of input value.
//
// Algorithm:
//
// result = xmos_dsp_math_reciprocal( xmos_dsp_math_invsqrroot( input ))
//
// Behavior:
//
// Example:
//
// int result = xmos_dsp_math_squareroot( sample, 28 );

int xmos_dsp_math_squareroot( int input_value, int q_format );

// ================================================================================================

#endif
