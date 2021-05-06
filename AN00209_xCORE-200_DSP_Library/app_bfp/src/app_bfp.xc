// Copyright 2015-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// XMOS DSP Library - DCT Functions test program

#include <stdio.h>
#include <stdlib.h>
#include <xs1.h>
#include <xclib.h>
#include <dsp_bfp.h>

#define MAXN 64

dsp_complex_t inp[MAXN], outp[MAXN];

int br(int i, int N) {
    return bitrev(i << (clz(N)+1));
}


unsigned max(unsigned a, unsigned b) {
    return a > b ? a : b;
}

int main(void) {
    int errors = 0;
    for(int N = 2; N <= 32; N++) {
        int maxv = 0;
        for(int i = 0; i < N; i++) {
            inp[i].re = (1 << i) - ((i >> 3) & 1);
            inp[i].im = -(1 << i) + 3 - ((i >> 3) & 1);
            if (i == 32) {
                inp[i].re = 0x80000000;
            }
            outp[i] = inp[i];
            unsigned ar = abs(inp[i].re);
            unsigned ai = abs(inp[i].im);
            maxv = max(max(ar, maxv), ai);
        }
        if (dsp_bfp_cls(inp, N) != clz(maxv)) {
            errors++;
            printf("Error: cls is %d not %d\n", dsp_bfp_cls(inp, N), clz(maxv));
        }
    }
    if (errors == 0) {
        printf("dsp_bfp_clz() passed\n");
    } else {
        printf("dsp_bfp_clz() FAIL with %d errors\n", errors);
    }
    
    for(int N = 2; N <= MAXN; N*=2) {
        errors = 0;
        for(int i = 0; i < N; i++) {
            inp[i].re = 1000 * i - 10000;
            inp[i].im = 1500 * i - 10000;
            outp[i] = inp[i];
        }

        dsp_bfp_bit_reverse_shl(outp, N, 5);
        for(int i = 0; i < N; i++) {
            if ((inp[br(i,N)].re << 5) != outp[i].re) {
                printf("Error dsp_bfp_bit_reverse_shl: re[%d] %d not %d\n", i, outp[i].re, inp[br(i,N)].re << 5);
                errors++;
            }
            if ((inp[br(i,N)].im << 5) != outp[i].im) {
                printf("Error dsp_bfp_bit_reverse_shl: im[%d] %d not %d\n", i, outp[i].im, inp[br(i,N)].im << 5);
                errors++;
            }
        }
        dsp_bfp_bit_reverse_shl(outp, N, -3);
        for(int i = 0; i < N; i++) {
            if ((inp[i].re << 2) != outp[i].re) {
                printf("Error dsp_bfp_bit_reverse_shl: re[%d] %d not %d\n", i, outp[i].re, inp[i].re << 2);
                errors++;
            }
            if ((inp[i].im << 2) != outp[i].im) {
                printf("Error dsp_bfp_bit_reverse_shl: im[%d] %d not %d\n", i, outp[i].im, inp[i].im << 2);
                errors++;
            }
        }
        dsp_bfp_shl(outp, N, -3);
        for(int i = 0; i < N; i++) {
            if ((inp[i].re >> 1) != outp[i].re) {
                printf("Error dsp_bfp_shl: re[%d] %d not %d\n", i, outp[i].re, inp[i].re << 2);
                errors++;
            }
            if ((inp[i].im >> 1) != outp[i].im) {
                printf("Error dsp_bfp_shl: im[%d] %d not %d\n", i, outp[i].im, inp[i].im << 2);
                errors++;
            }
        }
        dsp_bfp_shl(outp, N, 1);
        for(int i = 0; i < N; i++) {
            if ((inp[i].re << 0) != outp[i].re) {
                printf("Error dsp_bfp_shl: re[%d] %d not %d\n", i, outp[i].re, inp[i].re << 0);
                errors++;
            }
            if ((inp[i].im << 0) != outp[i].im) {
                printf("Error dsp_bfp_shl: im[%d] %d not %d\n", i, outp[i].im, inp[i].im << 0);
                errors++;
            }
        }
        dsp_bfp_shl2(outp, N, 3, 2);
        for(int i = 0; i < N; i++) {
            if ((inp[i].re << 3) != outp[i].re) {
                printf("Error dsp_bfp_shl: re[%d] %d not %d\n", i, outp[i].re, inp[i].re << 2);
                errors++;
            }
            if ((inp[i].im << 2) != outp[i].im) {
                printf("Error dsp_bfp_shl: im[%d] %d not %d\n", i, outp[i].im, inp[i].im << 2);
                errors++;
            }
        }
        dsp_bfp_shl2(outp, N, -1, 1);
        for(int i = 0; i < N; i++) {
            if ((inp[i].re << 2) != outp[i].re) {
                printf("Error dsp_bfp_shl: re[%d] %d not %d\n", i, outp[i].re, inp[i].re << 2);
                errors++;
            }
            if ((inp[i].im << 3) != outp[i].im) {
                printf("Error dsp_bfp_shl: im[%d] %d not %d\n", i, outp[i].im, inp[i].im << 2);
                errors++;
            }
        }
        dsp_bfp_shl2(outp, N, 1, -1);
        for(int i = 0; i < N; i++) {
            if ((inp[i].re << 3) != outp[i].re) {
                printf("Error dsp_bfp_shl: re[%d] %d not %d\n", i, outp[i].re, inp[i].re << 2);
                errors++;
            }
            if ((inp[i].im << 2) != outp[i].im) {
                printf("Error dsp_bfp_shl: im[%d] %d not %d\n", i, outp[i].im, inp[i].im << 2);
                errors++;
            }
        }
        dsp_bfp_shl2(outp, N, -3, -2);
        for(int i = 0; i < N; i++) {
            if ((inp[i].re << 0) != outp[i].re) {
                printf("Error dsp_bfp_shl: re[%d] %d not %d\n", i, outp[i].re, inp[i].re << 2);
                errors++;
            }
            if ((inp[i].im << 0) != outp[i].im) {
                printf("Error dsp_bfp_shl: im[%d] %d not %d\n", i, outp[i].im, inp[i].im << 2);
                errors++;
            }
        }
        if (errors == 0) {
            printf("dsp_bfp_...(..., %d, ...) passed\n", N);
        } else {
            printf("dsp_bfp_...(..., %d, ...) FAIL with %d errors\n", N, errors);
        }
    }

    return 0;
}

