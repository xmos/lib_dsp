// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <platform.h>
#include <stdio.h>
#include <math.h>

static double pi = 3.14159265359;

static int _float2fixed( float x, int q )
{
    if( x < 0 ) return (((double)(1<<q)) * x - 0.5);
    else if( x > 0 ) return (((double)((1<<q)-1)) * x + 0.5);
    return 0;
}

/** This function generates BiQuad filter coefficients for a notch filter.
 *
 *  The filter coefficients are stored in forward order (e.g. ``b0,b1,b2,a1,a2``).
 *  The frequency specification is normalized to the Nyquist frequency therefore the
 *  frequency value must be in the range of ``0.0 <= F < 0.5`` for valid filter coefficients.
 *
 *  Example showing a filter coefficients generation using Q28 fixed-point formatting.
 * 
 *  \code
 *  int coeffs[5];
 *  lib_dsp_design_biquad_notch( 0.25, 0.707, coeffs, 28 );
 *  \endcode
 *
 *  \param  filter_frequency   Filter center frequency normalized to the sampling frequency.
 *                             ``0 < frequency < 0.5``, where 0.5 represents Fs/2.
 *  \param  filter_Q           The filter Q-factor.
 *  \param  biquad_coeffs      The array used to contain the resulting filter coefficients.
 *                             Filter coefficients are ordered as ``[b0,b1,b2,a1,a2]``.
 *  \param  q_format           Fixed point format of coefficients (i.e. number of fractional bits).
 */

void lib_dsp_design_biquad_notch
(
    double frequency,
    double filter_Q,
    int    coefficients[5],
    int    q_format
) {
    // Compute common factors
	double w0    = 2.0 * pi * frequency;
	double alpha = sin(w0) / (2.0 * filter_Q);

    // Compute coefficients
	double b0 = +1.0;
	double b1 = -2.0 * cos(w0);
	double b2 = +1.0;
	double a0 = +1.0 + alpha;
	double a1 = -2.0 * cos(w0);
	double a2 = +1.0 - alpha;
	
	// Store as fixed-point values
	coefficients[0] = _float2fixed( b0/a0, q_format );
	coefficients[1] = _float2fixed( b1/a0, q_format );
	coefficients[2] = _float2fixed( b2/a0, q_format );
	coefficients[3] = _float2fixed( -a1/a0, q_format );
	coefficients[4] = _float2fixed( -a2/a0, q_format );
}

/** This function generates BiQuad filter coefficients for a low-pass filter.
 *
 *  The filter coefficients are stored in forward order (e.g. ``b0,b1,b2,a1,a2``).
 *  The frequency specification is normalized to the Nyquist frequency therefore the
 *  frequency value must be in the range of ``0.0 <= F < 0.5`` for valid filter coefficients.
 *
 *  Example showing a filter coefficients generation using Q28 fixed-point formatting.
 * 
 *  \code
 *  int coeffs[5];
 *  lib_dsp_design_biquad_lowpass( 0.25, 0.707, coeffs, 28 );
 *  \endcode
 *
 *  \param  filter_frequency   Filter cutoff (-3db) frequency normalized to the sampling frequency.
 *                             ``0 < frequency < 0.5``, where 0.5 represents Fs/2.
 *  \param  filter_Q           The filter Q-factor.
 *  \param  biquad_coeffs      The array used to contain the resulting filter coefficients.
 *                             Filter coefficients are ordered as ``[b0,b1,b2,a1,a2]``.
 *  \param  q_format           Fixed point format of coefficients (i.e. number of fractional bits).
 */

