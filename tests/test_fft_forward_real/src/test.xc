// Copyright 2016-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include <xs1.h>
#include <xclib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "dsp_fft.h"

int random(unsigned &x){
    crc32(x, -1, 0xEB31D82E);
    return (int)x;
}

#define FFT_LENGTH 16
#define ONE 65536

void test_linear() {
    dsp_complex_t f[FFT_LENGTH];
    int32_t sig[FFT_LENGTH*3/2+4] = {0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000, 7071, 0, -7071, -10000, -7071};
    dsp_complex_t conv[FFT_LENGTH] = {{ONE,0},{ONE,0},{ONE,0},{ONE,0},{0,0},{0,0},{0,0},{0,0}};
    for(int i = 0; i < FFT_LENGTH * 3/2; i++) {
        sig[i] = sin(6.283 / 10 * i) * 10000;
    }
    dsp_fft_bit_reverse(conv, FFT_LENGTH);
    dsp_fft_forward(conv, FFT_LENGTH, dsp_sine_8);
    for(int j = 0; j < FFT_LENGTH; j += FFT_LENGTH/2) {
        for(unsigned i=0;i<FFT_LENGTH;i++){
            f[i].re = sig[i+j];
            f[i].im = 0;
        }
        for(unsigned i=0;i<FFT_LENGTH;i++){
            printf("%6d,%6d  %6d\n", f[i].re, f[i].im, sig[i+j]);
        }
        printf("\n");
        dsp_fft_bit_reverse(f, FFT_LENGTH);
        dsp_fft_forward(f, FFT_LENGTH, dsp_sine_8);
        for(int i = 0; i < FFT_LENGTH; i++) {
            int re0 = f[i].re;
            int im0 = f[i].im;
            int re1 = conv[i].re;
            int im1 = conv[i].im;
            f[i].re = ((re0*re1 - im0*im1)*FFT_LENGTH)>>16;
            f[i].im = ((re0*im1 + im0*re1)*FFT_LENGTH)>>16;
        }
        dsp_fft_bit_reverse(f, FFT_LENGTH);
        dsp_fft_inverse(f, FFT_LENGTH, dsp_sine_8);
        for(unsigned i=0;i<FFT_LENGTH;i++){
//            printf("%6d,%6d  %6d\n", f[i].re, f[i].im, sig[i+j] +sig[i+j+1] + sig[i+j+2] + sig[i+j+3]);
        }
        printf("\n");
    }
}

void test_forward_fft() {
    timer tmr;
    int t0, t1, t2;

    dsp_complex_t f[FFT_LENGTH];
    int32_t sig[FFT_LENGTH] = {0, 7071, 10000, 7071, 8000, -7071, 17000, -7071, 6000, 3000};
    for(unsigned i=0;i<FFT_LENGTH;i++){
        f[i].re = sig[i];
        f[i].im = 0;
    }
    printf("\n");
tmr :> t0;
    dsp_fft_bit_reverse(f, FFT_LENGTH);
    dsp_fft_forward(f, FFT_LENGTH, dsp_sine_16);
tmr :> t1;
    dsp_fft_bit_reverse_and_forward_real(sig, FFT_LENGTH, dsp_sine_8, dsp_sine_16);
tmr :> t2;

    printf("%d for normal vs %d for fast\n", t1-t0, t2-t1);
    printf("%6d,%6d  %6d,%6d\n", f[0].re, f[0].im, sig[0], 0);
    for(unsigned i=1;i<FFT_LENGTH/2;i++){
        printf("%6d,%6d  %6d,%6d\n", f[i].re, f[i].im, sig[2*i], sig[2*i+1]);
    }
    printf("%6d,%6d  %6d,%6d\n", f[FFT_LENGTH/2].re, f[FFT_LENGTH/2].im, sig[1], 0);
    for(unsigned i=1;i<FFT_LENGTH/2;i++){
        printf("%6d,%6d\n", f[i+FFT_LENGTH/2].re, f[i+FFT_LENGTH/2].im);
    }
}

int main() {
//    test_linear();
    test_forward_fft();
    return 0;
}
