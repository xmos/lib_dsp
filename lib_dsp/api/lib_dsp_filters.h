// Copyright (c) 2015, XMOS Ltd, All rights reserved

#ifndef LIB_DSP_FILTERS
#define LIB_DSP_FILTERS

/** This function implements a Finite Impulse Response (FIR) filter.
 *  
 *  The function operates on a single sample of input and output data (i.e.
 *  each call to the function processes one sample).
 *
 *  The FIR filter algorithm is based upon a sequence of multiply-accumulate
 *  (MAC) operations. Each filter coefficient ``h[i]`` is multiplied by a state
 *  variable which equals a previous input sample ``x[i]``, or
 *  ``y[n]=x[n]*h[0]+x[n-1]*h[1]+x[n-2]*h[2]+x[n-N+1]*h[N-1]``
 *
 *  The parameter ``filter_coeffs`` points to a coefficient array of size
 *  N = ``num_taps``. The filter coefficients are stored in forward order
 *  (e.g. ``h[0],h[1],h[N-1]``).
 *
 *  The following example shows a five-tap (4th order) FIR filter with samples
 *  and coefficients represented in Q28 fixed-point format.
 *  \code
 *  int filter_coeff[5] = { Q28(0.5),Q(-0.5),Q28(0.0),Q28(-0.5),Q28(0.5) };
 *  int filter_state[4] = { 0, 0, 0, 0 };
 *  int result = lib_dsp_fir( sample, filter_coeff, filter_state, 5, 28 );
 *  \endcode
 *
 *  The FIR algorithm involves multiplication between 32-bit filter
 *  coefficients and 32-bit state data producing a 64-bit result for each
 *  coeffient and state data pair. Multiplication results
 *  are accumulated in 64-bit accumulator with the final result shifted
 *  to the required fixed-point format. Therefore overflow behavior of
 *  the 32-bit multiply operation and truncation behavior from final shifing
 *  of the accumulated multiplication results must be considered.
 *
 *  \param  input_sample    The new sample to be processed.
 *  \param  filter_coeffs   Pointer to FIR coefficients array arranged
 *                          as ``[b0,b1,b2,bN-1]``.
 *  \param  state_data      Pointer to filter state data array of length N.
 *                          Must be initialized at startup to all zero's.
 *  \param  tap_count       Filter tap count (N = ``tap_count`` = filter order + 1).
 *  \param q_format         Fixed point format (i.e. number of fractional bits).
 *  \returns                The resulting filter output sample.
 */
 
int lib_dsp_filters_fir
(
    int       input_sample,
    const int filter_coeffs[],
    int       state_data[],
    int       tap_count,
    int       q_format
);

/** This function implements an interpolating FIR filter.
 *
 *  The function operates on a single input sample and outputs a set of samples
 *  representing the interpolated data, whose sample count is equal to
 *  ``interp_factor``. (i.e. and each call to the function processes one sample
 *  and results in ``interp_factor`` output samples).
 * 
 *  The FIR filter algorithm is based upon a sequence of multiply-accumulate
 *  (MAC) operations. Each filter coefficient ``h[i]`` is multiplied by a state
 *  variable which equals a previous input sample ``x[i]``, or
 *  ``y[n]=x[n]*h[0]+x[n-1]*h[1]+x[n-2]*h[2]+x[n-N+1]*h[N-1]``
 * 
 *  ``filter_coeffs`` points to a coefficient array of size N = ``num_taps``.
 *  The filter coefficients
 *  are stored in forward order (e.g. ``h[0],h[1],h[N-1]``).
 * 
 *  The FIR algorithm involves multiplication between 32-bit filter
 *  coefficients and 32-bit state data producing a 64-bit result for each
 *  coefficient and state data pair. Multiplication results are accumulated in
 *  64-bit accumulater with the final result shifted to the required
 *  fixed-point format. Therefore overflow behavior of the 32-bit multiply
 *  operation and truncation behavior from final shifing of the accumulated
 *  multiplication results must be considered.
 * 
 *  \param input_sample    The new sample to be processed.
 *  \param filter_coeffs   Pointer to FIR coefficients array arranged as:
 *                         ``hM,h(1L+M),h(2L+M),h((N-1)L+M),``
 *                         ``h1,h(1L+1),h(2L+1),h((N-1)L+1),``
 *                         ``h0,h(1L+0),h(2L+0),h((N-1)L+0),``
 *                         where M = N-1
 *  \param state_data      Pointer to filter state data array of length N.
 *                         Must be initialized at startup to all zero's.
 *  \param  tap_count      Filter tap count (N = ``tap_count`` = filter order + 1).
 *  \param interp_factor   The interpolation factor/index (i.e. the up-sampling ratio).
 *                         The interpolation factor/index can range from 2 to 16.
 *  \param output_samples  The resulting interpolated samples.
 *  \param q_format        Fixed point format (i.e. number of fractional bits).
 */
 
