// Copyright 2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include <xs1.h>
#include <xclib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "dsp_fft_xs3.h"

float sqr(float x) {return x*x;}

#define FFT_Main(op, nop, in, out, p) { \
      fft_float_mono_forward_512(in);    \
      memcpy(out, in, sizeof(float)*512); }

#define IFFT_Main(op, nop, in, out, p) { \
      fft_float_mono_inverse_512(in);    \
      memcpy(out, in, sizeof(float)*512); }

void fft_xs3_math() {
int t0, t1, t2;
  float buffer1[FFT_LENGTH];
  float buffer2[FFT_LENGTH];

  float flts[FFT_LENGTH];

  // We'll just fill that vector with random mantissas.
  for(int k = 0; k < FFT_LENGTH; k++){
    flts[k] = buffer1[k] = k*1000.0;//sin(k/4.0*3.1415926535)*100;
  }

  asm volatile("gettime %0" : "=r"(t0));
  FFT_Main(0, 0, buffer1, buffer2, 0);
  asm volatile("gettime %0" : "=r"(t1));
  IFFT_Main(0, 0, buffer2, buffer1, 0);

  asm volatile("gettime %0" : "=r"(t2));
  float err = 0;
  for(int k = 0; k < FFT_LENGTH; k++){
    err += sqr(flts[k] - (buffer1, float[])[k]);
  }
  printf("Error %g instructions: %d %d\n", err, (t1-t0)*6/5, (t2-t1)*6/5);
}

int main()
{
  fft_xs3_math();
  return 0;
}
