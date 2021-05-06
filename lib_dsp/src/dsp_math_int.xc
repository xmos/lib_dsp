// Copyright 2017-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include <stdio.h>
#include <xclib.h>
#include <xs1.h>
#include "dsp_math_int.h"

uint32_t dsp_math_int_sqrt(uint32_t x) {
    int32_t zeroes;
    int32_t approx;
    int32_t corr;

    if (x < 2) {
        return x;
    }
    zeroes = clz(x);
    
    zeroes = zeroes & ~1;
    zeroes = (32-zeroes) >> 1;
    approx = x >> zeroes;
    for(uint32_t i = 0; i < 4; i++) {
        corr = ((((approx*approx) - (int)x) / approx)+1) >> 1;
        approx -= corr;
    }
    return approx;
}

uint32_t dsp_math_int_sqrt64(uint64_t hl) {
    uint32_t xh = hl >> 32;
    uint32_t xl = hl;
    uint32_t approx;
    int32_t corr;

    if (xh == 0) {
        return dsp_math_int_sqrt(xl);
    }
    
    approx = dsp_math_int_sqrt(xh);
    if (approx > 0xffff) {
        return ~0;
    }
    approx = approx << 16;
    xl = -xl;
    xh = -xh - 1;
    for(uint32_t i = 0; i < 4; i++) {
        int32_t h = xh;
        uint32_t l = xl;
        int32_t q, r;
        {h,l} = mac(approx, approx, h, l);
        if (h >= 0) {
            asm("ldivu %0,%1,%2,%3,%4" : "=r"(q), "=r"(r):"r" (h), "r"(l),"r"(approx));
        } else {
            l = -l;
            h = -h - 1;
            asm("ldivu %0,%1,%2,%3,%4" : "=r"(q), "=r"(r):"r" (h), "r"(l),"r"(approx));
            q = -q;
        }
        corr = (q+1) >> 1;
        approx -= corr;
    }
    return approx;

}
