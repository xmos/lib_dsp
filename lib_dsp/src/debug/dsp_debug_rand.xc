// Copyright (c) 2017-2019, XMOS Ltd, All rights reserved
#include <dsp_debug.h>

/*
 * Random number generation
 */

#define CRC_POLY (0xEB31D82E)

int16_t  dsp_rand_int16(unsigned &r){
    crc32(r, -1, CRC_POLY);
    return (int16_t)r;
}

uint16_t dsp_rand_uint16(unsigned &r){
    crc32(r, -1, CRC_POLY);
    return (uint16_t)r;
}

int32_t  dsp_rand_int32(unsigned &r){
    crc32(r, -1, CRC_POLY);
    return (int32_t)r;
}

uint32_t dsp_rand_uint32(unsigned &r){
    crc32(r, -1, CRC_POLY);
    return (uint32_t)r;
}

int64_t  dsp_rand_int64(unsigned &r){
    crc32(r, -1, CRC_POLY);
    int64_t a = (int64_t)r;
    crc32(r, -1, CRC_POLY);
    int64_t b = (int64_t)r;
    return (int64_t)(a + (b<<32));
}

uint64_t dsp_rand_uint64(unsigned &r){
    crc32(r, -1, CRC_POLY);
    int64_t a = (int64_t)r;
    crc32(r, -1, CRC_POLY);
    int64_t b = (int64_t)r;
    return (uint64_t)(a + (b<<32));
}

