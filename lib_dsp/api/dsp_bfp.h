// Copyright 2016-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#ifndef DSP_BFP_H_
#define DSP_BFP_H_

#include <stdint.h>
#include <dsp_complex.h>

#define DSP_BFP_ZERO_EXP (-1024)

#ifdef __XC__
#define UNSAFE unsafe
#else
#define UNSAFE
#endif //__XC_

/*
 * clz
 */
unsigned dsp_bfp_clz_uint8(const uint8_t d);
unsigned dsp_bfp_clz_uint16(const uint16_t d);
unsigned dsp_bfp_clz_uint32(const uint32_t d);
unsigned dsp_bfp_clz_uint64(const uint64_t d);

/*
 * cls
 */
unsigned dsp_bfp_cls_int8(const int8_t d);
unsigned dsp_bfp_cls_int16(const int16_t d);
unsigned dsp_bfp_cls_int32(const int32_t d);
unsigned dsp_bfp_cls_int64(const int64_t d);
unsigned dsp_bfp_cls_complex_int16(const dsp_complex_int16_t d);
unsigned dsp_bfp_cls_complex_int32(const dsp_complex_int32_t d);
unsigned dsp_bfp_cls_ch_pair_int16(const dsp_ch_pair_int16_t d, const unsigned channel_index);
unsigned dsp_bfp_cls_ch_pair_int32(const dsp_ch_pair_int32_t d, const unsigned channel_index);

/*
 * Vector clz
 */
unsigned dsp_bfp_clz_vect_uint8(const uint8_t * UNSAFE d, const unsigned length);
unsigned dsp_bfp_clz_vect_uint16(const uint16_t * UNSAFE d, const unsigned length);
unsigned dsp_bfp_clz_vect_uint32(const uint32_t * UNSAFE d, const unsigned length);
unsigned dsp_bfp_clz_vect_uint64(const uint64_t * UNSAFE d, const unsigned length);

/*
 * Vector cls
 */
unsigned dsp_bfp_cls_vect_int8(const int8_t * UNSAFE d, const unsigned length);
unsigned dsp_bfp_cls_vect_int16(const int16_t * UNSAFE d, const unsigned length);
unsigned dsp_bfp_cls_vect_int32(const int32_t * UNSAFE d, const unsigned length);
unsigned dsp_bfp_cls_vect_complex_int16(const dsp_complex_int16_t * UNSAFE d, const unsigned length);
unsigned dsp_bfp_cls_vect_complex_int32(const dsp_complex_int32_t * UNSAFE d, const unsigned length);
unsigned dsp_bfp_cls_vect_ch_pair_int16(const dsp_ch_pair_int16_t * UNSAFE d, const unsigned length,
        const unsigned channel_index);
unsigned dsp_bfp_cls_vect_ch_pair_int32(const dsp_ch_pair_int32_t * UNSAFE d, const unsigned length,
        const unsigned channel_index);

/*
 * Vector shl
 */
void dsp_bfp_shl_vect_uint8(uint8_t * UNSAFE d, const unsigned length, const int shl);
void dsp_bfp_shl_vect_uint16(uint16_t * UNSAFE d, const unsigned length, const int shl);
void dsp_bfp_shl_vect_uint32(uint32_t * UNSAFE d, const unsigned length, const int shl);
void dsp_bfp_shl_vect_int16(int16_t * UNSAFE d, const unsigned length, const int shl);
void dsp_bfp_shl_vect_int32(int32_t * UNSAFE d, const unsigned length, const int shl);
void dsp_bfp_shl_vect_complex_int16(dsp_complex_int16_t * UNSAFE d, const unsigned length, const int shl);
void dsp_bfp_shl_vect_complex_int32(dsp_complex_int32_t * UNSAFE d, const unsigned length, const int shl);
void dsp_bfp_shl_vect_ch_pair_int16(dsp_ch_pair_int16_t * UNSAFE d, const unsigned length,
        const int shl_ch_a, const int shl_ch_b);
void dsp_bfp_shl_vect_ch_pair_int32(dsp_ch_pair_int32_t * UNSAFE d, const unsigned length,
        const int shl_ch_a, const int shl_ch_b);





void dsp_sub_bfp_vect_int8(
    int8_t * UNSAFE a, int * UNSAFE a_exp, unsigned * UNSAFE a_hr,
    int8_t * UNSAFE b, int   b_exp, unsigned   b_hr,
    int8_t * UNSAFE c, int   c_exp, unsigned   c_hr,
    unsigned length);

void dsp_sub_bfp_vect_int16(
    int16_t * UNSAFE a, int * UNSAFE a_exp, unsigned * UNSAFE a_hr,
    int16_t * UNSAFE b, int   b_exp, unsigned   b_hr,
    int16_t * UNSAFE c, int   c_exp, unsigned   c_hr,
    unsigned length);

