// Copyright (c) 2015-2016, XMOS Ltd, All rights reserved

#ifndef DSP_FILTERS_H_
#define DSP_FILTERS_H_

#include "stdint.h"

#define DSP_NUM_COEFFS_PER_BIQUAD 5  // Number of coefficients per biquad
#define DSP_NUM_STATES_PER_BIQUAD 4  // Number of state values per biquad

/** This function implements a Finite Impulse Response (FIR) filter.
 *  
 *  The function operates on a single sample of input and output data (i.e.
 *  each call to the function processes one sample).
 *
 *  The FIR filter algorithm is based upon a sequence of multiply-accumulate
 *  (MAC) operations. Each filter coefficient ``b[i]`` is multiplied by a state
 *  variable which equals a previous input sample, or
 *  ``y[n] = x[n]*b0 + x[n-1]*b1 + x[n-2]*b2 + ... + x[n-N+1]*bN-1``
 *
 *  The parameter ``filter_coeffs`` points to a coefficient array of size
 *  N = ``num_taps``. The filter coefficients are stored in forward order
 *  (e.g. ``b0,b1,...,bN-1``).
 *
 *  The following example shows a five-tap (4th order) FIR filter with samples
 *  and coefficients represented in Q28 fixed-point format.
 *  \code
 *  int32_t filter_coeff[5] = { Q28(0.5),Q28(-0.5),Q28(0.0),Q28(-0.5),Q28(0.5) };
 *  int32_t filter_state[4] = { 0, 0, 0, 0 };
 *  int32_t result = dsp_filters_fir( sample, filter_coeff, filter_state, 5, 28 );
 *  \endcode
 *
 *  The FIR algorithm involves multiplication between 32-bit filter
 *  coefficients and 32-bit state data producing a 64-bit result for each
 *  coeffient and state data pair. 
 *  Multiplication results are accumulated in a 64-bit accumulator. 
 *  If overflow occurs in the final 64-bit result, it is saturated at the minimum/maximum value 
 *  given the fixed-point format and finally shifted right by ``q_format`` bits.
 *  The saturation is only done after the last multiplication.
 *  To avoid 64-bit overflow in the intermediate results, the fixed point format must be chosen 
 *  according to num_taps.
 *
 *  \param  input_sample    The new sample to be processed.
 *  \param  filter_coeffs   Pointer to FIR coefficients array arranged
 *                          as ``[b0,b1,b2,...,bN-1]``.
 *  \param  state_data      Pointer to filter state data array of length N-1.
 *                          Must be initialized at startup to all zeros.
 *  \param  num_taps        Number of filter taps (N = ``num_taps`` = filter order + 1).
 *  \param  q_format        Fixed point format (i.e. number of fractional bits).
 *  \returns                The resulting filter output sample.
 */
 
int32_t dsp_filters_fir
(
    int32_t       input_sample,
    const int32_t filter_coeffs[],
    int32_t       state_data[],
    const int32_t num_taps,
    const int32_t q_format
);

/** This function implements an interpolating FIR filter.
 *
 *  The function operates on a single input sample and outputs a set of samples
 *  representing the interpolated data, whose sample count is equal to
 *  ``interp_factor``. (i.e. and each call to the function processes one sample
 *  and results in ``interp_factor`` output samples).
 * 
 *  The FIR filter algorithm is based upon a sequence of multiply-accumulate
 *  (MAC) operations. Each filter coefficient ``b[i]`` is multiplied by a state
 *  variable which equals a previous input sample, or
 *  ``y[n] = x[n]*b0 + x[n-1]*b1 + x[n-2]*b2 + ... + x[n-N+1]*bN-1``
 * 
 *  ``filter_coeffs`` points to a coefficient array of size N = ``num_taps``.
 *  The filter coefficients
 *  are stored in forward order (e.g. ``b0,b1,...,bN-1``).
 * 
 *  Multiplication results are accumulated in a 64-bit accumulator. 
 *  If overflow occurs in the final 64-bit result, it is saturated at the minimum/maximum value 
 *  given the fixed-point format and finally shifted right by ``q_format`` bits.
 *  The saturation is only done after the last multiplication.
 *  To avoid 64-bit overflow in the intermediate results, the fixed point format must be chosen 
 *  according to num_taps.
 * 
 *  \param input_sample    The new sample to be processed.
 *  \param filter_coeffs   Pointer to FIR coefficients array arranged as:
 *                         ``bM,b(1L+M),b(2L+M),b((N-1)L+M),``
 *                         ``b1,b(1L+1),b(2L+1),b((N-1)L+1),``
 *                         ``b0,b(1L+0),b(2L+0),b((N-1)L+0),``
 *                         where M = N-1
 *  \param state_data      Pointer to filter state data array of length N-1.
 *                         Must be initialized at startup to all zeros.
 *  \param num_taps        Number of filter taps (N = ``num_taps`` = filter order + 1).
 *  \param interp_factor   The interpolation factor/index (i.e. the up-sampling ratio).
 *                         The interpolation factor/index can range from 2 to 16.
 *  \param output_samples  The resulting interpolated samples.
 *  \param q_format        Fixed point format (i.e. number of fractional bits).
 */
 
