// Copyright (c) 2019, XMOS Ltd, All rights reserved

#ifndef DSP_DEBUG_H_
#define DSP_DEBUG_H_

#include "dsp_complex.h"

/*
 * Random number generation
 */
int16_t  dsp_rand_int16(unsigned *r);
uint16_t dsp_rand_uint16(unsigned *r);
int32_t  dsp_rand_int32(unsigned *r);
uint32_t dsp_rand_uint32(unsigned *r);
int64_t  dsp_rand_int64(unsigned *r);
uint64_t dsp_rand_uint64(unsigned *r);
//TODO complex

/*
 * Type conversion
 */
typedef enum {

    //In trying to convert to a integer type the exponent of the float forces
    //the mantissa of the integer type to not fit in its storage type.
    CANNOT_FIT_MANTISSA=1,

    //In trying to convert to a unsinged type the float contains a negative number.
    NEGATIVE_TO_UNSIGNED=2,

    //In trying to convert to a float type the exponent of the integer is out of range.
    CANNOT_FIT_EXPONENT=4,

} dsp_conv_error_e;

dsp_float_t dsp_conv_int16_to_float (const int16_t x,  const int x_exp, int *error);
dsp_float_t dsp_conv_uint16_to_float(const uint16_t x, const int x_exp, int *error);
dsp_float_t dsp_conv_int32_to_float (const int32_t x,  const int x_exp, int *error);
dsp_float_t dsp_conv_uint32_to_float(const uint32_t x, const int x_exp, int *error);
dsp_float_t dsp_conv_int64_to_float (const int64_t x,  const int x_exp, int *error);
dsp_float_t dsp_conv_uint64_to_float(const uint64_t x, const int x_exp, int *error);

int16_t  dsp_conv_float_to_int16 (dsp_float_t d, const int d_exp, int *error);
uint16_t dsp_conv_float_to_uint16(dsp_float_t d, const int d_exp, int *error);
int32_t  dsp_conv_float_to_int32 (dsp_float_t d, const int d_exp, int *error);
uint32_t dsp_conv_float_to_uint32(dsp_float_t d, const int d_exp, int *error);
int64_t  dsp_conv_float_to_int64 (dsp_float_t d, const int d_exp, int *error);
uint64_t dsp_conv_float_to_uint64(dsp_float_t d, const int d_exp, int *error);

dsp_complex_float_t dsp_conv_complex_int16_to_complex_float(dsp_complex_int16_t x, const int x_exp, int *error);
dsp_complex_float_t dsp_conv_complex_int32_to_complex_float(dsp_complex_int32_t x, const int x_exp, int *error);

dsp_ch_pair_float_t dsp_conv_ch_pair_int16_to_ch_pair_float(dsp_ch_pair_int16_t x, const int x_exp, int *error);
dsp_ch_pair_float_t dsp_conv_ch_pair_int32_to_ch_pair_float(dsp_ch_pair_int32_t x, const int x_exp, int *error);

dsp_complex_int16_t dsp_conv_complex_float_to_complex_int16(dsp_complex_float_t x, const int x_exp, int *error);
dsp_complex_int32_t dsp_conv_complex_float_to_complex_int32(dsp_complex_float_t x, const int x_exp, int *error);

dsp_ch_pair_int16_t dsp_conv_ch_pair_float_to_ch_pair_int16(dsp_ch_pair_float_t x, const int x_exp, int *error);
dsp_ch_pair_int32_t dsp_conv_ch_pair_float_to_ch_pair_int32(dsp_ch_pair_float_t x, const int x_exp, int *error);

/*
 * Vector conversion
 */

void dsp_conv_vect_int16_to_float (const int16_t *x,  const int x_exp, dsp_float_t * f, unsigned length, int *error);
void dsp_conv_vect_uint16_to_float(const uint16_t *x, const int x_exp, dsp_float_t * f, unsigned length, int *error);
void dsp_conv_vect_int32_to_float (const int32_t *x,  const int x_exp, dsp_float_t * f, unsigned length, int *error);
void dsp_conv_vect_uint32_to_float(const uint32_t *x, const int x_exp, dsp_float_t * f, unsigned length, int *error);
void dsp_conv_vect_int64_to_float (const int64_t *x,  const int x_exp, dsp_float_t * f, unsigned length, int *error);
void dsp_conv_vect_uint64_to_float(const uint64_t *x, const int x_exp, dsp_float_t * f, unsigned length, int *error);

void dsp_conv_vect_float_to_int16 (dsp_float_t *f, int16_t *d, int *d_exp, unsigned length, int *error);
void dsp_conv_vect_float_to_uint16 (dsp_float_t *f, uint16_t *d, int *d_exp, unsigned length, int *error);
void dsp_conv_vect_float_to_int32 (dsp_float_t *f, int32_t *d, int *d_exp, unsigned length, int *error);
void dsp_conv_vect_float_to_uint32 (dsp_float_t *f, uint32_t *d, int *d_exp, unsigned length, int *error);
void dsp_conv_vect_float_to_int64 (dsp_float_t *f, int64_t *d, int *d_exp, unsigned length, int *error);
void dsp_conv_vect_float_to_uint64 (dsp_float_t *f, uint64_t *d, int *d_exp, unsigned length, int *error);


void dsp_conv_vect_complex_int16_to_complex_float(dsp_complex_int16_t * d, const int d_exp, dsp_complex_float_t *f,
        unsigned length, int *error);
void dsp_conv_vect_complex_int32_to_complex_float(dsp_complex_int32_t * d, const int d_exp, dsp_complex_float_t *f,
        unsigned length, int *error);