void lib_dsp_filters_interpolate
(
    int       input_sample,
    const int filter_coeffs[],
    int       state_data[],
    int       tap_count,
    int       interp_factor,
    int       output_samples[],
    int       q_format
);

/** This function implements an decimating FIR filter.
 *
 *  The function operates on a single set of input samples whose count is equal
 *  to the decimation factor. (i.e. and each call to the function processes
 *  ``decim_factor`` samples and results in one sample).
 * 
 *  The FIR filter algorithm is based upon a sequence of multiply-accumulate
 *  (MAC) operations. Each filter coefficient ``h[i]`` is multiplied by a state
 *  variable which equals a previous input sample ``x[i]``, or
 *  ``y[n]=x[n]*h[0]+x[n-1]*h[1]+x[n-2]*h[2]...+x[n-N+1]*h[N-1]
 * 
 *  ``filter_coeffs`` points to a coefficient array of size N = ``num_taps``.
 *  The filter coefficients
 *  are stored in forward order (e.g. ``h[0],h[1],h[N-1]``).
 * 
 *  The FIR algorithm involves multiplication between 32-bit filter
 *  coefficients and 32-bit state data producing a 64-bit result for each
 *  coefficient and state data pair. Multiplication results are accumulated in
 *  64-bit accumulater with the final result shifted to the required
 *  fixed-point format. Therefore overflow behavior of the 32-bit multiply
 *  operation and truncation behavior from final shifing of the accumulated
 *  multiplication results must be considered.
 * 
 *  \param  input_samples  The new samples to be decimated.
 *  \param  filter_coeffs  Pointer to FIR coefficients array arranged
 *                         as ``[b0,b1,b2,bN-1]``.
 *  \param  state_data     Pointer to filter state data array of length N.
 *                         Must be initialized at startup to all zero's.
 *  \param  tap_count      Filter tap count (N = tap_count = filter order + 1).
 *  \param  decim_factor   The decimation factor/index (i.e. the down-sampling ratio).
 *  \param  q_format       Fixed point format (i.e. number of fractional bits).
 *  \returns               The resulting decimated sample.
 */
 
int lib_dsp_filters_decimate
(
    int       input_samples[],
    const int filter_coeffs[],
    int       state_data[],
    int       tap_count,
    int       decim_factor,
    int       q_format
);

