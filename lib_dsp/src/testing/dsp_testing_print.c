// Copyright 2019-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include "dsp_testing.h"
#include <stdio.h>

void dsp_print_vect_ch_pair_int16(dsp_ch_pair_int16_t * B, const int B_exp, unsigned length,
        int channel_index, int * error){
    printf("np.asarray([");
    for(unsigned i=0;i<length;i++)
        printf("%.*f, ", DSP_FLOAT_NUM_DIGITS, dsp_conv_int16_to_float( ((int16_t*)&B[i])[channel_index], B_exp, error));
    printf("])\n");
}

void dsp_print_vect_ch_pair_int32(dsp_ch_pair_int32_t * B, const int B_exp, unsigned length,
        int channel_index, int * error){
    printf("np.asarray([");
    for(unsigned i=0;i<length;i++)
        printf("%.*f, ", DSP_FLOAT_NUM_DIGITS, dsp_conv_int32_to_float( ((int32_t*)&B[i])[channel_index], B_exp, error));
    printf("])\n");
}

void dsp_print_vect_ch_pair_float(dsp_ch_pair_float_t * B, unsigned length, int channel_index,
        int * error){
    printf("np.asarray([");
    for(unsigned i=0;i<length;i++)
        printf("%.*f, ", DSP_FLOAT_NUM_DIGITS, ((dsp_float_t*)&B[i])[channel_index]);
    printf("])\n");
}

void dsp_print_vect_complex_int16(dsp_complex_int16_t * B, const int B_exp, unsigned length,
        int * error){
    printf("np.asarray([");
    for(unsigned i=0;i<length;i++){
        dsp_float_t re = dsp_conv_int16_to_float( B[i].re, B_exp, error);
        dsp_float_t im = dsp_conv_int16_to_float( B[i].im, B_exp, error);
        printf("%.*f + %.*fj, ", DSP_FLOAT_NUM_DIGITS, re, DSP_FLOAT_NUM_DIGITS, im);
    }
    printf("])\n");
}

void dsp_print_vect_complex_int16_fft(dsp_complex_int16_t * B, const int B_exp, unsigned length,
        int * error){
    printf("np.asarray([%.*f, ", DSP_FLOAT_NUM_DIGITS, dsp_conv_int16_to_float( B[0].re, B_exp, error));
    for(unsigned i=1;i<length;i++){
        dsp_float_t re = dsp_conv_int16_to_float( B[i].re, B_exp, error);
        dsp_float_t im = dsp_conv_int16_to_float( B[i].im, B_exp, error);
        printf("%.*f + %.*fj, ", DSP_FLOAT_NUM_DIGITS, re, DSP_FLOAT_NUM_DIGITS, im);
    }
    printf("%.*f])\n", DSP_FLOAT_NUM_DIGITS, dsp_conv_int16_to_float( B[0].im, B_exp, error));
}

void dsp_print_vect_complex_int32(dsp_complex_int32_t * B, const int B_exp, unsigned length,
        int * error){
    printf("np.asarray([");
    for(unsigned i=0;i<length;i++){
        dsp_float_t re = dsp_conv_int32_to_float( B[i].re, B_exp, error);
        dsp_float_t im = dsp_conv_int32_to_float( B[i].im, B_exp, error);
        printf("%.*f + %.*fj, ", DSP_FLOAT_NUM_DIGITS, re, DSP_FLOAT_NUM_DIGITS, im);
    }
    printf("])\n");
}

void dsp_print_vect_complex_int32_fft(dsp_complex_int32_t * B, const int B_exp, unsigned length,
        int * error){
    printf("np.asarray([%.*f, ", DSP_FLOAT_NUM_DIGITS, dsp_conv_int32_to_float( B[0].re, B_exp, error));
    for(unsigned i=1;i<length;i++){
        dsp_float_t re = dsp_conv_int32_to_float( B[i].re, B_exp, error);
        dsp_float_t im = dsp_conv_int32_to_float( B[i].im, B_exp, error);
        printf("%.*f + %.*fj, ", DSP_FLOAT_NUM_DIGITS, re, DSP_FLOAT_NUM_DIGITS, im);
    }
    printf("%.*f])\n", DSP_FLOAT_NUM_DIGITS, dsp_conv_int32_to_float(B[0].im, B_exp, error));
}