void lib_dsp_design_biquad_lowpass
(
    double frequency,
    double filter_Q,
    int    coefficients[5],
    int    q_format
) {
    // Compute common factors
	double w0    = 2.0 * pi * frequency;
	double alpha = sin(w0) / (2.0 * filter_Q);

    // Compute coefficients
	double b0 = (+1.0 - cos(w0)) / 2.0;
	double b1 =  +1.0 - cos(w0);
	double b2 = (+1.0 - cos(w0)) / 2.0;
	double a0 = +1.0 + alpha;
	double a1 = -2.0 * cos(w0);
	double a2 = +1.0 - alpha;
	
	// Store as fixed-point values
	coefficients[0] = _float2fixed( b0/a0, q_format );
	coefficients[1] = _float2fixed( b1/a0, q_format );
	coefficients[2] = _float2fixed( b2/a0, q_format );
	coefficients[3] = _float2fixed( -a1/a0, q_format );
	coefficients[4] = _float2fixed( -a2/a0, q_format );
}

/** This function generates BiQuad filter coefficients for a high-pass filter.
 *
 *  The filter coefficients are stored in forward order (e.g. ``b0,b1,b2,a1,a2``).
 *  The frequency specification is normalized to the Nyquist frequency therefore the
 *  frequency value must be in the range of ``0.0 <= F < 0.5`` for valid filter coefficients.
 *
 *  Example showing a filter coefficients generation using Q28 fixed-point formatting.
 * 
 *  \code
 *  int coeffs[5];
 *  lib_dsp_design_biquad_highpass( 0.25, 0.707, coeffs, 28 );
 *  \endcode
 *
 *  \param  filter_frequency   Filter cutoff (-3db) frequency normalized to the sampling frequency.
 *                             ``0 < frequency < 0.5``, where 0.5 represents Fs/2.
 *  \param  filter_Q           The filter Q-factor.
 *  \param  biquad_coeffs      The array used to contain the resulting filter coefficients.
 *                             Filter coefficients are ordered as ``[b0,b1,b2,a1,a2]``.
 *  \param  q_format           Fixed point format of coefficients (i.e. number of fractional bits).
 */

void lib_dsp_design_biquad_highpass
(
    double frequency,
    double filter_Q,
    int    coefficients[5],
    int    q_format
) {
    // Compute common factors
	double w0    = 2.0 * pi * frequency;
	double alpha = sin(w0) / (2.0 * filter_Q);

    // Compute coefficients
	double b0 = (+1.0 + cos(w0)) / 2.0;
	double b1 = -(1.0 + cos(w0));
	double b2 = (+1.0 + cos(w0)) / 2.0;
	double a0 = +1.0 + alpha;
	double a1 = -2.0 * cos(w0);
	double a2 = +1.0 - alpha;
	
	// Store as fixed-point values
	coefficients[0] = _float2fixed( b0/a0, q_format );
	coefficients[1] = _float2fixed( b1/a0, q_format );
	coefficients[2] = _float2fixed( b2/a0, q_format );
	coefficients[3] = _float2fixed( -a1/a0, q_format );
	coefficients[4] = _float2fixed( -a2/a0, q_format );
}

/** This function generates BiQuad filter coefficients for an all-pass filter.
 *
 *  The filter coefficients are stored in forward order (e.g. ``b0,b1,b2,a1,a2``).
 *  The frequency specification is normalized to the Nyquist frequency therefore the
 *  frequency value must be in the range of ``0.0 <= F < 0.5`` for valid filter coefficients.
 *
 *  Example showing a filter coefficients generation using Q28 fixed-point formatting.
 * 
 *  \code
 *  int coeffs[5];
 *  lib_dsp_design_biquad_allpass( 0.25, 0.707, coeffs, 28 );
 *  \endcode
 *
 *  \param  filter_frequency   Filter center frequency normalized to the sampling frequency.
 *                             ``0 < frequency < 0.5``, where 0.5 represents Fs/2.
 *  \param  filter_Q           The filter Q-factor.
 *  \param  biquad_coeffs      The array used to contain the resulting filter coefficients.
 *                             Filter coefficients are ordered as ``[b0,b1,b2,a1,a2]``.
 *  \param  q_format           Fixed point format of coefficients (i.e. number of fractional bits).
 */

