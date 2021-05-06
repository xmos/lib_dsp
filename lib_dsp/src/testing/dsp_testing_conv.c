// Copyright 2019-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include "dsp_testing.h"
#include "dsp_bfp.h" //for DSP_BFP_ZERO_EXP
#include <math.h>
#include <string.h>

/*
 * Type conversion
 */
dsp_float_t dsp_conv_int8_to_float (const int8_t x,  const int x_exp, int *error){
    //TODO check for errors
    return (dsp_float_t)ldexp((dsp_float_t)x, x_exp);
}

dsp_float_t dsp_conv_uint8_to_float(const uint8_t x, const int x_exp, int *error){
    return (dsp_float_t)ldexp((dsp_float_t)x, x_exp);
}

dsp_float_t dsp_conv_int16_to_float (const int16_t x,  const int x_exp, int *error){
    //TODO check for errors
    return (dsp_float_t)ldexp((dsp_float_t)x, x_exp);
}

dsp_float_t dsp_conv_uint16_to_float(const uint16_t x, const int x_exp, int *error){
    return (dsp_float_t)ldexp((dsp_float_t)x, x_exp);
}

dsp_float_t dsp_conv_int32_to_float (const int32_t x,  const int x_exp, int *error){
    return (dsp_float_t)ldexp((dsp_float_t)x, x_exp);
}

dsp_float_t dsp_conv_uint32_to_float(const uint32_t x, const int x_exp, int *error){
    return (dsp_float_t)ldexp((dsp_float_t)x, x_exp);
}

dsp_float_t dsp_conv_int64_to_float (const int64_t x,  const int x_exp, int *error){
    return (dsp_float_t)ldexp((dsp_float_t)x, x_exp);
}

dsp_float_t dsp_conv_uint64_to_float(const uint64_t x, const int x_exp, int *error){
    return (dsp_float_t)ldexp((dsp_float_t)x, x_exp);
}

int8_t  dsp_conv_float_to_int8 (dsp_float_t d, const int d_exp, int *error){
    int m_exp;
    dsp_float_t m = (dsp_float_t)frexp (d, &m_exp);

    dsp_float_t r = (dsp_float_t)ldexp(m, m_exp - d_exp);
    int output_exponent;
    frexp(r, &output_exponent);
    if(output_exponent>7){
        *error |= CANNOT_FIT_MANTISSA;
        return 0;
    }
    error = 0;
    return r;
}

uint8_t dsp_conv_float_to_uint8(dsp_float_t d, const int d_exp, int *error){
    int m_exp;
    dsp_float_t m = (dsp_float_t)frexp (d, &m_exp);
    if(m<0.0){
        *error |= NEGATIVE_TO_UNSIGNED;
        return 0;
    }
    error = 0;
    return ldexp(m, m_exp - d_exp);
}

int16_t  dsp_conv_float_to_int16 (dsp_float_t d, const int d_exp, int *error){
    int m_exp;
    dsp_float_t m = (dsp_float_t)frexp (d, &m_exp);

    dsp_float_t r = (dsp_float_t)ldexp(m, m_exp - d_exp);
    int output_exponent;
    frexp(r, &output_exponent);
    if(output_exponent>15){
        *error |= CANNOT_FIT_MANTISSA;
        return 0;
    }
    error = 0;
    return r;
}

uint16_t dsp_conv_float_to_uint16(dsp_float_t d, const int d_exp, int *error){
    int m_exp;
    dsp_float_t m = (dsp_float_t)frexp (d, &m_exp);
    if(m<0.0){
        *error |= NEGATIVE_TO_UNSIGNED;
        return 0;
    }
    error = 0;
    return ldexp(m, m_exp - d_exp);
}

int32_t  dsp_conv_float_to_int32 (dsp_float_t d, const int d_exp, int *error){
    int m_exp;
    double m = frexp (d, &m_exp);

    double r = ldexp(m, m_exp - d_exp);
    int output_exponent;
    frexp(r, &output_exponent);
    if(output_exponent>31){
        *error |= CANNOT_FIT_MANTISSA;
        return 0;
    }
    error = 0;
    return r;
}

uint32_t dsp_conv_float_to_uint32(dsp_float_t d, const int d_exp, int *error){
    int m_exp;
    double m = frexp (d, &m_exp);
    if(m<0.0){
        *error |= NEGATIVE_TO_UNSIGNED;
        return 0;
    }
    error = 0;
    return ldexp(m, m_exp - d_exp);
}

