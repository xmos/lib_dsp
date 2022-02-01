// Copyright 2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include <xs1.h>
#include <xclib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bfp_math.h"
#include "dsp_bfp.h"

#define LENGTH    (512)

float sqr(float x) {return x*x;}

void fft_float_mono_forward_512(float buffer[]) {
  int exp = dsp_float_to_bfp(buffer, LENGTH/2);

  // Now call the three functions to do an FFT
  xs3_fft_index_bit_reversal((buffer, complex_s32_t[]), LENGTH/2);
  headroom_t hr = 2;
  xs3_fft_dit_forward((buffer, complex_s32_t[]), LENGTH/2, &hr, &exp);
  xs3_fft_mono_adjust((buffer, complex_s32_t[]), LENGTH, 0);

  // And unpack back to floating point values
  dsp_bfp_to_float(buffer, LENGTH/2, exp);
}

void fft_float_mono_inverse_512(float buffer[]) {
  int exp = dsp_float_to_bfp(buffer, LENGTH/2);

  xs3_fft_mono_adjust((buffer, complex_s32_t[]), LENGTH, 1);
  xs3_fft_index_bit_reversal((buffer, complex_s32_t[]), LENGTH/2);
  headroom_t hr = 2;
  xs3_fft_dit_inverse((buffer, complex_s32_t[]), LENGTH/2, &hr, &exp);

  dsp_bfp_to_float(buffer, LENGTH/2, exp);
}

void fft_xs3_math() {
int t0, t1, t2;
  float buffer[LENGTH];
  float flts[LENGTH];

  // We'll just fill that vector with random mantissas.
  for(int k = 0; k < LENGTH; k++){
    flts[k] = buffer[k] = k*1000.0;//sin(k/4.0*3.1415926535)*100;
  }

  asm volatile("gettime %0" : "=r"(t0));
  fft_float_mono_forward_512(buffer);
  asm volatile("gettime %0" : "=r"(t1));
  fft_float_mono_inverse_512(buffer);
  asm volatile("gettime %0" : "=r"(t2));
  float err = 0;
  for(int k = 0; k < LENGTH; k++){
    err += sqr(flts[k] - (buffer, float[])[k]);
  }
  printf("Error %g instructions: %d %d\n", err, (t1-t0)*6/5, (t2-t1)*6/5);
}

int main()
{
  fft_xs3_math();
  return 0;
}
