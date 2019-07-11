// Copyright (c) 2019, XMOS Ltd, All rights reserved

#ifndef DSP_DEBUG_H_
#define DSP_DEBUG_H_

#include "dsp_complex.h"

/*
 * Random number generation
 */
int16_t  dsp_rand_int16(unsigned &r);
uint16_t dsp_rand_uint16(unsigned &r);
int32_t  dsp_rand_int32(unsigned &r);
uint32_t dsp_rand_uint32(unsigned &r);
int64_t  dsp_rand_int64(unsigned &r);
uint64_t dsp_rand_uint64(unsigned &r);
//TODO complex


/*
 * Type conversion
 */
float_t dsp_conv_int16_to_float (const int16_t x,  const int x_exp);
float_t dsp_conv_uint16_to_float(const uint16_t x, const int x_exp);
float_t dsp_conv_int32_to_float (const int32_t x,  const int x_exp);
float_t dsp_conv_uint32_to_float(const uint32_t x, const int x_exp);
float_t dsp_conv_int64_to_float (const int64_t x,  const int x_exp);
float_t dsp_conv_uint64_to_float(const uint64_t x, const int x_exp);

int16_t  dsp_conv_float_to_int16 (float_t d, const int d_exp, int &error);
uint16_t dsp_conv_float_to_uint16(float_t d, const int d_exp, int &error);
int32_t  dsp_conv_float_to_int32 (float_t d, const int d_exp, int &error);
uint32_t dsp_conv_float_to_uint32(float_t d, const int d_exp, int &error);
int64_t  dsp_conv_float_to_int64 (float_t d, const int d_exp, int &error);
uint64_t dsp_conv_float_to_uint64(float_t d, const int d_exp, int &error);


dsp_complex_float_t dsp_conv_complex_int16_to_complex_float(dsp_complex_int16_t x, const int x_exp);
dsp_complex_float_t dsp_conv_complex_int32_to_complex_float(dsp_complex_int32_t x, const int x_exp);
dsp_ch_pair_float_t dsp_conv_ch_pair_int16_to_complex_float(dsp_ch_pair_int16_t x, const int x_exp);
dsp_ch_pair_float_t dsp_conv_ch_pair_int32_to_complex_float(dsp_ch_pair_int32_t x, const int x_exp);

/*
 * Float/Fixed comparision
 */
{unsigned, unsigned} dsp_abs_diff_ch_pair_int16(dsp_ch_pair_int16_t B, const int B_exp,
        dsp_ch_pair_float_t f);
{unsigned, unsigned} dsp_abs_diff_ch_pair_int32(dsp_ch_pair_int32_t B, const int B_exp,
        dsp_ch_pair_float_t f);
unsigned dsp_abs_diff_complex_int16(dsp_complex_int16_t B, const int B_exp, dsp_complex_float_t f);
unsigned dsp_abs_diff_complex_int32(dsp_complex_int32_t B, const int B_exp, dsp_complex_float_t f);
unsigned dsp_abs_diff_int16 ( int16_t B, const int B_exp, float_t f);
unsigned dsp_abs_diff_int32 ( int32_t B, const int B_exp, float_t f);
unsigned dsp_abs_diff_uint16(uint16_t B, const int B_exp, float_t f);
unsigned dsp_abs_diff_uint32(uint32_t B, const int B_exp, float_t f);


/*
 * Float/Fixed vector comparision
 */
{unsigned, unsigned} dsp_abs_diff_vect_ch_pair_int16(dsp_ch_pair_int16_t * B, const int B_exp,
        dsp_ch_pair_float_t * f, unsigned length);
{unsigned, unsigned} dsp_abs_diff_vect_ch_pair_int32(dsp_ch_pair_int32_t * B, const int B_exp,
        dsp_ch_pair_float_t * f, unsigned length);
unsigned dsp_abs_diff_vect_complex_int16(dsp_complex_int16_t * B, const int B_exp,
        dsp_complex_float_t * f, unsigned length);
unsigned dsp_abs_diff_vect_complex_int32(dsp_complex_int32_t * B, const int B_exp,
        dsp_complex_float_t * f, unsigned length);
unsigned dsp_abs_diff_vect_int16 ( int16_t * B, const int B_exp, float_t * f, unsigned length);
unsigned dsp_abs_diff_vect_int32 ( int32_t * B, const int B_exp, float_t * f, unsigned length);
unsigned dsp_abs_diff_vect_uint16(uint16_t * B, const int B_exp, float_t * f, unsigned length);
unsigned dsp_abs_diff_vect_uint32(uint32_t * B, const int B_exp, float_t * f, unsigned length);

/*
 * Python pretty printers
 */

void dsp_print_vect_ch_pair_int16(dsp_ch_pair_int16_t * B, const int B_exp, unsigned length, int print_imag);
void dsp_print_vect_ch_pair_int32(dsp_ch_pair_int32_t * B, const int B_exp, unsigned length, int print_imag);
void dsp_print_vect_ch_pair_float(dsp_ch_pair_float_t * B, unsigned length, int print_imag);

void dsp_print_vect_complex_int16(dsp_complex_int16_t * B, const int B_exp, unsigned length);
void dsp_print_vect_complex_int32(dsp_complex_int32_t * B, const int B_exp, unsigned length);
void dsp_print_vect_complex_float(dsp_complex_float_t * B, unsigned length);

void dsp_print_vect_int16 ( int16_t * B, const int B_exp, unsigned length);
void dsp_print_vect_int32 ( int32_t * B, const int B_exp, unsigned length);
void dsp_print_vect_int32 ( int64_t * B, const int B_exp, unsigned length);
void dsp_print_vect_uint16(uint16_t * B, const int B_exp, unsigned length);
void dsp_print_vect_uint32(uint32_t * B, const int B_exp, unsigned length);
void dsp_print_vect_uint32(uint64_t * B, const int B_exp, unsigned length);
void dsp_print_vect_float (float_t * B,  const int B_exp, unsigned length);








#endif /* DSP_DEBUG_H_ */
