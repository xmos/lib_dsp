// Copyright (c) 2015-2016, XMOS Ltd, All rights reserved
// XMOS DSP Library - DCT Functions test program

#include <stdio.h>
#include <xs1.h>
#include <xclib.h>
#include <dsp_bfp.h>

#define MAXN 64

dsp_complex_t inp[MAXN], outp[MAXN];

int br(int i, int N) {
    return bitrev(i << (clz(N)+1));
}

int main(void) {
    for(int N = 2; N <= MAXN; N*=2) {
        int errors = 0;
        for(int i = 0; i < N; i++) {
            inp[i].re = 1000 * i;
            inp[i].im = 1500 * i + 100;
            outp[i] = inp[i];
        }

        dsp_bfp_bit_reverse_shl(outp, N, 5);
        for(int i = 0; i < N; i++) {
            if ((inp[br(i,N)].re << 5) != outp[i].re) {
                printf("Error: re[%d] %d not %d\n", i, outp[i].re, inp[br(i,N)].re << 5);
                errors++;
            }
            if ((inp[br(i,N)].im << 5) != outp[i].im) {
                printf("Error: im[%d] %d not %d\n", i, outp[i].im, inp[br(i,N)].im << 5);
                errors++;
            }
        }
        dsp_bfp_bit_reverse_shl(outp, N, -3);
        for(int i = 0; i < N; i++) {
            if ((inp[i].re << 2) != outp[i].re) {
                printf("Error: re[%d] %d not %d\n", i, outp[i].re, inp[i].re << 2);
                errors++;
            }
            if ((inp[i].im << 2) != outp[i].im) {
                printf("Error: im[%d] %d not %d\n", i, outp[i].im, inp[i].im << 2);
                errors++;
            }
        }
        if (errors == 0) {
            printf("dsp_bfp_bit_reverse_shl(..., %d, ...) passed\n", N);
        } else {
            printf("dsp_bfp_bit_reverse_shl(..., %d, ...) FAIL with %d errors\n", N, errors);
        }
    }
    return 0;
}

