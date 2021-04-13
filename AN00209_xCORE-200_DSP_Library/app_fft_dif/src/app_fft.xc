// Copyright 2015-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// XMOS DSP Library - Example to use FFT and inverse FFT

#include <stdio.h>
#include <xs1.h>
#include <xclib.h>
#include <dsp.h>
#include <stdint.h>
#include <stdlib.h>

extern void dsp_fft_inverse_DIF_xs1 (
    dsp_complex_t pts[],
    const uint32_t  N,
    const int32_t   sine[] );

extern void dsp_fft_inverse_DIF_xs2 (
    dsp_complex_t pts[],
    const uint32_t  N,
    const int32_t   sine[] );

extern void dsp_fft_inverse_xs1 (
    dsp_complex_t pts[],
    const uint32_t  N,
    const int32_t   sine[] );

extern void dsp_fft_gc_xs2 (
    dsp_complex_t pts[],
    const uint32_t  N,
    const int32_t   sine[],
    const uint32_t overlap);

extern void dsp_fft_inverse_xs1 (
    dsp_complex_t pts[],
    const uint32_t  N,
    const int32_t   sine[] );

extern void dsp_fft_gc_xs1(
    dsp_complex_t pts[],
    const uint32_t  N,
    const int32_t   sine[],
    const uint32_t overlap) {
    dsp_fft_inverse_DIF_xs2(pts, N, sine);
    dsp_fft_bit_reverse(pts, N);
    for(int i = N/2-overlap; i < N; i++) {
        pts[i].re = 0;
        pts[i].im = 0;
    }
    dsp_fft_bit_reverse(pts, N);
    dsp_fft_forward(pts, N, sine);
}

#define N 512
#define SINE dsp_sine_512

dsp_complex_t in1[N], in2[N], in3[N];

int hist[7];

void addhist(int err) {
    err += 3;
    if (err < 0) err = 0;
    if (err > 6) err = 6;
    hist[err]++;
}

int main( void )
{
    timer tmr;
    int t0, t1, t2;
    for(int i = 0; i < N; i++) {
        in1[i].re = in2[i].re = in3[i].re = i*i*9;
        in1[i].im = in2[i].im = in3[i].im = i*7;
    }
    dsp_fft_inverse_DIF_xs1(in1, N, SINE);
    dsp_fft_bit_reverse(in1, N);
    dsp_fft_inverse_DIF_xs2(in2, N, SINE);
    dsp_fft_bit_reverse(in2, N);
    dsp_fft_bit_reverse(in3, N);
    dsp_fft_inverse_xs1(in3, N, SINE);

    for(int i = 0; i< N; i++) {
        if (in1[i].re != in2[i].re || in1[i].im != in2[i].im) {
//            printf("%d %d   %d %d\n", in1[i].re, in2[i].re, in1[i].im, in2[i].im);
        }
        if (in3[i].re != in2[i].re || in3[i].im != in2[i].im) {
//            printf("%d %d   %d %d\n", in3[i].re, in2[i].re, in3[i].im, in2[i].im);
        }
    }
    for(int shift = 0; shift <=7; shift += 7) {
        for(int i = 0; i < N; i++) {
            in2[i].re = in3[i].re = ((i*i*9+5) % 32768) << shift;
            in2[i].im = in3[i].im = ((i*7+3) % 32768) << shift;
        }
    tmr :> t0;
        dsp_fft_gc_xs1(in2, N, SINE, 16);
    tmr :> t1;
        dsp_fft_gc_xs2(in3, N, SINE, 16);
    tmr :> t2;
        for(int i = 0; i< N; i++) {
            int errre = in2[i].re - in3[i].re;
            int errim = in2[i].im - in3[i].im;
            if (abs(errre) > 2 || abs(errim) > 2) {
                printf("%d: %d %d   %d %d   %d %d\n", i, in3[i].re, in2[i].re, in3[i].im, in2[i].im, errre, errim);
            }
        }
        if (shift == 0) {
            for(int i = 0; i< N; i++) {
                in1[i] = in3[i];
            }            
        } else {
            for(int i = 0; i< N; i++) {
                int errre = ((in3[i].re  + (1 << (shift - 1)))>> shift) - in1[i].re;
                int errim = ((in3[i].im  + (1 << (shift - 1)))>> shift) - in1[i].im;
                addhist(errre);
                addhist(errim);
            }                        
        }
        printf("Times %d vs %d\n", t1-t0, t2-t1);
    }
    for(int i = 0; i < 7; i++) {
        printf("%2d %3d\n", i-3, hist[i]);
    }
    return 0;
};
