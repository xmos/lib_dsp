// Copyright (c) 2016, XMOS Ltd, All rights reserved
// ===========================================================================
// ===========================================================================
//
// FIRDS3 functions implementation file
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

// FIRDS3 includes
#include "dsp_ds3.h"
#include "fir_inner_loop_asm.h"


// ===========================================================================
//
// Defines
//
// ===========================================================================


// State init value
#define        DSP_DS3_STATE_INIT                        0




// ===========================================================================
//
// Variables
//
// ===========================================================================

int dsp_ds3_coeffs[DSP_DS3_N_COEFS] = {
    #include DSP_DS3_COEFS_FILE
};

// ===========================================================================
//
// Local Functions prototypes
//
// ===========================================================================



// ===========================================================================
//
// Functions implementations
//
// ===========================================================================

dsp_ds3_return_code_t dsp_ds3_init(dsp_ds3_ctrl_t* dsp_ds3_ctrl)
{
    // Check that delay line base has been set
    if (dsp_ds3_ctrl->delay_base == 0) {
        return DSP_DS3_ERROR;
    }

    // Setup from FIRDS2 descriptor
    dsp_ds3_ctrl->delay_len       = DSP_DS3_N_COEFS<<1;                                    // Double length for circular buffer simulation
    dsp_ds3_ctrl->delay_wrap      = dsp_ds3_ctrl->delay_base + DSP_DS3_N_COEFS;
    dsp_ds3_ctrl->delay_offset    = DSP_DS3_N_COEFS;
    dsp_ds3_ctrl->inner_loops     = DSP_DS3_N_COEFS>>1;                                    // Due to 2 x 32bits read for coefs per inner loop
    dsp_ds3_ctrl->num_coeffs      = DSP_DS3_N_COEFS;
    dsp_ds3_ctrl->coeffs          = dsp_ds3_coeffs;

    // Sync the FIRDS3
    if (dsp_ds3_sync(dsp_ds3_ctrl) != DSP_DS3_NO_ERROR) {
        return DSP_DS3_ERROR;
    }

    return DSP_DS3_NO_ERROR;
}

dsp_ds3_return_code_t dsp_ds3_sync(dsp_ds3_ctrl_t* dsp_ds3_ctrl)
{
    // Set delay line index back to base
    dsp_ds3_ctrl->delay_pos    = dsp_ds3_ctrl->delay_base;

    // Clear delay line
    for (unsigned int ui = 0; ui < dsp_ds3_ctrl->delay_len; ui++) {
        dsp_ds3_ctrl->delay_base[ui]    = DSP_DS3_STATE_INIT;
    }

    return DSP_DS3_NO_ERROR;
}

dsp_ds3_return_code_t dsp_ds3_proc(dsp_ds3_ctrl_t* dsp_ds3_ctrl)
{
    int*            data;
    int*            coeffs;
    int             data0;
    __int64_t       accumulator;

    // Get three new data samples to delay line (double write for circular buffer simulation)
    data0                    = *dsp_ds3_ctrl->in_data;
    *dsp_ds3_ctrl->delay_pos                                       = data0;
    *(dsp_ds3_ctrl->delay_pos + dsp_ds3_ctrl->delay_offset)        = data0;

    data0                    = *(dsp_ds3_ctrl->in_data + 1);
    *(dsp_ds3_ctrl->delay_pos + 1)                                 = data0;
    *(dsp_ds3_ctrl->delay_pos + dsp_ds3_ctrl->delay_offset + 1)    = data0;

    data0                    = *(dsp_ds3_ctrl->in_data + 2);
    *(dsp_ds3_ctrl->delay_pos + 2)                                 = data0;
    *(dsp_ds3_ctrl->delay_pos + dsp_ds3_ctrl->delay_offset + 2)    = data0;

    // Step delay with circular simulation (will also rewrite to control structure for next round)
    // Note as delay line length is a multiple of 3 (because filter coefficients length is a multiple of 3)
    // we only have to do the wrap around check after having written 3 input samples
    dsp_ds3_ctrl->delay_pos += 3;
    if (dsp_ds3_ctrl->delay_pos >= dsp_ds3_ctrl->delay_wrap) {
        dsp_ds3_ctrl->delay_pos = dsp_ds3_ctrl->delay_base;
    }

    // Clear accumulator and set access pointers
    data                    = dsp_ds3_ctrl->delay_pos;
    coeffs                  = dsp_ds3_ctrl->coeffs;
    accumulator             = 0;
    if ((unsigned)data & 0b0100) {
        fir_inner_loop_asm_odd(data, coeffs, &data0, dsp_ds3_ctrl->inner_loops);
    } else {
        fir_inner_loop_asm(data, coeffs, &data0, dsp_ds3_ctrl->inner_loops);
    }

    *dsp_ds3_ctrl->out_data = data0;

    return DSP_DS3_NO_ERROR;
}

