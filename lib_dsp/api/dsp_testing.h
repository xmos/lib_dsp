// Copyright 2019-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#ifndef DSP_DEBUG_H_
#define DSP_DEBUG_H_
#ifdef __XC__
#define UNSAFE unsafe
#else
#define UNSAFE
#endif //__XC_
#include "dsp_complex.h"

#define DSP_FLOAT_NUM_DIGITS 22

/*
 * Random number generation
 */

/**
 * Generates a pseudorandom number based on a 32 bit CRC. Requires a seed
 * which it will modify.
 *
 * \param[in]   r               Seed.
 *
 * \returns                     Pseudorandom value of the type given in the function name.
 */
int8_t  dsp_pseudo_rand_int8(unsigned * UNSAFE r);
uint8_t dsp_pseudo_rand_uint8(unsigned * UNSAFE r);
int16_t  dsp_pseudo_rand_int16(unsigned * UNSAFE r);
uint16_t dsp_pseudo_rand_uint16(unsigned * UNSAFE r);
int32_t  dsp_pseudo_rand_int32(unsigned * UNSAFE r);
uint32_t dsp_pseudo_rand_uint32(unsigned * UNSAFE r);
int64_t  dsp_pseudo_rand_int64(unsigned * UNSAFE r);
uint64_t dsp_pseudo_rand_uint64(unsigned * UNSAFE r);
dsp_float_t dsp_pseudo_rand_float(unsigned * UNSAFE r);
dsp_complex_int16_t dsp_pseudo_rand_complex_int16(unsigned * UNSAFE r);
dsp_complex_int32_t dsp_pseudo_rand_complex_int32(unsigned * UNSAFE r);
dsp_complex_float_t dsp_pseudo_rand_complex_float(unsigned * UNSAFE r);
dsp_ch_pair_int16_t dsp_pseudo_rand_ch_pair_int16(unsigned * UNSAFE r);
dsp_ch_pair_int32_t dsp_pseudo_rand_ch_pair_int32(unsigned * UNSAFE r);
dsp_ch_pair_float_t dsp_pseudo_rand_ch_pair_float(unsigned * UNSAFE r);

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


/**
 * Converts integer types with an exponent into the type given by dsp_float_t. Any
 * errors during the conversion will be added into the error flags. The error flags
 * must be initialised to zero before calling in order to detect errors.
 *
 * \param[in]   x               The mantissa of x.
 *
 * \param[in]   x_exp           The exponent of x.
 *
 * \param[in]   error           Pointer to where the error flags will be collected.
 *
 * \returns                     The floating point representation of x.
 */
dsp_float_t dsp_conv_int8_to_float (const int8_t x,  const int x_exp, int * UNSAFE error);
dsp_float_t dsp_conv_uint8_to_float(const uint8_t x, const int x_exp, int * UNSAFE error);
dsp_float_t dsp_conv_int16_to_float (const int16_t x,  const int x_exp, int * UNSAFE error);
dsp_float_t dsp_conv_uint16_to_float(const uint16_t x, const int x_exp, int * UNSAFE error);
dsp_float_t dsp_conv_int32_to_float (const int32_t x,  const int x_exp, int * UNSAFE error);
dsp_float_t dsp_conv_uint32_to_float(const uint32_t x, const int x_exp, int * UNSAFE error);
dsp_float_t dsp_conv_int64_to_float (const int64_t x,  const int x_exp, int * UNSAFE error);
dsp_float_t dsp_conv_uint64_to_float(const uint64_t x, const int x_exp, int * UNSAFE error);

int8_t  dsp_conv_float_to_int8 (dsp_float_t d, const int d_exp, int * UNSAFE error);
uint8_t dsp_conv_float_to_uint8(dsp_float_t d, const int d_exp, int * UNSAFE error);
int16_t  dsp_conv_float_to_int16 (dsp_float_t d, const int d_exp, int * UNSAFE error);
uint16_t dsp_conv_float_to_uint16(dsp_float_t d, const int d_exp, int * UNSAFE error);
int32_t  dsp_conv_float_to_int32 (dsp_float_t d, const int d_exp, int * UNSAFE error);
uint32_t dsp_conv_float_to_uint32(dsp_float_t d, const int d_exp, int * UNSAFE error);
int64_t  dsp_conv_float_to_int64 (dsp_float_t d, const int d_exp, int * UNSAFE error);
uint64_t dsp_conv_float_to_uint64(dsp_float_t d, const int d_exp, int * UNSAFE error);