int64_t  dsp_conv_float_to_int64 (dsp_float_t d, const int d_exp, int *error){
    int m_exp;
    double m = frexp (d, &m_exp);

    double r = ldexp(m, m_exp - d_exp);
    int output_exponent;
    frexp(r, &output_exponent);
    if(output_exponent>63){
        *error |= CANNOT_FIT_MANTISSA;
        return 0;
    }
    error = 0;
    return r;
}

uint64_t dsp_conv_float_to_uint64(dsp_float_t d, const int d_exp, int *error){
    int m_exp;
    double m = frexp (d, &m_exp);
    if(m<0.0){
        *error |= NEGATIVE_TO_UNSIGNED;
        return 0;
    }
    error = 0;
    return (uint64_t)ldexp(m, m_exp - d_exp);
}

dsp_complex_float_t dsp_conv_complex_int16_to_complex_float(dsp_complex_int16_t x, const int x_exp, int *error){
    dsp_complex_float_t r;
    r.re = dsp_conv_int16_to_float(x.re, x_exp, error);
    r.im = dsp_conv_int16_to_float(x.im, x_exp, error);
    return r;
}

dsp_complex_float_t dsp_conv_complex_int32_to_complex_float(dsp_complex_int32_t x, const int x_exp, int *error){
    dsp_complex_float_t r;
    r.re = dsp_conv_int32_to_float(x.re, x_exp, error);
    r.im = dsp_conv_int32_to_float(x.im, x_exp, error);
    return r;
}

dsp_ch_pair_float_t dsp_conv_ch_pair_int16_to_ch_pair_float(dsp_ch_pair_int16_t x, const int x_exp, int *error){
    dsp_ch_pair_float_t r;
    r.ch_a = dsp_conv_int16_to_float(x.ch_a, x_exp, error);
    r.ch_b = dsp_conv_int16_to_float(x.ch_b, x_exp, error);
    return r;
}

dsp_ch_pair_float_t dsp_conv_ch_pair_int32_to_ch_pair_float(dsp_ch_pair_int32_t x, const int x_exp, int *error){
    dsp_ch_pair_float_t r;
    r.ch_a = dsp_conv_int32_to_float(x.ch_a, x_exp, error);
    r.ch_b = dsp_conv_int32_to_float(x.ch_b, x_exp, error);
    return r;
}

dsp_complex_int16_t dsp_conv_complex_float_to_complex_int16(dsp_complex_float_t x, const int x_exp, int *error){
    dsp_complex_int16_t r;
    r.re = dsp_conv_float_to_int16(x.re, x_exp, error);
    r.im = dsp_conv_float_to_int16(x.im, x_exp, error);
    return r;
}
dsp_complex_int32_t dsp_conv_complex_float_to_complex_int32(dsp_complex_float_t x, const int x_exp, int *error){
    dsp_complex_int32_t r;
    r.re = dsp_conv_float_to_int32(x.re, x_exp, error);
    r.im = dsp_conv_float_to_int32(x.im, x_exp, error);
    return r;
}

dsp_ch_pair_int16_t dsp_conv_ch_pair_float_to_ch_pair_int16(dsp_ch_pair_float_t x, const int x_exp, int *error){
    dsp_ch_pair_int16_t r;
    r.ch_a = dsp_conv_float_to_int16(x.ch_a, x_exp, error);
    r.ch_b = dsp_conv_float_to_int16(x.ch_b, x_exp, error);
    return r;
}
dsp_ch_pair_int32_t dsp_conv_ch_pair_float_to_ch_pair_int32(dsp_ch_pair_float_t x, const int x_exp, int *error){
    dsp_ch_pair_int32_t r;
    r.ch_a = dsp_conv_float_to_int32(x.ch_a, x_exp, error);
    r.ch_b = dsp_conv_float_to_int32(x.ch_b, x_exp, error);
    return r;
}

/*
 * Vector conversion
 */

void dsp_conv_vect_int8_to_float (const int8_t *x,  const int x_exp, dsp_float_t * f, unsigned length, int *error){

    int8_t mask = 0;
    for(unsigned i=0;i<length;i++){
        mask |= x[i];
    }

    if(mask == 0){
        memset(f, 0, sizeof(dsp_float_t)*length);
        return;
    }
    for(unsigned i=0;i<length;i++){
        f[i] = dsp_conv_int8_to_float(x[i], x_exp, error);
    }
}

