// Copyright 2019-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include "dsp_bfp.h"
#include <xclib.h>
#include <dsp_complex.h>

#include "debug_print.h"

#include <stdlib.h>

unsigned clz2(unsigned m){
    if (m == 0)
        return 32;
    unsigned c=0;
    while((m&0x80000000)==0){
        c++;
        m<<=1;
    }
    return c;
}

static int32_t shr32(const int32_t d, const int s){
    if(s > 0){
        return d>>s;
    } else {
        return d<<(-s);
    }
}
static int16_t shr16(const int16_t d, const int s){
    if(s > 0){
        return d>>s;
    } else {
        return d<<(-s);
    }
}
static int8_t shr8(const int8_t d, const int s){
    if(s > 0){
        return d>>s;
    } else {
        return d<<(-s);
    }
}

unsigned sub_bfp_vect_int32_impl(int32_t * a, int32_t * b, int32_t * c,
    unsigned length, int shr_b, int shr_c){
    unsigned mask = 0;
    for(unsigned i=0;i<length;i++){

        a[i] = shr32 (b[i],shr_b) - shr32 (c[i],shr_c);

        int32_t v = a[i];
        if (v<0) v=-v;
        mask |= v;
    }
    return clz2(mask)-1;
}

unsigned sub_bfp_vect_int16_impl(int16_t * a, int16_t * b, int16_t * c,
    unsigned length, int shr_b, int shr_c){
    unsigned mask = 0;
    for(unsigned i=0;i<length;i++){

        a[i] = shr16 (b[i],shr_b) - shr16 (c[i],shr_c);

        int16_t v = a[i];
        if (v<0) v=-v;
        mask |= v;
    }
    return clz2(mask)-1-16;
}

unsigned sub_bfp_vect_int8_impl(int8_t * a, int8_t * b, int8_t * c,
    unsigned length, int shr_b, int shr_c){
    unsigned mask = 0;
    for(unsigned i=0;i<length;i++){

        a[i] = shr8 (b[i],shr_b) - shr8 (c[i],shr_c);

        int8_t v = a[i];
        if (v<0) v=-v;
        mask |= v;
    }
    return clz2(mask)-1-24;
}

void sub_vect_int32_impl(int32_t * a, int32_t * b, int32_t * c,
    unsigned length){
    for(unsigned i=0;i<length;i++){
        a[i] = b[i] - c[i];
    }
    return;
}

void sub_vect_int16_impl(int16_t * a, int16_t * b, int16_t * c,
    unsigned length){
    for(unsigned i=0;i<length;i++){
        a[i] = b[i] - c[i];
    }
    return;
}

void sub_vect_int8_impl(int8_t * a, int8_t * b, int8_t * c,
    unsigned length){
    for(unsigned i=0;i<length;i++){
        a[i] = b[i] - c[i];
    }
    return;
}

void dsp_sub_bfp_vect_int8(
    int8_t * a, int * a_exp, unsigned * a_hr,
    int8_t * b, int   b_exp, unsigned   b_hr,
    int8_t * c, int   c_exp, unsigned   c_hr,
    unsigned length){

    const unsigned h = 2;
    int shr_b = h, shr_c = h;

    shr_b -= b_hr;
    b_exp -= b_hr;
    shr_c -= c_hr;
    c_exp -= c_hr;

    if(b_exp > c_exp){
        shr_c += (b_exp - c_exp);
        *a_exp = b_exp + h;
    } else {
        shr_b += (c_exp - b_exp);
        *a_exp = c_exp + h;
    }

    *a_hr = sub_bfp_vect_int8_impl(a, b, c, length, shr_b, shr_c);
}

