// Copyright (c) 2015-2016, XMOS Ltd, All rights reserved
// XMOS DSP Library - Example to use FFT and inverse FFT

#include <stdio.h>
#include <stdlib.h>
#include <xs1.h>
#include <xclib.h>
#include <dsp.h>
#include <math.h>
#include <stdint.h>


int seed = 1234567;
int my_random() {
    seed = seed * 12345 + 1;
    return seed;
}

int32_t real_single_data[16] = {
    50000,
    35354,
    0,
    -35356,
    -50000,
    -35354,
    0,
    35356,
    50000,
    35354,
    0,
    -35356,
    -50000,
    -35354,
    0,
    35356,
};

dsp_complex_t check[16];


void do_single_real() {
    int errors = 0;
    for(int k = 0; k < 20; k++) {
        int32_t copy[16];
        for(int i = 0; i < 16; i++) {
            copy[i] = 0;
            check[i].re = 0;
            check[i].im = 0;
            real_single_data[i] = 0;
        }
        for(int i = 0; i < 16; i++) {
            copy[i] = real_single_data[i] = k*(my_random() & 0x3ffffff);
            check[i].re = copy[i];
        }
        
        dsp_fft_bit_reverse_and_forward_real( real_single_data, 16, dsp_sine_8, dsp_sine_16);
        dsp_fft_bit_reverse(check, 16);
        dsp_fft_forward(check, 16, dsp_sine_16);
        
        check[0].im = check[8].re;
        for(int i = 0; i < 16; i+=2) {
            int abserr = abs(real_single_data[i] - check[i/2].re) + abs(real_single_data[i+1]/(double)check[i/2].im);
            if (abserr > 2) {
                printf("Forward real err %d: %9d %9d %9d  %5.3f  %5.3f %9d %9d\n", abserr, copy[i], real_single_data[i], real_single_data[i+1], real_single_data[i]/(double)check[i/2].re, real_single_data[i+1]/(double)check[i/2].im, check[i/2].re, check[i/2].im);
                errors++;
            }
        }
        check[0].im = 0;
        dsp_fft_bit_reverse_and_inverse_real( real_single_data, 16, dsp_sine_8, dsp_sine_16);
        dsp_fft_bit_reverse(check, 16);
        dsp_fft_inverse(check, 16, dsp_sine_16);
        
        for(int i = 0; i < 16; i++) {
            int abserr = abs(real_single_data[i] - check[i].re);
            if (abserr > 16) {
                printf("%d Inverse real err %d: %9d %9d  %5.3f  %9d %9d\n", k, abserr, copy[i], real_single_data[i], real_single_data[i]/(double)check[i].re, check[i].re, check[i].im);
                errors++;
            }
        }

    }
    if (errors == 0) {
        printf("Real FFT forward and backwards: PASS\n");
    } else {
        printf("Real FFT forward and backwards: FAIL with %d errors\n", errors);
    }
}

int main( void )
{
    do_single_real();
    return 0;
};

