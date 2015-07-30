// ================================================================================================

#ifndef XMOS_DSP_FILTERS
#define XMOS_DSP_FILTERS

// ================================================================================================

// FIR filter
//
// This function implements the Finite Impulse Response (FIR). The function operates on a single
// sample of input and output data (i.e. and each call to the function processes one sample).
//
// Parameters:
//
// 'input_sample':  The new sample to be processed.
// 'filter_coeffs': Pointer to FIR coefficients array arranged as [b0,b1,b2, ...,bN-1].
// 'state_data':    Pointer to filter state data array of length N.
//                  Must be initialized at startup to all zero's.
// 'tap_count':     Filter tap count (N = tap_count = filter order + 1).
// 'q_format':      Fixed point format, the number of bits making up fractional part.
//
// Return value:    Resulting filter output sample.
//
// Algorithm:
//
// The FIR filter algorithm is based upon a sequence of multiply-accumulate (MAC) operations.
// Each filter coefficient h[i] is multiplied by a state variable which equals a previous
// input sample x[i]::
//  
//   y[n] = x[n] * h[0] + x[n-1] * h[1] + x[n-2] * h[2] ... + x[n-N+1] * h[N-1]
//  
// 'filter_coeffs' points to a coefficient array of size N = 'num_taps'.
// The filter coefficients are stored in forward order (e.g. h[0], h[1], ..., h[N-1]).
//
// Behavior:
//
// The FIR algorithm involves multiplication between 32-bit filter coefficients and 32-bit state
// data producing a 64-bit result for each coeffient and state data pair. Multiplication results
// are accumulated in 64-bit accumulater with the final result shifted to the required fixed-point
// format. Therefore overflow behavior of the 32-bit multiply operation and truncation behavior
// from final shifing of the accumulated multiplication results must be considered.
//
// Example:
//
// Five-tap (4th order) FIR filter with samples and coefficients represented in
// Q28 fixed-point format.
//
//   int filter_coeff[5] = { Q28(0.5), Q(-0.5), Q28(0.0), Q28(-0.5), Q28(0.5) };
//   int filter_state[4] = { 0, 0, 0, 0 };
//
//   int result = xmos_dsp_fir( sample, filter_coeff, filter_state, 5, 28 );

int xmos_dsp_filters_fir
(
    int       input_sample,
    const int filter_coeffs[],
    int       state_data[],
    int       tap_count,
    int       q_format
);

// ================================================================================================

// Interpolating FIR filter
//
// This function implements an interpolating Finite Impulse Response (FIR). The function operates
// on a single input sample and outputs a set of samples representing the interpolated data, whose
// sample count is equal to 'interp_factor'. (i.e. and each call to the function processes one
// sample and result in 'interp_factor' output samples).
//
// Parameters:
//
// 'input_sample':   The new sample to be processed.
// 'filter_coeffs':  Pointer to FIR coefficients array arranged as:
//                      hM,h(1L+M),h(2L+M),...h((N-1)L+M),
//                      ...
//                      h1,h(1L+1),h(2L+1),...h((N-1)L+1),
//                      h0,h(1L+0),h(2L+0),...h((N-1)L+0),
//                   where M = N-1
// 'state_data':     Pointer to filter state data array of length N.
//                   Must be initialized at startup to all zero's.
// 'phase_length':   Phase length (N), FIR filter tap count = N * interp_factor.
// 'interp_factor':  The interpolation factor/index (i.e. the up-sampling ratio).
//                   The interpolation factor/index can range from 2 to 16.
// 'output_samples': The resulting interpolated samples.
// 'q_format':       Fixed point format, the number of bits making up fractional part.
//
// Return value:     Resulting filter output sample.
//
// Algorithm:
//
// The FIR filter algorithm is based upon a sequence of multiply-accumulate (MAC) operations.
// Each filter coefficient h[i] is multiplied by a state variable which equals a previous
// input sample x[i]::
//
//   y[n] = x[n] * h[0] + x[n-1] * h[1] + x[n-2] * h[2] ... + x[n-N+1] * h[N-1]
//
// 'filter_coeffs' points to a coefficient array of size N = 'num_taps'.
// The filter coefficients are stored in forward order (e.g. h[0], h[1], ..., h[N-1]).
//
// Behavior:
//
// The FIR algorithm involves multiplication between 32-bit filter coefficients and 32-bit state
// data producing a 64-bit result for each coeffient and state data pair. Multiplication results
// are accumulated in 64-bit accumulater with the final result shifted to the required fixed-point
// format. Therefore overflow behavior of the 32-bit multiply operation and truncation behavior
// from final shifing of the accumulated multiplication results must be considered.

