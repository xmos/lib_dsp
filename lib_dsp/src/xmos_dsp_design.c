// ================================================================================================

#include <platform.h>
#include <stdio.h>
#include <math.h>

static double pi = 3.14159265359;

// ================================================================================================

static int _float2fixed( float x, int q )
{
    if( x > 0 ) return (((double)(1<<q)) * x - 0.5);
    else return (((double)((1<<q)-1)) * x + 0.5);
    return 0;
}

// Biquad filter design - notch filter
//
// 'frequency':    Filter center frequency normalized to the sampling frequency.
//                 0 < frequency < 0.5, where 0.5 represents Fs/2.
// 'Q':            Filter Q factor.
// 'coefficients': Resulting fixed-point filter coefficients ordered as [b0,b1,b2,a1,a2].
// 'q_format':     Fixed point format, the number of bits making up fractional part.

void xmos_dsp_design_biquad_notch( double frequency, double Q, int coefficients[5], int q_format )
{
    // Compute common factors
	double w0    = 2.0 * pi * frequency;
	double alpha = sin(w0) / (2.0 * Q);

    // Compute coefficients
	double b0 = +1.0;
	double b1 = -2.0 * cos(w0);
	double b2 = +1.0;
	double a0 = +1.0 + alpha;
	double a1 = -2.0 * cos(w0);
	double a2 = +1.0 - alpha;
	
	// Normalize gain to 1.0
	b1 /= b0; b2 /= b0; a1 /= a0; a2 /= a0; 
	
	// Store as fixed-point values
	coefficients[0] = _float2fixed( b0, q_format );
	coefficients[1] = _float2fixed( b1, q_format );
	coefficients[2] = _float2fixed( b2, q_format );
	coefficients[3] = _float2fixed( a1, q_format );
	coefficients[4] = _float2fixed( a2, q_format );
}

// Biquad filter design - low pass filter
//
// 'frequency':    Filter cutoff (-3db) frequency normalized to the sampling frequency.
//                 0 < frequency < 0.5, where 0.5 represents Fs/2.
// 'Q':            Filter Q factor.
// 'coefficients': Resulting fixed-point filter coefficients ordered as [b0,b1,b2,a1,a2].
// 'q_format':     Fixed point format, the number of bits making up fractional part.

void xmos_dsp_design_biquad_lowpass( double frequency, double Q, int coefficients[5], int q_format )
{
    // Compute common factors
	double w0    = 2.0 * pi * frequency;
	double alpha = sin(w0) / (2.0 * Q);

    // Compute coefficients
	double b0 = (+1.0 - cos(w0)) / 2.0;
	double b1 =  +1.0 - cos(w0);
	double b2 = (+1.0 - cos(w0)) / 2.0;
	double a0 = +1.0 + alpha;
	double a1 = -2.0 * cos(w0);
	double a2 = +1.0 - alpha;
	
	// Normalize gain to 1.0
	b1 /= b0; b2 /= b0; a1 /= a0; a2 /= a0; 
	
	// Store as fixed-point values
	coefficients[0] = _float2fixed( b0, q_format );
	coefficients[1] = _float2fixed( b1, q_format );
	coefficients[2] = _float2fixed( b2, q_format );
	coefficients[3] = _float2fixed( a1, q_format );
	coefficients[4] = _float2fixed( a2, q_format );
}

// Biquad filter design - high pass filter
//
// 'frequency':    Filter cutoff (-3db) frequency normalized to the sampling frequency.
//                 0 < frequency < 0.5, where 0.5 represents Fs/2.
// 'Q':            Filter Q factor.
// 'coefficients': Resulting fixed-point filter coefficients ordered as [b0,b1,b2,a1,a2].
// 'q_format':     Fixed point format, the number of bits making up fractional part.

void xmos_dsp_design_biquad_highpass( double frequency, double Q, int coefficients[5], int q_format )
{
    // Compute common factors
	double w0    = 2.0 * pi * frequency;
	double alpha = sin(w0) / (2.0 * Q);

    // Compute coefficients
	double b0 = (+1.0 + cos(w0)) / 2.0;
	double b1 = -(1.0 + cos(w0));
	double b2 = (+1.0 + cos(w0)) / 2.0;
	double a0 = +1.0 + alpha;
	double a1 = -2.0 * cos(w0);
	double a2 = +1.0 - alpha;
	
	// Normalize gain to 1.0
	b1 /= b0; b2 /= b0; a1 /= a0; a2 /= a0; 
	
	// Store as fixed-point values
	coefficients[0] = _float2fixed( b0, q_format );
	coefficients[1] = _float2fixed( b1, q_format );
	coefficients[2] = _float2fixed( b2, q_format );
	coefficients[3] = _float2fixed( a1, q_format );
	coefficients[4] = _float2fixed( a2, q_format );
}

