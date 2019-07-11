
#include "dsp_debug.h"

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

