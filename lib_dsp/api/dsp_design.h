// Copyright (c) 2015-2016, XMOS Ltd, All rights reserved

#ifndef DSP_DESIGN_H_
#define DSP_DESIGN_H_

#include "stdint.h"

/** This function generates BiQuad filter coefficients for a notch filter.
 *
 *  The filter coefficients are stored in forward order (e.g. ``b0,b1,b2,-a1,-a2``).
 *  The frequency specification is normalized to the Nyquist frequency therefore the
 *  frequency value must be in the range of ``0.0 <= F < 0.5`` for valid filter coefficients.
 *
 *  Example showing a filter coefficients generation using Q28 fixed-point formatting.
 * 
 *  \code
 *  int32_t coeffs[5];
 *  dsp_design_biquad_notch( 0.25, 0.707, coeffs, 28 );
 *  \endcode
 *
 *  \param  filter_frequency   Filter center frequency normalized to the sampling frequency.
 *                             ``0 < frequency < 0.5``, where 0.5 represents Fs/2.
 *  \param  filter_Q           The filter Q-factor.
 *  \param  biquad_coeffs      The array used to contain the resulting filter coefficients.
 *                             Filter coefficients are ordered as ``[b0,b1,b2,-a1,-a2]``.
 *  \param  q_format           Fixed point format of coefficients (i.e. number of fractional bits).
 */

void dsp_design_biquad_notch
(
    double filter_frequency,
    double filter_Q,
    int32_t    biquad_coeffs[5],
    const int32_t q_format
);

/** This function generates BiQuad filter coefficients for a low-pass filter.
 *
 *  The filter coefficients are stored in forward order (e.g. ``b0,b1,b2,-a1,-a2``).
 *  The frequency specification is normalized to the Nyquist frequency therefore the
 *  frequency value must be in the range of ``0.0 <= F < 0.5`` for valid filter coefficients.
 *
 *  Example showing a filter coefficients generation using Q28 fixed-point formatting.
 * 
 *  \code
 *  int32_t coeffs[5];
 *  dsp_design_biquad_lowpass( 0.25, 0.707, coeffs, 28 );
 *  \endcode
 *
 *  \param  filter_frequency   Filter cutoff (-3db) frequency normalized to the sampling frequency.
 *                             ``0 < frequency < 0.5``, where 0.5 represents Fs/2.
 *  \param  filter_Q           The filter Q-factor.
 *  \param  biquad_coeffs      The array used to contain the resulting filter coefficients.
 *                             Filter coefficients are ordered as ``[b0,b1,b2,-a1,-a2]``.
 *  \param  q_format           Fixed point format of coefficients (i.e. number of fractional bits).
 */

void dsp_design_biquad_lowpass
(
    double filter_frequency,
    double filter_Q,
    int32_t    biquad_coeffs[5],
    const int32_t q_format
);

/** This function generates BiQuad filter coefficients for a high-pass filter.
 *
 *  The filter coefficients are stored in forward order (e.g. ``b0,b1,b2,-a1,-a2``).
 *  The frequency specification is normalized to the Nyquist frequency therefore the
 *  frequency value must be in the range of ``0.0 <= F < 0.5`` for valid filter coefficients.
 *
 *  Example showing a filter coefficients generation using Q28 fixed-point formatting.
 * 
 *  \code
 *  int32_t coeffs[5];
 *  dsp_design_biquad_highpass( 0.25, 0.707, coeffs, 28 );
 *  \endcode
 *
 *  \param  filter_frequency   Filter cutoff (-3db) frequency normalized to the sampling frequency.
 *                             ``0 < frequency < 0.5``, where 0.5 represents Fs/2.
 *  \param  filter_Q           The filter Q-factor.
 *  \param  biquad_coeffs      The array used to contain the resulting filter coefficients.
 *                             Filter coefficients are ordered as ``[b0,b1,b2,-a1,-a2]``.
 *  \param  q_format           Fixed point format of coefficients (i.e. number of fractional bits).
 */