void dsp_sub_bfp_vect_int32(
    int32_t * UNSAFE a, int * UNSAFE a_exp, unsigned * UNSAFE a_hr,
    int32_t * UNSAFE b, int   b_exp, unsigned   b_hr,
    int32_t * UNSAFE c, int   c_exp, unsigned   c_hr,
    unsigned length);

void dsp_sub_vect_int8(
    int8_t * UNSAFE a,
    int8_t * UNSAFE b,
    int8_t * UNSAFE c,
    unsigned length);

void dsp_sub_vect_int16(
    int16_t * UNSAFE a,
    int16_t * UNSAFE b,
    int16_t * UNSAFE c,
    unsigned length);

void dsp_sub_vect_int32(
    int32_t * UNSAFE a,
    int32_t * UNSAFE b,
    int32_t * UNSAFE c,
    unsigned length);


void dsp_add_bfp_vect_int8(
    int8_t * UNSAFE a, int * UNSAFE a_exp, unsigned * UNSAFE a_hr,
    int8_t * UNSAFE b, int   b_exp, unsigned   b_hr,
    int8_t * UNSAFE c, int   c_exp, unsigned   c_hr,
    unsigned length);

void dsp_add_bfp_vect_int16(
    int16_t * UNSAFE a, int * UNSAFE a_exp, unsigned * UNSAFE a_hr,
    int16_t * UNSAFE b, int   b_exp, unsigned   b_hr,
    int16_t * UNSAFE c, int   c_exp, unsigned   c_hr,
    unsigned length);

void dsp_add_bfp_vect_int32(
    int32_t * UNSAFE a, int * UNSAFE a_exp, unsigned * UNSAFE a_hr,
    int32_t * UNSAFE b, int   b_exp, unsigned   b_hr,
    int32_t * UNSAFE c, int   c_exp, unsigned   c_hr,
    unsigned length);

void dsp_add_vect_int8(
    int8_t * UNSAFE a,
    int8_t * UNSAFE b,
    int8_t * UNSAFE c,
    unsigned length);

void dsp_add_vect_int16(
    int16_t * UNSAFE a,
    int16_t * UNSAFE b,
    int16_t * UNSAFE c,
    unsigned length);

void dsp_add_vect_int32(
    int32_t * UNSAFE a,
    int32_t * UNSAFE b,
    int32_t * UNSAFE c,
    unsigned length);



void dsp_mul_bfp_vect_complex_int32(
    dsp_complex_int32_t * UNSAFE a, int * UNSAFE a_exp, unsigned * UNSAFE a_hr,
    dsp_complex_int32_t * UNSAFE b, int   b_exp, unsigned   b_hr,
    dsp_complex_int32_t * UNSAFE c, int   c_exp, unsigned   c_hr,
    unsigned length);

void dsp_mul_vect_int8(
    int8_t * UNSAFE a,
    int8_t * UNSAFE b,
    int8_t * UNSAFE c,
    unsigned length);

void dsp_mul_vect_int16(
    int16_t * UNSAFE a,
    int16_t * UNSAFE b,
    int16_t * UNSAFE c,
    unsigned length);

void dsp_mul_vect_int32(
    int32_t * UNSAFE a,
    int32_t * UNSAFE b,
    int32_t * UNSAFE c,
    unsigned length);

void dsp_mul_bfp_vect_int8(
    int8_t * UNSAFE a, int * UNSAFE a_exp, unsigned * UNSAFE a_hr,
    int8_t * UNSAFE b, int   b_exp, unsigned   b_hr,
    int8_t * UNSAFE c, int   c_exp, unsigned   c_hr,
    unsigned length);

void dsp_mul_bfp_vect_int16(
    int16_t * UNSAFE a, int * UNSAFE a_exp, unsigned * UNSAFE a_hr,
    int16_t * UNSAFE b, int   b_exp, unsigned   b_hr,
    int16_t * UNSAFE c, int   c_exp, unsigned   c_hr,
    unsigned length);

void dsp_mul_bfp_vect_int32(
    int32_t * UNSAFE a, int * UNSAFE a_exp, unsigned * UNSAFE a_hr,
    int32_t * UNSAFE b, int   b_exp, unsigned   b_hr,
    int32_t * UNSAFE c, int   c_exp, unsigned   c_hr,
    unsigned length);

void dsp_muls_bfp_vect_int8(
    int8_t * UNSAFE a, int * UNSAFE a_exp, unsigned * UNSAFE a_hr,
    int8_t * UNSAFE b, int   b_exp, unsigned   b_hr,
    int8_t c,          int   c_exp, unsigned   c_hr,
    unsigned length);

