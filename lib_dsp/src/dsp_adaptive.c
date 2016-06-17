// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <platform.h>
#include "dsp_qformat.h"
#include "dsp_math.h"
#include "dsp_filters.h"
#include "dsp_vector.h"
#include "dsp_statistics.h"
#include "dsp_adaptive.h"

/** This function implements a least-mean-squares adaptive FIR filter.
 *
 *  LMS filters are a class of adaptive filters that adjust filter coefficients
 *  in order to create the a transfer function that minimizes the error between
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
 *  and 64-bit accumulation as a result of using an FIR as well as coefficient
 *  step size calculations). 
 * 
 *  Multiplication results are accumulated in 64-bit accumulator with the final
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
 *                            Must be initialized at startup to all zero's.
 *  \param  tap_count         Filter tap count where ``N`` = ``tap_count`` = filter order + 1.
 *  \param  step_size         Coefficient adjustment step size, controls rate of convergence.
 *  \param  q_format          Fixed point format (i.e. number of fractional bits).
 *  \returns                  The resulting filter output sample.
 */

int32_t dsp_adaptive_lms
(
    int32_t  source_sample,
    int32_t  reference_sample,
    int32_t* error_sample,
    int32_t* filter_coeffs,
    int32_t* state_data,
    int32_t  tap_count,
    int32_t  step_size,
    int32_t  q_format
) {
    int32_t output_sample, mu_err;
    
    // Output signal y[n] is computed via standard FIR filter:
    // y[n] = b[0] * x[n] + b[1] * x[n-1] + b[2] * x[n-2] + ...+ b[N-1] * x[n-N+1]

    output_sample = dsp_filters_fir( source_sample, filter_coeffs, state_data, tap_count, q_format );
    
    // Error equals difference between reference and filter output:
    // e[n] = d[n] - y[n]

    *error_sample = reference_sample - output_sample;
    
    // FIR filter coefficients b[k] are updated on a sample-by-sample basis:
    // b[k] = b[k] + mu_err * x[n-k] --- where mu_err = e[n] * step_size
    
    mu_err = dsp_math_multiply( *error_sample, step_size, q_format );
    dsp_vector_muls_addv( state_data, mu_err, filter_coeffs, filter_coeffs, tap_count, q_format );
        
    return output_sample;
}

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
 *  3) Normalise the error using the instantaneous power computed by:
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
 *  and 64-bit accumulation as a result of using an FIR as well as coefficient
 *  step size calculations). 
 * 
 *  Multiplication results are accumulated in 64-bit accumulator with the final
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
 *                            Must be initialized at startup to all zero's.
 *  \param  tap_count         Filter tap count where N = tap_count = filter order + 1.
 *  \param  step_size         Coefficient adjustment step size, controls rate of convergence.
 *  \param  q_format          Fixed point format (i.e. number of fractional bits).
 *  \returns                  The resulting filter output sample.
 */

int32_t dsp_adaptive_nlms
(
    int32_t  source_sample,
    int32_t  reference_sample,
    int32_t* error_sample,
    int32_t* filter_coeffs,
    int32_t* state_data,
    int32_t  tap_count,
    int32_t  step_size,
    int32_t  q_format
) {
    int32_t output_sample, energy, adjustment, ee, qq;
    
    // Output signal y[n] is computed via standard FIR filter:
    // y[n] = b[0] * x[n] + b[1] * x[n-1] + b[2] * x[n-2] + ...+ b[N-1] * x[n-N+1]

    output_sample = dsp_filters_fir( source_sample, filter_coeffs, state_data, tap_count, q_format );
    
    // Error equals difference between reference and filter output:
    // e[n] = d[n] - y[n]

    *error_sample = reference_sample - output_sample;
    
    // Compute the instantaneous enegry: E = x[n]^2 + x[n-1]^2 + ... + x[n-N+1]^2
    energy = dsp_vector_power( state_data, tap_count, q_format );
    //printf( "E = %08x %f\n", energy, F31(energy) );
    
    // adjustment = error * mu / energy
    
    // Adjust energy q_format to account for range of reciprocal
    for( qq = q_format, ee = energy; qq >= 0 && !(ee & 0x80000000); --qq ) ee <<= 1;
    energy = energy >> (q_format - qq);
    // Saturate the reciprocal value to max value for the given q_format
    if( energy < (1 << (31-(31-qq)*2)) ) energy = (1 << (31-(31-qq)*2)) + 0;

    energy = dsp_math_divide( (1 << qq), energy, qq );
    adjustment = dsp_math_multiply( *error_sample, step_size, q_format );
    adjustment = dsp_math_multiply( energy, adjustment, qq + q_format - q_format );
    
    // FIR filter coefficients b[k] are updated on a sample-by-sample basis:
    // b[k] = b[k] + mu_err * x[n-k] --- where mu_err = e[n] * step_size
    
    dsp_vector_muls_addv( state_data, adjustment, filter_coeffs, filter_coeffs, tap_count, q_format );
        
    return output_sample;
}