void dsp_design_biquad_highpass
(
    double filter_frequency,
    double filter_Q,
    int32_t    biquad_coeffs[5],
    const int32_t q_format
);

/** This function generates BiQuad filter coefficients for an all-pass filter.
 *
 *  The filter coefficients are stored in forward order (e.g. ``b0,b1,b2,-a1,-a2``).
 *  The frequency specification is normalized to the Nyquist frequency therefore the
 *  frequency value must be in the range of ``0.0 <= F < 0.5`` for valid filter coefficients.
 *
 *  Example showing a filter coefficients generation using Q28 fixed-point formatting.
 * 
 *  \code
 *  int32_t coeffs[5];
 *  dsp_design_biquad_allpass( 0.25, 0.707, coeffs, 28 );
 *  \endcode
 *
 *  \param  filter_frequency   Filter center frequency normalized to the sampling frequency.
 *                             ``0 < frequency < 0.5``, where 0.5 represents Fs/2.
 *  \param  filter_Q           The filter Q-factor.
 *  \param  biquad_coeffs      The array used to contain the resulting filter coefficients.
 *                             Filter coefficients are ordered as ``[b0,b1,b2,-a1,-a2]``.
 *  \param  q_format           Fixed point format of coefficients (i.e. number of fractional bits).
 */

void dsp_design_biquad_allpass
(
    double filter_frequency,
    double filter_Q,
    int32_t    biquad_coeffs[5],
    const int32_t q_format
);

/** This function generates BiQuad filter coefficients for a band-pass filter.
 *
 *  The filter coefficients are stored in forward order (e.g. ``b0,b1,b2,-a1,-a2``).
 *  The frequency specification is normalized to the Nyquist frequency therefore the
 *  frequency value must be in the range of ``0.0 <= F < 0.5`` for valid filter coefficients.
 *
 *  Example showing a filter coefficients generation using Q28 fixed-point formatting.
 * 
 *  \code
 *  int32_t coeffs[5];
 *  dsp_design_biquad_bandpass( 0.20, 0.30, coeffs, 28 );
 *  \endcode
 *
 *  \param  filter_frequency1  Filter cutoff #1 (-3db) frequency normalized to the sampling frequency.
 *                             ``0 < frequency1 < 0.5``, where 0.5 represents Fs/2.
 *  \param  filter_frequency2  Filter cutoff #2 (-3db) frequency normalized to the sampling frequency.
 *                             ``0 < frequency2 < 0.5``, where 0.5 represents Fs/2.
 *                             Note that frequency1 must be less than to frequency2.
 *  \param  biquad_coeffs      The array used to contain the resulting filter coefficients.
 *                             Filter coefficients are ordered as ``[b0,b1,b2,-a1,-a2]``.
 *  \param  q_format           Fixed point format of coefficients (i.e. number of fractional bits).
 */

void dsp_design_biquad_bandpass
(
    double filter_frequency1,
    double filter_frequency2,
    int32_t    biquad_coeffs[5],
    const int32_t q_format
);

/** This function generates BiQuad filter coefficients for a peaking filter.
 *
 *  The filter coefficients are stored in forward order (e.g. ``b0,b1,b2,-a1,-a2``).
 *  The frequency specification is normalized to the Nyquist frequency therefore the
 *  frequency value must be in the range of ``0.0 <= F < 0.5`` for valid filter coefficients.
 *
 *  Example showing a filter coefficients generation using Q28 fixed-point formatting.
 * 
 *  \code
 *  int32_t coeffs[5];
 *  dsp_design_biquad_peaking( 0.25, 0.707, 3.0, coeff[5], 28 );
 *  \endcode
 *
 *  \param  filter_frequency   Filter center frequency normalized to the sampling frequency.
 *                             ``0 < frequency < 0.5``, where 0.5 represents Fs/2.
 *  \param  filter_Q           The filter Q-factor.
 *  \param  peak_qain_db       The filter gain in dB (postive or negative).
 *                             +gain results in peaking gain (gain at peak center = ``gain_db``).
 *                             -gain results in attenuation (gain at peak center = -``gain_db``).
 *  \param  biquad_coeffs      The array used to contain the resulting filter coefficients.
 *                             Filter coefficients are ordered as ``[b0,b1,b2,-a1,-a2]``.
 *  \param  q_format           Fixed point format of coefficients (i.e. number of fractional bits).
 */

