// Copyright (c) 2015-2016, XMOS Ltd, All rights reserved

#ifndef DSP_ADAPTIVE_H_
#define DSP_ADAPTIVE_H_

#include <stdint.h>

#ifdef __XC__
extern "C" {
#endif

/** This function implements a least-mean-squares adaptive FIR filter.
 *
 *  LMS filters are a class of adaptive filters that adjust filter coefficients
 *  in order to create a transfer function that minimizes the error between
 *  the input and reference signals. FIR coefficients are adjusted on a  per
 *  sample basis by an amount calculated from the given step size and the
 *  instantaneous error.
 * 
 *  The function operates on a single sample of input and output data (i.e. and
 *  each call to the function processes one sample and each call results in
 *  changes to the FIR coefficients).
 *  The general LMS algorithm, on a per sample basis, is to:
 *
 *  \code 
 *  1) Apply the transfer function: output = FIR( input )
 *  2) Compute the instantaneous error value: error = reference - output
 *  3) Compute current coefficient adjustment delta: delta = mu * error
 *  4) Adjust transfer function coefficients:
 *     FIR_COEFFS[n] = FIR_COEFFS[n] + FIR_STATE[n] * delta
 *  \endcode
 *
 *  Example of a 100-tap LMS filter with samples and coefficients represented
 *  in Q28 fixed-point format:
 * 
 *  \code
 *  int32_t filter_coeff[100] = { ... not shown for brevity };
 *  int32_t filter_state[100] = { 0, 0, 0, 0, ... not shown for brevity };
 *
 *  int32_t output_sample = dsp_adaptive_lms
 *  (
 *    input_sample, reference_sample, &error_sample,
 *    filter_coeff_array, filter_state_array, 100, Q28(0.01), 28
 *  );
 *  \endcode
 * 
 *  The LMS filter algorithm involves multiplication between two 32-bit values
 *  and 64-bit accumulation as a result of using a FIR filter as well as coefficient
 *  step size calculations. 
 * 
 *  Multiplication results are accumulated in a 64-bit accumulator with the final
 *  result shifted to the required fixed-point format. Therefore overflow
 *  behavior of the 32-bit multiply operation and truncation behavior from
 *  final shifing of the accumulated multiplication results must be considered
 *  for both FIR operations as well as for coefficient step size calculation
 *  and FIR coefficient adjustment.
 * 
 *  \param  input_sample      The new sample to be processed.
 *  \param  reference_sample  Reference sample.
 *  \param  error_sample      Pointer to resulting error sample (error = reference - output)
 *  \param  filter_coeffs     Pointer to FIR coefficients arranged as [b0,b1,b2, ...,bN-1].
 *  \param  state_data        Pointer to FIR filter state data array of length ``N``.
 *                            Must be initialized at startup to all zeros.
 *  \param  num_taps          Filter tap count where ``N`` = ``num_taps`` = filter order + 1.
 *  \param  mu                Coefficient adjustment step size, controls rate of convergence.
 *  \param  q_format          Fixed point format (i.e. number of fractional bits).
 *  \returns                  The resulting filter output sample.
 */

int32_t dsp_adaptive_lms
(
    int32_t input_sample,
    int32_t reference_sample,
    int32_t *error_sample,
    const int32_t filter_coeffs[],
    int32_t state_data[],
    const int32_t num_taps,
    const int32_t mu,
    int32_t q_format
);

/** This function implements a normalized LMS FIR filter. LMS filters are a class of
 *  adaptive filters that adjust filter coefficients in order to create the a transfer function that
 *  minimizes the error between the input and reference signals. FIR coefficients are adjusted on a 
 *  per sample basis by an amount calculated from the given step size and the instantaneous error.
 * 
 *  The function operates on a single sample of input and output data (i.e. and each call to the
 *  function processes one sample and each call results in changes to the FIR coefficients).
 * 
 *  The general NLMS algorithm, on a per sample basis, is to:
 *
 *  \code 
 *  1) Apply the transfer function: output = FIR( input )
 *  2) Compute the instantaneous error value: error = reference - output
 *  3) Normalize the error using the instantaneous power computed by:
 *     E = x[n]^2 + ... + x[n-N+1]^2
 *  4) Update error value:  error = (reference - output) / E
 *  5) Compute current coefficient adjustment delta: delta = mu * error
 *  6) Adjust transfer function coefficients:
 *     FIR_COEFFS[n] = FIR_COEFFS[n] + FIR_STATE[n] * delta
 *  \endcode
 * 
 *  Example of a 100-tap NLMS filter with samples and coefficients represented
 *  in Q28 fixed-point format:
 * 
 *  \code
 *  int32_t filter_coeff[100] = { ... not shown for brevity };
 *  int32_t filter_state[100] = { 0, 0, 0, 0, ... not shown for brevity };
 * 
 *  int32_t output_sample = dsp_adaptive_nlms
 *  (
 *    input_sample, reference_sample, &error_sample,
 *    filter_coeff_array, filter_state_array, 100, Q28(0.01), 28
 *  );
 *  \endcode
 * 
 *  The LMS filter algorithm involves multiplication between two 32-bit values
 *  and 64-bit accumulation as a result of using a FIR filter as well as coefficient
 *  step size calculations. 
 * 
 *  Multiplication results are accumulated in a 64-bit accumulator with the final
 *  result shifted to the required fixed-point format. Therefore overflow
 *  behavior of the 32-bit multiply operation and truncation behavior from
 *  final shifing of the accumulated multiplication results must be considered
 *  for both FIR operations as well as for coefficient step size calculation
 *  and FIR coefficient adjustment.
 * 
 *  Computing the coefficient adjustment involves taking the reciprocal of the
 *  instantaneous power computed by
 *  ``E = x[n]^2 + x[n-1]^2 + ... + x[n-N+1]^2.``
 *  The reciprocal is subject to overflow since the instantaneous power may be
 *  less than one.
 * 
 *  \param  input_sample      The new sample to be processed.
 *  \param  reference_sample  Reference sample.
 *  \param  error_sample      Pointer to resulting error sample (error = reference - output)
 *  \param  filter_coeffs     Pointer to FIR coefficients arranged as [b0,b1,b2, ...,bN-1].
 *  \param  state_data        Pointer to FIR filter state data array of length N.
 *                            Must be initialized at startup to all zeros.
 *  \param  num_taps          Filter tap count where N = num_taps = filter order + 1.
 *  \param  mu                Coefficient adjustment step size, controls rate of convergence.
 *  \param  q_format          Fixed point format (i.e. number of fractional bits).
 *  \returns                  The resulting filter output sample.
 */

int32_t dsp_adaptive_nlms
(
    int32_t input_sample,
    int32_t reference_sample,
    int32_t *error_sample,
    const int32_t filter_coeffs[],
    int32_t state_data[],
    const int32_t num_taps,
    const int32_t mu,
    int32_t q_format
);

#ifdef __XC__
}
#endif

#endif
