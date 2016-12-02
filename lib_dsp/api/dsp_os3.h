// Copyright (c) 2016, XMOS Ltd, All rights reserved


#ifndef _DSP_OS3_H_
#define _DSP_OS3_H_

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

#define     DSP_OS3_N_COEFS                          144                     // Number of coefficients must be a multiple of 6
#define     DSP_OS3_N_PHASES                         3                       // Number of output phases (3 as OS3 over-sample by 3)

// This filter has about 20dB attenuation at 8kHz but later cutoff
//#define     DSP_OS3_COEFS_FILE                       "FilterData/firos3_144.dat"
// This filter has more than 60dB attenuation at 8kHz
#define       DSP_OS3_COEFS_FILE                       "FilterData/firos3_b_144.dat"

// ===========================================================================
//
// TypeDefs
//
// ===========================================================================

// To avoid C type definitions when including this file from assembler
#ifndef INCLUDE_FROM_ASM

/** Oversample by 3 return codes
 *
 * This type describes the possible error status states from calls to the os3 API.
 */
typedef enum dsp_os3_return_code_t 
{
    DSP_OS3_NO_ERROR = 0,
    DSP_OS3_ERROR    = 1
} dsp_os3_return_code_t;

// FIROS3 Ctrl
// -----------
typedef struct dsp_os3_ctrl_t
{
    int                                     in_data;          // Input data (to be updated every 3 output samples, i.e. when iPhase == 0)
    int                                     out_data;         // Output data (1 sample)
    int                                     phase;            // Current output phase (when reaching '0', a new input sample is required)

    int*                                    delay_base;       // Pointer to delay line base
    unsigned int                            delay_len;       // Total length of delay line
    int*                                    delay_pos;        // Pointer to current position in delay line
    int*                                    delay_wrap;       // Delay buffer wrap around address (for circular buffer simulation)
    unsigned int                            delay_offset;     // Delay line offset for second write (for circular buffer simulation)

    unsigned int                            inner_loops;      // Number of inner loop iterations
    unsigned int                            num_coeffs;       // Number of coefficients
    int*                                    coeffs;           // Pointer to coefficients
} dsp_os3_ctrl_t;


/** This function initialises the oversample by 3 function for a given instance
 *
 *
 *  \param      dsp_os3_ctrl   OS3 control structure
 *  \returns    DSP_OS3_NO_ERROR on success, DSP_OS3_ERROR on failure
 */
dsp_os3_return_code_t             dsp_os3_init(dsp_os3_ctrl_t* dsp_os3_ctrl)
    __attribute__((deprecated("Please use 'src_os3_init' in lib_src instead")));

/** This function clears the oversample by 3 delay line for a given instance
 *
 *
 *  \param      dsp_os3_ctrl   OS3 control structure
 *  \returns    DSP_OS3_NO_ERROR on success, DSP_OS3_ERROR on failure
 */
dsp_os3_return_code_t             dsp_os3_sync(dsp_os3_ctrl_t* dsp_os3_ctrl)
    __attribute__((deprecated("Please use 'src_os3_sync' in lib_src instead")));

/** This function pushes a single input sample into the filter
 *  It should be called three times for each FIROS3_proc call
 *
 *
 *  \param      dsp_os3_ctrl   OS3 control structure
 *  \returns    DSP_OS3_NO_ERROR on success, DSP_OS3_ERROR on failure
 */
dsp_os3_return_code_t             dsp_os3_input(dsp_os3_ctrl_t* dsp_os3_ctrl)
    __attribute__((deprecated("Please use 'src_os3_input' in lib_src instead")));

/** This function performs the oversampling by 3 and outputs one sample
 *  The input and output buffers are pointed to by members of the dsp_os3_ctrl structure
 *
 *
 *  \param      dsp_os3_ctrl   OS3 control structure
 *  \returns    DSP_OS3_NO_ERROR on success, DSP_OS3_ERROR on failure
 */
dsp_os3_return_code_t             dsp_os3_proc(dsp_os3_ctrl_t* dsp_os3_ctrl)
    __attribute__((deprecated("Please use 'src_os3_proc' in lib_src instead")));

#endif // INCLUDE_FROM_ASM

#ifdef __XC__
}
#endif

#endif // _DSP_OS3_H_
