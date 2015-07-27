// ================================================================================================

#ifndef XMOS_DSP_ADAPTIVE
#define XMOS_DSP_ADAPTIVE

// ================================================================================================

// LMS filter
//
// This function implements a least-mean-squares adaptive FIR filter. LMS filters are a class of
// adaptive filters that adjust filter coefficients in order to create the a transfer function that
// minimizes the error between the input and reference signals. FIR coefficients are adjusted on a 
// per sample basis by an amount calculated from the given step size and the instantaneous error.
//
// The function operates on a single sample of input and output data (i.e. and each call to the
// function processes one sample and each call results in changes to the FIR coefficients).
//
// Parameters:
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
//
// Algorithm:
//
// The general LMS algorithm, on a per sample basis, is to:
//
// 1) Apply the transfer function: output = FIR( input )
// 2) Compute the instantaneous error value: error = reference - output
// 3) Compute current coefficient adjustment delta: delta = mu * error
// 4) Adjust transfer function coefficients: FIR_COEFFS[n] = FIR_COEFFS[n] + FIR_STATE[n] * delta
//
// Behavior:
//
// The LMS filter algorithm involves multiplication between two 32-bit values and 64-bit
// accumulation as a result of using an FIR as well as coefficient step size calculations).  
// Multiplication results are accumulated in 64-bit accumulater with the final result shifted
// to the required fixed-point format. Therefore overflow behavior of the 32-bit multiply operation
// and truncation behavior from final shifing of the accumulated multiplication results must be
// considered for both FIR operations as well as for coefficient step size calculation and FIR
// coefficient adjustment.
//
// Example:
//
// 100-tap LMS filter with samples and coefficients represented in Q28 fixed-point format
//
//   int filter_coeff[100] = { ... not shown for brevity };
//   int filter_state[100] = { 0, 0, 0, 0, ... not shown for brevity };
//
//   int output_sample = xmos_dsp_adaptive_lms
//   (
//     input_sample, reference_sample, &error_sample,
//     filter_coeff_array, filter_state_array, 100, Q28(0.01), 28
//   );

int xmos_dsp_adaptive_lms
(
    int source_sample,
    int reference_sample,
    int error_sample[],
    int filter_coeffs[],
    int state_data[],
    int tap_count,
    int step_size,
    int q_format
);

// ================================================================================================

// Normalized LMS filter
//
// This function implements a normalized LMS FIR filter. LMS filters are a class of
// adaptive filters that adjust filter coefficients in order to create the a transfer function that
// minimizes the error between the input and reference signals. FIR coefficients are adjusted on a 
// per sample basis by an amount calculated from the given step size and the instantaneous error.
//
// The function operates on a single sample of input and output data (i.e. and each call to the
// function processes one sample and each call results in changes to the FIR coefficients).
//
// Parameters:
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
//
// Algorithm:
//
// The general NLMS algorithm, on a per sample basis, is to:
//
// 1) Apply the transfer function: output = FIR( input )
// 2) Compute the instantaneous error value: error = reference - output
// 3) Normalise the error using the instantaneous power computed by: E = x[n]^2 + ... + x[n-N+1]^2
// 4) Compute current coefficient adjustment delta: delta = mu * error
// 5) Adjust transfer function coefficients: FIR_COEFFS[n] = FIR_COEFFS[n] + FIR_STATE[n] * delta
//
// Behavior:
//
// The NLMS filter algorithm involves multiplication between two 32-bit values and 64-bit
// accumulation as a result of using an FIR as well as coefficient step size calculations).  
// Multiplication results are accumulated in 64-bit accumulater with the final result shifted
// to the required fixed-point format. Therefore overflow behavior of the 32-bit multiply operation
// and truncation behavior from final shifing of the accumulated multiplication results must be
// considered for both FIR operations as well as for coefficient step size calculation and FIR
// coefficient adjustment.
//
// Computing the coeficient adjustment involves taking the reciprocal of the instantaneous power
// computed by E = x[n]^2 + x[n-1]^2 + ... + x[n-N+1]^2.  The reciprocal is subject to overflow
// since the instantaneous power may be less than one.
//
// Example:
//
// 100-tap LMS filter with samples and coefficients represented in Q28 fixed-point format
//
//   int filter_coeff[100] = { ... not shown for brevity };
//   int filter_state[100] = { 0, 0, 0, 0, ... not shown for brevity };
//
//   int output_sample = xmos_dsp_adaptive_lms
//   (
//     input_sample, reference_sample, &error_sample,
//     filter_coeff_array, filter_state_array, 100, Q28(0.01), 28
//   );

int xmos_dsp_adaptive_nlms
(
    int source_sample,
    int reference_sample,
    int error_sample[],
    int filter_coeffs[],
    int state_data[],
    int tap_count,
    int step_size,
    int q_format
);

// ================================================================================================

#endif
