/******************************************************************************\
 * File:	common_utils.c
 *
 * Description: Contains common routines used by DSP audio effects
 *
 * Version: 0v1
 * Build:
 *
 * The copyrights, all other intellectual and industrial
 * property rights are retained by XMOS and/or its licensors.
 * Terms and conditions covering the use of this code can
 * be found in the Xmos End User License Agreement.
 *
 * Copyright XMOS Ltd 2012
 *
 * In the case where this code is a modification of existing code
 * under a separate license, the separate license terms are shown
 * below. The modifications to the code are still covered by the
 * copyright notice above.
 *
\******************************************************************************/

#include "common_utils.h"

/******************************************************************************/
U32_T get_word_address( // Converts word reference to address
	S32_T inp_arr[] // input array
) // Return wrapped offset
{
	return (U32_T)&(inp_arr[0]); // Return Address
} // get_wrod_address
/******************************************************************************/
S32_T increment_circular_offset( // Increment a circular buffer offset. I.e. wraps if at end of buffer
	S32_T new_off, // required new offset (without wrapping)
	S32_T buf_siz // circular buffer size
) // Return wrapped offset
{
	// Check if at and of buffer
	if (buf_siz <= new_off)
	{
		return 0; // Move to start of buffer
	}

	return new_off; // Normal Increment
} // increment_circular_offset
/*****************************************************************************/
void cross_fade_sample( // Returns Cross-faded sample
	S32_T out_samps[],	// Buffer for cross-faded Output samples
	S32_T fade_out_samps[],	// Buffer for Input samples being faded-out
	S32_T fade_in_samps[],	// Buffer for Input samples being faded-in
	S32_T num_chans,	// Number of channels
	S32_T weight	// Weighting for fade-in sample
) // Return cross-faded sample
{
	S32_T chan_cnt; // Channel counter
	S64_T tmp_samp; // Intermediate sample value


	for(chan_cnt = 0; chan_cnt < num_chans; chan_cnt++)
	{
		tmp_samp = (S64_T)fade_in_samps[chan_cnt] - (S64_T)fade_out_samps[chan_cnt];
		tmp_samp = ((tmp_samp * weight) + (S64_T)HALF_FADE) >> FADE_BITS;
		out_samps[chan_cnt] = (S32_T)(tmp_samp + fade_out_samps[chan_cnt]);
	} // for chan_cnt
} // cross_fade_sample
/*****************************************************************************/
void fade_in_sample( // Returns faded-in sample
	S32_T out_samps[],	// Buffer for cross-faded Output samples
	S32_T fade_in_samps[],	// Buffer for Input samples being faded-in
	S32_T num_chans,	// Number of channels
	S32_T in_weight	// Weighting for fade-in sample
) // Return faded-in sample
{
	S32_T chan_cnt; // Channel counter


	for(chan_cnt = 0; chan_cnt < num_chans; chan_cnt++)
	{
		out_samps[chan_cnt] = (S32_T)((((S64_T)fade_in_samps[chan_cnt] * in_weight) + (S64_T)HALF_FADE) >> FADE_BITS);
	} // for chan_cnt
} // fade_in_sample
/*****************************************************************************/
void fade_out_sample( // Returns faded-out sample
	S32_T out_samps[],	// Buffer for cross-faded Output samples
	S32_T fade_out_samps[],	// Buffer for Input samples being faded-out
	S32_T num_chans,	// Number of channels
	S32_T in_weight	// Weighting for fade-in sample
) // Return faded-out sample
{
	S32_T chan_cnt; // Channel counter
	S64_T out_weight = (FADE_LEN - in_weight); // Compute weighting for fade-out sample


	for(chan_cnt = 0; chan_cnt < num_chans; chan_cnt++)
	{
		out_samps[chan_cnt] = (S32_T)((((S64_T)fade_out_samps[chan_cnt] * out_weight) + (S64_T)HALF_FADE) >> FADE_BITS);
	} // for chan_cnt
} // fade_out_sample
/******************************************************************************/
void scale_coef( // Scale and round floating point coeffiecient
	FIX_POINT_S * fix_coef_ps, // Pointer to structure for fixed point format
	R64_T un_coef // input unscaled floating point coef.
)
{
	S32_T sign_coef = 1; // sign of coef. preset to postive. NB S8_T has bug


	fix_coef_ps->exp = 0;

	// Check for special case of zero coef.
	if (0 == un_coef )
	{
		fix_coef_ps->mant = 0;
		fix_coef_ps->exp = 123456789; // NB Arbitary very large number. Required for A/B coef comparison
	} // if (0 == un_coef )
	else
	{	// Non-zero coef.

		// Create absolute and sign components
		if (un_coef < 0)
		{
			sign_coef = -1; // Update sign
			un_coef = -un_coef; // NB un_coef is now absolute value
		} // if (un_coef < 0)

		// Loop while mantissa NOT at full accuracy
		while (un_coef < (S64_T)0x40000000)
		{
			fix_coef_ps->exp++; // Increment exponent
			un_coef *= 2; // Double input
		} // while (scale_coef < 0x40000000)

		// Coef should now be in range 0x4000_0000 .. 0x7fff_ffff
		fix_coef_ps->mant = sign_coef * (COEF_T)floor( (R64_T)0.5 + un_coef ); // calculate signed mantissa
	} // else !(0 == un_coef )

} // scale_coef
/******************************************************************************/
// common_utils.c
