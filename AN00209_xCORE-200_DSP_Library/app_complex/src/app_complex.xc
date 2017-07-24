// Copyright (c) 2015-2016, XMOS Ltd, All rights reserved
// XMOS DSP Library - Filtering Functions Test Program
// Uses Q24 format

// Include files
#include <stdio.h>
#include <xs1.h>
#include <dsp.h>

#define MAXN 5

dsp_complex_t data[MAXN] = {
    {1000,-1000},
    {500,500},
    {-8000,1000},
    {100,0},
    {1000,-100000},
};

dsp_complex_t fir[MAXN] = {
    {0x0100000,-0x0100000},
    {0x1000000, 0x1000000},
    {0x2000000,-0x1000000},
    {-0x0400000,0x0200000},
    {-0x0000040,0x0020000},
};

dsp_complex_t d[MAXN];
dsp_complex_t o[MAXN];

int main(void) {
    for(int N = 3; N <= MAXN; N++) {
        int errors = 0;
        for(int i = 0; i < N; i++) {
            d[i] = dsp_complex_mul(data[i], fir[i], 24);
        }
        dsp_complex_mul_vector(data, fir, N, 24);
        for(int i = 0; i < N; i++) {
            if (d[i].re != data[i].re || d[i].im != data[i].im) {
                errors++;
                printf("%d %d %d %d\n", d[i].re, d[i].im, data[i].re, data[i].im);
            }
        }
    
        for(int i = 0; i < N; i++) {
            d[i] = dsp_complex_mul_conjugate(data[i], fir[i], 24);
        }
        dsp_complex_mul_conjugate_vector3(o, data, fir, N, 24);
        dsp_complex_mul_conjugate_vector(data, fir, N, 24);
        for(int i = 0; i < N; i++) {
            if (d[i].re != data[i].re || d[i].im != data[i].im) {
                errors++;
                printf("mul_conjugate_vector: %d %d %d %d\n", d[i].re, d[i].im, data[i].re, data[i].im);
            }
            if (d[i].re != o[i].re || d[i].im != o[i].im) {
                errors++;
                printf("mul_conjugate_vector3: %d %d %d %d\n", d[i].re, d[i].im, o[i].re, o[i].im);
            }
        }
    
        for(int i = 0; i < N; i++) {
            d[i] = dsp_complex_add(data[i], fir[i]);
        }
        dsp_complex_add_vector3(o, data, fir, N);
        dsp_complex_add_vector(data, fir, N);
        for(int i = 0; i < N; i++) {
            if (d[i].re != data[i].re || d[i].im != data[i].im) {
                errors++;
                printf("vector_add: %d %d %d %d\n", d[i].re, d[i].im, data[i].re, data[i].im);
            }
            if (d[i].re != o[i].re || d[i].im != o[i].im) {
                errors++;
                printf("add_vector3: %d %d %d %d\n", d[i].re, d[i].im, o[i].re, o[i].im);
            }
        }
    
        int shift = 5;
        for(int i = 0; i < N; i++) {
            dsp_complex_t z;
            z.re = fir[i].re << shift;
            z.im = fir[i].im << shift;
            d[i] = dsp_complex_add(data[i], z);
        }
        dsp_complex_add_vector_shl(data, fir, N, 5);
        for(int i = 0; i < N; i++) {
            if (d[i].re != data[i].re || d[i].im != data[i].im) {
                errors++;
                printf("vector_add_shl: %d %d %d %d\n", d[i].re, d[i].im, data[i].re, data[i].im);
            }
        }

        shift = -3;
        for(int i = 0; i < N; i++) {
            dsp_complex_t z;
            z.re = fir[i].re >> -shift;
            z.im = fir[i].im >> -shift;
            d[i] = dsp_complex_add(data[i], z);
        }
        dsp_complex_add_vector_shl(data, fir, N, shift);
        for(int i = 0; i < N; i++) {
            if (d[i].re != data[i].re || d[i].im != data[i].im) {
                errors++;
                printf("vector_add_shr: %d %d %d %d\n", d[i].re, d[i].im, data[i].re, data[i].im);
            }
        }

        int mults[4] = {0x00100000, -0x00100000, 0x00465132, -0x0231AAA};
        for(int j = 0; j < 4; j++) {
            long long mult = mults[j];
            for(int i = 0; i < N; i++) {
                dsp_complex_t z;
                z.re = (fir[i].re * mult) >> 24;
                z.im = (fir[i].im * mult) >> 24;
                d[i] = dsp_complex_add(data[i], z);
            }
            dsp_complex_add_vector_scale(data, fir, N, mult);
            for(int i = 0; i < N; i++) {
                if (d[i].re != data[i].re || d[i].im != data[i].im) {
                    errors++;
                    printf("vector_add_scale: %d %d %d %d\n", d[i].re, d[i].im, data[i].re, data[i].im);
                }
            }
        }
        for(int i = 0; i < N; i++) {
            dsp_complex_t z;
            z.re = -fir[i].re;
            z.im = -fir[i].im;
            d[i] = dsp_complex_add(data[i], z);
        }
        dsp_complex_sub_vector3(o, data, fir, N);
        dsp_complex_sub_vector(data, fir, N);
        for(int i = 0; i < N; i++) {
            if (d[i].re != data[i].re || d[i].im != data[i].im) {
                errors++;
                printf("vector_sub: %d %d %d %d\n", d[i].re, d[i].im, data[i].re, data[i].im);
            }
            if (d[i].re != o[i].re || d[i].im != o[i].im) {
                errors++;
                printf("sub_vector3: %d %d %d %d\n", d[i].re, d[i].im, o[i].re, o[i].im);
            }
        }

        int Q = 5;
        for(int i = 0; i < N; i++) {
            dsp_complex_t z;
            z = dsp_complex_mul(data[i], fir[i], Q);
            d[i] = dsp_complex_add(o[i], z);
        }
        dsp_complex_macc_vector(o, data, fir, N, Q);
        for(int i = 0; i < N; i++) {
            if (d[i].re != o[i].re || d[i].im != o[i].im) {
                errors++;
                printf("macc_vector3: %d %d %d %d\n", d[i].re, d[i].im, o[i].re, o[i].im);
            }
        }

        if (errors == 0) {
            printf("Vector complex length %d pass\n", N);
        } else {
            printf("Vector complex length %d FAIL %d errors\n", N, errors);
        }
    }
    return 0;
}
