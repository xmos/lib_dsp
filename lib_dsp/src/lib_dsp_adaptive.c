// ============================================================================
// Copyright (c) 2015, XMOS Ltd, All rights reserved

#include <platform.h>
#include "lib_dsp_qformat.h"
#include "lib_dsp_math.h"
#include "lib_dsp_filters.h"
#include "lib_dsp_vector.h"
#include "lib_dsp_statistics.h"
#include "lib_dsp_adaptive.h"

// ============================================================================

// LMS filter
//
// 'input_sample':     The new sample to be processed.
// 'reference_sample': Reference sample.
// 'error_sample':     Pointer to resulting error sample (error = reference - output)
// 'filter_coeffs':    Pointer to FIR coefficients arranged as [b0,b1,b2, ...,bN-1].
// 'state_data':       Pointer to FIR filter state data array of length N.
//                     Must be initialized at startup to all zero's.
// 'tap_count':        Filter tap count where N = tap_count = filter order + 1.
// 'step_size':        Coefficient adjustment step size, controls rate of convergence.
// 'q_format':         Fixed point format, the number of bits making up fractional part.
//
// Return value:       Resulting filter output sample.

int lib_dsp_adaptive_lms
(
    int  source_sample,
    int  reference_sample,
    int* error_sample,
    int* filter_coeffs,
    int* state_data,
    int  tap_count,
    int  step_size,
    int  q_format
) {
    int output_sample, mu_err;
    
    // Output signal y[n] is computed via standard FIR filter:
    // y[n] = b[0] * x[n] + b[1] * x[n-1] + b[2] * x[n-2] + ...+ b[N-1] * x[n-N+1]

    output_sample = lib_dsp_filters_fir( source_sample, filter_coeffs, state_data, tap_count, q_format );
    
    // Error equals difference between reference and filter output:
    // e[n] = d[n] - y[n]

    *error_sample = reference_sample - output_sample;
    
    // FIR filter coefficients b[k] are updated on a sample-by-sample basis:
    // b[k] = b[k] + mu_err * x[n-k] --- where mu_err = e[n] * step_size
    
    mu_err = lib_dsp_math_multiply( *error_sample, step_size, q_format );
    lib_dsp_vector_muls_addv( state_data, mu_err, filter_coeffs, filter_coeffs, tap_count, q_format );
        
    return output_sample;
}

// ============================================================================

// Normalized LMS filter
//
// 'input_sample':     The new sample to be processed.
// 'reference_sample': Reference sample.
// 'error_sample':     Pointer to resulting error sample (error = reference - output)
// 'filter_coeffs':    Pointer to FIR coefficients arranged as [b0,b1,b2, ...,bN-1].
// 'state_data':       Pointer to FIR filter state data array of length N.
//                     Must be initialized at startup to all zero's.
// 'tap_count':        Filter tap count where N = tap_count = filter order + 1.
// 'step_size':        Coefficient adjustment step size, controls rate of convergence.
// 'q_format':         Fixed point format, the number of bits making up fractional part.
//
// Return value:       Resulting filter output sample.

int lib_dsp_adaptive_nlms
(
    int  source_sample,
    int  reference_sample,
    int* error_sample,
    int* filter_coeffs,
    int* state_data,
    int  tap_count,
    int  step_size,
    int  q_format
) {
    int output_sample, energy, adjustment, ee, qq;
    
    // Output signal y[n] is computed via standard FIR filter:
    // y[n] = b[0] * x[n] + b[1] * x[n-1] + b[2] * x[n-2] + ...+ b[N-1] * x[n-N+1]

    output_sample = lib_dsp_filters_fir( source_sample, filter_coeffs, state_data, tap_count, q_format );
    
    // Error equals difference between reference and filter output:
    // e[n] = d[n] - y[n]

    *error_sample = reference_sample - output_sample;
    
    // Compute the instantaneous enegry: E = x[n]^2 + x[n-1]^2 + ... + x[n-N+1]^2
    energy = lib_dsp_vector_power( state_data, tap_count, q_format );
    //printf( "E = %08x %f\n", energy, F31(energy) );
    
    // adjustment = error * mu / energy
    
    // Adjust energy q_format to account for range of reciprocal
    for( qq = q_format, ee = energy; qq >= 0 && !(ee & 0x80000000); --qq ) ee <<= 1;
    energy = energy >> (q_format - qq);
    // Saturate the reciprocal value to max value for the given q_format
    if( energy < (1 << (31-(31-qq)*2)) ) energy = (1 << (31-(31-qq)*2)) + 0;
    energy = lib_dsp_math_reciprocal( energy, qq );
    adjustment = lib_dsp_math_multiply( *error_sample, step_size, q_format );
    adjustment = lib_dsp_math_multiply( energy, adjustment, qq + q_format - q_format );
    
    // FIR filter coefficients b[k] are updated on a sample-by-sample basis:
    // b[k] = b[k] + mu_err * x[n-k] --- where mu_err = e[n] * step_size
    
    lib_dsp_vector_muls_addv( state_data, adjustment, filter_coeffs, filter_coeffs, tap_count, q_format );
        
    return output_sample;
}

// ============================================================================