void dsp_conv_vect_ch_pair_int16_to_ch_pair_float(dsp_ch_pair_int16_t * x, const int x_exp, dsp_ch_pair_float_t *f,
        unsigned length, int *error);
void dsp_conv_vect_ch_pair_int32_to_ch_pair_float(dsp_ch_pair_int32_t * x, const int x_exp, dsp_ch_pair_float_t *f,
        unsigned length, int *error);

void dsp_conv_vect_complex_float_to_complex_int16( dsp_complex_float_t *f, dsp_complex_int16_t * d, int * d_exp,
        unsigned length, int *error);
void dsp_conv_vect_complex_float_to_complex_int32( dsp_complex_float_t *f, dsp_complex_int32_t * d, int * d_exp,
        unsigned length, int *error);

void dsp_conv_vect_ch_pair_float_to_ch_pair_int16(dsp_ch_pair_float_t *f, dsp_ch_pair_int16_t * d, int * d_exp,
        unsigned length, int *error);
void dsp_conv_vect_ch_pair_float_to_ch_pair_int32(dsp_ch_pair_float_t *f, dsp_ch_pair_int32_t * d, int * d_exp,
        unsigned length, int *error);


/*
 * Float/Fixed comparision
 */
unsigned dsp_abs_diff_ch_pair_int16(dsp_ch_pair_int16_t B, const int B_exp,
        dsp_ch_pair_float_t f, int channel_index, int *error);
unsigned dsp_abs_diff_ch_pair_int32(dsp_ch_pair_int32_t B, const int B_exp,
        dsp_ch_pair_float_t f, int channel_index, int *error);
unsigned dsp_abs_diff_complex_int16(dsp_complex_int16_t B, const int B_exp, dsp_complex_float_t f, int *error);
unsigned dsp_abs_diff_complex_int32(dsp_complex_int32_t B, const int B_exp, dsp_complex_float_t f, int *error);
unsigned dsp_abs_diff_int16 ( int16_t B, const int B_exp, dsp_float_t f, int *error);
unsigned dsp_abs_diff_int32 ( int32_t B, const int B_exp, dsp_float_t f, int *error);
unsigned dsp_abs_diff_uint16(uint16_t B, const int B_exp, dsp_float_t f, int *error);
unsigned dsp_abs_diff_uint32(uint32_t B, const int B_exp, dsp_float_t f, int *error);


/*
 * Float/Fixed vector comparision
 */
unsigned dsp_abs_diff_vect_ch_pair_int16(dsp_ch_pair_int16_t * B, const int B_exp,
        dsp_ch_pair_float_t * f, unsigned length, int channel_index, int *error);
unsigned dsp_abs_diff_vect_ch_pair_int32(dsp_ch_pair_int32_t * B, const int B_exp,
        dsp_ch_pair_float_t * f, unsigned length, int channel_index, int *error);
unsigned dsp_abs_diff_vect_complex_int16(dsp_complex_int16_t * B, const int B_exp,
        dsp_complex_float_t * f, unsigned length, int *error);
unsigned dsp_abs_diff_vect_complex_int32(dsp_complex_int32_t * B, const int B_exp,
        dsp_complex_float_t * f, unsigned length, int *error);
unsigned dsp_abs_diff_vect_int16 ( int16_t * B, const int B_exp, dsp_float_t * f, unsigned length, int *error);
unsigned dsp_abs_diff_vect_int32 ( int32_t * B, const int B_exp, dsp_float_t * f, unsigned length, int *error);
unsigned dsp_abs_diff_vect_uint16(uint16_t * B, const int B_exp, dsp_float_t * f, unsigned length, int *error);
unsigned dsp_abs_diff_vect_uint32(uint32_t * B, const int B_exp, dsp_float_t * f, unsigned length, int *error);

/*
 * Python pretty printers
 */

void dsp_print_vect_ch_pair_int16(dsp_ch_pair_int16_t * B, const int B_exp, unsigned length, int channel_index, int * error);
void dsp_print_vect_ch_pair_int32(dsp_ch_pair_int32_t * B, const int B_exp, unsigned length, int channel_index, int * error);
void dsp_print_vect_ch_pair_float(dsp_ch_pair_float_t * B, unsigned length, int channel_index, int * error);

void dsp_print_vect_complex_int16(dsp_complex_int16_t * B, const int B_exp, unsigned length, int * error);
void dsp_print_vect_complex_int32(dsp_complex_int32_t * B, const int B_exp, unsigned length, int * error);
void dsp_print_vect_complex_float(dsp_complex_float_t * B, unsigned length, int * error);

//For DC and NQ in bin zero
void dsp_print_vect_complex_int16_fft(dsp_complex_int16_t * B, const int B_exp, unsigned length, int * error);
void dsp_print_vect_complex_int32_fft(dsp_complex_int32_t * B, const int B_exp, unsigned length, int * error);
void dsp_print_vect_complex_float_fft(dsp_complex_float_t * B, unsigned length, int * error);

void dsp_print_vect_int16 ( int16_t * B, const int B_exp, unsigned length, int * error);
void dsp_print_vect_int32 ( int32_t * B, const int B_exp, unsigned length, int * error);
void dsp_print_vect_int64 ( int64_t * B, const int B_exp, unsigned length, int * error);
void dsp_print_vect_uint16(uint16_t * B, const int B_exp, unsigned length, int * error);
void dsp_print_vect_uint32(uint32_t * B, const int B_exp, unsigned length, int * error);
void dsp_print_vect_uint64(uint64_t * B, const int B_exp, unsigned length, int * error);
void dsp_print_vect_float (dsp_float_t * B, unsigned length, int * error);








#endif /* DSP_DEBUG_H_ */