void xmos_dsp_filters_interpolate
(
    int       input_sample,
    const int filter_coeffs[],
    int       state_data[],
    int       tap_count,
    int       interp_factor,
    int       output_samples[],
    int       q_format
);

// ================================================================================================

// Decimating FIR filter
//
// This function implements an decimating Finite Impulse Response (FIR). The function operates on a
// single set of input samples whose count is equal to the decimation factor. (i.e. and each call
// to the function processes 'decim_factor' samples and results in one sample).
//
// Parameters:
//
// 'input_samples': The new samples to be decimated.
// 'filter_coeffs': Pointer to FIR coefficients array arranged as [b0,b1,b2, ...,bN-1].
// 'state_data':    Pointer to filter state data array of length N.
//                  Must be initialized at startup to all zero's.
// 'tap_count':     Filter tap count (N = tap_count = filter order + 1).
// 'decim_factor':  The decimation factor/index (i.e. the down-sampling ratio).
// 'q_format':      Fixed point format, the number of bits making up fractional part.
//
// Return value:    The resulting decimated sample.
//
// Algorithm:
//
// The FIR filter algorithm is based upon a sequence of multiply-accumulate (MAC) operations.
// Each filter coefficient h[i] is multiplied by a state variable which equals a previous
// input sample x[i]::
//
//   y[n] = x[n] * h[0] + x[n-1] * h[1] + x[n-2] * h[2] ... + x[n-N+1] * h[N-1]
//
// 'filter_coeffs' points to a coefficient array of size N = 'num_taps'.
// The filter coefficients are stored in forward order (e.g. h[0], h[1], ..., h[N-1]).
//
// Behavior:
//
// The FIR algorithm involves multiplication between 32-bit filter coefficients and 32-bit state
// data producing a 64-bit result for each coeffient and state data pair. Multiplication results
// are accumulated in 64-bit accumulater with the final result shifted to the required fixed-point
// format Therefore overflow behavior of the 32-bit multiply operation and truncation behavior from
// final shifing of the accumulated multiplication results must be considered.

int xmos_dsp_filters_decimate
(
    int       input_samples[],
    const int filter_coeffs[],
    int       state_data[],
    int       tap_count,
    int       decim_factor,
    int       q_format
);

// ================================================================================================

// Biquadratic IIR filter (direct form I)
//
// This function implements a second order Infinite Impulse Response (IIR) direct form I.
// The function operates on a single sample of input and output data (i.e. and each call to the
// function processes one sample).
//
// Parameters:
//
// 'input_sample':  The new sample to be processed.
// 'filter_coeffs': Pointer to biquad coefficients array arranged as [b0,b1,b2,a1,a2].
// 'state_data':    Pointer to filter state data array (initialized at startup to zero's).
//                  The length of the state data array is 4.
// 'q_format':      Fixed point format, number of bits making up fractional part.
//
// Return value:    Resulting filter output sample.
//
// Algorithm:
//
// The IIR filter algorithm is based upon a sequence of multiply-accumulate (MAC) operations.
// Each filter coefficient b[i] is multiplied by a state variable which equals a previous
// input sample x[i]::
//
//   y[i] = x[n] * b[0] + x[n-1] * b[1] + x[n-2] * b2 + x[n-1] * a[1] + x[n-2] * a[2]
//
// The filter coefficients are stored in forward order (e.g. b0, b1, b2, a1, a2).
//
// Behavior:
//
// The IIR algorithm involves multiplication between 32-bit filter coefficients and 32-bit state
// data producing a 64-bit result for each coeffient and state data pair. Multiplication results
// are accumulated in 64-bit accumulater with the final result shifted to the required fixed-point
// format. Therefore overflow behavior of the 32-bit multiply operation and truncation behavior
// from final shifing of the accumulated multiplication results must be considered.
//
// Example:
//
// Single Biquad filter with samples and coefficients represented in Q28 fixed-point format.
//
//   int filter_coeff[5] = { Q28(+0.5), Q(-0.1), Q28(-0.5), Q28(-0.1), Q28(0.1) };
//   int filter_state[4] = { 0, 0, 0, 0 };
//
//   int result = xmos_dsp_biquad( sample, filter_coeff, filter_state, 28 );

