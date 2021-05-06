// Copyright 2015-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdio.h>
#include "dsp.h"

// Results of biquad coeff generation ara normalized such that a0 == 1.
// When using lib_dsp biquad functions that data arrays must be DWORD aligned.
// Declaring coeffs/state data globablly ensures this while locally does not!
// The biquad functions utilize coeffs arrays of length 5 but ...
// The coeffs below are arraned as N x 6 to keep coeff arrays DWORD aligned.

int32_t [[aligned(8)]]coeff[8][6]; // Coefficients for each filter
//Note: array size of 6 is chosen to enforce 64 bit alignment for ldd and std
int32_t [[aligned(8)]]state[8][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
                       {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}; // State data for each filter

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

	dsp_design_biquad_notch    ( 0.25, 0.707,      coeff[0], 28 );
	dsp_design_biquad_lowpass  ( 0.25, 0.9,      coeff[1], 28 );
	dsp_design_biquad_highpass ( 0.25, 0.707,      coeff[2], 28 );
	dsp_design_biquad_allpass  ( 0.25, 0.707,      coeff[3], 28 );
	dsp_design_biquad_bandpass ( 0.24, 0.26,       coeff[4], 28 );
	dsp_design_biquad_peaking  ( 0.25, 0.707, 3.0, coeff[5], 28 );
	dsp_design_biquad_lowshelf ( 0.25, 0.707, 3.0, coeff[6], 28 );
	dsp_design_biquad_highshelf( 0.25, 0.707, 3.0, coeff[7], 28 );

	// Initialize filter state data to zeros
	
	for( int32_t ii = 0; ii < 8; ++ii ) {
		for( int32_t jj = 0; jj < 4; ++jj ) state[ii][jj] = 0;
	}

	// Perform 8 passes through filters independently using step function

	for( int32_t ii = 0; ii < 8; ++ii )
	{
		int32_t sample;
		printf( "Impulse response of %s, ", names[ii] );
		for( int32_t jj = 0; jj < 8; ++jj ) {
			if(jj==0) { // Dirac delta function. 1 at 0, 0 otherwise
				sample = Q28( 1.0 );
			} else {
				sample = 0;
			}
			int32_t y = dsp_filters_biquad( sample, coeff[ii], state[ii], 28 );
			printf( "%+04.8f, ", F28(y) );
		}
		printf( "\n" );
	}

	return 0;
}

