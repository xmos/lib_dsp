#include <platform.h>
#include "xmos_dsp_qformat.h"
#include "xmos_dsp_math.h"
#include "xmos_dsp_filters.h"
#include "xmos_dsp_vector.h"
#include "xmos_dsp_statistics.h"
#include "xmos_dsp_adaptive.h"

// LMS filter
//
// 'input_sample':     The new sample to be processed.
// 'reference_sample': Reference sample
// 'error_sample':     Pointer to resulting error sample (error = reference - output)
// 'filter_coeffs':    Pointer to FIR coefficients arranged as [b0,b1,b2, ...,bN-1].
// 'state_data':       Pointer to FIR filter state data array of length N.
//                     Must be initialized at startup to all zero's.
// 'tap_count':        Filter tap count where N = tap_count = filter order + 1.
// 'step_size':        Coefficient adjustment step size, controls rate of convergence.
// 'q_format':         Fixed point format, the number of bits making up fractional part.
//
// return value:    Resulting filter output sample.

int xmos_dsp_adaptive_lms
(
    int  input_sample,
    int  reference_sample,
    int* error_sample,
    int* filter_coeffs,
    int* state_data,
    int  tap_count,
    int  step_size,
    int  q_format
)
{
    int output_sample, mu_err;
    
    // Output signal y[n] is computed via standard FIR filter:
    // y[n] = b[0] * x[n] + b[1] * x[n-1] + b[2] * x[n-2] + ...+ b[N-1] * x[n-N+1]

    output_sample = xmos_dsp_filters_fir( input_sample, filter_coeffs, state_data, tap_count, q_format );
    
    // Error equals difference between reference and filter output:
    // e[n] = d[n] - y[n]

    *error_sample = reference_sample - output_sample;
    
    // FIR filter coefficients b[k] are updated on a sample-by-sample basis:
    // b[k] = b[k] + mu_err * x[n-k] --- where mu_err = e[n] * step_size
    
    mu_err = xmos_dsp_math_multiply( *error_sample, step_size, q_format );
    xmos_dsp_vector_muls_addv( filter_coeffs, state_data, mu_err, filter_coeffs, tap_count, q_format );
        
    return output_sample;
}

// Normalized LMS filter
//
// 'input_sample':     The new sample to be processed.
// 'reference_sample': Reference sample
// 'error_sample':     Pointer to resulting error sample (error = reference - output)
// 'filter_coeffs':    Pointer to FIR coefficients arranged as [b0,b1,b2, ...,bN-1].
// 'state_data':       Pointer to FIR filter state data array of length N.
//                     Must be initialized at startup to all zero's.
// 'tap_count':        Filter tap count where N = tap_count = filter order + 1.
// 'step_size':        Coefficient adjustment step size, controls rate of convergence.
// 'q_format':         Fixed point format, the number of bits making up fractional part.
//
// return value:    Resulting filter output sample.

int xmos_dsp_adaptive_nlms
(
    int  input_sample,
    int  reference_sample,
    int* error_sample,
    int* filter_coeffs,
    int* state_data,
    int  tap_count,
    int  step_size,
    int  q_format
)
{
    int output_sample, mu_err_egy, energy;
    
    // Output signal y[n] is computed via standard FIR filter:
    // y[n] = b[0] * x[n] + b[1] * x[n-1] + b[2] * x[n-2] + ...+ b[N-1] * x[n-N+1]

    output_sample = xmos_dsp_filters_fir( input_sample, filter_coeffs, state_data, tap_count, q_format );
    
    // Error equals difference between reference and filter output:
    // e[n] = d[n] - y[n]

    *error_sample = reference_sample - output_sample;
    
    // Compute the instantaneous enegry: E = x[n]^2 + x[n-1]^2 + ... + x[n-N+1]^2
    energy = xmos_dsp_vector_power( state_data, tap_count, q_format );
    
    // mu_err_egy = error * mu / energy
    energy     = xmos_dsp_math_reciprocal( energy, q_format );
    mu_err_egy = xmos_dsp_math_multiply  ( *error_sample, step_size, q_format );
    mu_err_egy = xmos_dsp_math_multiply  ( energy, mu_err_egy, q_format );
    
    // FIR filter coefficients b[k] are updated on a sample-by-sample basis:
    // b[k] = b[k] + mu_err * x[n-k] --- where mu_err = e[n] * step_size
    
    xmos_dsp_vector_muls_addv( filter_coeffs, state_data, mu_err_egy, filter_coeffs, tap_count, q_format );
        
    return output_sample;
}

