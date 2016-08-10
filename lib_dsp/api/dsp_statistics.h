// Copyright (c) 2015-2016, XMOS Ltd, All rights reserved

#ifndef DSP_STATISTICS_H_
#define DSP_STATISTICS_H_

#include "stdint.h"

/** Vector absolute sum: ``R = (abs(X[0]) + abs(X[1]) + ... + abs(X[N-1])``
 * 
 *  This function computes the absolute sum of all vector elements
 * 
 *  Due to successive 32-bit additions being accumulated using 64-bit
 *  arithmetic overflow during the summation process is unlikely. The final
 *  value, being effectively the result of a left-shift by ``q_format`` bits
 *  will potentially overflow the final fixed-point value depending on the
 *  resulting summed value and the chosen Q-format.
 * 
 *  Example:
 * 
 *  \code
 *  int32_t result;
 *  result = dsp_vector_abs_sum( input_vector, 256, 28 );
 *  \endcode
 * 
 *  \param  input_vector_X  Pointer to source data array X.
 *  \param  vector_length   Length of the input vector.
 *  \param  q_format        Fixed point format (i.e. number of fractional bits).
 */

int32_t dsp_vector_abs_sum
(
    const int32_t* input_vector_X,
    const int32_t  vector_length,
    const int32_t  q_format
);


/** Vector mean: ``R = (X[0] + X[1] + ... + X[N-1]) / N``
 * 
 *  This function computes the mean of the values contained within
 *  the input vector.
 * 
 *  Due to successive 32-bit additions being accumulated using 64-bit
 *  arithmetic overflow during the summation process is unlikely. The final
 *  value, being effectively the result of a left-shift by ``q_format`` bits
 *  will potentially overflow the final fixed-point value depending on the
 *  resulting summed value and the chosen Q-format.
 * 
 *  Example:
 * 
 *  \code
 *  int32_t result;
 *  result = dsp_vector_mean( input_vector, 256, 28 );
 *  \endcode
 * 
 *  \param  input_vector_X  Pointer to source data array X.
 *  \param  vector_length   Length of the input vector.
 *  \param  q_format        Fixed point format (i.e. number of fractional bits).
 */

int32_t dsp_vector_mean
(
    const int32_t input_vector_X[],
    const int32_t vector_length,
    const int32_t q_format
);

/** Vector power (sum of squares): ``R = X[0]^2 + X[1]^2 + ... + X[N-1]^2``
 * 
 *  This function computes the power (also know as the sum-of-squares) of the
 *  values contained within the input vector.
 * 
 *  Since each element in the vector is squared the behavior for fixed-point
 *  multiplication should be considered (see behavior for the function
 *  ``dsp_math_multiply``). Due to successive 32-bit additions being
 *  accumulated using 64-bit arithmetic overflow during the summation process
 *  is unlikely. The final value, being effectively the result of a left-shift
 *  by ``q_format`` bits will potentially overflow the final fixed-point value
 *  depending on the resulting summed value and the chosen Q-format.
 * 
 *  Example:
 * 
 *  \code
 *  int32_t result;
 *  result = dsp_vector_power( input_vector, 256, 28 );
 *  \endcode
 * 
 *  \param  input_vector_X  Pointer to source data array X.
 *  \param  vector_length   Length of the input vector.
 *  \param  q_format        Fixed point format (i.e. number of fractional bits).
 */

int32_t dsp_vector_power
(
    const int32_t input_vector_X[],
    const int32_t vector_length,
    const int32_t q_format
);

/** Vector root mean square: ``R = ((X[0]^2 + X[1]^2 + ... + X[N-1]^2) / N) ^ 0.5)``
 * 
 *  This function computes the root-mean-square (RMS) of the values contained
 *  within the input vector.
 * 
 *  \code
 *  result = 0
 *  for i = 0 to N-1: result += input_vector_X[i]
 *  return dsp_math_sqrt( result / vector_length )
 *  \endcode
 * 
 *  Since each element in the vector is squared the behavior for fixed-point
 *  multiplication should be considered (see behavior for the function
 *  ``dsp_math_multiply``). Due to successive 32-bit additions being
 *  accumulated using 64-bit arithmetic overflow during the summation
 *  process is unlikely. The squareroot of the 'sum-of-squares divided by N
 *  uses the function ``dsp_math_sqrt``; see behavior for that
 *  function. The final value, being effectively the result of a left-shift by
 *  ``q_format`` bits will potentially overflow the final fixed-point value
 *  depending on the resulting summed value and the chosen Q-format.
 * 
 *  Example:
 * 
 *  \code
 *  int32_t result;
 *  result = dsp_vector_rms( input_vector, 256, 28 );
 *  \endcode
 * 
 *  \param  input_vector_X  Pointer to source data array X.
 *  \param  vector_length   Length (N) of the input vector.
 *  \param  q_format        Fixed point format (i.e. number of fractional bits).
 */

int32_t dsp_vector_rms
(
    const int32_t input_vector_X[],
    const int32_t vector_length,
    const int32_t q_format
);

/** Vector dot product: ``R = X[0] * Y[0] + X[1] * Y[1] + ... + X[N-1] * Y[N-1]``
 * 
 *  This function computes the dot-product of two equal length vectors.
 * 
 *  The elements in the input vectors are multiplied before being summed
 *  therefore fixed-point multiplication behavior must be considered
 *  (see behavior for the function ``dsp_math_multiply``). Due to
 *  successive 32-bit additions being accumulated using 64-bit arithmetic
 *  overflow during the summation process is unlikely. The final value, being
 *  effectively the result of a left-shift by ``q_format`` bits will
 *  potentially overflow the final fixed-point value depending on the resulting
 *  summed value and the chosen Q-format.
 * 
 *  Example:
 * 
 *  \code
 *  int32_t result;
 *  result = dsp_vector_dotprod( input_vector, 256, 28 );
 *  \endcode
 * 
 *  \param  input_vector_X  Pointer to source data array X.
 *  \param  input_vector_Y  Pointer to source data array Y.
 *  \param  vector_length   Length of the input vectors.
 *  \param  q_format        Fixed point format (i.e. number of fractional bits).
 */

int32_t dsp_vector_dotprod
(
    const int32_t input_vector_X[],
    const int32_t input_vector_Y[],
    const int32_t vector_length,
    const int32_t q_format
);

#endif