void dsp_sub_bfp_vect_int16(
    int16_t * a, int * a_exp, unsigned * a_hr,
    int16_t * b, int   b_exp, unsigned   b_hr,
    int16_t * c, int   c_exp, unsigned   c_hr,
    unsigned length){

    const unsigned h = 2;
    int shr_b = h, shr_c = h;

    shr_b -= b_hr;
    b_exp -= b_hr;
    shr_c -= c_hr;
    c_exp -= c_hr;

    if(b_exp > c_exp){
        shr_c += (b_exp - c_exp);
        *a_exp = b_exp + h;
    } else {
        shr_b += (c_exp - b_exp);
        *a_exp = c_exp + h;
    }

    *a_hr = sub_bfp_vect_int16_impl(a, b, c, length, shr_b, shr_c);
}
void dsp_sub_bfp_vect_int32(
    int32_t * a, int * a_exp, unsigned * a_hr,
    int32_t * b, int   b_exp, unsigned   b_hr,
    int32_t * c, int   c_exp, unsigned   c_hr,
    unsigned length){

    const unsigned h = 2;
    int shr_b = h, shr_c = h;

    shr_b -= b_hr;
    b_exp -= b_hr;
    shr_c -= c_hr;
    c_exp -= c_hr;

    if(b_exp > c_exp){
        shr_c += (b_exp - c_exp);
        *a_exp = b_exp + h;
    } else {
        shr_b += (c_exp - b_exp);
        *a_exp = c_exp + h;
    }

    *a_hr = sub_bfp_vect_int32_impl(a, b, c, length, shr_b, shr_c);
}


void dsp_sub_vect_int8(
    int8_t * a,
    int8_t * b,
    int8_t * c,
    unsigned length){

    sub_vect_int8_impl(a, b, c, length);
}

void dsp_sub_vect_int16(
    int16_t * a,
    int16_t * b,
    int16_t * c,
    unsigned length){

    sub_vect_int16_impl(a, b, c, length);
}
void dsp_sub_vect_int32(
    int32_t * a,
    int32_t * b,
    int32_t * c,
    unsigned length){

    sub_vect_int32_impl(a, b, c, length);
}


unsigned add_bfp_vect_int32_impl(int32_t * a, int32_t * b, int32_t * c,
    unsigned length, int shr_b, int shr_c){
    unsigned mask = 0;
    for(unsigned i=0;i<length;i++){

        a[i] = shr32 (b[i],shr_b) + shr32 (c[i],shr_c);

        int32_t v = a[i];
        if (v<0) v=(~v) + 1;
        mask |= v;
    }
    return clz2(mask)-1;
}

unsigned add_bfp_vect_int16_impl(int16_t * a, int16_t * b, int16_t * c,
    unsigned length, int shr_b, int shr_c){
    unsigned mask = 0;
    for(unsigned i=0;i<length;i++){

        a[i] = shr16 (b[i],shr_b) + shr16 (c[i],shr_c);

        int16_t v = a[i];
        // if (v<0) v=(~v) + 1;
        if (v<0) v=-v;
        mask |= v;
    }
    return clz2(mask)-1-16;
}

unsigned add_bfp_vect_int8_impl(int8_t * a, int8_t * b, int8_t * c,
    unsigned length, int shr_b, int shr_c){
    unsigned mask = 0;
    for(unsigned i=0;i<length;i++){

        a[i] = shr8 (b[i],shr_b) + shr8 (c[i],shr_c);

        int8_t v = a[i];
        if (v<0) v=(~v) + 1;
        mask |= v;
    }
    return clz2(mask)-1-24;
}

void add_vect_int32_impl(int32_t * a, int32_t * b, int32_t * c,
    unsigned length){
    for(unsigned i=0;i<length;i++){
        a[i] = b[i] + c[i];
    }
    return;
}

void add_vect_int16_impl(int16_t * a, int16_t * b, int16_t * c,
    unsigned length){
    for(unsigned i=0;i<length;i++){
        a[i] = b[i] + c[i];
    }
    return;
}