// Biquad filter design - all pass filter
//
// 'frequency':    Filter center frequency normalized to the sampling frequency.
//                 0 < frequency < 0.5, where 0.5 represents Fs/2.
// 'Q':            Filter Q factor.
// 'coefficients': Resulting fixed-point filter coefficients ordered as [b0,b1,b2,a1,a2].
// 'q_format':     Fixed point format, the number of bits making up fractional part.

void xmos_dsp_design_biquad_allpass( double frequency, double Q, int coefficients[5], int q_format )
{
    // Compute common factors
	double w0    = 2.0 * pi * frequency;
	double alpha = sin(w0) / (2.0 * Q);

    // Compute coefficients
	double b0 =  +1.0 - alpha;
	double b1 =  -2.0 * cos(w0);
	double b2 =  +1.0 + alpha;
	double a0 =  +1.0 + alpha;
	double a1 =  -2.0 * cos(w0);
	double a2 =  +1.0 - alpha;
	
	// Normalize gain to 1.0
	b1 /= b0; b2 /= b0; a1 /= a0; a2 /= a0; 
	
	// Store as fixed-point values
	coefficients[0] = _float2fixed( b0, q_format );
	coefficients[1] = _float2fixed( b1, q_format );
	coefficients[2] = _float2fixed( b2, q_format );
	coefficients[3] = _float2fixed( a1, q_format );
	coefficients[4] = _float2fixed( a2, q_format );
}

// Biquad filter design - band pass filter
//
// 'frequency1':   Filter cutoff #1 (-3db) frequency normalized to the sampling frequency.
//                 0 < frequency1 < 0.5, where 0.5 represents Fs/2.
// 'frequency2':   Filter cutoff #2 (-3db) frequency normalized to the sampling frequency.
//                 0 < frequency2 < 0.5, where 0.5 represents Fs/2.
// 'coefficients': Resulting fixed-point filter coefficients ordered as [b0,b1,b2,a1,a2].
// 'q_format':     Fixed point format, the number of bits making up fractional part.

void xmos_dsp_design_biquad_bandpass( double frequency1, double frequency2, int coefficients[5], int q_format )
{
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
	
	// Normalize gain to 1.0
	b1 /= b0; b2 /= b0; a1 /= a0; a2 /= a0; 
	
	// Store as fixed-point values
	coefficients[0] = _float2fixed( b0, q_format );
	coefficients[1] = _float2fixed( b1, q_format );
	coefficients[2] = _float2fixed( b2, q_format );
	coefficients[3] = _float2fixed( a1, q_format );
	coefficients[4] = _float2fixed( a2, q_format );
}

// Biquad filter design - peaking filter
//
// 'frequency':    Filter center frequency normalized to the sampling frequency.
//                 0 < frequency < 0.5, where 0.5 represents Fs/2.
// 'Q':            Filter Q factor.
// 'qain_db'       The filter gain in dB (postive or negative).
//                 +gain results in peaking gain (max gain at peak center = 'gain_db').
//                 -gain results in peaking attenuation (min gain at peak center = 'gain_db').
// 'coefficients': Resulting fixed-point filter coefficients ordered as [b0,b1,b2,a1,a2].
// 'q_format':     Fixed point format, the number of bits making up fractional part.

void xmos_dsp_design_biquad_peaking( double frequency, double Q, double gain_db, int coefficients[5], int q_format )
{
    // Compute common factors
	double A  = sqrt( pow(10,(gain_db/20)) );
	double w0 = 2.0 * pi * frequency;
	double alpha = sin(w0) / (2.0 * Q);

    // Compute coefficients
	double b0 = +1.0 + alpha * A;
	double b1 = -2.0 * cos(w0);
	double b2 = +1.0 - alpha * A;
	double a0 = +1.0 + alpha / A;
	double a1 = -2.0 * cos(w0);
	double a2 = +1.0 - alpha / A;
	
	// Normalize gain to 'gain_db'
	b1 /= b0; b2 /= b0; a1 /= a0; a2 /= a0; 
	
	// Store as fixed-point values
	coefficients[0] = _float2fixed( b0, q_format );
	coefficients[1] = _float2fixed( b1, q_format );
	coefficients[2] = _float2fixed( b2, q_format );
	coefficients[3] = _float2fixed( a1, q_format );
	coefficients[4] = _float2fixed( a2, q_format );
}

