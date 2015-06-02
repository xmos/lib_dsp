/******************************************************************************\
 * Header:  common_utils
 * File:    common_utils.h
 *
 * Description: Definitions, types, and prototypes for common_utils.xc
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

#ifndef _COMMON_UTILS_H_
#define _COMMON_UTILS_H_

#include <math.h>
#include "common_audio.h"

/** Swap effect after this many samples: (1 << 19) */
#define SWAP_NUM (1 << 19)

/** Log2[FADE_LEN] (in Samples): 14 */
#define FADE_BITS 14 //
#define FADE_LEN (1 << FADE_BITS) // Length of cross-fade in samples
#define HALF_FADE (FADE_LEN >> 1) // Half cross-fade length

/** Coefficients are represented as Fixed-point values. The Mantissa & Exponent are both of type COEF_T */
typedef S32_T COEF_T; //

/** Structure containing coefficients expressed as fixed point */
typedef struct FIX_POINT_TAG //
{
	COEF_T mant; // Mantissa
	COEF_T exp; // Exponent expressed as power of 2 (Log2 of scaling factor)
} FIX_POINT_S;

/** Different Processing States */
typedef enum PROC_STATE_ETAG //
{
  EFFECT = 0,			// DSP Effect On
  FX2DRY,					// Fade Effect to Dry
  DRY_ONLY,				// No Effect
  DRY2FX,					// Fade Dry to Effect
  START,					// Start-up mode
  NUM_PROC_STATES	// Handy Value!-)
} PROC_STATE_ENUM;

/** Control Token Codes */
typedef enum CNTRL_TOK_TAG //
{
  CFG_BIQUAD = 10,	// Configure BiQuad Filter
  CFG_GAIN,			// Configure Gain-shaping
  CFG_VOCODER,			// Configure Vocoder
  NUM_CNTRL_TOKS	// Handy Value!-)
} CNTRL_TOK_TYP;

/******************************************************************************/
/** Converts word reference to address.
 * \param inp_arr[] // input array of words
 * \return Address
 */
U32_T get_word_address( // Converts word reference to address
	S32_T inp_arr[] // input array of words
); // Return address
/******************************************************************************/
/** Increment a circular buffer offset. I.e. wraps if at end of buffer.
 * \param new_off // required new offset (without wrapping)
 * \param buf_siz // circular buffer size
 * \return Wrapped offset
 */
S32_T increment_circular_offset( // Increment a circular buffer offset. I.e. wraps if at end of buffer
	S32_T new_off, // required new offset (without wrapping)
	S32_T buf_siz // circular buffer size
); // Return wrapped offset
/*****************************************************************************/
/** Returns Cross-faded sample.
 * \param out_samps[]	// Buffer for cross-faded Output samples
 * \param fade_out_samps[]	// Buffer for Input samples being faded-out
 * \param fade_in_samps[]	// Buffer for Input samples being faded-in
 * \param num_chans	// Number of channels
 * \param weight	// Weighting for fade-in sample
 * \return Cross-faded sample
 */
void cross_fade_sample( // Returns Cross-faded sample
	S32_T out_samps[],	// Buffer for cross-faded Output samples
	S32_T fade_out_samps[],	// Buffer for Input samples being faded-out
	S32_T fade_in_samps[],	// Buffer for Input samples being faded-in
	S32_T num_chans,	// Number of channels
	S32_T weight	// Weighting for fade-in sample
); // Return cross-faded sample
/*****************************************************************************/
/** Returns faded-in sample.
 * \param out_samps[]	// Buffer for cross-faded Output samples
 * \param fade_in_samps[]	// Buffer for Input samples being faded-in
 * \param num_chans	// Number of channels
 * \param weight	// Weighting for fade-in sample
 * \return Faded-in sample
 */
void fade_in_sample( // Returns faded-in sample
	S32_T out_samps[],	// Buffer for cross-faded Output samples
	S32_T fade_in_samps[],	// Buffer for Input samples being faded-in
	S32_T num_chans,	// Number of channels
	S32_T weight	// Weighting for fade-in sample
); // Return faded-in sample
/*****************************************************************************/
/** Returns faded-out sample.
 * \param out_samps[]	// Buffer for cross-faded Output samples
 * \param fade_out_samps[]	// Buffer for Input samples being faded-out
 * \param num_chans	// Number of channels
 * \param weight	// Weighting for fade-in sample (NB This will be reversed for fade-out sample)
 * \return Faded-out sample
 */
void fade_out_sample( // Returns faded-out sample
	S32_T out_samps[],	// Buffer for cross-faded Output samples
	S32_T fade_out_samps[],	// Buffer for Input samples being faded-out
	S32_T num_chans,	// Number of channels
	S32_T weight	// Weighting for fade-in sample (NB This will be reversed for fade-out sample)
); // Return faded-out sample
/******************************************************************************/

#ifdef __XC__
// XC File

#else //if __XC__
// 'C' File

/******************************************************************************/
void scale_coef( // Scale and round floating point coeffiecient
	FIX_POINT_S * fix_coef_ps, // pointer to structure for fixed point format
	R64_T un_coef // input unscaled floating point coef.
);
/******************************************************************************/

#endif // else __XC__

#endif // _COMMON_UTILS_H_
// common_utils.h
