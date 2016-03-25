// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <stdio.h>
#include "lib_dsp.h"

// Results of biquad coeff generation ara normalized such that a0 == 1.
// When using lib_dsp biquad functions that data arrays must be DWORD aligned.
// Declaring coeffs/state data globablly ensures this while locally does not!
// The biquad functions utilize coeffs arrays of length 5 but ...
// The coeffs below are arraned as N x 6 to keep coeff arrays DWORD aligned.

int32_t coeff[8][6]; // Coefficients for each filter
int32_t state[8][4]; // State data for each filter

const char* names[] =
{
	"notch    ", "lowpass  ", "highpass ", "allpass  ",
	"bandpass ", "peaking  ", "lowshelf ", "highshelf",
};

int main( void )
{
	// Generate biquad filter coefficients for various filters
	// Frequency specifications are normalized to Fs (i.e. 0.25 = 1/4 Fs)
	// Q for filters is 0.707.
	// Gain for peaking/lowshelf/highshelf filters is 3.0 dB.
	// Q format Q28 (4-bit signed integer, 28-bit fraction) is used.

	lib_dsp_design_biquad_notch    ( 0.25, 0.707,      coeff[0], 28 );
	lib_dsp_design_biquad_lowpass  ( 0.25, 0.707,      coeff[1], 28 );
	lib_dsp_design_biquad_highpass ( 0.25, 0.707,      coeff[2], 28 );
	lib_dsp_design_biquad_allpass  ( 0.25, 0.707,      coeff[3], 28 );
	lib_dsp_design_biquad_bandpass ( 0.24, 0.26,       coeff[4], 28 );
	lib_dsp_design_biquad_peaking  ( 0.25, 0.707, 3.0, coeff[5], 28 );
	lib_dsp_design_biquad_lowshelf ( 0.25, 0.707, 3.0, coeff[6], 28 );
	lib_dsp_design_biquad_highshelf( 0.25, 0.707, 3.0, coeff[7], 28 );

	// Initialize filter state data to zeros
	
	for( int32_t ii = 0; ii < 8; ++ii ) {
		for( int32_t jj = 0; jj < 4; ++jj ) state[ii][jj] = 0;
	}

	// Perform 8 passes through filters independently using step function

	for( int32_t ii = 0; ii < 8; ++ii )
	{
		int sample = Q28( 1.0 );
		printf( "%s ", names[ii] );
		for( int32_t jj = 0; jj < 8; ++jj ) {
			sample = lib_dsp_filters_biquad( sample, coeff[ii], state[ii], 28 );
			printf( "%+04.3f ", F28(sample) );
		}
		printf( "\n" );
	}

	return 0;
}

