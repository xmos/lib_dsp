// Copyright 2019-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#ifndef DSP_FILTERS_FP_H_
#define DSP_FILTERS_FP_H_

#include "dsp_filters.h"

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
 *  Example showing a single Biquad filter
 *
 *  \code
 *  double filter_coeff[DSP_NUM_COEFFS_PER_BIQUAD] = { +0.5, -0.1, -0.5, -0.1, 0.1) };
 *  double filter_state[DSP_NUM_STATES_PER_BIQUAD] = { 0, 0, 0, 0 };
 *  double result = dsp_filters_biquad_fp( sample, filter_coeff, filter_state, 28 );
 *  \endcode
 *
 *  The IIR algorithm involves multiplication between double precision filter
 *  coefficients and double precision state data producing a double precision result for each
 *  coefficient and state data pair.
 *
 *
 *  \param  input_sample   The new sample to be processed.
 *  \param  filter_coeffs  Pointer to biquad coefficients array arranged as ``[b0,b1,b2,-a1,-a2]``.
 *  \param  state_data     Pointer to filter state data array (initialized at startup to zeros).
 *                         The length of the state data array is 4.
 *  \returns               The resulting filter output sample.
 */

double dsp_filters_biquad_fp
(
    double       input_sample,
    const double filter_coeffs[],
    double       state_data[]
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
 *  Example showing a 4x cascaded Biquad filter
 *
 *  \code
 *  double filter_coeff[4*DSP_NUM_COEFFS_PER_BIQUAD] = {
 *                           +0.5, -0.1, -0.5, -0.1, 0.1,
 *                           +0.5, -0.1, -0.5, -0.1, 0.1,
 *                           +0.5, -0.1, -0.5, -0.1, 0.1,
 *                           +0.5, -0.1, -0.5, -0.1, 0.1) };
 *  double filter_state[4*DSP_NUM_STATES_PER_BIQUAD] = { 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 };
 *  double result = dsp_filters_biquads_fp( sample, filter_coeff, filter_state, 4, 28 );
 *  \endcode
 *
 *  The IIR algorithm involves multiplication between double precision filter
 *  coefficients and double precision state data producing a double precision result for each
 *  coefficient and state data pair.
 *
 *  \param  input_sample   The new sample to be processed.
 *  \param  filter_coeffs  Pointer to biquad coefficients array for all BiQuad sections.
 *                         Arranged as ``[section1:b0,b1,b2,-a1,-a2,...sectionN:b0,b1,b2,-a1,-a2]``.
 *  \param  state_data     Pointer to filter state data array (initialized at startup to zeros).
 *                         The length of the state data array is ``num_sections`` * 4.
 *  \param  num_sections   Number of BiQuad sections.
 *  \returns               The resulting filter output sample.
 */

double dsp_filters_biquads_fp
(
    double       input_sample,
    const double filter_coeffs[],
    double       state_data[],
    const uint32_t num_sections
);

#endif
