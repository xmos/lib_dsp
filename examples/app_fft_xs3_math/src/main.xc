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

void fft_float_mono_512(float buffer[]) {
  int exp = dsp_float_to_bfp(buffer, LENGTH/2);

  // Now call the three functions to do an FFT
  xs3_fft_index_bit_reversal((buffer, complex_s32_t[]), LENGTH/2);
  headroom_t hr = 2;
  xs3_fft_dit_forward((buffer, complex_s32_t[]), LENGTH/2, &hr, &exp);
  xs3_fft_mono_adjust((buffer, complex_s32_t[]), LENGTH, 0);

  // And unpack back to floating point values
  dsp_bfp_to_float(buffer, LENGTH/2, exp);
}

void fft_xs3_math() {
int t0, t1, t2, t3, t4, t5;
  complex_s32_t buffer[LENGTH/2];
  float flts[LENGTH];

  // We'll just fill that vector with random mantissas.
  for(int k = 0; k < LENGTH; k++){
    flts[k] = (buffer, float[])[k] = k;//sin(k/4.0*3.1415926535)*100;
  }

  unsafe {
asm volatile("gettime %0":"=r"(t0));
    
    // First unpack float to integer, remember the exponent
    int exp = dsp_float_to_bfp((buffer, float[]), LENGTH/2);
asm volatile("gettime %0":"=r"(t1));

    // Now call the three functions to do an FFT
    xs3_fft_index_bit_reversal(buffer, LENGTH/2);
asm volatile("gettime %0":"=r"(t2));
    headroom_t hr = 2;
    xs3_fft_dit_forward(buffer, LENGTH/2, &hr, &exp);
asm volatile("gettime %0":"=r"(t3));
    xs3_fft_mono_adjust(buffer, LENGTH, 0);
asm volatile("gettime %0":"=r"(t4));

    // And unpack back to floating point values
    dsp_bfp_to_float((buffer, float[]), LENGTH/2, exp);
asm volatile("gettime %0":"=r"(t5));

printf("%d   %d %d %d %d %d\n", t5-t0, t1-t0, t2-t1, t3-t2, t4-t3, t5-t4);

    // Inverse FFT.
    // First unpack float to integer, remember the exponent
asm volatile("gettime %0":"=r"(t2));
    exp = dsp_float_to_bfp((buffer, float[]), LENGTH/2);

    xs3_fft_mono_adjust(buffer, LENGTH, 1);
    xs3_fft_index_bit_reversal(buffer, LENGTH/2);
    xs3_fft_dit_inverse(buffer, LENGTH/2, &hr, &exp);

    dsp_bfp_to_float((buffer, float[]), LENGTH/2, exp);
asm volatile("gettime %0":"=r"(t3));

printf("%d\n", t3-t2);
  } // unsafe
float err = 0;
  for(int k = 0; k < LENGTH; k++){
err += sqr(flts[k] - (buffer, float[])[k]);
  }
printf("%g\n", err);
for(int k = 2048; k < sizeof(buffer); k++){
if ((buffer, uint8_t[])[k] != (k & 0xff)) {
printf("%d\n", k);
}
  }
}

int main()
{
  fft_xs3_math();
  return 0;
}
