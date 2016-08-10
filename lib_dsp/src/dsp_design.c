// Copyright (c) 2015-2016, XMOS Ltd, All rights reserved

#include <platform.h>
#include <stdio.h>
#include <dsp_design.h>
#include <math.h>

static double pi = 3.14159265359;

static int32_t _float2fixed( float x, int32_t q )
{
    if( x < 0 ) return (((double)(1<<q)) * x - 0.5);
    else if( x > 0 ) return (((double)((1<<q)-1)) * x + 0.5);
    return 0;
}



void dsp_design_biquad_notch
(
    double frequency,
    double filter_Q,
    int32_t    coefficients[5],
    const int32_t q_format
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



void dsp_design_biquad_lowpass
(
    double frequency,
    double filter_Q,
    int32_t    coefficients[5],
    const int32_t q_format
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



void dsp_design_biquad_highpass
(
    double frequency,
    double filter_Q,
    int32_t    coefficients[5],
    const int32_t q_format
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



void dsp_design_biquad_allpass
(
    double frequency,
    double filter_Q,
    int32_t    coefficients[5],
    const int32_t q_format
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



void dsp_design_biquad_bandpass
(
    double frequency1,
    double frequency2,
    int32_t    coefficients[5],
    const int32_t q_format
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



void dsp_design_biquad_peaking
(
    double frequency,
    double filter_Q,
    double gain_db,
    int32_t    coefficients[5],
    const int32_t q_format
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



void dsp_design_biquad_lowshelf
(
    double frequency,
    double filter_Q,
    double shelf_gain_db,
    int32_t    coefficients[5],
    const int32_t q_format
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



void dsp_design_biquad_highshelf
(
    double frequency,
    double filter_Q,
    double shelf_gain_db,
    int32_t    coefficients[5],
    const int32_t q_format
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