void add_vect_int8_impl(int8_t * a, int8_t * b, int8_t * c,
    unsigned length){
    for(unsigned i=0;i<length;i++){
        a[i] = b[i] + c[i];
    }
    return;
}


void dsp_add_bfp_vect_int8(
    int8_t * a, int * a_exp, unsigned * a_hr,
    int8_t * b, int   b_exp, unsigned   b_hr,
    int8_t * c, int   c_exp, unsigned   c_hr,
    unsigned length){

    const unsigned h = 2;
    int shr_b = h, shr_c = h;

    shr_b -= b_hr;
    b_exp -= b_hr;
    shr_c -= c_hr;
    c_exp -= c_hr;

    if(b_exp > c_exp){
        shr_c += (b_exp - c_exp);
        *a_exp = b_exp + h;
    } else {
        shr_b += (c_exp - b_exp);
        *a_exp = c_exp + h;
    }

    *a_hr = add_bfp_vect_int8_impl(a, b, c, length, shr_b, shr_c);
}

void dsp_add_bfp_vect_int16(
    int16_t * a, int * a_exp, unsigned * a_hr,
    int16_t * b, int   b_exp, unsigned   b_hr,
    int16_t * c, int   c_exp, unsigned   c_hr,
    unsigned length){

    const unsigned h = 2;
    int shr_b = h, shr_c = h;

    shr_b -= b_hr;
    b_exp -= b_hr;
    shr_c -= c_hr;
    c_exp -= c_hr;

    if(b_exp > c_exp){
        shr_c += (b_exp - c_exp);
        *a_exp = b_exp + h;
    } else {
        shr_b += (c_exp - b_exp);
        *a_exp = c_exp + h;
    }

    *a_hr = add_bfp_vect_int16_impl(a, b, c, length, shr_b, shr_c);
}
void dsp_add_bfp_vect_int32(
    int32_t * a, int * a_exp, unsigned * a_hr,
    int32_t * b, int   b_exp, unsigned   b_hr,
    int32_t * c, int   c_exp, unsigned   c_hr,
    unsigned length){

    const unsigned h = 2;
    int shr_b = h, shr_c = h;

    shr_b -= b_hr;
    b_exp -= b_hr;
    shr_c -= c_hr;
    c_exp -= c_hr;

    if(b_exp > c_exp){
        shr_c += (b_exp - c_exp);
        *a_exp = b_exp + h;
    } else {
        shr_b += (c_exp - b_exp);
        *a_exp = c_exp + h;
    }

    *a_hr = add_bfp_vect_int32_impl(a, b, c, length, shr_b, shr_c);
}


void dsp_add_vect_int8(
    int8_t * a,
    int8_t * b,
    int8_t * c,
    unsigned length){

    add_vect_int8_impl(a, b, c, length);
}

void dsp_add_vect_int16(
    int16_t * a,
    int16_t * b,
    int16_t * c,
    unsigned length){

    add_vect_int16_impl(a, b, c, length);
}
void dsp_add_vect_int32(
    int32_t * a,
    int32_t * b,
    int32_t * c,
    unsigned length){

    add_vect_int32_impl(a, b, c, length);
}


unsigned mul_bfp_vect_complex_int32_impl(dsp_complex_int32_t * a, dsp_complex_int32_t * b,
    dsp_complex_int32_t * c, unsigned length, unsigned shr_b, unsigned shr_c){
    unsigned mask = 0;
    for(unsigned i=0;i<length;i++){
        int64_t re = (int64_t)shr32(b[i].re,shr_b) * (int64_t)shr32(c[i].re,shr_c) - (int64_t)shr32(b[i].im,shr_b) * (int64_t)shr32(c[i].im,shr_c);
        int64_t im = (int64_t)shr32(b[i].re,shr_b) * (int64_t)shr32(c[i].im,shr_c) + (int64_t)shr32(b[i].im,shr_b) * (int64_t)shr32(c[i].re,shr_c);

        a[i].re = re>>30;
        a[i].im = im>>30;

        int32_t v = a[i].re;
        if (v<0) v=-v;
        mask |= v;
        v = a[i].im;
        if (v<0) v=-v;
        mask |= v;
    }
    return clz2(mask)-1;
}

