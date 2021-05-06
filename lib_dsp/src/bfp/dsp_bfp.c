// Copyright 2019-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include "dsp_bfp.h"
#include <xclib.h>
#include <dsp_complex.h>

#include <stdio.h>

unsigned dsp_bfp_clz_uint8(const uint8_t d){
    return clz((unsigned)d)-24;
}

unsigned dsp_bfp_clz_uint16(const uint16_t d){
    return clz((unsigned)d)-16;
}

unsigned dsp_bfp_clz_uint32(const uint32_t d){
    return clz(d);
}

unsigned dsp_bfp_clz_uint64(const uint64_t d){
    uint32_t hi = (uint32_t)(d>>32);
    uint32_t lo = (uint32_t)d;
    unsigned clz_lo;// = clz(lo);
    unsigned clz_hi;//= clz(hi);
    asm("clz %0, %1":"=r"(clz_lo):"r"(lo)); //work around for bug
    asm("clz %0, %1":"=r"(clz_hi):"r"(hi));
    if(clz_hi != 32){
        return clz_hi;
    } else {
        return 32 + clz_lo;
    }
}

unsigned dsp_bfp_cls_int8(const int8_t d){
    if(d == INT8_MIN)
        return 0;
    int8_t v=d;
    if(v<0)
        v=-v;
    return dsp_bfp_clz_uint8((uint8_t)v)-1;
}

unsigned dsp_bfp_cls_int16(const int16_t d){
    if(d == INT16_MIN)
        return 0;
    int16_t v=d;
    if(v<0)
        v=-v;
    return dsp_bfp_clz_uint16((uint16_t)v)-1;
}

unsigned dsp_bfp_cls_int32(const int32_t d){
    if(d == INT32_MIN)
        return 0;
    int32_t v=d;
    if(v<0)
        v=-v;
    return dsp_bfp_clz_uint32((uint32_t)v)-1;
}

unsigned dsp_bfp_cls_int64(const int64_t d){
    if(d == INT64_MIN)
        return 0;
    int64_t v=d;
    if(v<0)
        v=-v;
    return dsp_bfp_clz_uint64((uint64_t)v)-1;
}

unsigned dsp_bfp_cls_complex_int16(const dsp_complex_int16_t d){
    return dsp_bfp_cls_vect_int16((int16_t *) &d, 2);
}

unsigned dsp_bfp_cls_complex_int32(const dsp_complex_int32_t d){
    return dsp_bfp_cls_vect_int32((int32_t *) &d, 2);

}
unsigned dsp_bfp_cls_ch_pair_int16(const dsp_ch_pair_int16_t d, const unsigned channel_index){
    return dsp_bfp_cls_int16(((int16_t*)&d)[channel_index]);
}
unsigned dsp_bfp_cls_ch_pair_int32(const dsp_ch_pair_int32_t d, const unsigned channel_index){
    return dsp_bfp_cls_int32(((int32_t*)&d)[channel_index]);
}

unsigned dsp_bfp_clz_vect_uint8(const uint8_t *d, const unsigned length){
    uint8_t mask = 0;
    for(unsigned i=0;i<length;i++){
        mask |= d[i];
    }
    return dsp_bfp_clz_uint8(mask);
}

unsigned dsp_bfp_clz_vect_uint16(const uint16_t *d, const unsigned length){
    uint16_t mask = 0;
    for(unsigned i=0;i<length;i++){
        mask |= d[i];
    }
    return dsp_bfp_clz_uint16(mask);
}

unsigned dsp_bfp_clz_vect_uint32(const uint32_t *d, const unsigned length){
    uint32_t mask = 0;
    for(unsigned i=0;i<length;i++){
        mask |= d[i];
    }
    return dsp_bfp_clz_uint32(mask);
}

unsigned dsp_bfp_clz_vect_uint64(const uint64_t *d, const unsigned length){
    uint64_t mask = 0;
    for(unsigned i=0;i<length;i++){
        mask |= d[i];
    }
    return dsp_bfp_clz_uint64(mask);
}
/*
 * Vector cls
 */
unsigned dsp_bfp_cls_vect_int8(const int8_t *d, const unsigned length){
    uint8_t mask = 0;
    for(unsigned i=0;i<length;i++){
        if(d[i] == INT8_MIN)
            return 0;
        int8_t v = d[i];
        if(v<0)
            v=-v;
        mask |= v;
    }
    unsigned c = dsp_bfp_clz_uint8((uint8_t)mask);
    return (c - 1);
}

unsigned dsp_bfp_cls_vect_int16(const int16_t *d, const unsigned length){
    uint16_t mask = 0;
    for(unsigned i=0;i<length;i++){
        if(d[i] == INT16_MIN)
            return 0;
        int16_t v = d[i];
        if(v<0)
            v=-v;
        mask |= v;
    }
    return dsp_bfp_clz_uint16((uint16_t)mask) - 1;
}

unsigned dsp_bfp_cls_vect_int32(const int32_t *d, const unsigned length){
    uint32_t mask = 0;
    for(unsigned i=0;i<length;i++){
        if(d[i] == INT32_MIN)
            return 0;
        int32_t v = d[i];
        if(v<0)
            v=-v;
        mask |= v;
    }
    return dsp_bfp_clz_uint32((uint32_t)mask) - 1;
}