void dsp_conv_vect_uint8_to_float(const uint8_t *x, const int x_exp, dsp_float_t * f, unsigned length, int *error){
    uint8_t mask = 0;
    for(unsigned i=0;i<length;i++){
        mask |= x[i];
    }

    if(mask == 0){
        memset(f, 0, sizeof(dsp_float_t)*length);
        return;
    }
    for(unsigned i=0;i<length;i++){
        f[i] = dsp_conv_uint8_to_float(x[i], x_exp, error);
    }
}

void dsp_conv_vect_int16_to_float (const int16_t *x,  const int x_exp, dsp_float_t * f, unsigned length, int *error){

    int16_t mask = 0;
    for(unsigned i=0;i<length;i++){
        mask |= x[i];
    }

    if(mask == 0){
        memset(f, 0, sizeof(dsp_float_t)*length);
        return;
    }
    for(unsigned i=0;i<length;i++){
        f[i] = dsp_conv_int16_to_float(x[i], x_exp, error);
    }
}

void dsp_conv_vect_uint16_to_float(const uint16_t *x, const int x_exp, dsp_float_t * f, unsigned length, int *error){
    uint16_t mask = 0;
    for(unsigned i=0;i<length;i++){
        mask |= x[i];
    }

    if(mask == 0){
        memset(f, 0, sizeof(dsp_float_t)*length);
        return;
    }
    for(unsigned i=0;i<length;i++){
        f[i] = dsp_conv_uint16_to_float(x[i], x_exp, error);
    }
}

void dsp_conv_vect_int32_to_float (const int32_t *x,  const int x_exp, dsp_float_t * f, unsigned length, int *error){

    int32_t mask = 0;
    for(unsigned i=0;i<length;i++){
        mask |= x[i];
    }

    if(mask == 0){
        memset(f, 0, sizeof(dsp_float_t)*length);
        return;
    }
    for(unsigned i=0;i<length;i++){
        f[i] = dsp_conv_int32_to_float(x[i], x_exp, error);
    }
}
void dsp_conv_vect_uint32_to_float(const uint32_t *x, const int x_exp, dsp_float_t * f, unsigned length, int *error){

    uint32_t mask = 0;
    for(unsigned i=0;i<length;i++){
        mask |= x[i];
    }

    if(mask == 0){
        memset(f, 0, sizeof(dsp_float_t)*length);
        return;
    }
    for(unsigned i=0;i<length;i++){
        f[i] = dsp_conv_uint32_to_float(x[i], x_exp, error);
    }
}
void dsp_conv_vect_int64_to_float (const int64_t *x,  const int x_exp, dsp_float_t * f, unsigned length, int *error){

    int64_t mask = 0;
    for(unsigned i=0;i<length;i++){
        mask |= x[i];
    }

    if(mask == 0){
        memset(f, 0, sizeof(dsp_float_t)*length);
        return;
    }
    for(unsigned i=0;i<length;i++){
        f[i] = dsp_conv_int64_to_float(x[i], x_exp, error);
    }
}

void dsp_conv_vect_uint64_to_float(const uint64_t *x, const int x_exp, dsp_float_t * f, unsigned length, int *error){

    uint64_t mask = 0;
    for(unsigned i=0;i<length;i++){
        mask |= x[i];
    }

    if(mask == 0){
        memset(f, 0, sizeof(dsp_float_t)*length);
        return;
    }
    for(unsigned i=0;i<length;i++){
        f[i] = dsp_conv_uint64_to_float(x[i], x_exp, error);
    }
}

void dsp_conv_vect_float_to_int8 (dsp_float_t *f, int8_t *d, int *d_exp, unsigned length, int *error){
    if(!length)
        return;

    dsp_float_t min = f[0];
    dsp_float_t max = f[0];

    for(unsigned i=1;i<length;i++){
        max = fmax(max, f[i]);
        min = fmin(min, f[i]);
    }

    if((min == 0.0) && (max == 0.0)){
        memset(d, 0, sizeof(uint8_t)*length);
        *d_exp = DSP_BFP_ZERO_EXP;
        return;
    }
    dsp_float_t abs_max = fmax(fabs(min), fabs(max));

    int e;
    frexp(abs_max, &e);
    *d_exp = e - 7;
    for(unsigned i=0;i<length;i++){
        d[i] = dsp_conv_float_to_int8(f[i], *d_exp, error);
    }
}