void dsp_mul_bfp_vect_complex_int32(
    dsp_complex_int32_t * a, int * a_exp, unsigned * a_hr,
    dsp_complex_int32_t * b, int   b_exp, unsigned   b_hr,
    dsp_complex_int32_t * c, int   c_exp, unsigned   c_hr,
    unsigned length){

    const unsigned h = 1;
    int shr_b = b_hr+h;
    int shr_c = c_hr+h;
    *a_exp =  b_exp + c_exp + shr_b + shr_c + 32 - (h*2);

    *a_hr = mul_bfp_vect_complex_int32_impl(a, b, c, length, shr_b, shr_c);

}

unsigned mul_bfp_vect_int32_impl(int32_t * a, int32_t * b, int32_t * c,
    unsigned length, int shr_b, int shr_c){
    unsigned mask = 0;
    for(unsigned i=0;i<length;i++){

        a[i] = ((int64_t)shr32(b[i],shr_b) * (int64_t)shr32(c[i],shr_c))>>30;

        int32_t v = a[i];
        if (v<0) v=-v;
        mask |= v;
    }
    return clz2(mask)-1;
}


unsigned mul_bfp_vect_int16_impl(int16_t * a, int16_t * b, int16_t * c,
    unsigned length, int shr_b, int shr_c){
    unsigned mask = 0;
    for(unsigned i=0;i<length;i++){

        a[i] = ((int64_t)shr16(b[i], shr_b) * (int64_t)shr16(c[i],shr_c))>>14;
        int16_t v = a[i];
        if (v<0) v=-v;
        mask |= v;
    }
    return clz2(mask)-1-16;
}

unsigned mul_bfp_vect_int8_impl(int8_t * a, int8_t * b, int8_t * c,
    unsigned length, int shr_b, int shr_c){
    unsigned mask = 0;
    for(unsigned i=0;i<length;i++){

        a[i] = ((int64_t)shr8(b[i],shr_b) * (int64_t)shr8(c[i],shr_c))>>6;

        int8_t v = a[i];
        if (v<0) v=-v;
        mask |= v;
    }
    return clz2(mask)-1-24;
}

unsigned muls_bfp_vect_int32_impl(int32_t * a, int32_t * b, int32_t c,
    unsigned length, int shr_b, int shr_c){
    unsigned mask = 0;
    for(unsigned i=0;i<length;i++){

        a[i] = ((int64_t)shr32(b[i],shr_b) * (int64_t)shr32(c,shr_c))>>30;

        int32_t v = a[i];
        if (v<0) v=-v;
        mask |= v;
    }
    return clz2(mask)-1;
}

unsigned muls_bfp_vect_int16_impl(int16_t * a, int16_t * b, int16_t c,
    unsigned length, int shr_b, int shr_c){
    unsigned mask = 0;
    for(unsigned i=0;i<length;i++){

        a[i] = ((int64_t)shr16(b[i], shr_b) * (int64_t)shr16(c,shr_c))>>14;
        int16_t v = a[i];
        if (v<0) v=-v;
        mask |= v;
    }
    return clz2(mask)-1-16;
}

unsigned muls_bfp_vect_int8_impl(int8_t * a, int8_t * b, int8_t c,
    unsigned length, int shr_b, int shr_c){
    unsigned mask = 0;
    for(unsigned i=0;i<length;i++){

        a[i] = ((int64_t)shr8(b[i],shr_b) * (int64_t)shr8(c,shr_c))>>6;

        int8_t v = a[i];
        if (v<0) v=-v;
        mask |= v;
    }
    return clz2(mask)-1-24;
}