void lib_dsp_design_biquad_allpass
(
    double frequency,
    double filter_Q,
    int    coefficients[5],
    int    q_format
) {
    // Compute common factors
	double w0    = 2.0 * pi * frequency;
	double alpha = sin(w0) / (2.0 * filter_Q);

    // Compute coefficients
	double b0 =  +1.0 - alpha;
	double b1 =  -2.0 * cos(w0);
	double b2 =  +1.0 + alpha;
	double a0 =  +1.0 + alpha;
	double a1 =  -2.0 * cos(w0);
	double a2 =  +1.0 - alpha;
	
	// Store as fixed-point values
	coefficients[0] = _float2fixed( b0/a0, q_format );
	coefficients[1] = _float2fixed( b1/a0, q_format );
	coefficients[2] = _float2fixed( b2/a0, q_format );
	coefficients[3] = _float2fixed( -a1/a0, q_format );
	coefficients[4] = _float2fixed( -a2/a0, q_format );
}

/** This function generates BiQuad filter coefficients for a band-pass filter.
 *
 *  The filter coefficients are stored in forward order (e.g. ``b0,b1,b2,a1,a2``).
 *  The frequency specification is normalized to the Nyquist frequency therefore the
 *  frequency value must be in the range of ``0.0 <= F < 0.5`` for valid filter coefficients.
 *
 *  Example showing a filter coefficients generation using Q28 fixed-point formatting.
 * 
 *  \code
 *  int coeffs[5];
 *  lib_dsp_design_biquad_bandpass( 0.20, 0.30, coeffs, 28 );
 *  \endcode
 *
 *  \param  filter_frequency1  Filter cutoff #1 (-3db) frequency normalized to the sampling frequency.
 *                             ``0 < frequency1 < 0.5``, where 0.5 represents Fs/2.
 *  \param  filter_frequency2  Filter cutoff #2 (-3db) frequency normalized to the sampling frequency.
 *                             ``0 < frequency2 < 0.5``, where 0.5 represents Fs/2.
 *                             Note that frequency1 must be less than to frequency2.
 *  \param  biquad_coeffs      The array used to contain the resulting filter coefficients.
 *                             Filter coefficients are ordered as ``[b0,b1,b2,a1,a2]``.
 *  \param  q_format           Fixed point format of coefficients (i.e. number of fractional bits).
 */

void lib_dsp_design_biquad_bandpass
(
    double frequency1,
    double frequency2,
    int    coefficients[5],
    int    q_format
) {
    // Compute common factors
	double w0    = 2.0 * pi * frequency1;
	double bw    = frequency2 - frequency1;
	double alpha = sin(w0) * sinh( log(2)/2 * bw * w0/sin(w0) );

    // Compute coefficients
	double b0 =  sin(w0) / 2.0;
	double b1 =  +0.0;
	double b2 = -sin(w0) / 2.0;
	double a0 =  +1.0 + alpha;
	double a1 =  -2.0 * cos(w0);
	double a2 =  +1.0 - alpha;
	
	// Store as fixed-point values
	coefficients[0] = _float2fixed( b0/a0, q_format );
	coefficients[1] = _float2fixed( b1/a0, q_format );
	coefficients[2] = _float2fixed( b2/a0, q_format );
	coefficients[3] = _float2fixed( -a1/a0, q_format );
	coefficients[4] = _float2fixed( -a2/a0, q_format );
}

