// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __dsp_h__
#define __dsp_h__

#include <lib_dsp_adaptive.h>
#include <lib_dsp_design.h>
#include <lib_dsp_filters.h>
#include <lib_dsp_matrix.h>
#include <lib_dsp_statistics.h>
#include <lib_dsp_math.h>
#include <lib_dsp_qformat.h>
#include <lib_dsp_vector.h>
#include <lib_dsp_fft.h>


/* Macro to time function calls
 * After execution of this line the value in cycle_taken is valid.
 * The calling context must contain the variables tmr, start_time, end_time, overhead_time, cycle_time
 * overhead_time must have a valid value before the call.
 */
#define TIME_FUNCTION(function_call) do {tmr :> start_time; function_call; tmr :> end_time; cycles_taken = end_time-start_time-overhead_time;} while(0)


#endif

