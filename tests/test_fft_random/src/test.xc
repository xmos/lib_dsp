// Copyright (c) 2016-2017, XMOS Ltd, All rights reserved
#include <xs1.h>
#include <xclib.h>
#include <stdio.h>
#include <stdlib.h>

#include "dsp_fft.h"

#ifndef SEED
#define SEED 255
#endif

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
        //case 16384:
        //    for(int i=0; i<(fft_length>>2)+1; i++)
        //        sine_array[i] = dsp_sine_16384[i];
        //    break;
        default:
            for(int i=0; i<(fft_length>>2)+1; i++)
                sine_array[i] = dsp_sine_4[i];
    }
}

void copy_vector(dsp_complex_t src[], dsp_complex_t dst[], unsigned length) {
    for (int i=0; i<length; ++i) {
        dst[i] = src[i];
    }
}

void test_forward_fft(){
    unsigned x=SEED;

    unsigned max_error = 0;
    unsigned average_error = 0;
    unsigned test_count = 2;

    unsigned num_lengths = 1;
    unsigned fft_lengths[] = {
        8//, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192
    };

    dsp_complex_t f[MAX_FFT_LENGTH];
    dsp_complex_t f_tmp[MAX_FFT_LENGTH];
    dsp_complex_t f_freq[MAX_FFT_LENGTH];
    int32_t sine_array[MAX_FFT_LENGTH];
    printf("SEED=%d\n", SEED);

    for(unsigned j=0; j<num_lengths; j++) {
        unsigned fft_length = fft_lengths[j];

        for(unsigned t=0;t<test_count;t++){

            printf("LENGTH%d\n", fft_length);
            printf("GENERATE\n");
            for(unsigned i=0;i<fft_length;i++){
                f[i].re = random(x)>>DATA_SHIFT;
                f[i].im = random(x)>>DATA_SHIFT;
            }

            // Forward FFT
            printf("FFT\n");
            copy_vector(f, f_tmp, fft_length);
            get_dsp_sine(fft_length, sine_array);
            dsp_fft_bit_reverse(f_tmp, fft_length);
            dsp_fft_forward(f_tmp, fft_length, sine_array);

            for(unsigned i=0;i<fft_length;i++){
                printf("%d,%d;", f_tmp[i].re, f_tmp[i].im);
            }
            printf("\n");

            // Copy FFT for later use...
            copy_vector(f_tmp, f_freq, fft_length);

            // Inverse FFT
            printf("IFFT\n");
            copy_vector(f_freq, f_tmp, fft_length);
            get_dsp_sine(fft_length, sine_array);
            dsp_fft_bit_reverse(f_tmp, fft_length);
            dsp_fft_inverse(f_tmp, fft_length, sine_array);

            for(unsigned i=0;i<fft_length;i++){
                printf("%d,%d;", f_tmp[i].re, f_tmp[i].im);
            }
            printf("\n");

            // Index Bit Reverse
            printf("BIT_REVERSE\n");
            copy_vector(f, f_tmp, fft_length);
            for (int i=0; i<fft_length; i++) {
                f_tmp[i].re = (unsigned) f_tmp[i].re % fft_length;
                f_tmp[i].im = (unsigned) f_tmp[i].im % fft_length;
            }
            dsp_fft_bit_reverse(f_tmp, fft_length);

            for(unsigned i=0;i<fft_length;i++){
                printf("%d,%d;", f_tmp[i].re, f_tmp[i].im);
            }
            printf("\n");
        }
    }
    printf("Tests completed.\n");
}

unsafe int main(){
    test_forward_fft();
    _Exit(0);
    return 0;
}