/** This function generates BiQuad filter coefficients for a peaking filter.
 *
 *  The filter coefficients are stored in forward order (e.g. ``b0,b1,b2,a1,a2``).
 *  The frequency specification is normalized to the Nyquist frequency therefore the
 *  frequency value must be in the range of ``0.0 <= F < 0.5`` for valid filter coefficients.
 *
 *  Example showing a filter coefficients generation using Q28 fixed-point formatting.
 * 
 *  \code
 *  int coeffs[5];
 *  lib_dsp_design_biquad_notch( 0.25, 0.707, coeffs, 28 );
 *  \endcode
 *
 *  \param  filter_frequency   Filter center frequency normalized to the sampling frequency.
 *                             ``0 < frequency < 0.5``, where 0.5 represents Fs/2.
 *  \param  filter_Q           The filter Q-factor.
 *  \param  peak_qain_db       The filter gain in dB (postive or negative).
 *                             +gain results in peaking gain (gain at peak center = ``gain_db``).
 *                             -gain results in attenuation (gain at peak center = -``gain_db``).
 *  \param  biquad_coeffs      The array used to contain the resulting filter coefficients.
 *                             Filter coefficients are ordered as ``[b0,b1,b2,a1,a2]``.
 *  \param  q_format           Fixed point format of coefficients (i.e. number of fractional bits).
 */

void lib_dsp_design_biquad_peaking
(
    double frequency,
    double filter_Q,
    double gain_db,
    int    coefficients[5],
    int    q_format
) {
    // Compute common factors
	double A  = sqrt( pow(10,(gain_db/20)) );
	double w0 = 2.0 * pi * frequency;
	double alpha = sin(w0) / (2.0 * filter_Q);

    // Compute coefficients
	double b0 = +1.0 + alpha * A;
	double b1 = -2.0 * cos(w0);
	double b2 = +1.0 - alpha * A;
	double a0 = +1.0 + alpha / A;
	double a1 = -2.0 * cos(w0);
	double a2 = +1.0 - alpha / A;
	
	// Store as fixed-point values
	coefficients[0] = _float2fixed( b0/a0, q_format );
	coefficients[1] = _float2fixed( b1/a0, q_format );
	coefficients[2] = _float2fixed( b2/a0, q_format );
	coefficients[3] = _float2fixed( -a1/a0, q_format );
	coefficients[4] = _float2fixed( -a2/a0, q_format );
}

/** This function generates BiQuad filter coefficients for a bass shelving filter.
 *
 *  The filter coefficients are stored in forward order (e.g. ``b0,b1,b2,a1,a2``).
 *  The frequency specification is normalized to the Nyquist frequency therefore the
 *  frequency value must be in the range of ``0.0 <= F < 0.5`` for valid filter coefficients.
 *
 *  Example showing a filter coefficients generation using Q28 fixed-point formatting.
 * 
 *  \code
 *  int coeffs[5];
 *  lib_dsp_design_biquad_lowshelf( 0.25, 0.707, +6.0, coeffs, 28 );
 *  \endcode
 *
 *  \param  filter_frequency   Filter frequency (+3db or -3db point) normalized to Fs.
 *                             ``0 < frequency < 0.5``, where 0.5 represents Fs/2.
 *  \param  filter_Q           The filter Q-factor.
 *  \param  shelf_gain_db      The filter shelf gain in dB (postive or negative).
 *                             +gain results in bass shelf with gain of 'shelf_gain_db'.
 *                             -gain results in bass shelf with attenuation of 'shelf_gain_db'.
 *  \param  biquad_coeffs      The array used to contain the resulting filter coefficients.
 *                             Filter coefficients are ordered as ``[b0,b1,b2,a1,a2]``.
 *  \param  q_format           Fixed point format of coefficients (i.e. number of fractional bits).
 */

