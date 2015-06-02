/******************************************************************************\
 * File:	biquad_simple.h
 *
 * Description: Definitions, types, and prototypes for module biquad_simple
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

#ifndef _BIQUAD_SIMPLE_H_
#define _BIQUAD_SIMPLE_H_

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <print.h>
#include "module_dsp_biquad_conf.h"
#include "types64bit.h"
#include "common_utils.h"

#ifndef NUM_BIQUAD_CHANS
	#error Define. NUM_BIQUAD_CHANS in module_dsp_biquad_conf.h
#endif // NUM_BIQUAD_CHANS

/** Different Processing States  */
typedef enum FILT_MODE_TAG
{
  LO_PASS = 0,	// Low Pass Filter
  HI_PASS,			// High Pass Filter
  BAND_PASS,		// Band Pass Filter
  BAND_STOP,		// Band Stop (Notch) Filter
  ALL_PASS,			// Phase-Shift Filter
  CUSTOM,				// Custom Filter
  NUM_FILT_MODES	// Handy Value!-)
} FILT_MODE_TYP;

/**  Default Filter Mode  LO_PASS */
#define DEF_FILT_MODE LO_PASS

/** Default Sample Frequency (In Hz) 48000 */
#define DEF_SAMP_FREQ 48000 // Default Sample Frequency (In Hz)

/** Default Significant Filter Frequency (In Hz) 1000  */
#define DEF_SIG_FREQ 1000 // Default Significant Filter Frequency (In Hz)

/** Number of bits used to scale Quality-factor 8 */
#define QUAL_BITS 8 //

/** Default Quality-factor of 1, scaled by (1 < QUAL_BITS) */
#define DEF_QUAL_FACT (1 << QUAL_BITS) //

/** Structure containing BiQuad parameters */
typedef struct BIQUAD_PARAM_TAG //
{
	FILT_MODE_TYP filt_mode; // BiQuad filter type (e.g. Low-pass)
	S32_T samp_freq; // Sample frequency
	S32_T sig_freq; // Significant filter frequency (e.g. centre-freq of band-pass)
	S32_T qual; // Quality-factor
} BIQUAD_PARAM_S;


/******************************************************************************/
/** Use BiQuad filter on one sample from one channel.
 * Samples are left-aligned signed values.
 * e.g. 24-bit audio will look like 0x12345600 (positive) or 0xFF123400 (negative)
 * \param biquad_id // Identifies which BiQuad to use
 * \param inp_samp // Unfiltered input sample from channel
 * \param cur_chan // current channel
 * \return The Filtered Output Sample
 */
S32_T use_biquad_filter( // Use BiQuad filter on one sample from one channel
	S32_T biquad_id, // Identifies which BiQuad to use
	S32_T inp_samp, // Unfiltered input sample from channel
	S32_T cur_chan  // current channel
); // Return filtered Output Sample
/******************************************************************************/

#ifdef __XC__
// XC File

/******************************************************************************/
/** Configure BiQuad filter.
 * \param biquad_id // Identifies which BiQuad to use
 * \param cur_param_ps // Reference to structure containing current biquad filter parameters
 */
void config_biquad_filter( // Configure BiQuad filter
	S32_T biquad_id, // Identifies which BiQuad to use
	BIQUAD_PARAM_S &cur_param_ps // Reference to structure containing current biquad filter parameters
);

/******************************************************************************/
#else //if __XC__
// 'C' File

/******************************************************************************/
void config_biquad_filter( // Configure BiQuad filter
	S32_T biquad_id, // Identifies which BiQuad to use
	BIQUAD_PARAM_S * cur_param_ps // Pointer to structure containing current biquad filter parameters
);
/******************************************************************************/

#define PI 3.14159265

// Filter parameters
#define MIN_SAMP_FREQ 20 // Minimum Sample Frequency (In Hz)

typedef R64_T REAL_T; // Real or floating-point type (NB not supported in earlier XC compilers)
typedef S64_T FILT_T; // type used for internal filter calculations

/* Scaling Data
 * BiQuad requires 5 MAC's, Therefore 3 bits of headroom required for accumulator
 * On Input, samples are stored in the MS-bits of a 32-bit integer.
 * The input samples are down-scaled to the LS-Bits of the integer, before any calculation.
 * This provides 8 bits of headroom
 */
#define SAMP_BITS ((sizeof(SAMP_CHAN_T) << 3) - 1) // Bits used to represent magnitude of input/output sample (i.e without sign bit)
#define COEF_BITS ((sizeof(COEF_T) << 3) - 1) // Bits used to represent magnitude of full precision sample (i.e without sign bit)
#define HEAD_BITS 8 // Headroom Bits

#define HEAD_FACT (1 << HEAD_BITS) // coefficient up-scaling factor
#define HALF_HEAD (HEAD_FACT >> 1) // half up-scaling factor

#define NUM_FILT_TAPS 3 // Number of filter taps (for FIR or IIR, I.E. FIR+IIR = 2 * NUM_FILT_TAPS)

typedef struct FIX_CONST_TAG // Structure containing data for multiply by fixed-point constant
{
	FIX_POINT_S coef; // Constant coefficient in fixed-point format
	COEF_T half; // Half of scaling factor (used in rounding)
	COEF_T err; // rounding error
} FIX_CONST_S;

typedef struct BIQUAD_COEF_TAG // Structure containing BiQuad coefficients
{
	FIX_CONST_S b[NUM_FILT_TAPS]; // Array of weighting for input samples (FIR taps)
	FIX_CONST_S a[NUM_FILT_TAPS]; // Array of weighting for output results (IIR taps)
} BIQUAD_COEF_S;

typedef struct BIQUAD_CHAN_TAG // Structure containing intermediate data for one channel, updated every sample
{
	FILT_T iir[NUM_FILT_TAPS]; // Array of previous IIR filter results
	FILT_T filt[NUM_FILT_TAPS]; // Array of previous filterd output samples
	FILT_T inp_err; // Rounding error due to down-sampling input
} BIQUAD_CHAN_S;

typedef struct BIQUAD_TAG // Structure containing all biquad data
{
	BIQUAD_CHAN_S bq_chan_s[NUM_BIQUAD_CHANS]; // Array of structure containing intermediate data for each channel, updated every sample
	BIQUAD_COEF_S common_coefs_s;  // Structure containing BiQuad coefficients
	S32_T init_done;	// Flag indicating structure is initialised
	S32_T params_set; // Flag indicating Parameters have been set
} BIQUAD_S;

typedef struct BIQUAD_ALL_TAG // Structure containing all biquad data
{
	BIQUAD_S biquad_s[NUM_APP_BIQUADS]; // Array of structures for each BiQuad
	S32_T first_config; // Flag set until 1st BiQuad configuration complete
} BIQUAD_ALL_S;

#endif // else __XC__

#endif /* _BIQUAD_SIMPLE_H_ */

// biquad_simple.h