void dsp_filters_interpolate
(
    int32_t       input_sample,
    const int32_t filter_coeffs[],
    int32_t       state_data[],
    const int32_t num_taps,
    const int32_t interp_factor,
    int32_t       output_samples[],
    const int32_t q_format
);

/** This function implements an decimating FIR filter.
 *
 *  The function operates on a single set of input samples whose count is equal
 *  to the decimation factor. (i.e. and each call to the function processes
 *  ``decim_factor`` samples and results in one sample).
 * 
 *  The FIR filter algorithm is based upon a sequence of multiply-accumulate
 *  (MAC) operations. Each filter coefficient ``b[i]`` is multiplied by a state
 *  variable which equals a previous input sample, or
 *  ``y[n] = x[n]*b0 + x[n-1]*b1 + x[n-2]*b2 + ... + x[n-N+1]*bN-1``
 * 
 *  ``filter_coeffs`` points to a coefficient array of size N = ``num_taps``.
 *  The filter coefficients
 *  are stored in forward order (e.g. ``b0,b1,...,bN-1``).
 * 
 *  The FIR algorithm involves multiplication between 32-bit filter
 *  coefficients and 32-bit state data producing a 64-bit result for each
 *  coefficient and state data pair. Multiplication results are accumulated in a
 *  64-bit accumulator. 
 *  If overflow occurs in the final 64-bit result, it is saturated at the minimum/maximum value 
 *  given the fixed-point format and finally shifted right by ``q_format`` bits.
 *  The saturation is only done after the last multiplication.
 *  To avoid 64-bit overflow in the intermediate results, the fixed point format must be chosen 
 *  according to num_taps.
 *  
 * 
 *  \param  input_samples  The new samples to be decimated.
 *  \param  filter_coeffs  Pointer to FIR coefficients array arranged
 *                         as ``[b0,b1,b2,...,bN-1]``.
 *  \param  state_data     Pointer to filter state data array of length N-1.
 *                         Must be initialized at startup to all zeros.
 *  \param  num_taps       Number of filter taps (N = num_taps = filter order + 1).
 *  \param  decim_factor   The decimation factor/index (i.e. the down-sampling ratio).
 *  \param  q_format       Fixed point format (i.e. number of fractional bits).
 *  \returns               The resulting decimated sample.
 */
 
int32_t dsp_filters_decimate
(
    int32_t       input_samples[],
    const int32_t filter_coeffs[],
    int32_t       state_data[],
    const int32_t num_taps,
    const int32_t decim_factor,
    const int32_t q_format
);