dsp_complex_float_t dsp_conv_complex_int16_to_complex_float(dsp_complex_int16_t x, const int x_exp, int * UNSAFE error);
dsp_complex_float_t dsp_conv_complex_int32_to_complex_float(dsp_complex_int32_t x, const int x_exp, int * UNSAFE error);

dsp_ch_pair_float_t dsp_conv_ch_pair_int16_to_ch_pair_float(dsp_ch_pair_int16_t x, const int x_exp, int * UNSAFE error);
dsp_ch_pair_float_t dsp_conv_ch_pair_int32_to_ch_pair_float(dsp_ch_pair_int32_t x, const int x_exp, int * UNSAFE error);

dsp_complex_int16_t dsp_conv_complex_float_to_complex_int16(dsp_complex_float_t x, const int x_exp, int * UNSAFE error);
dsp_complex_int32_t dsp_conv_complex_float_to_complex_int32(dsp_complex_float_t x, const int x_exp, int * UNSAFE error);

dsp_ch_pair_int16_t dsp_conv_ch_pair_float_to_ch_pair_int16(dsp_ch_pair_float_t x, const int x_exp, int * UNSAFE error);
dsp_ch_pair_int32_t dsp_conv_ch_pair_float_to_ch_pair_int32(dsp_ch_pair_float_t x, const int x_exp, int * UNSAFE error);

/*
 * Vector conversion
 */

void dsp_conv_vect_int8_to_float (const int8_t * UNSAFE x,  const int x_exp, dsp_float_t *  UNSAFE f, unsigned length, int * UNSAFE error);
void dsp_conv_vect_uint8_to_float(const uint8_t * UNSAFE x, const int x_exp, dsp_float_t *  UNSAFE f, unsigned length, int * UNSAFE error);
void dsp_conv_vect_int16_to_float (const int16_t * UNSAFE x,  const int x_exp, dsp_float_t *  UNSAFE f, unsigned length, int * UNSAFE error);
void dsp_conv_vect_uint16_to_float(const uint16_t * UNSAFE x, const int x_exp, dsp_float_t *  UNSAFE f, unsigned length, int * UNSAFE error);
void dsp_conv_vect_int32_to_float (const int32_t * UNSAFE x,  const int x_exp, dsp_float_t *  UNSAFE f, unsigned length, int * UNSAFE error);
void dsp_conv_vect_uint32_to_float(const uint32_t * UNSAFE x, const int x_exp, dsp_float_t *  UNSAFE f, unsigned length, int * UNSAFE error);
void dsp_conv_vect_int64_to_float (const int64_t * UNSAFE x,  const int x_exp, dsp_float_t *  UNSAFE f, unsigned length, int * UNSAFE error);
void dsp_conv_vect_uint64_to_float(const uint64_t * UNSAFE x, const int x_exp, dsp_float_t *  UNSAFE f, unsigned length, int * UNSAFE error);

void dsp_conv_vect_float_to_int8 (dsp_float_t * UNSAFE f, int8_t * UNSAFE d, int * UNSAFE d_exp, unsigned length, int * UNSAFE error);
void dsp_conv_vect_float_to_uint8 (dsp_float_t * UNSAFE f, uint8_t * UNSAFE d, int * UNSAFE d_exp, unsigned length, int * UNSAFE error);
void dsp_conv_vect_float_to_int16 (dsp_float_t * UNSAFE f, int16_t * UNSAFE d, int * UNSAFE d_exp, unsigned length, int * UNSAFE error);
void dsp_conv_vect_float_to_uint16 (dsp_float_t * UNSAFE f, uint16_t * UNSAFE d, int * UNSAFE d_exp, unsigned length, int * UNSAFE error);
void dsp_conv_vect_float_to_int32 (dsp_float_t * UNSAFE f, int32_t * UNSAFE d, int * UNSAFE d_exp, unsigned length, int * UNSAFE error);
void dsp_conv_vect_float_to_uint32 (dsp_float_t * UNSAFE f, uint32_t * UNSAFE d, int * UNSAFE d_exp, unsigned length, int * UNSAFE error);
void dsp_conv_vect_float_to_int64 (dsp_float_t * UNSAFE f, int64_t * UNSAFE d, int * UNSAFE d_exp, unsigned length, int * UNSAFE error);
void dsp_conv_vect_float_to_uint64 (dsp_float_t * UNSAFE f, uint64_t * UNSAFE d, int * UNSAFE d_exp, unsigned length, int * UNSAFE error);


