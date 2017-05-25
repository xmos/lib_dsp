// Copyright (c) 2016, XMOS Ltd, All rights reserved
#include <xs1.h>
#include <xclib.h>
#include <print.h>
#include <stdlib.h>
#include <stdio.h>
#include <platform.h>

#include "dsp_bfp.h"

#define M 9
#define N 5

int headroom[M] = {
    23, 23, 23, 23,
    32,
    23, 23, 23, 23,
};
    
dsp_complex_t inp[M][N] = {
    {
        {0x00000000,0x00000001},
        {0x00000004,0x00000002},
        {0x00000010,0x00000008},
        {0x00000040,0x00000020},
        {0x00000100,0x00000080},
    },
    {
        {0x00000100,0x00000080},
        {0x00000004,0x00000002},
        {0x00000010,0x00000008},
        {0x00000040,0x00000020},
        {0x00000000,0x00000001},
    },
    {
        {0xFFFFFFFF,0xFFFFFFFE},
        {0xFFFFFFFD,0xFFFFFFFB},
        {0xFFFFFFF7,0xFFFFFFEF},
        {0xFFFFFFDF,0xFFFFFFBF},
        {0xFFFFFF7F,0xFFFFFEFF},
    },
    {
        {0xFFFFFF7F,0xFFFFFEFF},
        {0xFFFFFFFD,0xFFFFFFFB},
        {0xFFFFFFF7,0xFFFFFFEF},
        {0xFFFFFFDF,0xFFFFFFBF},
        {0xFFFFFFFF,0xFFFFFFFE},
    },
    {
        {0x00000000,0x00000000},
        {0x00000000,0x00000000},
        {0x00000000,0x00000000},
        {0x00000000,0x00000000},
        {0x00000000,0x00000000},
    },
    {
        {0x00000001,0x00000000},
        {0x00000002,0x00000004},
        {0x00000008,0x00000010},
        {0x00000020,0x00000040},
        {0x00000080,0x00000100},
    },
    {
        {0x00000080,0x00000100},
        {0x00000002,0x00000004},
        {0x00000008,0x00000010},
        {0x00000020,0x00000040},
        {0x00000001,0x00000000},
    },
    {
        {0xFFFFFFFE,0xFFFFFFFF},
        {0xFFFFFFFB,0xFFFFFFFD},
        {0xFFFFFFEF,0xFFFFFFF7},
        {0xFFFFFFBF,0xFFFFFFDF},
        {0xFFFFFEFF,0xFFFFFF7F},
    },
    {
        {0xFFFFFEFF,0xFFFFFF7F},
        {0xFFFFFFFB,0xFFFFFFFD},
        {0xFFFFFFEF,0xFFFFFFF7},
        {0xFFFFFFBF,0xFFFFFFDF},
        {0xFFFFFFFE,0xFFFFFFFF},
    }
};

    
dsp_complex_t outp[M][N] = {
    {
        {0x00000000,0x00400000},
        {0x01000000,0x00800000},
        {0x04000000,0x02000000},
        {0x10000000,0x08000000},
        {0x40000000,0x20000000},
    },
    {
        {0x40000000,0x20000000},
        {0x01000000,0x00800000},
        {0x04000000,0x02000000},
        {0x10000000,0x08000000},
        {0x00000000,0x00400000},
    },
    {
        {0xFFC00000,0xFF800000},
        {0xFF400000,0xFEC00000},
        {0xFDC00000,0xFBC00000},
        {0xF7C00000,0xEFC00000},
        {0xDFC00000,0xBFC00000},
    },
    {
        {0xDFC00000,0xBFC00000},
        {0xFF400000,0xFEC00000},
        {0xFDC00000,0xFBC00000},
        {0xF7C00000,0xEFC00000},
        {0xFFC00000,0xFF800000},
    },
    {
        {0x00000000,0x00000000},
        {0x00000000,0x00000000},
        {0x00000000,0x00000000},
        {0x00000000,0x00000000},
        {0x00000000,0x00000000},
    },
    {
        {0x00400000,0x00000000},
        {0x00800000,0x01000000},
        {0x02000000,0x04000000},
        {0x08000000,0x10000000},
        {0x20000000,0x40000000},
    },
    {
        {0x20000000,0x40000000},
        {0x00800000,0x01000000},
        {0x02000000,0x04000000},
        {0x08000000,0x10000000},
        {0x00400000,0x00000000},
    },
    {
        {0xFF800000,0xFFC00000},
        {0xFEC00000,0xFF400000},
        {0xFBC00000,0xFDC00000},
        {0xEFC00000,0xF7C00000},
        {0xBFC00000,0xDFC00000},
    },
    {
        {0xBFC00000,0xDFC00000},
        {0xFEC00000,0xFF400000},
        {0xFBC00000,0xFDC00000},
        {0xEFC00000,0xF7C00000},
        {0xFF800000,0xFFC00000},
    }
};

int main(void) {
    timer tmr;
    int t0, t1;
    int error = 0;
    for(int i = 0; i < M; i++) {
        int h = dsp_bfp_cls(inp[i], N);
        if (h != headroom[i]) {
            printf("Error: Headroom %d %d != %d\n", i, h, headroom[i]);
            error++;
        }
    }
    for(int i = 0; i < M; i++) {
        dsp_bfp_shl(inp[i], headroom[i]-1, N);
    }
    for(int i = 0; i < M; i++) {
        for(int j = 0; j < N; j++) {
            if (inp[i][j].re != outp[i][j].re) {
                printf("Error: real %d %d: %08x != %08x\n", i, j, inp[i][j].re, outp[i][j].re);
                error++;
            }
            if (inp[i][j].im != outp[i][j].im) {
                printf("Error: imaginary %d %d: %08x != %08x\n", i, j, inp[i][j].im, outp[i][j].im);
                error++;
            }
        }
    }
    tmr :> t0;
    dsp_bfp_shl(inp[0], -3, N*M);
    tmr :> t1;
    if (t1 - t0 > N*M*6 + 15) {
        printf("SHL Took %d cycles for %d\n", t1-t0, N*M);
        error++;
    }
    for(int i = 0; i < M; i++) {
        for(int j = 0; j < N; j++) {
            inp[i][j].im = -1;
            inp[i][j].re = -1;
        }
    }
    tmr :> t0;
    dsp_bfp_cls(inp[0], N*M);
    tmr :> t1;
    if (t1 - t0 > N*M*9 + 15) {
        printf("CLS Took %d cycles for %d\n", t1-t0, N*M);
        error++;
    }
    if (error == 0) {
        printf("PASS\n");
    } else {
        printf("Failure count: %d\n", error);
    }
    return 0;
}
