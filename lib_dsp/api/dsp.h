// Copyright 2015-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#ifndef DSP_H_
#define DSP_H_

#include <stdint.h>

#include <dsp_adaptive.h>
#include <dsp_design.h>
#include <dsp_filters.h>
#include <dsp_matrix.h>
#include <dsp_statistics.h>
#include <dsp_math.h>
#include <dsp_math_int.h>
#include <dsp_fast_float.h>
#include <dsp_qformat.h>
#include <dsp_vector.h>
#include <dsp_fft.h>
#include <dsp_bfp.h>
#include <dsp_dct.h>

/* Macro to time function calls
 * After execution of this line the value in cycle_taken is valid.
 * The calling context must contain the variables tmr, start_time, end_time, overhead_time, cycle_time
 * overhead_time must have a valid value before the call.
 */
#define TIME_FUNCTION(function_call) do {tmr :> start_time; function_call; tmr :> end_time; cycles_taken = end_time-start_time-overhead_time;} while(0)


#endif

