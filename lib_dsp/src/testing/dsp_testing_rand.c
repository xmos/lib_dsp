// Copyright 2017-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include <dsp_testing.h>
#include <xs1.h>

#include <xclib.h>


// usnigned crc32_c(unsigned, unsigned, unsigned)
/*
 * Random number generation
 */

void crc32_c(unsigned *checksum, unsigned data, unsigned poly);

#define CRC_POLY (0xEB31D82E)

int8_t  dsp_pseudo_rand_int8(unsigned *r){
    crc32_c(r, -1, CRC_POLY);
    return (int8_t)*r;
}

uint8_t  dsp_pseudo_rand_uint8(unsigned *r){
    crc32_c(r, -1, CRC_POLY);
    return (uint8_t)*r;
}

int16_t  dsp_pseudo_rand_int16(unsigned *r){
    crc32_c(r, -1, CRC_POLY);
    return (int16_t)*r;
}

uint16_t dsp_pseudo_rand_uint16(unsigned *r){
    crc32_c(r, -1, CRC_POLY);
    return (uint16_t)*r;
}

int32_t  dsp_pseudo_rand_int32(unsigned *r){
    crc32_c(r, -1, CRC_POLY);
    return (int32_t)*r;
}

uint32_t dsp_pseudo_rand_uint32(unsigned *r){
    crc32_c(r, -1, CRC_POLY);
    return (uint32_t)*r;
}

int64_t  dsp_pseudo_rand_int64(unsigned *r){
    crc32_c(r, -1, CRC_POLY);
    int64_t a = (int64_t)*r;
    crc32_c(r, -1, CRC_POLY);
    int64_t b = (int64_t)*r;
    return (int64_t)(a + (b<<32));
}

uint64_t dsp_pseudo_rand_uint64(unsigned *r){
    crc32_c(r, -1, CRC_POLY);
    int64_t a = (int64_t)*r;
    crc32_c(r, -1, CRC_POLY);
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