void dsp_conv_vect_complex_int16_to_complex_float(dsp_complex_int16_t *  UNSAFE d, const int d_exp, dsp_complex_float_t * UNSAFE f,
        unsigned length, int * UNSAFE error);
void dsp_conv_vect_complex_int32_to_complex_float(dsp_complex_int32_t *  UNSAFE d, const int d_exp, dsp_complex_float_t * UNSAFE f,
        unsigned length, int * UNSAFE error);

void dsp_conv_vect_ch_pair_int16_to_ch_pair_float(dsp_ch_pair_int16_t *  UNSAFE x, const int x_exp, dsp_ch_pair_float_t * UNSAFE f,
        unsigned length, int * UNSAFE error);
void dsp_conv_vect_ch_pair_int32_to_ch_pair_float(dsp_ch_pair_int32_t *  UNSAFE x, const int x_exp, dsp_ch_pair_float_t * UNSAFE f,
        unsigned length, int * UNSAFE error);

void dsp_conv_vect_complex_float_to_complex_int16( dsp_complex_float_t * UNSAFE f, dsp_complex_int16_t * UNSAFE  d, int * UNSAFE  d_exp,
        unsigned length, int * UNSAFE error);
void dsp_conv_vect_complex_float_to_complex_int32( dsp_complex_float_t * UNSAFE f, dsp_complex_int32_t * UNSAFE  d, int * UNSAFE  d_exp,
        unsigned length, int * UNSAFE error);

void dsp_conv_vect_ch_pair_float_to_ch_pair_int16(dsp_ch_pair_float_t * UNSAFE f, dsp_ch_pair_int16_t *  UNSAFE d, int *  UNSAFE d_exp,
        unsigned length, int * UNSAFE error);
void dsp_conv_vect_ch_pair_float_to_ch_pair_int32(dsp_ch_pair_float_t * UNSAFE f, dsp_ch_pair_int32_t *  UNSAFE d, int *  UNSAFE d_exp,
        unsigned length, int * UNSAFE error);


/*
 * Float/Fixed comparision
 */
unsigned dsp_abs_diff_ch_pair_int16(dsp_ch_pair_int16_t B, const int B_exp,
        dsp_ch_pair_float_t f, int channel_index, int * UNSAFE error);
unsigned dsp_abs_diff_ch_pair_int32(dsp_ch_pair_int32_t B, const int B_exp,
        dsp_ch_pair_float_t f, int channel_index, int * UNSAFE error);
unsigned dsp_abs_diff_complex_int16(dsp_complex_int16_t B, const int B_exp, dsp_complex_float_t f, int * UNSAFE error);
unsigned dsp_abs_diff_complex_int32(dsp_complex_int32_t B, const int B_exp, dsp_complex_float_t f, int * UNSAFE error);
unsigned dsp_abs_diff_int8  (  int8_t B, const int B_exp, dsp_float_t f, int * UNSAFE error);
unsigned dsp_abs_diff_int16 ( int16_t B, const int B_exp, dsp_float_t f, int * UNSAFE error);
unsigned dsp_abs_diff_int32 ( int32_t B, const int B_exp, dsp_float_t f, int * UNSAFE error);
unsigned dsp_abs_diff_uint8 ( uint8_t B, const int B_exp, dsp_float_t f, int * UNSAFE error);
unsigned dsp_abs_diff_uint16(uint16_t B, const int B_exp, dsp_float_t f, int * UNSAFE error);
unsigned dsp_abs_diff_uint32(uint32_t B, const int B_exp, dsp_float_t f, int * UNSAFE error);


/*
 * Float/Fixed vector comparision
 */
unsigned dsp_abs_diff_vect_ch_pair_int16(dsp_ch_pair_int16_t * UNSAFE B, const int B_exp,
        dsp_ch_pair_float_t * UNSAFE f, unsigned length, int channel_index, int * UNSAFE error);
unsigned dsp_abs_diff_vect_ch_pair_int32(dsp_ch_pair_int32_t * UNSAFE B, const int B_exp,
        dsp_ch_pair_float_t * UNSAFE f, unsigned length, int channel_index, int * UNSAFE error);
