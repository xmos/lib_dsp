// Copyright 2016-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include <xs1.h>
#include <xclib.h>
#include <stdio.h>
#include <stdlib.h>

#include "dsp_fft.h"
#include "generated.h"

#define PRINT_STATS 0

int random(unsigned &x){
    crc32(x, -1, 0xEB31D82E);
    return (int)x;
}



void test_forward_speed(){
    unsigned x=SEED;

    //unsigned reps = 1<<16;
    unsigned reps = 1;
    unsigned now, then;
    timer t;

    dsp_complex_t f[FFT_LENGTH];
    for(unsigned i=0;i<FFT_LENGTH;i++){
        f[i].re = random(x);
        f[i].im = random(x);
    }

    t :> then;
    for(unsigned t=0;t<reps;t++)
        dsp_fft_forward(f, FFT_LENGTH, FFT_SINE_LUT);
    t:> now;

    printf("Time for one forward FFT(%d points): %.2f ns\n", FFT_LENGTH, (float)(now - then)*8.0 / (float)reps);
}

void test_inverse_speed(){
    unsigned x=SEED;

    //unsigned reps = 1<<16;
    unsigned reps = 1;
    unsigned now, then;
    timer t;

    dsp_complex_t f[FFT_LENGTH];
    for(unsigned i=0;i<FFT_LENGTH;i++){
        f[i].re = random(x);
        f[i].im = random(x);
    }

    t :> then;
    for(unsigned t=0;t<reps;t++)
        dsp_fft_inverse(f, FFT_LENGTH, FFT_SINE_LUT);
    t:> now;

    printf("Time for one inverse FFT(%d points): %.2f ns\n", FFT_LENGTH, (float)(now - then)*8.0 / (float)reps);
}


unsafe int main(){
    test_forward_speed();
    test_inverse_speed();
    _Exit(0);
    return 0;
}
