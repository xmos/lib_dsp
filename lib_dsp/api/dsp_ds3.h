// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef _DPS_DS3_H_
#define _DPS_DS3_H_

#ifdef __XC__
extern "C" {
#endif

// ===========================================================================
//
// Defines
//
// ===========================================================================

// General defines
// ---------------
// Filter related
#define     DSP_DS3_N_COEFS                          144                     // Number of coefficients must be a multiple of 6
// Filters with "_b_" in their filenames in FIRDS3.h have higher attenuation at Nyquist (> 60dB compared with 20dB ) but with an earlier cutoff.
#define     DSP_DS3_COEFS_FILE                       "FilterData/firds3_b_144.dat"
//#define     DSP_DS3_COEFS_FILE                       "FilterData/firds3_144.dat"

// ===========================================================================
//
// TypeDefs
//
// ===========================================================================

// To avoid C type definitions when including this file from assembler
#ifndef INCLUDE_FROM_ASM

/** Downsample by 3 return codes
 *
 * This type describes the possible error status states from calls to the ds3 API.
 */
typedef enum dsp_ds3_return_code_t
{
    DSP_DS3_NO_ERROR = 0,
    DSP_DS3_ERROR    = 1
} dsp_ds3_return_code_t;

// FIRDS3 Ctrl
// -----------
typedef struct dsp_ds3_ctrl_t
{
    int*                                    in_data;          // Pointer to input data (3 samples)
    int*                                    out_data;         // Pointer to output data (1 sample)

    int*                                    delay_base;       // Pointer to delay line base
    unsigned int                            delay_len;        // Total length of delay line
    int*                                    delay_pos;        // Pointer to current position in delay line
    int*                                    delay_wrap;       // Delay buffer wrap around address (for circular buffer simulation)
    unsigned int                            delay_offset;     // Delay line offset for second write (for circular buffer simulation)

    unsigned int                            inner_loops;      // Number of inner loop iterations
    unsigned int                            num_coeffs;       // Number of coefficients
    int*                                    coeffs;           // Pointer to coefficients
} dsp_ds3_ctrl_t;

/** This function initialises the decimate by 3 function for a given instance
 *
 *
 *  \param      dsp_ds3_ctrl   DS3 control structure
 *  \returns    DSP_DS3_NO_ERROR on success, DSP_DS3_ERROR on failure
 */
dsp_ds3_return_code_t dsp_ds3_init(dsp_ds3_ctrl_t* dsp_ds3_ctrl)
    __attribute__((deprecated("Please use 'src_ds3_init' in lib_src instead")));

/** This function clears the decimate by 3 delay line for a given instance
 *
 *
 *  \param      dsp_ds3_ctrl   DS3 control structure
 *  \returns    DSP_DS3_NO_ERROR on success, DSP_DS3_ERROR on failure
 */
dsp_ds3_return_code_t dsp_ds3_sync(dsp_ds3_ctrl_t* dsp_ds3_ctrl)
    __attribute__((deprecated("Please use 'src_ds3_sync' in lib_src instead")));

/** This function performs the decimation on three input samples and outputs on sample
 *  The input and output buffers are pointed to by members of the dsp_ds3_ctrl structure
 *
 *
 *  \param      dsp_ds3_ctrl   DS3 control structure
 *  \returns    DSP_DS3_NO_ERROR on success, DSP_DS3_ERROR on failure
 */
dsp_ds3_return_code_t dsp_ds3_proc(dsp_ds3_ctrl_t* dsp_ds3_ctrl)
    __attribute__((deprecated("Please use 'src_ds3_proc' in lib_src instead")));

#endif // INCLUDE_FROM_ASM

#ifdef __XC__
}
#endif

#endif // _DPS_DS3_H_