/** This function implements a second order IIR (direct form I).
 *
 *  The function operates on a single sample of input and output data (i.e. and
 *  each call to the function processes one sample).
 * 
 *  The IIR filter algorithm is based upon a sequence of multiply-accumulate
 *  (MAC) operations. Each filter coefficient ``b[i]`` is multiplied by a state
 *  variable which equals a previous input sample ``x[i]``, or
 *  ``y[i]=x[n]*b[0]+x[n-1]*b[1]+x[n-2]*b2+x[n-1]*a[1]+x[n-2]*a[2]``
 * 
 *  The filter coefficients are stored in forward order (e.g. ``b0,b1,b2,a1,a2``).
 * 
 *  Example showing a single Biquad filter with samples and coefficients
 *  represented in Q28 fixed-point format:
 * 
 *  \code
 *  int filter_coeff[5] = { Q28(+0.5), Q(-0.1), Q28(-0.5), Q28(-0.1), Q28(0.1) };
 *  int filter_state[4] = { 0, 0, 0, 0 };
 *  int result = lib_dsp_biquad( sample, filter_coeff, filter_state, 28 );
 *  \endcode
 * 
 *  The IIR algorithm involves multiplication between 32-bit filter
 *  coefficients and 32-bit state data producing a 64-bit result for each
 *  coefficient and state data pair. Multiplication results are accumulated in
 *  64-bit accumulater with the final result shifted to the required fixed-point
 *  format. Therefore overflow behavior of the 32-bit multiply operation and
 *  truncation behavior from final shifing of the accumulated multiplication
 *  results must be considered.
 *
 *  \param  input_sample   The new sample to be processed.
 *  \param  filter_coeffs  Pointer to biquad coefficients array arranged as ``[b0,b1,b2,a1,a2]``.
 *  \param  state_data     Pointer to filter state data array (initialized at startup to zero's).
 *                         The length of the state data array is 4.
 *  \param  q_format       Fixed point format (i.e. number of fractional bits).
 *  \returns               The resulting filter output sample.
 */
 
int lib_dsp_filters_biquad
(
    int       input_sample,
    const int filter_coeffs[],
    int       state_data[],
    int       q_format
);

/** This function implements a cascaded direct form I BiQuad filter
 *
 *  The function operates on a single sample of input and output data
 * (i.e. and each call to the function processes one sample).
 * 
 *  The IIR filter algorithm is based upon a sequence of multiply-accumulate
 *  (MAC) operations. Each filter coefficient ``b[i]`` is multiplied by a state
 *  variable which equals a previous input sample ``x[i]``, or
 *  ``y[n]=x[n]*b[0]+x[n-1]*b[1]+x[n-2]*b2+x[n-1]*a[1]+x[n-2]*a[2]``
 * 
 *  The filter coefficients are stored in forward order
 *  (e.g. ``section1:b0,b1,b2,a1,a2,sectionN:b0,b1,b2,a1,a2``).
 * 
 *  Example showing a 4x cascaded Biquad filter with samples and coefficients
 *  represented in Q28 fixed-point format:
 *   
 *  \code
 *  int filter_coeff[20] = { Q28(+0.5), Q(-0.1), Q28(-0.5), Q28(-0.1), Q28(0.1),
 *                           Q28(+0.5), Q(-0.1), Q28(-0.5), Q28(-0.1), Q28(0.1),
 *                           Q28(+0.5), Q(-0.1), Q28(-0.5), Q28(-0.1), Q28(0.1),
 *                           Q28(+0.5), Q(-0.1), Q28(-0.5), Q28(-0.1), Q28(0.1) };
 *  int filter_state[16] = { 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 };
 *  int result = lib_dsp_cascaded_biquad( sample, filter_coeff, filter_state, 4, 28 );
 *  \endcode
 * 
 *  The IIR algorithm involves multiplication between 32-bit filter
 *  coefficients and 32-bit state data producing a 64-bit result for each
 *  coefficient and state data pair. Multiplication results are accumulated in
 *  64-bit accumulater with the final result shifted to the required fixed-point
 *  format. Therefore overflow behavior of the 32-bit multiply operation and
 *  truncation behavior from final shifing of the accumulated multiplication
 *  results must be considered.
 * 
 *  \param  input_sample   The new sample to be processed.
 *  \param  filter_coeffs  Pointer to biquad coefficients array for all BiQuad sections.
 *                         Arranged as ``[section1:b0,b1,b2,a1,a2,...sectionN:b0,b1,b2,a1,a2]``.
 *  \param  state_data     Pointer to filter state data array (initialized at startup to zero's).
 *                         The length of the state data array is ``num_sections`` * 4.
 *  \param  num_sections   Number of BiQuad sections.
 *  \param  q_format       Fixed point format (i.e. number of fractional bits).
 *  \returns               The resulting filter output sample.
 */
 
int lib_dsp_filters_biquads
(
    int       input_sample,
    const int filter_coeffs[],
    int       state_data[],
    int       num_sections,
    int       q_format
);

// ============================================================================

#endif