// Biquad filter design - low side (bass) shelving filter
//
// 'frequency':    Filter frequency (+3db or -3db) normalized to the sampling frequency.
//                 0 < frequency < 0.5, where 0.5 represents Fs/2.
// 'Q':            Filter Q factor.
// 'qain_db'       The filter gain in dB (postive or negative).
//                 +gain results in bass shelf with max gain of 'gain_db'.
//                 -gain results in bass shelf with max attenuation of '-gain_db'.
// 'coefficients': Resulting fixed-point filter coefficients ordered as [b0,b1,b2,a1,a2].
// 'q_format':     Fixed point format, the number of bits making up fractional part.

void xmos_dsp_design_biquad_lowshelf( double frequency, double Q, double gain_db, int coefficients[5], int q_format )
{
    // Compute common factors
	double A  = pow( 10.0, (gain_db / 40.0) );
	double w0 = 2.0 * pi * frequency;
	double alpha = sin(w0)/2.0 * sqrt( (A + 1.0/A)*(1.0/Q - 1.0) + 2.0 );

    // Compute coefficients
	double b0 =   A * ( (A+1) - (A-1)*cos(w0) + 2*sqrt(A)*alpha );
	double b1 = 2*A * ( (A-1) - (A+1)*cos(w0) );
	double b2 =   A * ( (A+1) - (A-1)*cos(w0) - 2*sqrt(A)*alpha );
	double a0 =         (A+1) + (A-1)*cos(w0) + 2*sqrt(A)*alpha;
	double a1 =  -2 * ( (A-1) + (A+1)*cos(w0) );
	double a2 =         (A+1) + (A-1)*cos(w0) - 2*sqrt(A)*alpha;
	
	// Normalize gain to 'gain_db'
	b1 /= b0; b2 /= b0; a1 /= a0; a2 /= a0; 
	
	// Store as fixed-point values
	coefficients[0] = _float2fixed( b0, q_format );
	coefficients[1] = _float2fixed( b1, q_format );
	coefficients[2] = _float2fixed( b2, q_format );
	coefficients[3] = _float2fixed( a1, q_format );
	coefficients[4] = _float2fixed( a2, q_format );
}

// Biquad filter design - high side (treble) shelving filter
//
// 'frequency':    Filter frequency (+3db or -3db) normalized to the sampling frequency.
//                 0 < frequency < 0.5, where 0.5 represents Fs/2.
// 'Q':            Filter Q factor.
// 'qain_db'       The filter gain in dB (postive or negative).
//                 +gain results in treble shelf with max gain of 'gain_db'.
//                 -gain results in treble shelf with max attenuation of '-gain_db'.
// 'coefficients': Resulting fixed-point filter coefficients ordered as [b0,b1,b2,a1,a2].
// 'q_format':     Fixed point format, the number of bits making up fractional part.

void xmos_dsp_design_biquad_highshelf( double frequency, double Q, double gain_db, int coefficients[5], int q_format )
{
    // Compute common factors
	double A  = pow( 10.0, (gain_db / 40.0) );
	double w0 = 2.0 * pi * frequency;
	double alpha = sin(w0)/2.0 * sqrt( (A + 1.0/A)*(1.0/Q - 1.0) + 2.0 );

    // Compute coefficients
	double b0 =    A * ( (A+1) + (A-1)*cos(w0) + 2*sqrt(A)*alpha );
	double b1 = -2*A * ( (A-1) + (A+1)*cos(w0) );
	double b2 =    A * ( (A+1) + (A-1)*cos(w0) - 2*sqrt(A)*alpha );
	double a0 =          (A+1) - (A-1)*cos(w0) + 2*sqrt(A)*alpha;
	double a1 =    2 * ( (A-1) - (A+1)*cos(w0) );
	double a2 =          (A+1) - (A-1)*cos(w0) - 2*sqrt(A)*alpha;
	
	// Normalize gain to 'gain_db'
	b1 /= b0; b2 /= b0; a1 /= a0; a2 /= a0; 
	
	// Store as fixed-point values
	coefficients[0] = _float2fixed( b0, q_format );
	coefficients[1] = _float2fixed( b1, q_format );
	coefficients[2] = _float2fixed( b2, q_format );
	coefficients[3] = _float2fixed( a1, q_format );
	coefficients[4] = _float2fixed( a2, q_format );
}

// ================================================================================================
