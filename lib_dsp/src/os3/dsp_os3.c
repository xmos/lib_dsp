// Copyright (c) 2016, XMOS Ltd, All rights reserved
// ===========================================================================
// ===========================================================================
//
// FIROS3 functions implementation file
//
// ===========================================================================
// ===========================================================================


// ===========================================================================
//
// Includes
//
// ===========================================================================
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

// FIROS3 includes
#include "dsp_os3.h"
#include "fir_inner_loop_asm.h"

// ===========================================================================
//
// Defines
//
// ===========================================================================

// State init value
#define        DSP_OS3_STATE_INIT                        0

// ===========================================================================
//
// Variables
//
// ===========================================================================

int                    dsp_os3_coeffs[DSP_OS3_N_COEFS]     = {
    #include DSP_OS3_COEFS_FILE
};

// ===========================================================================
//
// Functions implementations
//
// ===========================================================================

dsp_os3_return_code_t dsp_os3_init(dsp_os3_ctrl_t* dsp_ds3_ctrl)
{
    // Check that delay line base has been set
    if (dsp_ds3_ctrl->delay_base == 0) {
        return DSP_OS3_ERROR;
    }

    // Setup from FIROS2 descriptor
    dsp_ds3_ctrl->delay_len         = (DSP_OS3_N_COEFS/DSP_OS3_N_PHASES)<<1;                        // Double length for circular buffer simulation. x3 over-sampler, so only 1/3rd of coefs length needed
    dsp_ds3_ctrl->delay_wrap        = dsp_ds3_ctrl->delay_base + (DSP_OS3_N_COEFS/DSP_OS3_N_PHASES);
    dsp_ds3_ctrl->delay_offset      = (DSP_OS3_N_COEFS/DSP_OS3_N_PHASES);
    dsp_ds3_ctrl->inner_loops       = (DSP_OS3_N_COEFS/DSP_OS3_N_PHASES)>>1;                        // Due to 2 x 32bits read for coefs per inner loop and x3 over-sampler, so only 1/3rd of coefs length needed
    dsp_ds3_ctrl->num_coeffs        = DSP_OS3_N_COEFS;
    dsp_ds3_ctrl->coeffs            = dsp_os3_coeffs;

    // Sync the FIROS3
    if (dsp_os3_sync(dsp_ds3_ctrl) != DSP_OS3_NO_ERROR) {
        return DSP_OS3_ERROR;
    }

    return DSP_OS3_NO_ERROR;
}

dsp_os3_return_code_t dsp_os3_sync(dsp_os3_ctrl_t* dsp_ds3_ctrl)
{
    // Set delay line index back to base
    dsp_ds3_ctrl->delay_pos = dsp_ds3_ctrl->delay_base;

    // Clear delay line
    for (unsigned int ui = 0; ui < dsp_ds3_ctrl->delay_len; ui++) {
        dsp_ds3_ctrl->delay_base[ui]    = DSP_OS3_STATE_INIT;
    }

    // Set phase to '0' to indicate that a new input sample is needed
    dsp_ds3_ctrl->phase = 0;

    return DSP_OS3_NO_ERROR;
}

dsp_os3_return_code_t dsp_os3_input(dsp_os3_ctrl_t* dsp_ds3_ctrl)
{
    // Write new input sample from control structure to delay line
    // at both positions for circular buffer simulation
    *dsp_ds3_ctrl->delay_pos                                     = dsp_ds3_ctrl->in_data;
    *(dsp_ds3_ctrl->delay_pos + dsp_ds3_ctrl->delay_offset)      = dsp_ds3_ctrl->in_data;

    // Step delay index with circular buffer simulation
    dsp_ds3_ctrl->delay_pos                += 1;
    if (dsp_ds3_ctrl->delay_pos >= dsp_ds3_ctrl->delay_wrap) {
        dsp_ds3_ctrl->delay_pos = dsp_ds3_ctrl->delay_base;
    }

    return DSP_OS3_NO_ERROR;
}

dsp_os3_return_code_t                dsp_os3_proc(dsp_os3_ctrl_t* dsp_ds3_ctrl)
{
    int*          data;
    int*          coeffs;
    int           data0;
    __int64_t     accumulator;

    // Clear accumulator and set data/coefs access pointers
    data            = dsp_ds3_ctrl->delay_pos;
    coeffs          = dsp_ds3_ctrl->coeffs + dsp_ds3_ctrl->phase;
    accumulator     = 0;

    if ((unsigned)data & 0b0100) {
        fir_inner_loop_asm_odd(data, coeffs, &data0, dsp_ds3_ctrl->inner_loops);
    } else {
        fir_inner_loop_asm(data, coeffs, &data0, dsp_ds3_ctrl->inner_loops);
    }

    // Write output
    dsp_ds3_ctrl->out_data = data0;

    // Step phase
    dsp_ds3_ctrl->phase += (DSP_OS3_N_COEFS/DSP_OS3_N_PHASES);
    if (dsp_ds3_ctrl->phase == DSP_OS3_N_COEFS) {
        dsp_ds3_ctrl->phase = 0;
    }

    return DSP_OS3_NO_ERROR;
}

