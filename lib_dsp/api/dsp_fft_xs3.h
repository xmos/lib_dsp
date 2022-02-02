// Copyright 2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#ifndef DSP_FFT_XS3_H_
#define DSP_FFT_XS3_H_

#include <stdint.h>

#define FFT_LENGTH    (512)

/** This function computes a forward FFT. The real input signal is
 * supplied in an array of real floating-point values.
 * The same array is also used to store the output.
 * The number of points must be 512.
 *
 * \param[in,out] buffer   Array of float values.
 */
void fft_float_mono_forward_512 (
                float buffer[] );


/** This function computes an inverse FFT. The real input signal is
 * supplied in an array of real floating-point values.
 * The same array is also used to store the output.
 * The number of points must be 512.
 *
 * \param[in,out] buffer   Array of float values.
 */
void fft_float_mono_inverse_512 (
                float buffer[] );

#endif
