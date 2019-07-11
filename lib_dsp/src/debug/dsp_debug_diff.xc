/*
 * dsp_debug_diff.xc
 *
 *  Created on: Jul 10, 2019
 *      Author: andrew
 */


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