void mul_vect_int32_impl(int32_t * a, int32_t * b, int32_t * c,
    unsigned length){
    for(unsigned i=0;i<length;i++){
        a[i] = ((int64_t)(b[i]) * (int64_t)(c[i]))>>30;
    }
}


void mul_vect_int16_impl(int16_t * a, int16_t * b, int16_t * c,
    unsigned length){
    for(unsigned i=0;i<length;i++){
        a[i] = ((int64_t)(b[i]) * (int64_t)(c[i]))>>14;
    }
}

void mul_vect_int8_impl(int8_t * a, int8_t * b, int8_t * c,
    unsigned length){
    for(unsigned i=0;i<length;i++){
        a[i] = ((int64_t)(b[i]) * (int64_t)(c[i]))>>6;
    }
}


void dsp_mul_vect_int8(
    int8_t * a,
    int8_t * b,
    int8_t * c,
    unsigned length){

    mul_vect_int8_impl(a, b, c, length);
}

void dsp_mul_vect_int16(
    int16_t * a,
    int16_t * b,
    int16_t * c,
    unsigned length){

    mul_vect_int16_impl(a, b, c, length);
}
void dsp_mul_vect_int32(
    int32_t * a,
    int32_t * b,
    int32_t * c,
    unsigned length){

    mul_vect_int32_impl(a, b, c, length);
}

void dsp_mul_bfp_vect_int8(
    int8_t * a, int * a_exp, unsigned * a_hr,
    int8_t * b, int   b_exp, unsigned   b_hr,
    int8_t * c, int   c_exp, unsigned   c_hr,
    unsigned length){

    const unsigned h = 1;
    int shr_b = b_hr+h;
    int shr_c = c_hr+h;
    *a_exp =  b_exp + c_exp + shr_b + shr_c + 8 - (h*2);

    *a_hr = mul_bfp_vect_int8_impl(a, b, c, length, shr_b, shr_c);
}

void dsp_mul_bfp_vect_int16(
    int16_t * a, int * a_exp, unsigned * a_hr,
    int16_t * b, int   b_exp, unsigned   b_hr,
    int16_t * c, int   c_exp, unsigned   c_hr,
    unsigned length){

    const unsigned h = 1;
    int shr_b = b_hr+h;
    int shr_c = c_hr+h;
    *a_exp =  b_exp + c_exp + shr_b + shr_c + 16 - (h*2);

    *a_hr = mul_bfp_vect_int16_impl(a, b, c, length, shr_b, shr_c);
}
void dsp_mul_bfp_vect_int32(
    int32_t * a, int * a_exp, unsigned * a_hr,
    int32_t * b, int   b_exp, unsigned   b_hr,
    int32_t * c, int   c_exp, unsigned   c_hr,
    unsigned length){


    const unsigned h = 1;
    int shr_b = b_hr+h;
    int shr_c = c_hr+h;
    *a_exp =  b_exp + c_exp + shr_b + shr_c + 32 - (h*2);

    *a_hr = mul_bfp_vect_int32_impl(a, b, c, length, shr_b, shr_c);
}

void dsp_muls_bfp_vect_int8(
    int8_t * a, int * a_exp, unsigned * a_hr,
    int8_t * b, int   b_exp, unsigned   b_hr,
    int8_t c,   int   c_exp, unsigned   c_hr,
    unsigned length){

    const unsigned h = 1;
    int shr_b = b_hr+h;
    int shr_c = c_hr+h;
    *a_exp =  b_exp + c_exp + shr_b + shr_c + 8 - (h*2);

    *a_hr = muls_bfp_vect_int8_impl(a, b, c, length, shr_b, shr_c);
}

void dsp_muls_bfp_vect_int16(
    int16_t * a, int * a_exp, unsigned * a_hr,
    int16_t * b, int   b_exp, unsigned   b_hr,
    int16_t c,   int   c_exp, unsigned   c_hr,
    unsigned length){


    const unsigned h = 1;
    int shr_b = b_hr+h;
    int shr_c = c_hr+h;
    *a_exp =  b_exp + c_exp + shr_b + shr_c + 16 - (h*2);

    *a_hr = muls_bfp_vect_int16_impl(a, b, c, length, shr_b, shr_c);
}

