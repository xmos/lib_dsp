// Copyright (c) 2015, XMOS Ltd, All rights reserved

#ifndef LIB_DSP_DCT
#define LIB_DSP_DCT

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
void dct32(int output[32], int input[32]);

/** This function performs a 24 point DCT
 *
 *  The first output is the DC value, subsequent values are the values for
 *  the basis vectors of half a cosine, a whole cosine, 1.5 cosine, 2
 *  consines, etc.
 *
 *  \param  input           input values to the DCT
 *  \param  output          DCT values.
 */
void dct24(int output[24], int input[24]);

/** This function performs a 16 point DCT
 *
 *  The first output is the DC value, subsequent values are the values for
 *  the basis vectors of half a cosine, a whole cosine, 1.5 cosine, 2
 *  consines, etc.
 *
 *  \param  input           input values to the DCT
 *  \param  output          DCT values.
 */
void dct16(int output[16], int input[16]);

/** This function performs a 12 point DCT
 *
 *  The first output is the DC value, subsequent values are the values for
 *  the basis vectors of half a cosine, a whole cosine, 1.5 cosine, 2
 *  consines, etc.
 *
 *  \param  input           input values to the DCT
 *  \param  output          DCT values.
 */
void dct12(int output[12], int input[12]);

/** This function performs a 8 point DCT
 *
 *  The first output is the DC value, subsequent values are the values for
 *  the basis vectors of half a cosine, a whole cosine, 1.5 cosine, 2
 *  consines, etc.
 *
 *  \param  input           input values to the DCT
 *  \param  output          DCT values.
 */
void dct8(int output[8], int input[8]);

/** This function performs a 6 point DCT
 *
 *  The first output is the DC value, subsequent values are the values for
 *  the basis vectors of half a cosine, a whole cosine, 1.5 cosine, 2
 *  consines, etc.
 *
 *  \param  input           input values to the DCT
 *  \param  output          DCT values.
 */
void dct6(int output[6], int input[6]);

/** This function performs a 4 point DCT
 *
 *  The first output is the DC value, subsequent values are the values for
 *  the basis vectors of half a cosine, a whole cosine, 1.5 cosine, 2
 *  consines, etc.
 *
 *  \param  input           input values to the DCT
 *  \param  output          DCT values.
 */
void dct4(int output[4], int input[4]);

/** This function performs a 3 point DCT
 *
 *  The first output is the DC value, subsequent values are the values for
 *  the basis vectors of half a cosine, a whole cosine, 1.5 cosine, 2
 *  consines, etc.
 *
 *  \param  input           input values to the DCT
 *  \param  output          DCT values.
 */
void dct3(int output[3], int input[3]);

/** This function performs a 2 point DCT
 *
 *  The first output is the DC value, subsequent values are the values for
 *  the basis vectors of half a cosine, a whole cosine, 1.5 cosine, 2
 *  consines, etc.
 *
 *  \param  input           input values to the DCT
 *  \param  output          DCT values.
 */
void dct2(int output[2], int input[2]);

/** This function performs a single point DCT; it copies its input to its
 * output.
 *
 *  \param  input           input values to the DCT
 *  \param  output          DCT values.
 */
void dct1(int output[1], int input[1]);


/** This function performs a 4 point inverse DCT
 *
 *  \param  input           Basis vector values
 *  \param  output          output signal
 */
void idct4(int output[4], int input[4]);

/** This function performs a 3 point inverse DCT
 *
 *  \param  input           Basis vector values
 *  \param  output          output signal
 */
void idct3(int output[3], int input[3]);

/** This function performs a 2 point inverse DCT
 *
 *  \param  input           Basis vector values
 *  \param  output          output signal
 */
void idct2(int output[2], int input[2]);

/** This function performs a 1 point inverse DCT
 *
 *  \param  input           Basis vector values
 *  \param  output          output signal
 */
void idct1(int output[1], int input[1]);


#ifdef INCLUDE_REFERENCE_DCT
#include <math.h>

void referenceDCT(int output[], int input[], int N);
void referenceIDCT(int output[], int input[], int N);


#endif
#endif