void lib_dsp_design_biquad_lowshelf
(
    double frequency,
    double filter_Q,
    double shelf_gain_db,
    int    coefficients[5],
    int    q_format
) {
    // Compute common factors
	double A  = pow( 10.0, (shelf_gain_db / 40.0) );
	double w0 = 2.0 * pi * frequency;
	double alpha = sin(w0)/2 * sqrt( (A + 1.0/A)*(1.0/filter_Q - 1.0) + 2.0 );
	
    // Compute coefficients
	double b0 =   A * ( (A+1) - (A-1)*cos(w0) + 2*sqrt(A)*alpha );
	double b1 = 2*A * ( (A-1) - (A+1)*cos(w0) );
	double b2 =   A * ( (A+1) - (A-1)*cos(w0) - 2*sqrt(A)*alpha );
	double a0 =         (A+1) + (A-1)*cos(w0) + 2*sqrt(A)*alpha;
	double a1 =  -2 * ( (A-1) + (A+1)*cos(w0) );
	double a2 =         (A+1) + (A-1)*cos(w0) - 2*sqrt(A)*alpha;
	
	// Store as fixed-point values
	coefficients[0] = _float2fixed( b0/a0, q_format );
	coefficients[1] = _float2fixed( b1/a0, q_format );
	coefficients[2] = _float2fixed( b2/a0, q_format );
	coefficients[3] = _float2fixed( -a1/a0, q_format );
	coefficients[4] = _float2fixed( -a2/a0, q_format );
}

/** This function generates BiQuad filter coefficients for a treble shelving filter.
 *
 *  The filter coefficients are stored in forward order (e.g. ``b0,b1,b2,a1,a2``).
 *  The frequency specification is normalized to the Nyquist frequency therefore the
 *  frequency value must be in the range of ``0.0 <= F < 0.5`` for valid filter coefficients.
 *
 *  Example showing a filter coefficients generation using Q28 fixed-point formatting.
 * 
 *  \code
 *  int coeffs[5];
 *  lib_dsp_design_biquad_lowshelf( 0.25, 0.707, +6.0, coeffs, 28 );
 *  \endcode
 *
 *  \param  filter_frequency   Filter frequency (+3db or -3db point) normalized to Fs.
 *                             ``0 < frequency < 0.5``, where 0.5 represents Fs/2.
 *  \param  filter_Q           The filter Q-factor.
 *  \param  shelf_gain_db      The filter shelf gain in dB (postive or negative).
 *                             +gain results in bass shelf with gain of 'shelf_gain_db'.
 *                             -gain results in bass shelf with attenuation of 'shelf_gain_db'.
 *  \param  biquad_coeffs      The array used to contain the resulting filter coefficients.
 *                             Filter coefficients are ordered as ``[b0,b1,b2,a1,a2]``.
 *  \param  q_format           Fixed point format of coefficients (i.e. number of fractional bits).
 */

void lib_dsp_design_biquad_highshelf
(
    double frequency,
    double filter_Q,
    double shelf_gain_db,
    int    coefficients[5],
    int    q_format
) {
    // Compute common factors
	double A  = pow( 10.0, (shelf_gain_db / 40.0) );
	double w0 = 2.0 * pi * frequency;
	double alpha = sin(w0)/2.0 * sqrt( (A + 1.0/A)*(1.0/filter_Q - 1.0) + 2.0 );

    // Compute coefficients
	double b0 =    A * ( (A+1) + (A-1)*cos(w0) + 2*sqrt(A)*alpha );
	double b1 = -2*A * ( (A-1) + (A+1)*cos(w0) );
	double b2 =    A * ( (A+1) + (A-1)*cos(w0) - 2*sqrt(A)*alpha );
	double a0 =          (A+1) - (A-1)*cos(w0) + 2*sqrt(A)*alpha;
	double a1 =    2 * ( (A-1) - (A+1)*cos(w0) );
	double a2 =          (A+1) - (A-1)*cos(w0) - 2*sqrt(A)*alpha;
	
	// Store as fixed-point values
	coefficients[0] = _float2fixed( b0/a0, q_format );
	coefficients[1] = _float2fixed( b1/a0, q_format );
	coefficients[2] = _float2fixed( b2/a0, q_format );
	coefficients[3] = _float2fixed( -a1/a0, q_format );
	coefficients[4] = _float2fixed( -a2/a0, q_format );
}
