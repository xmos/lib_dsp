// Copyright 2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include <xs1.h>
#include <xclib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "dsp_fft_xs3.h"
#include "bfp_math.h"
#include "dsp_bfp.h"

void fft_float_mono_forward_512(float buffer[]) {
  int exp = dsp_float_to_bfp(buffer, FFT_LENGTH/2);

  // Now call the three functions to do an FFT
  xs3_fft_index_bit_reversal((buffer, complex_s32_t[]), FFT_LENGTH/2);
  headroom_t hr = 2;
  xs3_fft_dit_forward((buffer, complex_s32_t[]), FFT_LENGTH/2, &hr, &exp);
  xs3_fft_mono_adjust((buffer, complex_s32_t[]), FFT_LENGTH, 0);

  // And unpack back to floating point values
  dsp_bfp_to_float(buffer, FFT_LENGTH/2, exp);
}

void fft_float_mono_inverse_512(float buffer[]) {
  int exp = dsp_float_to_bfp(buffer, FFT_LENGTH/2);

  xs3_fft_mono_adjust((buffer, complex_s32_t[]), FFT_LENGTH, 1);
  xs3_fft_index_bit_reversal((buffer, complex_s32_t[]), FFT_LENGTH/2);
  headroom_t hr = 2;
  xs3_fft_dit_inverse((buffer, complex_s32_t[]), FFT_LENGTH/2, &hr, &exp);

  dsp_bfp_to_float(buffer, FFT_LENGTH/2, exp);
}

