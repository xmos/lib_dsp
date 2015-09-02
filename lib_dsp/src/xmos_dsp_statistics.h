// Copyright (c) 2015, XMOS Ltd, All rights reserved
// ================================================================================================

#ifndef XMOS_DSP_STATISTICS
#define XMOS_DSP_STATISTICS

// ================================================================================================

// Vector mean: result = (X[0] + ... X[N-1]) / N
//
// This function computes the mean of the values contained within the input vector.
//
// Parameters:
//
// 'input_vector_X': Pointer to source data array X.
// 'vector_length':  Length of the input vector.
// 'q_format':       Fixed point format, the number of bits making up fractional part.
//
// Algorithm:
//
//  result = 0
//  for i = 0 to N-1: result += input_vector_X[i]
//  return result / vector_length
//
// Behavior:
//
// Due to successive 32-bit additions being accumulated using 64-bit arithmetic overflow during the
// summation process is unlikely. The final value, being effectively the result of a left-shift by
// 'q_format' bits will potentially overflow the final fixed-point value depending on the resulting
// summed value and the chosen Q-format.
//
// Example:
//
//  int result = xmos_dsp_vector_mean( input_vector, 256, 28 );

int xmos_dsp_vector_mean
(
    const int input_vector_X[],
    int       vector_length,
    int       q_format
);

// ================================================================================================

// Vector power (sum of squares): result = X[0]^2 + ... X[N-1]^2
//
// This function computes the power (also know as the sum-of-squares) of the values contained
// within the input vector.
//
// Parameters:
//
// 'input_vector_X': Pointer to source data array X.
// 'vector_length':  Length of the input vector.
// 'q_format':       Fixed point format, the number of bits making up fractional part.
//
// Algorithm:
//
//  result = 0
//  for i = 0 to N-1: result += input_vector_X[i] ^ 2
//  return result
//
// Behavior:
//
// Since each element in the vector is squared the behavior for fixed-point multiplication should
// be considered (see behavior for the function 'xmos_dsp_math_multiply').
//
// Due to successive 32-bit additions being accumulated using 64-bit arithmetic overflow during the
// summation process is unlikely. The final value, being effectively the result of a left-shift by
// 'q_format' bits will potentially overflow the final fixed-point value depending on the resulting
// summed value and the chosen Q-format.
//
// Example:
//
//  int result = xmos_dsp_vector_power( input_vector, 256, 28 );

int xmos_dsp_vector_power
(
    const int input_vector_X[],
    int       vector_length,
    int       q_format
);

// ================================================================================================

// Vector root mean square: result = ((X[0]^2 + ... + X[N-1]^2) / N) ^ 0.5)
//
// This function computes the root-mean-square (RMS) of the values contained within the
// input vector.
//
// Parameters:
//
// 'input_vector_X': Pointer to source data array X.
// 'vector_length':  Length (N) of the input vector.
// 'q_format':       Fixed point format, the number of bits making up fractional part.
//
// Algorithm:
//
//  result = 0
//  for i = 0 to N-1: result += input_vector_X[i]
//  return xmos_dsp_math_squareroot( result / vector_length )
//
// Behavior:
//
// Since each element in the vector is squared the behavior for fixed-point multiplication should be
// considered (see behavior for the function 'xmos_dsp_math_multiply').
//
// Due to successive 32-bit additions being accumulated using 64-bit arithmetic overflow during the
// summation process is unlikely.
//
// The squareroot of the 'sum-of-squares divided by N value' uses the function
// 'xmos_dsp_math_squareroot'; see behavior for that function.
//
// The final value, being effectively the result of a left-shift by
// 'q_format' bits will potentially overflow the final fixed-point value depending on the resulting
// summed value and the chosen Q-format.
//
// Example:
//
//  int result = xmos_dsp_vector_rms( input_vector, 256, 28 );

int xmos_dsp_vector_rms
(
    const int input_vector_X[],
    int       vector_length,
    int       q_format
);

// ================================================================================================

// Vector dot product: result = X[0] * Y[0] + ... X[N-1] * Y[N-1]
//
// This function computes the dot-product of two equal length vectors.
//
// Parameters:
//
// 'input_vector_X': Pointer to source data array X.
// 'input_vector_Y': Pointer to source data array Y.
// 'vector_length':  Length of the input vectors.
// 'q_format':       Fixed point format, the number of bits making up fractional part.
//
// Algorithm:
//
//  result = 0
//  for i = 0 to N-1: result += input_vector_X[i] * input_vector_Y[i]
//  return result
//
// Behavior:
//
// The elements in the input vectors are multiplied before being summed therefore fixed-point
// multiplication behavior must be considered (see behavior for the function
// 'xmos_dsp_math_multiply').

// Due to successive 32-bit additions being accumulated using 64-bit arithmetic overflow during the
// summation process is unlikely. The final value, being effectively the result of a left-shift by
// 'q_format' bits will potentially overflow the final fixed-point value depending on the resulting
// summed value and the chosen Q-format.
//
// Example:
//
//  int result = xmos_dsp_vector_dotprod( input_vector, 256, 28 );

int xmos_dsp_vector_dotprod
(
    const int input_vector_X[],
    const int input_vector_Y[],
    int       vector_length,
    int       q_format
);

// ================================================================================================

#endif