void dsp_muls_bfp_vect_int16(
    int16_t * UNSAFE a, int * UNSAFE a_exp, unsigned * UNSAFE a_hr,
    int16_t * UNSAFE b, int   b_exp, unsigned   b_hr,
    int16_t c,          int   c_exp, unsigned   c_hr,
    unsigned length);

void dsp_muls_bfp_vect_int32(
    int32_t * UNSAFE a, int * UNSAFE a_exp, unsigned * UNSAFE a_hr,
    int32_t * UNSAFE b, int   b_exp, unsigned   b_hr,
    int32_t c,          int   c_exp, unsigned   c_hr,
    unsigned length);

void dsp_div_bfp_vect_int8(
    int8_t * UNSAFE a, int * UNSAFE a_exp, unsigned * UNSAFE a_hr,
    int8_t * UNSAFE b, int   b_exp, unsigned   b_hr,
    int8_t * UNSAFE c, int   c_exp, unsigned   c_hr,
    unsigned length);

void dsp_div_bfp_vect_int16(
    int16_t * UNSAFE a, int * UNSAFE a_exp, unsigned * UNSAFE a_hr,
    int16_t * UNSAFE b, int   b_exp, unsigned   b_hr,
    int16_t * UNSAFE c, int   c_exp, unsigned   c_hr,
    unsigned length);

void dsp_div_bfp_vect_int32(
    int32_t * UNSAFE a, int * UNSAFE a_exp, unsigned * UNSAFE a_hr,
    int32_t * UNSAFE b, int   b_exp, unsigned   b_hr,
    int32_t * UNSAFE c, int   c_exp, unsigned   c_hr,
    unsigned length);

void dsp_div_bfp_vect_complex_int32(
    dsp_complex_t * UNSAFE a, int * UNSAFE a_exp, unsigned * UNSAFE a_hr,
    dsp_complex_t * UNSAFE b, int   b_exp, unsigned   b_hr,
    int32_t * UNSAFE c, int   c_exp, unsigned   c_hr,
    unsigned length);

#if (defined(__XS2A__) || defined (__XS3A__))


/** This function computes the number of leading sign bits in an array of
 * complex numbers. This can be used to calculate the current headroom, and
 * using dsp_bfp_shl to increase the headroom or to reduce the headroom (in
 * order to increase dynamic range).
 *
 * Requires at most 7 x N instructions.
 *
 * \param[in]     pts   Array of dsp_complex_t elements.
 * \param[in]     N     Number of points.
 *
 * \returns       number of bits of headroom
 */
uint32_t dsp_bfp_cls(dsp_complex_t pts[], const uint32_t N);

/** This function shifts an array of complex numbers to adjust the headroom
 * It shifts left if the shift argument is positive, or right if the shift
 * argument is negative. Hence, a sequence::
 *
 *   unsigned ls = dsp_bfp_cls(x,N);
 *   dsp_bfp_shl(x, ls-1, N);
 *
 * will result in one leading sign bit; minimising headroom and maximising
 * dynamic range.
 *
 * Requires at most 6 x N instructions.
 *
 * \param[in,out] pts       Array of dsp_complex_t elements.
 * \param[in]     N         Number of points. Must be a power of two.
 * \param[in]     shift     Number of places to shift left by.
 */
void dsp_bfp_shl( dsp_complex_t pts[], const uint32_t N, const int32_t shift );

/** This function shifts an array of complex numbers to adjust the headroom
 * It shifts left if the shift argument is positive, or right if the shift
 * argument is negative. Hence, a sequence::
 *
 *   unsigned ls = dsp_bfp_cls(x,N)
 *   dsp_bfp_shl(x, ls-1, N)
 *
 * will result in one leading sign bit; minimising headroom and maximising
 * dynamic range.
 *
 * Requires at most 6 x N instructions.
 *
 * \param[in,out] pts       Array of dsp_complex_t elements.
 * \param[in]     N         Number of points. Must be a power of two.
 * \param[in]     shift_re  Number of places to shift the real part left by.
 * \param[in]     shift_im  Number of places to shift the imaginary part left by
 */
void dsp_bfp_shl2( dsp_complex_t * UNSAFE pts, const uint32_t N,
                   const int32_t shift_re, const int32_t shift_im );

/** This function shifts an array of complex numbers to adjust the headroom,
 * whilst simultaneously shuffling the array around in preparation for an FFT.
 * It shifts left if the shift argument is positive, or right if the shift
 * argument is negative.
 *
 * \param[in,out] pts       Array of dsp_complex_t elements.
 * \param[in]     N         Number of points. Must be a power of two.
 * \param[in]     shift     Number of places to shift left by.
 */
void dsp_bfp_bit_reverse_shl( dsp_complex_t pts[], const uint32_t N, const int32_t shift );

#endif

#endif