void dsp_muls_bfp_vect_int32(
    int32_t * a, int * a_exp, unsigned * a_hr,
    int32_t * b, int   b_exp, unsigned   b_hr,
    int32_t c,   int   c_exp, unsigned   c_hr,
    unsigned length){


    const unsigned h = 1;
    int shr_b = b_hr+h;
    int shr_c = c_hr+h;
    *a_exp =  b_exp + c_exp + shr_b + shr_c + 32 - (h*2);

    *a_hr = muls_bfp_vect_int32_impl(a, b, c, length, shr_b, shr_c);
}

void dsp_div_bfp_vect_int32_impl(
    int32_t * UNSAFE a, int * UNSAFE a_exp, unsigned * UNSAFE a_hr,
    int32_t * UNSAFE b, int   b_exp, unsigned   b_hr,
    int32_t * UNSAFE c, int   c_exp, unsigned   c_hr,
    unsigned length, unsigned nums_per_den)
{
    // Figure out maximum possible shift that doesn't overflow
    unsigned max_c_clz = 0;
    for (unsigned i=0; i<length; i++) {
        unsigned c_abs = abs(c[i]);
        if (c_abs == 0) {
            max_c_clz = 32;
            break;
        }
        unsigned cur_clz;
        asm("clz %0, %1" : "=r"(cur_clz) : "r"(c_abs));
        if (cur_clz > max_c_clz) {
            max_c_clz = cur_clz;
        }
    }
    int c_shift = (32 - max_c_clz - 1);
    if (c_shift <=0) { c_shift = 0; }

    // Do the divide
    unsigned mask = 0;
    for (unsigned i=0; i<length; i++) {
        int32_t den = c[i / nums_per_den];
        int sign = 1;
        if ((den < 0 && b[i] >= 0) || (den >= 0 && b[i] < 0)) {
            sign = -1;
        }
        if (den) {
            uint64_t num = ((uint64_t) abs(b[i])) << (uint64_t) (b_hr + c_shift);
            uint32_t num_hi = (uint32_t) (num >> 32);
            uint32_t num_low = (uint32_t) (num & 0xFFFFFFFF);
            uint32_t mod;
            asm("ldivu %0, %1, %2, %3, %4" : "=r"(a[i]), "=r"(mod) : "r"(num_hi), "r"(num_low), "r"(abs(den)));
        } else {
            a[i] = INT32_MAX;
            debug_printf("div_bfp: DIVIDE BY ZERO\n");
        }
        mask |= a[i];
        a[i] *= sign;
    }
    int32_t output_exp = (b_exp - b_hr) - c_exp - c_shift;
    *a_hr = clz2(mask)-1;
    *a_exp = output_exp;
}

void dsp_div_bfp_vect_int32(
    int32_t * UNSAFE a, int * UNSAFE a_exp, unsigned * UNSAFE a_hr,
    int32_t * UNSAFE b, int   b_exp, unsigned   b_hr,
    int32_t * UNSAFE c, int   c_exp, unsigned   c_hr,
    unsigned length) {

    return dsp_div_bfp_vect_int32_impl(
    a, a_exp, a_hr,
    b, b_exp, b_hr,
    c, c_exp, c_hr,
    length, 1
    );
}

void dsp_div_bfp_vect_complex_int32(
    dsp_complex_t * UNSAFE a, int * UNSAFE a_exp, unsigned * UNSAFE a_hr,
    dsp_complex_t * UNSAFE b, int   b_exp, unsigned   b_hr,
    int32_t * UNSAFE c, int   c_exp, unsigned   c_hr,
    unsigned length) {

    return dsp_div_bfp_vect_int32_impl(
    (int32_t *) a, a_exp, a_hr,
    (int32_t *) b, b_exp, b_hr,
    c, c_exp, c_hr,
    length * 2, 2
    );
}

