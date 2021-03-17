// Copyright 2019-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include "dsp.h"
#define IIR_ORDERS 8
#define MAX_BIQUAD_ORDER 4
#define IIRS_PER_ORDER 16
extern unsigned biquad_orders[IIR_ORDERS][IIRS_PER_ORDER];
extern int32_t filter[IIR_ORDERS][IIRS_PER_ORDER][DSP_NUM_COEFFS_PER_BIQUAD*MAX_BIQUAD_ORDER];
extern unsigned q_factors[IIR_ORDERS][IIRS_PER_ORDER];
