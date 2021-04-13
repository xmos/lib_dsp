// Copyright 2015-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
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

int main(void) {
    timer tmr;
    int t0, t1;
tmr :> t0;
    dsp_complex_t x = dsp_complex_fir(data, fir, 4, 0, 24);
tmr :> t1;
//    printf("%d\n", t1-t0);
    dsp_complex_t sum = {0,0};
    for(int i = 0; i < 4; i++) {
        sum = dsp_complex_add(sum, dsp_complex_mul(data[i], fir[i], 24));
    }
    printf("%d, %d    %d,%d\n", x.re, x.im, sum.re, sum.im);
    if (x.re == sum.re && x.im == sum.im) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }
            
    return 0;
}
