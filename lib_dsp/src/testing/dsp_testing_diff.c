// Copyright 2019-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include "dsp_testing.h"

/*
 * Float/Fixed comparision
 */
unsigned dsp_abs_diff_ch_pair_int16(dsp_ch_pair_int16_t B, const int B_exp,
        dsp_ch_pair_float_t f, int channel_index, int *error){
    return dsp_abs_diff_int16(((int16_t*)&B)[channel_index], B_exp,
            ((dsp_float_t*)&f)[channel_index], error);
}

unsigned dsp_abs_diff_ch_pair_int32(dsp_ch_pair_int32_t B, const int B_exp,
        dsp_ch_pair_float_t f, int channel_index, int *error){
    return dsp_abs_diff_int32(((int32_t*)&B)[channel_index], B_exp,
            ((dsp_float_t*)&f)[channel_index], error);
}

unsigned dsp_abs_diff_complex_int16(dsp_complex_int16_t B, const int B_exp, dsp_complex_float_t f, int *error){
    unsigned re_diff = dsp_abs_diff_int16(B.re, B_exp, f.re, error);
    unsigned im_diff = dsp_abs_diff_int16(B.im, B_exp, f.im, error);
    if(re_diff > im_diff){
        return re_diff;
    } else {
        return im_diff;
    }
}

unsigned dsp_abs_diff_complex_int32(dsp_complex_int32_t B, const int B_exp, dsp_complex_float_t f, int *error){
    unsigned re_diff = dsp_abs_diff_int32(B.re, B_exp, f.re, error);
    unsigned im_diff = dsp_abs_diff_int32(B.im, B_exp, f.im, error);
    if(re_diff > im_diff){
        return re_diff;
    } else {
        return im_diff;
    }
}

unsigned dsp_abs_diff_int8 ( int8_t B, const int B_exp, dsp_float_t f,  int *error){
    int8_t v = dsp_conv_float_to_int8(f, B_exp, error);
    int diff = v - B;
    if (diff < 0 ) diff = -diff;
    return (unsigned)diff;
}
unsigned dsp_abs_diff_int16 ( int16_t B, const int B_exp, dsp_float_t f,  int *error){
    int16_t v = dsp_conv_float_to_int16(f, B_exp, error);
    int diff = v - B;
    if (diff < 0 ) diff = -diff;
    return (unsigned)diff;
}

unsigned dsp_abs_diff_int32 ( int32_t B, const int B_exp, dsp_float_t f, int *error){
    int32_t v = dsp_conv_float_to_int32(f, B_exp, error);
    int diff = v - B;
    if (diff < 0 ) diff = -diff;
    return (unsigned)diff;
}

unsigned dsp_abs_diff_uint8(uint8_t B, const int B_exp, dsp_float_t f, int *error){
    uint8_t v = dsp_conv_float_to_uint8(f, B_exp, error);
    int diff = v - B;
    if (diff < 0 ) diff = -diff;
    return (unsigned)diff;
}
unsigned dsp_abs_diff_uint16(uint16_t B, const int B_exp, dsp_float_t f, int *error){
    uint16_t v = dsp_conv_float_to_uint16(f, B_exp, error);
    int diff = v - B;
    if (diff < 0 ) diff = -diff;
    return (unsigned)diff;
}

unsigned dsp_abs_diff_uint32(uint32_t B, const int B_exp, dsp_float_t f, int *error){
    if ((B == 0) && (f != 0.0)){
        *error = 1;
        return 0;
    }
    uint32_t v = dsp_conv_float_to_uint32(f, B_exp, error);
    int diff = v - B;
    if (diff < 0 ) diff = -diff;
    return (unsigned)diff;
}


/*
 * Float/Fixed vector comparision
 */