/** This function implements a second order IIR filter (direct form I).
 *
 *  The function operates on a single sample of input and output data (i.e. and
 *  each call to the function processes one sample).
 * 
 *  The IIR filter algorithm executes a difference equation on current and past input values x 
 *  and past output values y:
 *  ``y[n] = x[n]*b0 + x[n-1]*b1 + x[n-2]*b2 + y[n-1]*-a1 + y[n-2]*-a2``
 * 
 *  The filter coefficients are stored in forward order (e.g. ``b0,b1,b2,-a1,-a2``).
 * 
 *  Example showing a single Biquad filter with samples and coefficients
 *  represented in Q28 fixed-point format:
 * 
 *  \code
 *  int32_t filter_coeff[DSP_NUM_COEFFS_PER_BIQUAD] = { Q28(+0.5), Q28(-0.1), Q28(-0.5), Q28(-0.1), Q28(0.1) };
 *  int32_t filter_state[DSP_NUM_STATES_PER_BIQUAD] = { 0, 0, 0, 0 };
 *  int32_t result = dsp_filters_biquad( sample, filter_coeff, filter_state, 28 );
 *  \endcode
 * 
 *  The IIR algorithm involves multiplication between 32-bit filter
 *  coefficients and 32-bit state data producing a 64-bit result for each
 *  coefficient and state data pair. Multiplication results are accumulated in a
 *  64-bit accumulator.
 *  If overflow occurs in the final 64-bit result, it is saturated at the minimum/maximum value 
 *  given the fixed-point format and finally shifted right by ``q_format`` bits.
 *
 *
 *  \param  input_sample   The new sample to be processed.
 *  \param  filter_coeffs  Pointer to biquad coefficients array arranged as ``[b0,b1,b2,-a1,-a2]``.
 *  \param  state_data     Pointer to filter state data array (initialized at startup to zeros).
 *                         The length of the state data array is 4.
 *  \param  q_format       Fixed point format (i.e. number of fractional bits).
 *  \returns               The resulting filter output sample.
 */
 
int32_t dsp_filters_biquad
(
    int32_t       input_sample,
    const int32_t filter_coeffs[DSP_NUM_COEFFS_PER_BIQUAD],
    int32_t       state_data   [DSP_NUM_STATES_PER_BIQUAD],
    const int32_t q_format
);

/** This function implements a cascaded direct form I BiQuad filter
 *
 *  The function operates on a single sample of input and output data
 * (i.e. and each call to the function processes one sample).
 * 
 *  The IIR filter algorithm executes a difference equation on current and past input values x 
 *  and past output values y:
 *  ``y[n] = x[n]*b0 + x[n-1]*b1 + x[n-2]*b2 + y[n-1]*-a1 + y[n-2]*-a2``
 * 
 *  The filter coefficients are stored in forward order
 *  (e.g. ``section1:b0,b1,b2,-a1,-a2,sectionN:b0,b1,b2,-a1,-a2``).
 * 
 *  Example showing a 4x cascaded Biquad filter with samples and coefficients
 *  represented in Q28 fixed-point format:
 *   
 *  \code
 *  int32_t filter_coeff[4*DSP_NUM_COEFFS_PER_BIQUAD] = { 
 *                           Q28(+0.5), Q28(-0.1), Q28(-0.5), Q28(-0.1), Q28(0.1),
 *                           Q28(+0.5), Q28(-0.1), Q28(-0.5), Q28(-0.1), Q28(0.1),
 *                           Q28(+0.5), Q28(-0.1), Q28(-0.5), Q28(-0.1), Q28(0.1),
 *                           Q28(+0.5), Q28(-0.1), Q28(-0.5), Q28(-0.1), Q28(0.1) };
 *  int32_t filter_state[4*DSP_NUM_STATES_PER_BIQUAD] = { 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 };
 *  int32_t result = dsp_filters_biquads( sample, filter_coeff, filter_state, 4, 28 );
 *  \endcode
 * 
 *  The IIR algorithm involves multiplication between 32-bit filter
 *  coefficients and 32-bit state data producing a 64-bit result for each
 *  coefficient and state data pair. Multiplication results are accumulated in a
 *  64-bit accumulator.
 *  If overflow occurs in the final 64-bit result, it is saturated at the minimum/maximum value 
 *  given the fixed-point format and finally shifted right by ``q_format`` bits.
 * 
 *  \param  input_sample   The new sample to be processed.
 *  \param  filter_coeffs  Pointer to biquad coefficients array for all BiQuad sections.
 *                         Arranged as ``[section1:b0,b1,b2,-a1,-a2,...sectionN:b0,b1,b2,-a1,-a2]``.
 *  \param  state_data     Pointer to filter state data array (initialized at startup to zeros).
 *                         The length of the state data array is ``num_sections`` * 4.
 *  \param  num_sections   Number of BiQuad sections.
 *  \param  q_format       Fixed point format (i.e. number of fractional bits).
 *  \returns               The resulting filter output sample.
 */
 
int32_t dsp_filters_biquads
(
    int32_t       input_sample,
    const int32_t filter_coeffs[],
    int32_t       state_data[],
    const int32_t num_sections,
    const int32_t q_format
);

#endif
