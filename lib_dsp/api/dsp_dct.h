// Copyright (c) 2015-2016, XMOS Ltd, All rights reserved

#ifndef DSP_DCT_H_
#define DSP_DCT_H_

#include "stdint.h"

/* This library provides a limited set of discrete cosine transforms */
/* DCT: 32, 24, 16, 12, 8, 6, 4, 3, 2 and 1 point */
/* inverse DCT: 4, 3, 2, 1 point only */

/** This function performs a 32 point DCT
 *
 *  The first output is the DC value, subsequent values are the values for
 *  the basis vectors of half a cosine, a whole cosine, 1.5 cosine, 2
 *  consines, etc.
 *
 *  \param  input           input values to the DCT
 *  \param  output          DCT values.
 */
void dsp_dct_forward32(int32_t output[32], int32_t input[32]);

/** This function performs a 24 point DCT
 *
 *  The first output is the DC value, subsequent values are the values for
 *  the basis vectors of half a cosine, a whole cosine, 1.5 cosine, 2
 *  consines, etc.
 *
 *  \param  input           input values to the DCT
 *  \param  output          DCT values.
 */
void dsp_dct_forward24(int32_t output[24], int32_t input[24]);

/** This function performs a 16 point DCT
 *
 *  The first output is the DC value, subsequent values are the values for
 *  the basis vectors of half a cosine, a whole cosine, 1.5 cosine, 2
 *  consines, etc.
 *
 *  \param  input           input values to the DCT
 *  \param  output          DCT values.
 */
void dsp_dct_forward16(int32_t output[16], int32_t input[16]);

/** This function performs a 12 point DCT
 *
 *  The first output is the DC value, subsequent values are the values for
 *  the basis vectors of half a cosine, a whole cosine, 1.5 cosine, 2
 *  consines, etc.
 *
 *  \param  input           input values to the DCT
 *  \param  output          DCT values.
 */
void dsp_dct_forward12(int32_t output[12], int32_t input[12]);

/** This function performs a 8 point DCT
 *
 *  The first output is the DC value, subsequent values are the values for
 *  the basis vectors of half a cosine, a whole cosine, 1.5 cosine, 2
 *  consines, etc.
 *
 *  \param  input           input values to the DCT
 *  \param  output          DCT values.
 */
void dsp_dct_forward8(int32_t output[8], int32_t input[8]);

/** This function performs a 6 point DCT
 *
 *  The first output is the DC value, subsequent values are the values for
 *  the basis vectors of half a cosine, a whole cosine, 1.5 cosine, 2
 *  consines, etc.
 *
 *  \param  input           input values to the DCT
 *  \param  output          DCT values.
 */
void dsp_dct_forward6(int32_t output[6], int32_t input[6]);

/** This function performs a 4 point DCT
 *
 *  The first output is the DC value, subsequent values are the values for
 *  the basis vectors of half a cosine, a whole cosine, 1.5 cosine, 2
 *  consines, etc.
 *
 *  \param  input           input values to the DCT
 *  \param  output          DCT values.
 */
void dsp_dct_forward4(int32_t output[4], int32_t input[4]);

/** This function performs a 3 point DCT
 *
 *  The first output is the DC value, subsequent values are the values for
 *  the basis vectors of half a cosine, a whole cosine, 1.5 cosine, 2
 *  consines, etc.
 *
 *  \param  input           input values to the DCT
 *  \param  output          DCT values.
 */
void dsp_dct_forward3(int32_t output[3], int32_t input[3]);

/** This function performs a 2 point DCT
 *
 *  The first output is the DC value, subsequent values are the values for
 *  the basis vectors of half a cosine, a whole cosine, 1.5 cosine, 2
 *  consines, etc.
 *
 *  \param  input           input values to the DCT
 *  \param  output          DCT values.
 */
void dsp_dct_forward2(int32_t output[2], int32_t input[2]);

/** This function performs a single point DCT; it copies its input to its
 * output.
 *
 *  \param  input           input values to the DCT
 *  \param  output          DCT values.
 */
void dsp_dct_forward1(int32_t output[1], int32_t input[1]);


/** This function performs a 4 point inverse DCT
 *
 *  \param  input           Basis vector values
 *  \param  output          output signal
 */
void dsp_dct_inverse4(int32_t output[4], int32_t input[4]);

/** This function performs a 3 point inverse DCT
 *
 *  \param  input           Basis vector values
 *  \param  output          output signal
 */
void dsp_dct_inverse3(int32_t output[3], int32_t input[3]);

/** This function performs a 2 point inverse DCT
 *
 *  \param  input           Basis vector values
 *  \param  output          output signal
 */
void dsp_dct_inverse2(int32_t output[2], int32_t input[2]);

/** This function performs a 1 point inverse DCT
 *
 *  \param  input           Basis vector values
 *  \param  output          output signal
 */
void dsp_dct_inverse1(int32_t output[1], int32_t input[1]);


#ifdef INCLUDE_REFERENCE_DCT
#include <math.h>

void referenceDCT(int32_t output[], int32_t input[], int32_t N);
void referenceIDCT(int32_t output[], int32_t input[], int32_t N);


#endif
#endif