void dsp_conv_vect_float_to_uint8 (dsp_float_t *f, uint8_t *d, int *d_exp, unsigned length, int *error){
    if(!length)
        return;

    dsp_float_t min = f[0];
    dsp_float_t max = f[0];

    for(unsigned i=1;i<length;i++){
        max = fmax(max, f[i]);
        min = fmin(min, f[i]);
    }

    if((min == 0.0) && (max == 0.0)){
        memset(d, 0, sizeof(uint8_t)*length);
        *d_exp = DSP_BFP_ZERO_EXP;
        return;
    }

    if(min < 0.0){
        *error = NEGATIVE_TO_UNSIGNED;
        return;
    }

    int e;
    frexp(max, &e);
    *d_exp = e - 8;
    for(unsigned i=0;i<length;i++){
        d[i] = dsp_conv_float_to_uint8(f[i], *d_exp, error);
    }
}

void dsp_conv_vect_float_to_int16 (dsp_float_t *f, int16_t *d, int *d_exp, unsigned length, int *error){
    if(!length)
        return;

    dsp_float_t min = f[0];
    dsp_float_t max = f[0];

    for(unsigned i=1;i<length;i++){
        max = fmax(max, f[i]);
        min = fmin(min, f[i]);
    }

    if((min == 0.0) && (max == 0.0)){
        memset(d, 0, sizeof(int16_t)*length);
        *d_exp = DSP_BFP_ZERO_EXP;
        return;
    }
    dsp_float_t abs_max = fmax(fabs(min), fabs(max));

    int e;
    frexp(abs_max, &e);
    *d_exp = e - 15;
    for(unsigned i=0;i<length;i++){
        d[i] = dsp_conv_float_to_int16(f[i], *d_exp, error);
    }
}

void dsp_conv_vect_float_to_uint16 (dsp_float_t *f, uint16_t *d, int *d_exp, unsigned length, int *error){
    if(!length)
        return;

    dsp_float_t min = f[0];
    dsp_float_t max = f[0];

    for(unsigned i=1;i<length;i++){
        max = fmax(max, f[i]);
        min = fmin(min, f[i]);
    }

    if((min == 0.0) && (max == 0.0)){
        memset(d, 0, sizeof(uint16_t)*length);
        *d_exp = DSP_BFP_ZERO_EXP;
        return;
    }

    if(min < 0.0){
        *error = NEGATIVE_TO_UNSIGNED;
        return;
    }

    int e;
    frexp(max, &e);
    *d_exp = e - 16;
    for(unsigned i=0;i<length;i++){
        d[i] = dsp_conv_float_to_uint16(f[i], *d_exp, error);
    }
}
void dsp_conv_vect_float_to_int32 (dsp_float_t *f, int32_t *d, int *d_exp, unsigned length, int *error){
    if(!length)
        return;

    dsp_float_t min = f[0];
    dsp_float_t max = f[0];

    for(unsigned i=0;i<length;i++){
        max = fmax(max, f[i]);
        min = fmin(min, f[i]);
    }

    if((min == 0.0) && (max == 0.0)){
        memset(d, 0, sizeof(int32_t)*length);
        *d_exp = DSP_BFP_ZERO_EXP;
        return;
    }
    dsp_float_t abs_max = fmax(fabs(min), fabs(max));

    int e;
    frexp(abs_max, &e);
    *d_exp = e - 31;
    for(unsigned i=0;i<length;i++){
        d[i] = dsp_conv_float_to_int32(f[i], *d_exp, error);
    }
}

void dsp_conv_vect_float_to_uint32 (dsp_float_t *f, uint32_t *d, int *d_exp, unsigned length, int *error){
    if(!length)
        return;

    dsp_float_t min = f[0];
    dsp_float_t max = f[0];

    for(unsigned i=1;i<length;i++){
        max = fmax(max, f[i]);
        min = fmin(min, f[i]);
    }

    if((min == 0.0) && (max == 0.0)){
        memset(d, 0, sizeof(uint32_t)*length);
        *d_exp = DSP_BFP_ZERO_EXP;
        return;
    }

    if(min < 0.0){
        *error = NEGATIVE_TO_UNSIGNED;
        return;
    }

    int e;
    frexp(max, &e);
    *d_exp = e - 32;
    for(unsigned i=0;i<length;i++){
        d[i] = dsp_conv_float_to_uint32(f[i], *d_exp, error);
    }
}