int xmos_dsp_filters_biquad
(
    int       input_sample,
    const int filter_coeffs[],
    int       state_data[],
    int       q_format
);

// ================================================================================================

// Cascaded biquadratic IIR filter (direct form I)
//
// This function implements multiple second order Infinite Impulse Response (IIR) direct form I
// filters in series (cascaded Biquads). The function operates on a single sample of input and
// output data (i.e. and each call to the function processes one sample).
//
// Parameters:
//
// 'input_sample':  The new sample to be processed.
// 'filter_coeffs': Pointer to biquad coefficients array for all BiQuad sections.
//                  Arranged as [section 1: b0,b1,b2,a1,a2, ... section N: b0,b1,b2,a1,a2].
// 'state_data':    Pointer to filter state data array (initialized at startup to zero's).
//                  The length of the state data array is 'num_sections' * 4.
// 'num_sections':  Number of BiQuad sections.
// 'q_format':      Fixed point format, the number of bits making up fractional part.
//
// Return value:    Resulting filter output sample.
//
// Algorithm:
//
// The IIR filter algorithm is based upon a sequence of multiply-accumulate (MAC) operations.
// Each filter coefficient b[i] is multiplied by a state variable which equals a previous
// input sample x[i]::
//
//   y[n] = x[n] * b[0] + x[n-1] * b[1] + x[n-2] * b2 + x[n-1] * a[1] + x[n-2] * a[2]
//
// The filter coefficients are stored in forward order
// (e.g. section 1: b0, b1, b2, a1, a2, ..., section N: b0, b1, b2, a1, a2).
//
// Behavior:
//
// The IIR algorithm involves multiplication between 32-bit filter coefficients and 32-bit state
// data producing a 64-bit result for each coeffient and state data pair. Multiplication results
// are accumulated in 64-bit accumulater with the final result shifted to the required fixed-point
// format. Therefore overflow behavior of the 32-bit multiply operation and truncation behavior
// from final shifing of the accumulated multiplication results must be considered.
//
// Example:
//
// 4x Cascaded Biquad filter with samples and coefficients represented in Q28 fixed-point format.
//  
//   int filter_coeff[20] = { Q28(+0.5), Q(-0.1), Q28(-0.5), Q28(-0.1), Q28(0.1),
//                            Q28(+0.5), Q(-0.1), Q28(-0.5), Q28(-0.1), Q28(0.1),
//                            Q28(+0.5), Q(-0.1), Q28(-0.5), Q28(-0.1), Q28(0.1),
//                            Q28(+0.5), Q(-0.1), Q28(-0.5), Q28(-0.1), Q28(0.1) };
//   int filter_state[16] = { 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 };
//
//   int result = xmos_dsp_cascaded_biquad( sample, filter_coeff, filter_state, 4, 28 );

int xmos_dsp_filters_biquads
(
    int       input_sample,
    const int filter_coeffs[],
    int       state_data[],
    int       num_sections,
    int       q_format
);

// ================================================================================================

#endif