void dsp_design_biquad_peaking
(
    double filter_frequency,
    double filter_Q,
    double peak_qain_db,
    int32_t    biquad_coeffs[5],
    const int32_t q_format
);

/** This function generates BiQuad filter coefficients for a bass shelving filter.
 *
 *  The filter coefficients are stored in forward order (e.g. ``b0,b1,b2,-a1,-a2``).
 *  The frequency specification is normalized to the Nyquist frequency therefore the
 *  frequency value must be in the range of ``0.0 <= F < 0.5`` for valid filter coefficients.
 *
 *  Example showing a filter coefficients generation using Q28 fixed-point formatting.
 * 
 *  \code
 *  int32_t coeffs[5];
 *  dsp_design_biquad_lowshelf( 0.25, 0.707, +6.0, coeffs, 28 );
 *  \endcode
 *
 *  \param  filter_frequency   Filter frequency (+3db or -3db point) normalized to Fs.
 *                             ``0 < frequency < 0.5``, where 0.5 represents Fs/2.
 *  \param  filter_Q           The filter Q-factor.
 *  \param  shelf_gain_db      The filter shelf gain in dB (postive or negative).
 *                             +gain results in bass shelf with gain of 'shelf_gain_db'.
 *                             -gain results in bass shelf with attenuation of 'shelf_gain_db'.
 *  \param  biquad_coeffs      The array used to contain the resulting filter coefficients.
 *                             Filter coefficients are ordered as ``[b0,b1,b2,-a1,-a2]``.
 *  \param  q_format           Fixed point format of coefficients (i.e. number of fractional bits).
 */

void dsp_design_biquad_lowshelf
(
    double filter_frequency,
    double filter_Q,
    double shelf_gain_db,
    int32_t    biquad_coeffs[5],
    const int32_t q_format
);

/** This function generates BiQuad filter coefficients for a treble shelving filter.
 *
 *  The filter coefficients are stored in forward order (e.g. ``b0,b1,b2,-a1,-a2``).
 *  The frequency specification is normalized to the Nyquist frequency therefore the
 *  frequency value must be in the range of ``0.0 <= F < 0.5`` for valid filter coefficients.
 *
 *  Example showing a filter coefficients generation using Q28 fixed-point formatting.
 * 
 *  \code
 *  int32_t coeffs[5];
 *  dsp_design_biquad_highshelf( 0.25, 0.707, +6.0, coeffs, 28 );
 *  \endcode
 *
 *  \param  filter_frequency   Filter frequency (+3db or -3db point) normalized to Fs.
 *                             ``0 < frequency < 0.5``, where 0.5 represents Fs/2.
 *  \param  filter_Q           The filter Q-factor.
 *  \param  shelf_gain_db      The filter shelf gain in dB (postive or negative).
 *                             +gain results in bass shelf with gain of 'shelf_gain_db'.
 *                             -gain results in bass shelf with attenuation of 'shelf_gain_db'.
 *  \param  biquad_coeffs      The array used to contain the resulting filter coefficients.
 *                             Filter coefficients are ordered as ``[b0,b1,b2,-a1,-a2]``.
 *  \param  q_format           Fixed point format of coefficients (i.e. number of fractional bits).
 */

void dsp_design_biquad_highshelf
(
    double filter_frequency,
    double filter_Q,
    double shelf_gain_db,
    int32_t    biquad_coeffs[5],
    const int32_t q_format
);

#endif