unsigned dsp_bfp_cls_vect_complex_int16(const dsp_complex_int16_t *d, const unsigned length){
    return dsp_bfp_cls_vect_int16((int16_t*)d, length*2);
}

unsigned dsp_bfp_cls_vect_complex_int32(const dsp_complex_int32_t *d, const unsigned length){
    return dsp_bfp_cls_vect_int32((int32_t*)d, length*2);
}

unsigned dsp_bfp_cls_vect_ch_pair_int16(const dsp_ch_pair_int16_t *d, const unsigned length,
        const unsigned channel_index){
    uint16_t mask = 0;
    for(unsigned i=0;i<length;i++){
        if(((int16_t*)&d[i])[channel_index] == INT16_MIN)
            return 0;
        int16_t v = ((int16_t*)&d[i])[channel_index];
        if(v<0)
            v=-v;
        mask |= v;
    }
    return dsp_bfp_clz_uint16((uint16_t)mask) - 1;
}

unsigned dsp_bfp_cls_vect_ch_pair_int32(const dsp_ch_pair_int32_t *d, const unsigned length,
        const unsigned channel_index){
    uint32_t mask = 0;
    for(unsigned i=0;i<length;i++){
        if(((int32_t*)&d[i])[channel_index] == INT32_MIN)
            return 0;
        int32_t v = ((int32_t*)&d[i])[channel_index];
        if(v<0)
            v=-v;
        mask |= v;
    }
    return dsp_bfp_clz_uint32((uint32_t)mask) - 1;
}



/*
 * Vector shl
 */
void dsp_bfp_shl_vect_uint8(uint8_t *d, const unsigned length, const int shl){
    if(shl == 0)
        return;
    for(unsigned i=0;i<length;i++){
        if(shl>0){
            d[i] <<= shl;
        } else {
            d[i] >>= (-shl);
        }
    }
}

void dsp_bfp_shl_vect_uint16(uint16_t *d, const unsigned length, const int shl){
    if(shl == 0)
        return;
    for(unsigned i=0;i<length;i++){
        if(shl>0){
            d[i] <<= shl;
        } else {
            d[i] >>= (-shl);
        }
    }
}

void dsp_bfp_shl_vect_uint32(uint32_t *d, const unsigned length, const int shl){
    if(shl == 0)
        return;
    for(unsigned i=0;i<length;i++){
        if(shl>0){
            d[i] <<= shl;
        } else {
            d[i] >>= (-shl);
        }
    }
}

void dsp_bfp_shl_vect_int8(int8_t *d, const unsigned length, const int shl){
    if(shl == 0)
        return;
    for(unsigned i=0;i<length;i++){
        if(shl>0){
            d[i] <<= shl;
        } else {
            d[i] >>= (-shl);
        }
    }
}

void dsp_bfp_shl_vect_int16(int16_t *d, const unsigned length, const int shl){
    if(shl == 0)
        return;
    for(unsigned i=0;i<length;i++){
        if(shl>0){
            d[i] <<= shl;
        } else {
            d[i] >>= (-shl);
        }
    }
}

void dsp_bfp_shl_vect_int32(int32_t *d, const unsigned length, const int shl){
    if(shl == 0)
        return;
    for(unsigned i=0;i<length;i++){
        if(shl>0){
            d[i] <<= shl;
        } else {
            d[i] >>= (-shl);
        }
    }
}

void dsp_bfp_shl_vect_complex_int16(dsp_complex_int16_t *d, const unsigned length, const int shl){
    dsp_bfp_shl_vect_int16((int16_t*)d, length*2, shl);
}

void dsp_bfp_shl_vect_complex_int32(dsp_complex_int32_t *d, const unsigned length, const int shl){
    dsp_bfp_shl_vect_int32((int32_t*)d, length*2, shl);
}

void dsp_bfp_shl_vect_ch_pair_int16(dsp_ch_pair_int16_t *d, const unsigned length,
        const int shl_ch_a, const int shl_ch_b){
    if((shl_ch_a == 0) && (shl_ch_b == 0))
        return;

    for(unsigned i=0;i<length;i++){
        if(shl_ch_a>0){
            d[i].ch_a <<= shl_ch_a;
        } else {
            d[i].ch_a >>= (-shl_ch_a);
        }
        if(shl_ch_b>0){
            d[i].ch_b <<= shl_ch_b;
        } else {
            d[i].ch_b >>= (-shl_ch_b);
        }
    }
}

void dsp_bfp_shl_vect_ch_pair_int32(dsp_ch_pair_int32_t *d, const unsigned length,
        const int shl_ch_a, const int shl_ch_b){
    if((shl_ch_a == 0) && (shl_ch_b == 0))
        return;

    for(unsigned i=0;i<length;i++){
        if(shl_ch_a>0){
            d[i].ch_a <<= shl_ch_a;
        } else {
            d[i].ch_a >>= (-shl_ch_a);
        }
        if(shl_ch_b>0){
            d[i].ch_b <<= shl_ch_b;
        } else {
            d[i].ch_b >>= (-shl_ch_b);
        }
    }
}


