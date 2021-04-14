// Copyright 2015-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// XMOS DSP Library - Filtering Functions Test Program
// Uses Q24 format

// Include files
#include <stdio.h>
#include <stdlib.h>
#include <xs1.h>
#include <dsp.h>
#include <math.h>
#include <string.h>

#define MAXN 5

dsp_complex_t data[MAXN] = {
    {1000,-1000},
    {500,500},
    {-8000,1000},
    {100,0},
    {1000,-100000},
};

dsp_complex_t unscaled_data[MAXN] = {
    {1000,-1000},
    {500,500},
    {-8000,1000},
    {100,0},
    {1000,-100000},
};

dsp_complex_t scaled_data[MAXN] = {
    {1000,-1000},
    {5000,5000},
    {-0x7fffffff,1000000000},
    {100,0},
    {100,-10000},
};

dsp_complex_t fir[MAXN] = {
    {0x0100000,-0x0100000},
    {0x1000000, 0x1000000},
    {0x2000000,-0x1000000},
    {-0x0400000,0x0200000},
    {-0x0000040,0x0020000},
};

uint32_t numer[5] = {10000, 50000, 1000000, 1000000000, 1 };
uint32_t denom[5] = {10000,  5000,       1, 1000000000, 10 };

dsp_complex_t d[MAXN];
dsp_complex_t o[MAXN];


#define VECTOR_LENGTH           12

int32_t A_real[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
int32_t A_imag[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120};
dsp_complex_t A_cplx[VECTOR_LENGTH];

int main(void) {



    for(int N = 3; N <= MAXN; N++) {
        int errors = 0;

        for(int i = 0; i < MAXN; i++) {
            d[i] = unscaled_data[i];
        }
        dsp_complex_scale_vector(d, numer, denom, MAXN);
        for(int i = 0; i < MAXN; i++) {
            if (d[i].re != scaled_data[i].re || d[i].im != scaled_data[i].im) {
                errors++;
                printf("%d %d %d %d\n", d[i].re, d[i].im, scaled_data[i].re, scaled_data[i].im);
            }
        }

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

        int scalar_ = 0x01800000;
        Q = 5;
        for(int i = 0; i < N; i++) {
            dsp_complex_t z;
            d[i].re = dsp_math_multiply(data[i].re, scalar_, 24) >> Q;
            d[i].im = dsp_math_multiply(data[i].im, scalar_, 24) >> Q;
        }

        dsp_complex_scalar_vector3(o, data, N, scalar_, Q);
        for(int i = 0; i < N; i++) {
            if (d[i].re != o[i].re || d[i].im != o[i].im) {
                errors++;
                printf("dsp_complex_scalar_vector3: shoudl be %d %d is %d %d\n", d[i].re, d[i].im, o[i].re, o[i].im);
            }
        }

        Q = 16;
        for(int i = 0; i < N; i++) {
            dsp_complex_t z;
            z = dsp_complex_mul(data[i], fir[i], Q);
            d[i] = dsp_complex_sub(o[i], z);
        }
        dsp_complex_nmacc_vector(o, data, fir, N, Q);
        for(int i = 0; i < N; i++) {
            if (d[i].re != o[i].re || d[i].im != o[i].im) {
                errors++;
                printf("nmacc_vector3: shoudl be %d %d is %d %d\n", d[i].re, d[i].im, o[i].re, o[i].im);
            }
        }

        uint32_t magnitude[MAXN];
        for(int k = 32-MAXN; k >= 1; k-=3) {
            dsp_complex_magnitude_vector(magnitude, d, N, 0);
            for(int i = 0; i < N; i++) {
                double h = hypot(d[i].re, d[i].im);
                int err = abs((int) (h - magnitude[i]));
                int acceptable = h / (1<<24);
                if (err > acceptable + 1) {
                    errors++;
                    printf("dsp_complex_magnitude_vector: shoudl be %f is %u: %d %d\n", h, magnitude[i], d[i].re, d[i].im);
                }
            }
            d[0].re = sext(d[0].re, k);
            d[1].im = sext(d[1].im, k);
            d[2].re = sext(d[2].re, k);
            d[2].im = sext(d[2].im, k);
        }


        if (errors == 0) {
            printf("Vector complex length %d pass\n", N);
        } else {
            printf("Vector complex length %d FAIL %d errors\n", N, errors);
        }
    }

    dsp_complex_combine (A_real, A_imag, A_cplx, VECTOR_LENGTH);

    printf ("Combined Complex Array = ");
    for (int i = 0; i < VECTOR_LENGTH; i++)
        printf ("%d, %d, ", A_cplx[i].re, A_cplx[i].im);
    printf ("\n");

    memset (A_real, 0, VECTOR_LENGTH*sizeof(int));      // Clear separate arrays to test that dsp_complex_split function works correctly
    memset (A_imag, 0, VECTOR_LENGTH*sizeof(int));

    dsp_complex_split (A_cplx, A_real, A_imag, VECTOR_LENGTH);

    printf ("Split arrays (real) = ");
    for (int i = 0; i < VECTOR_LENGTH; i++) {
        printf ("%d, ", A_real[i]);
    }
    printf ("\n             (imag) = ");
    for (int i = 0; i < VECTOR_LENGTH; i++) {
        printf ("%d, ", A_imag[i]);
    }
    printf ("\n");

    return 0;
}
