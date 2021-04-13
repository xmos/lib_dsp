// Copyright 2016-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include <xs1.h>
#include <xclib.h>
#include <print.h>
#include <stdlib.h>
#include <stdio.h>
#include <platform.h>

#include "dsp_fft.h"

#define M 3
#define FFT_LENGTH 512
#define FFT_SI dsp_sine_512

int random(unsigned &x){
    crc32(x, -1, 0xEB31D82E);
    return (int)x;
}

extern void dsp_fft_zero_reverse_forward(dsp_complex_t f[], int l,
                                         const int32_t         sine[]);
extern void dsp_fft_inverse_xs2(dsp_complex_t f[], int l,
                                         const int32_t         sine[]);

void gc1(    dsp_complex_t g[FFT_LENGTH]) {
    timer tmr;
    int r0, r1;
tmr:>r0;
    dsp_fft_bit_reverse(g, FFT_LENGTH);
    dsp_fft_inverse_xs2(g, FFT_LENGTH, FFT_SI);
    dsp_fft_zero_reverse_forward(g, FFT_LENGTH, FFT_SI);
tmr:>r1;
//    printf("%d ticks for integrated\n", r1-r0);
}

void gc2(    dsp_complex_t g[FFT_LENGTH]) {
    timer tmr;
    int r0, r1;
tmr:>r0;
    dsp_fft_bit_reverse(g, FFT_LENGTH);
    dsp_fft_inverse_xs2(g, FFT_LENGTH, FFT_SI);
    for(int i = FFT_LENGTH/2; i < FFT_LENGTH; i++) {
        g[i].re = 0;
        g[i].im = 0;
    }
    dsp_fft_bit_reverse(g, FFT_LENGTH);
    dsp_fft_forward(g, FFT_LENGTH, FFT_SI);
tmr:>r1;
//    printf("%d ticks for separate\n", r1-r0);
}

dsp_complex_t f[FFT_LENGTH], g[FFT_LENGTH];

int main(void) {
    int error = 0;
    for(int i = 0; i < FFT_LENGTH; i++) {
        g[i].re = f[i].re = i*i-1000;
        g[i].im = f[i].im = 1000-i;
    }
    gc1(f);
    gc2(g);
    for(int i = 0; i < FFT_LENGTH; i++) {
        if (g[i].re != f[i].re) {
            printf("%d: re %08x not %08x\n", i, g[i].re, f[i].re);
            error++;
        }
        if (g[i].im != f[i].im) {
            printf("%d: im %08x not %08x\n", i, g[i].im, f[i].im);
            error++;
        }
    }
    if (error == 0) {
        printf("test FFT inverse blank forward PASS\n");
    } else {
        printf("test FFT inverse blank forward failure count: %d\n");
    }
    return 0;
}