void dsp_conv_vect_float_to_int64 (dsp_float_t *f, int64_t *d, int *d_exp, unsigned length, int *error){
    if(!length)
        return;

    dsp_float_t min = f[0];
    dsp_float_t max = f[0];

    for(unsigned i=0;i<length;i++){
        max = fmax(max, f[i]);
        min = fmin(min, f[i]);
    }

    if((min == 0.0) && (max == 0.0)){
        memset(d, 0, sizeof(int64_t)*length);
        *d_exp = DSP_BFP_ZERO_EXP;
        return;
    }
    dsp_float_t abs_max = fmax(fabs(min), fabs(max));

    int e;
    frexp(abs_max, &e);
    *d_exp = e - 63;
    for(unsigned i=0;i<length;i++){
        d[i] = dsp_conv_float_to_int64(f[i], *d_exp, error);
    }
}

void dsp_conv_vect_float_to_uint64 (dsp_float_t *f, uint64_t *d, int *d_exp, unsigned length, int *error){
    if(!length)
        return;

    dsp_float_t min = f[0];
    dsp_float_t max = f[0];

    for(unsigned i=1;i<length;i++){
        max = fmax(max, f[i]);
        min = fmin(min, f[i]);
    }

    if((min == 0.0) && (max == 0.0)){
        memset(d, 0, sizeof(uint64_t)*length);
        *d_exp = DSP_BFP_ZERO_EXP;
        return;
    }

    if(min < 0.0){
        *error = NEGATIVE_TO_UNSIGNED;
        return;
    }

    int e;
    frexp(max, &e);
    *d_exp = e - 64;
    for(unsigned i=0;i<length;i++){
        d[i] = dsp_conv_float_to_uint64(f[i], *d_exp, error);
    }
}

void dsp_conv_vect_complex_int16_to_complex_float(dsp_complex_int16_t * d, const int d_exp, dsp_complex_float_t *f,
        unsigned length, int *error){
    for(unsigned i=0;i<length;i++){
        f[i] = dsp_conv_complex_int16_to_complex_float(d[i], d_exp, error);
    }
}

void dsp_conv_vect_complex_int32_to_complex_float(dsp_complex_int32_t * d, const int d_exp, dsp_complex_float_t *f,
        unsigned length, int *error){
    for(unsigned i=0;i<length;i++){
        f[i] = dsp_conv_complex_int32_to_complex_float(d[i], d_exp, error);
    }
}

void dsp_conv_vect_ch_pair_int16_to_ch_pair_float(dsp_ch_pair_int16_t * d, const int d_exp, dsp_ch_pair_float_t *f,
        unsigned length, int *error){
    for(unsigned i=0;i<length;i++){
        f[i] = dsp_conv_ch_pair_int16_to_ch_pair_float(d[i], d_exp, error);
    }
}

void dsp_conv_vect_ch_pair_int32_to_ch_pair_float(dsp_ch_pair_int32_t * d, const int d_exp, dsp_ch_pair_float_t *f,
        unsigned length, int *error){
    for(unsigned i=0;i<length;i++){
        f[i] = dsp_conv_ch_pair_int32_to_ch_pair_float(d[i], d_exp, error);
    }
}

void dsp_conv_vect_complex_float_to_complex_int16( dsp_complex_float_t *f, dsp_complex_int16_t * d, int *d_exp,
        unsigned length, int *error){
    dsp_conv_vect_float_to_int16((dsp_float_t *)f, (int16_t*)d, d_exp, length*2, error);
}

void dsp_conv_vect_complex_float_to_complex_int32( dsp_complex_float_t *f, dsp_complex_int32_t * d, int *d_exp,
        unsigned length, int *error){
    dsp_conv_vect_float_to_int32((dsp_float_t *)f, (int32_t*)d, d_exp, length*2, error);
}

void dsp_conv_vect_ch_pair_float_to_ch_pair_int16(dsp_ch_pair_float_t *f, dsp_ch_pair_int16_t * d, int * d_exp,
        unsigned length, int *error){
    dsp_conv_vect_float_to_int16((dsp_float_t *)f, (int16_t*)d, d_exp, length*2, error);
}

void dsp_conv_vect_ch_pair_float_to_ch_pair_int32(dsp_ch_pair_float_t *f, dsp_ch_pair_int32_t * d, int * d_exp,
        unsigned length, int *error){
    dsp_conv_vect_float_to_int32((dsp_float_t *)f, (int32_t*)d, d_exp, length*2, error);
}

