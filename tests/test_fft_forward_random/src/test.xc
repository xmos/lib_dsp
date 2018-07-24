// Copyright (c) 2016-2017, XMOS Ltd, All rights reserved
#include <xs1.h>
#include <xclib.h>
#include <stdio.h>
#include <stdlib.h>

#include "dsp_fft.h"

#define SEED 1
#define DATA_SHIFT 1

#define MAX_FFT_LENGTH 8192

int random(unsigned &x){
    crc32(x, -1, 0xEB31D82E);
    return (int)x;
}

void get_dsp_sine(unsigned fft_length, int32_t sine_array[]) {
    switch(fft_length) {
        case 4:
            for(int i=0; i<(fft_length>>2)+1; i++)
                sine_array[i] = dsp_sine_4[i];
            break;
        case 8:
            for(int i=0; i<(fft_length>>2)+1; i++)
                sine_array[i] = dsp_sine_8[i];
            break;
        case 16:
            for(int i=0; i<(fft_length>>2)+1; i++)
                sine_array[i] = dsp_sine_16[i];
            break;
        case 32:
            for(int i=0; i<(fft_length>>2)+1; i++)
                sine_array[i] = dsp_sine_32[i];
            break;
        case 64:
            for(int i=0; i<(fft_length>>2)+1; i++)
                sine_array[i] = dsp_sine_64[i];
            break;
        case 128:
            for(int i=0; i<(fft_length>>2)+1; i++)
                sine_array[i] = dsp_sine_128[i];
            break;
        case 256:
            for(int i=0; i<(fft_length>>2)+1; i++)
                sine_array[i] = dsp_sine_256[i];
            break;
        case 512:
            for(int i=0; i<(fft_length>>2)+1; i++)
                sine_array[i] = dsp_sine_512[i];
            break;
        case 1024:
            for(int i=0; i<(fft_length>>2)+1; i++)
                sine_array[i] = dsp_sine_1024[i];
            break;
        case 2048:
            for(int i=0; i<(fft_length>>2)+1; i++)
                sine_array[i] = dsp_sine_2048[i];
            break;
        case 4096:
            for(int i=0; i<(fft_length>>2)+1; i++)
                sine_array[i] = dsp_sine_4096[i];
            break;
        case 8192:
            for(int i=0; i<(fft_length>>2)+1; i++)
                sine_array[i] = dsp_sine_8192[i];
            break;
        case 16384:
            for(int i=0; i<(fft_length>>2)+1; i++)
                sine_array[i] = dsp_sine_16384[i];
            break;
        default:
            for(int i=0; i<(fft_length>>2)+1; i++)
                sine_array[i] = dsp_sine_4[i];
    }
}


void test_forward_fft(){
    unsigned x=SEED;

    unsigned max_error = 0;
    unsigned average_error = 0;
    unsigned test_count = 2;

    unsigned num_lengths = 11;
    unsigned fft_lengths[] = {
        8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192
    };

    dsp_complex_t f[MAX_FFT_LENGTH];
    int32_t sine_array[MAX_FFT_LENGTH];
    printf("x=%d\n", x);

    for(unsigned j=0; j<num_lengths; j++) {
        unsigned fft_length = fft_lengths[j];

        for(unsigned t=0;t<test_count;t++){

            for(unsigned i=0;i<fft_length;i++){
                f[i].re = random(x)>>DATA_SHIFT;
                f[i].im = random(x)>>DATA_SHIFT;
            }
            printf("FFT%d:\n", fft_length);
            for(unsigned i=0;i<fft_length;i++){
                printf("%d,%d;", f[i].re, f[i].im);
            }
            printf("\n");

            get_dsp_sine(fft_length, sine_array);
            dsp_fft_bit_reverse(f, fft_length);
            dsp_fft_forward(f, fft_length, sine_array);

            for(unsigned i=0;i<fft_length;i++){
                printf("%d,%d;", f[i].re, f[i].im);
            }
            printf("\n");
        }
    }
    printf("Forward FFT: Pass.\n");
}

unsafe int main(){
    test_forward_fft();
    _Exit(0);
    return 0;
}