unsigned dsp_abs_diff_vect_complex_int16(dsp_complex_int16_t * UNSAFE B, const int B_exp,
        dsp_complex_float_t * UNSAFE f, unsigned length, int * UNSAFE error);
unsigned dsp_abs_diff_vect_complex_int32(dsp_complex_int32_t * UNSAFE B, const int B_exp,
        dsp_complex_float_t * UNSAFE f, unsigned length, int * UNSAFE error);
unsigned dsp_abs_diff_vect_int8  (  int8_t *  UNSAFE B, const int B_exp, dsp_float_t *  UNSAFE f, unsigned length, int * UNSAFE error);
unsigned dsp_abs_diff_vect_int16 ( int16_t *  UNSAFE B, const int B_exp, dsp_float_t *  UNSAFE f, unsigned length, int * UNSAFE error);
unsigned dsp_abs_diff_vect_int32 ( int32_t *  UNSAFE B, const int B_exp, dsp_float_t *  UNSAFE f, unsigned length, int * UNSAFE error);
unsigned dsp_abs_diff_vect_uint8 ( uint8_t *  UNSAFE B, const int B_exp, dsp_float_t *  UNSAFE f, unsigned length, int * UNSAFE error);
unsigned dsp_abs_diff_vect_uint16(uint16_t *  UNSAFE B, const int B_exp, dsp_float_t *  UNSAFE f, unsigned length, int * UNSAFE error);
unsigned dsp_abs_diff_vect_uint32(uint32_t *  UNSAFE B, const int B_exp, dsp_float_t *  UNSAFE f, unsigned length, int * UNSAFE error);

/*
 * Python pretty printers
 */

void dsp_print_vect_ch_pair_int16(dsp_ch_pair_int16_t * UNSAFE B, const int B_exp, unsigned length, int channel_index, int * UNSAFE error);
void dsp_print_vect_ch_pair_int32(dsp_ch_pair_int32_t * UNSAFE B, const int B_exp, unsigned length, int channel_index, int * UNSAFE error);
void dsp_print_vect_ch_pair_float(dsp_ch_pair_float_t * UNSAFE B, unsigned length, int channel_index, int * UNSAFE error);

void dsp_print_vect_complex_int16(dsp_complex_int16_t * UNSAFE B, const int B_exp, unsigned length, int * UNSAFE error);
void dsp_print_vect_complex_int32(dsp_complex_int32_t * UNSAFE B, const int B_exp, unsigned length, int * UNSAFE error);
void dsp_print_vect_complex_float(dsp_complex_float_t * UNSAFE B, unsigned length, int * UNSAFE error);

//For DC and NQ in bin zero
void dsp_print_vect_complex_int16_fft(dsp_complex_int16_t * UNSAFE B, const int B_exp, unsigned length, int * UNSAFE error);
void dsp_print_vect_complex_int32_fft(dsp_complex_int32_t * UNSAFE B, const int B_exp, unsigned length, int * UNSAFE error);
void dsp_print_vect_complex_float_fft(dsp_complex_float_t * UNSAFE B, unsigned length, int * UNSAFE error);

void dsp_print_vect_int8  (  int8_t * UNSAFE B, const int B_exp, unsigned length, int * UNSAFE error);
void dsp_print_vect_int16 ( int16_t * UNSAFE B, const int B_exp, unsigned length, int * UNSAFE error);
void dsp_print_vect_int32 ( int32_t * UNSAFE B, const int B_exp, unsigned length, int * UNSAFE error);
void dsp_print_vect_int64 ( int64_t * UNSAFE B, const int B_exp, unsigned length, int * UNSAFE error);
void dsp_print_vect_uint8 ( uint8_t * UNSAFE B, const int B_exp, unsigned length, int * UNSAFE error);
void dsp_print_vect_uint16(uint16_t * UNSAFE B, const int B_exp, unsigned length, int * UNSAFE error);
void dsp_print_vect_uint32(uint32_t * UNSAFE B, const int B_exp, unsigned length, int * UNSAFE error);
void dsp_print_vect_uint64(uint64_t * UNSAFE B, const int B_exp, unsigned length, int * UNSAFE error);
void dsp_print_vect_float (dsp_float_t * UNSAFE B, unsigned length, int * UNSAFE error);








#endif /* DSP_DEBUG_H_ */