void dsp_print_vect_complex_float(dsp_complex_float_t * B, unsigned length, int * error){
    printf("np.asarray([");
    for(unsigned i=0;i<length;i++){
        printf("%.*f + %.*fj, ", DSP_FLOAT_NUM_DIGITS, B[i].re, DSP_FLOAT_NUM_DIGITS, B[i].im);
    }
    printf("])\n");
}

void dsp_print_vect_complex_float_fft(dsp_complex_float_t * B, unsigned length, int * error){
    printf("np.asarray([%.*f, ", DSP_FLOAT_NUM_DIGITS, B[0].re);
    for(unsigned i=1;i<length;i++){
        printf("%.*f + %.*fj, ", DSP_FLOAT_NUM_DIGITS, B[i].re, DSP_FLOAT_NUM_DIGITS, B[i].im);
    }
    printf("%.*f])\n", DSP_FLOAT_NUM_DIGITS, B[0].im);
}

void dsp_print_vect_int8 ( int8_t * B, const int B_exp, unsigned length, int * error){
    printf("np.asarray([");
    for(unsigned i=0;i<length;i++)
        printf("%.*f, ", DSP_FLOAT_NUM_DIGITS, dsp_conv_int8_to_float( B[i], B_exp, error));
    printf("])\n");
}

void dsp_print_vect_int16 ( int16_t * B, const int B_exp, unsigned length, int * error){
    printf("np.asarray([");
    for(unsigned i=0;i<length;i++)
        printf("%.*f, ", DSP_FLOAT_NUM_DIGITS, dsp_conv_int16_to_float( B[i], B_exp, error));
    printf("])\n");
}

void dsp_print_vect_int32 ( int32_t * B, const int B_exp, unsigned length, int * error){
    printf("np.asarray([");
    for(unsigned i=0;i<length;i++)
        printf("%.*f, ", DSP_FLOAT_NUM_DIGITS, dsp_conv_int32_to_float(B[i], B_exp, error));
    printf("])\n");
}

void dsp_print_vect_int64 ( int64_t * B, const int B_exp, unsigned length, int * error){
    printf("np.asarray([");
    for(unsigned i=0;i<length;i++)
        printf("%.*f, ", DSP_FLOAT_NUM_DIGITS, dsp_conv_int64_to_float(B[i], B_exp, error));
    printf("])\n");
}

void dsp_print_vect_uint8(uint8_t * B, const int B_exp, unsigned length, int * error){
    printf("np.asarray([");
    for(unsigned i=0;i<length;i++)
        printf("%.*f, ", DSP_FLOAT_NUM_DIGITS, dsp_conv_uint8_to_float(B[i], B_exp, error));
    printf("])\n");
}

void dsp_print_vect_uint16(uint16_t * B, const int B_exp, unsigned length, int * error){
    printf("np.asarray([");
    for(unsigned i=0;i<length;i++)
        printf("%.*f, ", DSP_FLOAT_NUM_DIGITS, dsp_conv_uint16_to_float(B[i], B_exp, error));
    printf("])\n");
}

void dsp_print_vect_uint32(uint32_t * B, const int B_exp, unsigned length, int * error){
    printf("np.asarray([");
    for(unsigned i=0;i<length;i++)
        printf("%.*f, ", DSP_FLOAT_NUM_DIGITS, dsp_conv_uint32_to_float(B[i], B_exp, error));
    printf("])\n");
}

void dsp_print_vect_uint64(uint64_t * B, const int B_exp, unsigned length, int * error){
    printf("np.asarray([");
    for(unsigned i=0;i<length;i++)
        printf("%.*f, ", DSP_FLOAT_NUM_DIGITS, dsp_conv_uint64_to_float(B[i], B_exp, error));
    printf("])\n");
}

void dsp_print_vect_float (dsp_float_t * B, unsigned length, int * error){
    printf("np.asarray([");
    for(unsigned i=0;i<length;i++)
        printf("%.*f, ", DSP_FLOAT_NUM_DIGITS, B[i]);
    printf("])\n");
}

