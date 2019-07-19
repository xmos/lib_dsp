// Copyright (c) 2017-2019, XMOS Ltd, All rights reserved
#include <dsp_testing.h>
#include <xs1.h>
#include <xclib.h>

/*
 * Random number generation
 */

#define CRC_POLY (0xEB31D82E)

int16_t  dsp_pseudo_rand_int16(unsigned *r){
    crc32(*r, -1, CRC_POLY);
    return (int16_t)*r;
}

uint16_t dsp_pseudo_rand_uint16(unsigned *r){
    crc32(*r, -1, CRC_POLY);
    return (uint16_t)*r;
}

int32_t  dsp_pseudo_rand_int32(unsigned *r){
    crc32(*r, -1, CRC_POLY);
    return (int32_t)*r;
}

uint32_t dsp_pseudo_rand_uint32(unsigned *r){
    crc32(*r, -1, CRC_POLY);
    return (uint32_t)*r;
}

int64_t  dsp_pseudo_rand_int64(unsigned *r){
    crc32(*r, -1, CRC_POLY);
    int64_t a = (int64_t)*r;
    crc32(*r, -1, CRC_POLY);
    int64_t b = (int64_t)*r;
    return (int64_t)(a + (b<<32));
}

uint64_t dsp_pseudo_rand_uint64(unsigned *r){
    crc32(*r, -1, CRC_POLY);
    int64_t a = (int64_t)*r;
    crc32(*r, -1, CRC_POLY);
    int64_t b = (int64_t)*r;
    return (uint64_t)(a + (b<<32));
}

dsp_float_t dsp_pseudo_rand_float(unsigned *r){
    return (dsp_float_t)dsp_pseudo_rand_int64(r);
}

dsp_complex_int16_t dsp_pseudo_rand_complex_int16(unsigned *r){
    dsp_complex_int16_t v;
    v.re = dsp_pseudo_rand_int16(r);
    v.im = dsp_pseudo_rand_int16(r);
    return v;
}

dsp_complex_int32_t dsp_pseudo_rand_complex_int32(unsigned *r){
    dsp_complex_int32_t v;
    v.re = dsp_pseudo_rand_int32(r);
    v.im = dsp_pseudo_rand_int32(r);
    return v;
}

dsp_complex_float_t dsp_pseudo_rand_complex_float(unsigned *r){
    dsp_complex_float_t v;
    v.re = dsp_pseudo_rand_float(r);
    v.im = dsp_pseudo_rand_float(r);
    return v;
}

dsp_ch_pair_int16_t dsp_pseudo_rand_ch_pair_int16(unsigned *r){
    dsp_ch_pair_int16_t v;
    v.ch_a = dsp_pseudo_rand_int16(r);
    v.ch_b = dsp_pseudo_rand_int16(r);
    return v;
}

dsp_ch_pair_int32_t dsp_pseudo_rand_ch_pair_int32(unsigned *r){
    dsp_ch_pair_int32_t v;
    v.ch_a = dsp_pseudo_rand_int32(r);
    v.ch_b = dsp_pseudo_rand_int32(r);
    return v;
}

dsp_ch_pair_float_t dsp_pseudo_rand_ch_pair_float(unsigned *r){
    dsp_ch_pair_float_t v;
    v.ch_a = dsp_pseudo_rand_float(r);
    v.ch_b = dsp_pseudo_rand_float(r);
    return v;
}