unsigned dsp_abs_diff_vect_ch_pair_int16(dsp_ch_pair_int16_t * B, const int B_exp,
        dsp_ch_pair_float_t * f, unsigned length, int channel_index, int *error){
    unsigned max_diff = 0;

    for(unsigned i=0;i<length;i++){
        unsigned diff = dsp_abs_diff_ch_pair_int16(B[i], B_exp, f[i], channel_index, error);
        if( diff > max_diff) {
            max_diff = diff;
        }
    }
    return max_diff;
}
unsigned dsp_abs_diff_vect_ch_pair_int32(dsp_ch_pair_int32_t * B, const int B_exp,
        dsp_ch_pair_float_t * f, unsigned length, int channel_index, int * error){
    unsigned max_diff = 0;

    for(unsigned i=0;i<length;i++){
        unsigned diff = dsp_abs_diff_ch_pair_int32(B[i], B_exp, f[i], channel_index, error);
        if( diff > max_diff) {
            max_diff = diff;
        }
    }
    return max_diff;
}
unsigned dsp_abs_diff_vect_complex_int16(dsp_complex_int16_t * B, const int B_exp,
        dsp_complex_float_t * f, unsigned length, int * error){
    unsigned max_diff = 0;

    for(unsigned i=0;i<length;i++){
        unsigned diff = dsp_abs_diff_complex_int16(B[i], B_exp, f[i], error);
        if( diff > max_diff) {
            max_diff = diff;
        }
    }
    return max_diff;
}
unsigned dsp_abs_diff_vect_complex_int32(dsp_complex_int32_t * B, const int B_exp,
        dsp_complex_float_t * f, unsigned length, int * error){
    unsigned max_diff = 0;

    for(unsigned i=0;i<length;i++){
        unsigned diff = dsp_abs_diff_complex_int32(B[i], B_exp, f[i], error);
        if( diff > max_diff) {
            max_diff = diff;
        }
    }
    return max_diff;
}
unsigned dsp_abs_diff_vect_int8 ( int8_t * B, const int B_exp, dsp_float_t * f, unsigned length,
        int * error){
    unsigned max_diff = 0;

    for(unsigned i=0;i<length;i++){
        unsigned diff = dsp_abs_diff_int8(B[i], B_exp, f[i], error);
        if( diff > max_diff) {
            max_diff = diff;
        }
    }
    return max_diff;
}
unsigned dsp_abs_diff_vect_int16 ( int16_t * B, const int B_exp, dsp_float_t * f, unsigned length,
        int * error){
    unsigned max_diff = 0;

    for(unsigned i=0;i<length;i++){
        unsigned diff = dsp_abs_diff_int16(B[i], B_exp, f[i], error);
        if( diff > max_diff) {
            max_diff = diff;
        }
    }
    return max_diff;
}
unsigned dsp_abs_diff_vect_int32 ( int32_t * B, const int B_exp, dsp_float_t * f, unsigned length,
        int * error){
    unsigned max_diff = 0;

    for(unsigned i=0;i<length;i++){
        unsigned diff = dsp_abs_diff_int32(B[i], B_exp, f[i], error);
        if( diff > max_diff) {
            max_diff = diff;
        }
    }
    return max_diff;
}
unsigned dsp_abs_diff_vect_uint8(uint8_t * B, const int B_exp, dsp_float_t * f, unsigned length,
        int * error){
    unsigned max_diff = 0;

    for(unsigned i=0;i<length;i++){
        unsigned diff = dsp_abs_diff_uint8(B[i], B_exp, f[i], error);
        if( diff > max_diff) {
            max_diff = diff;
        }
    }
    return max_diff;
}
unsigned dsp_abs_diff_vect_uint16(uint16_t * B, const int B_exp, dsp_float_t * f, unsigned length,
        int * error){
    unsigned max_diff = 0;

    for(unsigned i=0;i<length;i++){
        unsigned diff = dsp_abs_diff_uint16(B[i], B_exp, f[i], error);
        if( diff > max_diff) {
            max_diff = diff;
        }
    }
    return max_diff;
}
unsigned dsp_abs_diff_vect_uint32(uint32_t * B, const int B_exp, dsp_float_t * f, unsigned length,
        int * error){
    unsigned max_diff = 0;

    for(unsigned i=0;i<length;i++){
        unsigned diff = dsp_abs_diff_uint32(B[i], B_exp, f[i], error);
        if( diff > max_diff) {
            max_diff = diff;
        }
    }
    return max_diff;
}
