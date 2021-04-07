// Copyright 2016-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include <xs1.h>
#include <xclib.h>
#include <stdio.h>
#include <stdlib.h>

#include "dsp_fft.h"
#include "generated.h"

int random(unsigned &x){
    crc32(x, -1, 0xEB31D82E);
    return (int)x;
}

void test_split_and_merge(){
    unsigned x=SEED;
    dsp_complex_t f[FFT_LENGTH];
    dsp_complex_t g[FFT_LENGTH];
    unsigned test_count = 2;
    for(unsigned i=0;i<test_count;i++){
        for(unsigned i=0; i < FFT_LENGTH; i++){
            f[i].re = random(x)>>DATA_SHIFT;
            f[i].im = 0;
            g[i].re = f[i].re;
            g[i].im = f[i].im;
        }

        dsp_fft_forward(f, FFT_LENGTH, FFT_SINE_LUT);
        dsp_fft_forward(g, FFT_LENGTH, FFT_SINE_LUT);

        dsp_fft_split_spectrum(f, FFT_LENGTH);

        for(unsigned i=1;i<FFT_LENGTH/2;i++){
            int e = g[i].re - f[i].re;
            if (e<0) e=-e;
            if(e > FFT_LENGTH_LOG2){
                printf("Error: error in split spectrum operation(real)\n");
                _Exit(1);
            }
            e = g[i].im - f[i].im;
            if (e<0) e=-e;
            if(e > FFT_LENGTH_LOG2){
                printf("Error: error in split spectrum operation(imaginary)\n");
                _Exit(1);
            }
        }
        for(unsigned i=FFT_LENGTH/2 ;i<FFT_LENGTH;i++){
            int e = 0 - f[i].re;
            if (e<0) e=-e;
            if(e > FFT_LENGTH_LOG2){
                printf("Error: error in split spectrum operation(real)\n");
                _Exit(1);
            }
            e = 0 - f[i].im;
            if (e<0) e=-e;
            if(e > FFT_LENGTH_LOG2){
                printf("Error: error in split spectrum operation(imaginary)\n");
                _Exit(1);
            }
        }
    }
    printf("Split spectrum: Pass.\n");

    unsigned average_error = 0;
    //Merge spectrum is correct iff it is the inverse of the split operation.
    for(unsigned i=0;i<test_count;i++){
        for(unsigned i=0; i < FFT_LENGTH; i++){
            f[i].re = random(x)>>DATA_SHIFT;
            f[i].im = random(x)>>DATA_SHIFT;
            g[i].re = f[i].re;
            g[i].im = f[i].im;
        }

        dsp_fft_split_spectrum(f, FFT_LENGTH);
        dsp_fft_merge_spectra(f, FFT_LENGTH);

        for(unsigned i=0; i < FFT_LENGTH; i++){
            int e = g[i].re - f[i].re;
            if (e<0) e=-e;
            average_error += e;
            if(e > 1){
                printf("Error: error in merge spectrum operation(real)\n");
                _Exit(1);
            }
            e = g[i].im - f[i].im;
            if (e<0) e=-e;
            average_error += e;

            if(e > 1){
                printf("Error: error in merge spectrum operation(imaginary)\n");
                _Exit(1);
            }
        }
    }
    printf("Merge spectra: Pass.\n");
}

unsafe int main(){
    test_split_and_merge();
    _Exit(0);
    return 0;
}
