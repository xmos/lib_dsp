// Copyright 2015-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
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

#define MAXN  256

int32_t real_single_data[MAXN];
dsp_complex_t check[MAXN];
int32_t copy[MAXN];
int errors = 0;

int32_t * unsafe s(int N) {
    unsafe {
        switch(N) {
        case 4: return dsp_sine_4;
        case 8: return dsp_sine_8;
        case 16: return dsp_sine_16;
        case 32: return dsp_sine_32;
        case 64: return dsp_sine_64;
        case 128: return dsp_sine_128;
        case 256: return dsp_sine_256;
        }
        printf("Error: Should not have asked for %d\n", N);
        return dsp_sine_512;
    }   
}

static int max(int a, int b) {
    return a > b ? a : b;
}

void do_single_real() {
    unsafe {
        for(int N = 8; N <= MAXN; N *= 2) {
            int maxferr = 0;
            int maxberr = 0;
            for(int k = 1; k < 20; k++) {
                for(int i = 0; i < N; i++) {
                    copy[i] = 0;
                    check[i].re = 0;
                    check[i].im = 0;
                    real_single_data[i] = 0;
                }
                for(int i = 0; i < N; i++) {
                    copy[i] = real_single_data[i] = k*(my_random() & 0x3ffffff);
                    check[i].re = copy[i];
                }
        
                dsp_fft_bit_reverse_and_forward_real(real_single_data, N,
                                                     (int32_t *) s(N/2),
                                                     (int32_t *) s(N));
                dsp_fft_bit_reverse(check, N);
                dsp_fft_forward(check, N, (int32_t *)s(N));
        
                check[0].im = check[N/2].re;
                for(int i = 0; i < N; i+=2) {
                    int abserr = abs(real_single_data[i] - check[i/2].re) + abs(real_single_data[i+1]/(double)check[i/2].im);
                    if (abserr > 3) {
                        printf("N: %d, k: %d Forward real err %d: %9d %9d %9d  %5.3f  %5.3f %9d %9d\n", N, k, abserr, copy[i], real_single_data[i], real_single_data[i+1], real_single_data[i]/(double)check[i/2].re, real_single_data[i+1]/(double)check[i/2].im, check[i/2].re, check[i/2].im);
                        errors++;
                    }
                    maxferr = max(abserr, maxferr);
                }
                check[0].im = 0;
                dsp_fft_bit_reverse_and_inverse_real(real_single_data, N,
                                                     (int32_t *) s(N/2),
                                                     (int32_t *) s(N));
                dsp_fft_bit_reverse(check, N);
                dsp_fft_inverse(check, N, (int32_t *) s(N));
        
                for(int i = 0; i < N; i++) {
                    int abserr = abs(real_single_data[i] - check[i].re);
                    if (abserr > N) {
                        printf("N: %d, k: %d Inverse real err %d: %9d %9d  %5.3f  %9d %9d\n", N, k, abserr, copy[i], real_single_data[i], real_single_data[i]/(double)check[i].re, check[i].re, check[i].im);
                        errors++;
                    }
                    maxberr = max(abserr, maxberr);
                }

            }
//            printf("N: %d max forward error %d backward %d\n", N, maxferr, maxberr);
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