void dsp_div_bfp_vect_int16(
    int16_t * UNSAFE a, int * UNSAFE a_exp, unsigned * UNSAFE a_hr,
    int16_t * UNSAFE b, int   b_exp, unsigned   b_hr,
    int16_t * UNSAFE c, int   c_exp, unsigned   c_hr,
    unsigned length)
{
    // Figure out maximum possible shift that doesn't overflow
    unsigned max_c_clz = 0;
    for (unsigned i=0; i<length; i++) {
        unsigned c_abs = abs(c[i]);
        if (c_abs == 0) {
            max_c_clz = 16;
            break;
        }
        unsigned cur_clz;
        asm("clz %0, %1" : "=r"(cur_clz) : "r"(c_abs));
        cur_clz -= 16;
        if (cur_clz > max_c_clz) {
            max_c_clz = cur_clz;
        }
    }
    int c_shift = (16 - max_c_clz - 1);
    if (c_shift <=0) { c_shift = 0; }

    // Do the divide
    unsigned mask = 0;
    for (unsigned i=0; i<length; i++) {
        int sign = 1;
        if ((c[i] < 0 && b[i] >= 0) || (c[i] >= 0 && b[i] < 0)) {
            sign = -1;
        }
        if (c[i]) {
            uint16_t den = abs(c[i]);
            int32_t num = ((uint32_t) abs(b[i])) << (uint32_t) (b_hr + c_shift);
            a[i] = (int16_t) (num / den);
        } else {
            a[i] = INT16_MAX;
            debug_printf("div_bfp: DIVIDE BY ZERO\n");
        }
        mask |= a[i];
        a[i] *= sign;
    }
    *a_hr = clz2(mask)-1 - 16;
    int32_t output_exp = (b_exp - b_hr) - c_exp - c_shift;
    *a_exp = output_exp;
}

void dsp_div_bfp_vect_int8(
    int8_t * UNSAFE a, int * UNSAFE a_exp, unsigned * UNSAFE a_hr,
    int8_t * UNSAFE b, int   b_exp, unsigned   b_hr,
    int8_t * UNSAFE c, int   c_exp, unsigned   c_hr,
    unsigned length)
{
    // Figure out maximum possible shift that doesn't overflow
    unsigned max_c_clz = 0;
    for (unsigned i=0; i<length; i++) {
        unsigned c_abs = abs(c[i]);
        if (c_abs == 0) {
            max_c_clz = 8;
            break;
        }
        unsigned cur_clz;
        asm("clz %0, %1" : "=r"(cur_clz) : "r"(c_abs));
        cur_clz -= 24;
        if (cur_clz > max_c_clz) {
            max_c_clz = cur_clz;
        }
    }
    int c_shift = (8 - max_c_clz - 1);
    if (c_shift <=0) { c_shift = 0; }

    // Do the divide
    unsigned mask = 0;
    for (unsigned i=0; i<length; i++) {
        int sign = 1;
        if ((c[i] < 0 && b[i] >= 0) || (c[i] >= 0 && b[i] < 0)) {
            sign = -1;
        }
        if (c[i]) {
            uint8_t den = abs(c[i]);
            int32_t num = ((uint64_t) abs(b[i])) << (uint64_t) (b_hr + c_shift);
            a[i] = (int8_t) (num / den);
        } else {
            a[i] = INT8_MAX;
            debug_printf("div_bfp: DIVIDE BY ZERO\n");
        }
        mask |= a[i];
        a[i] *= sign;
    }
    *a_hr = clz2(mask)-1 - 24;
    int32_t output_exp = (b_exp - b_hr) - c_exp - c_shift;
    *a_exp = output_exp;
}
