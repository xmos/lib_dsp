// Copyright (c) 2015-2016, XMOS Ltd, All rights reserved
// XMOS DSP Library - Filtering Functions Test Program
// Uses Q24 format

// Include files
#include <stdio.h>
#include <xs1.h>
#include <dsp.h>

dsp_complex_t data[4] = {
    {1000,-1000},
    {500,500},
    {-8000,1000},
    {100,0},
};

dsp_complex_t fir[4] = {
    {0x0100000,-0x0100000},
    {0x1000000, 0x1000000},
    {0x2000000,-0x1000000},
    {-0x0400000,0x0200000},
};

dsp_complex_t d[4];

int main(void) {
    timer tmr;
    int t0, t1;
    int error = 0;
    for(int i = 0; i < 4; i++) {
        d[i] = dsp_complex_mul(data[i], fir[i], 24);
    }
tmr :> t0;
    dsp_complex_mul_vector(data, fir, 4, 24);
tmr :> t1;
    for(int i = 0; i < 4; i++) {
        if (d[i].re != data[i].re || d[i].im != data[i].im) {
            error = 1;
            printf("%d %d %d %d\n", d[i].re, d[i].im, data[i].re, data[i].im);
        }
    }
    printf("%d\n", t1-t0);
    for(int i = 0; i < 4; i++) {
        d[i] = dsp_complex_mul_conjugate(data[i], fir[i], 24);
    }
tmr :> t0;
    dsp_complex_mul_conjugate_vector(data, fir, 4, 24);
tmr :> t1;
    for(int i = 0; i < 4; i++) {
        if (d[i].re != data[i].re || d[i].im != data[i].im) {
            error = 1;
            printf("%d %d %d %d\n", d[i].re, d[i].im, data[i].re, data[i].im);
        }
    }
    printf("%d\n", t1-t0);
    return 0;
}
