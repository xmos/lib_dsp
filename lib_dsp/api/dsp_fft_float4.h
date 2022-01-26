// Copyright (c) 2022, XMOS Ltd, All rights reserved

#ifndef DSP_FFT_FLOAT4_H_
#define DSP_FFT_FLOAT4_H_

#include <stdint.h>
#include <dsp_complex.h>
#include <dsp_fft.h> // for bitreverse

extern const float dsp_sine_float4_4[];
extern const float dsp_sine_float4_8[];
extern const float dsp_sine_float4_16[];
extern const float dsp_sine_float4_32[];
extern const float dsp_sine_float4_64[];
extern const float dsp_sine_float4_128[];
extern const float dsp_sine_float4_256[];
extern const float dsp_sine_float4_512[];
extern const float dsp_sine_float4_1024[];
extern const float dsp_sine_float4_2048[];
extern const float dsp_sine_float4_4096[];
extern const float dsp_sine_float4_8192[];
extern const float dsp_sine_float4_16384[];

#define FFT_FLOAT_SINE0(N) dsp_sine_float4_ ## N
#define FFT_FLOAT_SINE(N) FFT_FLOAT_SINE0(N)

/** This function splits the spectrum of the FFT of two real sequences. Takes
 * the result of a double-packed dsp_complex_float4_t array that has undergone
 * an FFT. This function splits the result into two arrays, one for each real
 * sequence, of length N/2.
 * It is expected that the output will be cast by:
 *   dsp_complex_float4_t (* restrict w)[2] = (dsp_complex_float4_t (*)[2])pts;
 * or a C equivalent. The 2 dimensional array w[2][N/2] can now be used to access
 * the frequency information of the two real sequences independently, with the
 * first index denoting the corresponding real sequence and the second index denoting
 * the FFT frequency bin.
 * Note that the DC component of the imaginary output spectrum (index zero) will
 * contain the real component for the Nyquest rate.
 *
 * \param[in,out] pts      Array of dsp_complex_t elements.
 * \param[in]     N        Number of points. Must be a power of two.
 * \param[in]     mul_0_5  multiplier, set to 0.5 to get the normal
 *                         behaviour
 */
void dsp_fft_float4_split_spectrum( dsp_complex_float4_t pts[],
                                    const uint32_t N,
                                    float m_0_5);

/** This function merges two split spectra. It is the exact inverse operation of
 * dsp_fft_float4_split_spectrum.
 *
 * \param[in,out] pts   Array of dsp_complex_t elements.
 * \param[in]     N     Number of points. Must be a power of two.
 */
void dsp_fft_float4_merge_spectra( dsp_complex_float4_t pts[], const uint32_t N );

/** This function preforms index bit reversing on the the arrays around prior to computing an FFT. A
 * calling sequence for a forward FFT involves dsp_fft_float4_bit_reverse() followed by
 * dsp_fft_forward(), and for an inverse FFT it involves dsp_fft_float4_bit_reverse() followed
 * by dsp_fft_inverse(). In some cases bit reversal can be avoided, for example
 * when computing a convolution.
 *
 * \param[in,out] pts   Array of dsp_complex_t elements.
 * \param[in]     N     Number of points. Must be a power of two.
 */
static inline void dsp_fft_float4_bit_reverse( dsp_complex_float4_t pts[],
                                              const uint32_t N ) {
#ifdef __XC__
    dsp_fft_bit_reverse((pts, dsp_complex_t[]), N);
#else
    dsp_fft_bit_reverse((dsp_complex_t*) pts, N);
#endif
}

/** This function computes a forward FFT. The complex input signal is
 * supplied in an array of real and imaginary floating-point values.
 * The same array is also used to store the output.
 * The number of points must be a power of 2, and the array of sine values should contain a quarter sine-wave.
 * Use one of the dsp_sine_float4_N tables. The function does not perform bit reversed indexing of the input data.
 * If required then dsp_fft_float4_bit_reverse() should be called beforehand.
 *
 * \param[in,out] pts   Array of dsp_complex_float4_t elements.
 * \param[in]     N     Number of points. Must be a power of two.
 * \param[in]     sine  Array of N/4+1 sine values, each represented as a sign bit,
 *                      and a 31 bit fraction. 1 should be represented as 0x7fffffff.
 *                      Arrays are provided in dsp_tables.c; for example, for a 1024 point
 *                      FFT use dsp_sine_1024.
 */
void dsp_fft_float4_forward (
    dsp_complex_float4_t pts[],
    const uint32_t  N,
    const float   sine[] );


/** This function computes an inverse FFT. The input is a complex float array.
 * The number of points must be a power of 2, and the
 * array of sine values should contain a quarter sine-wave. Use one of the
 * dsp_sine_float4_N tables. The function does not perform bit reversed indexing of the input data.
 * if required then dsp_fft_float4_bit_reverse() should be called beforehand.
 *
 * Only available on XS3.
 *
 * \param[in,out] pts   Array of dsp_complex_float4_t elements.
 * \param[in]     N     Number of points. Must be a power of two.
 *
 * \param[in]     sine  Array of N/4+1 sine values.
 *                      Arrays are provided in dsp_tables.c; for example, for a 1024 point
 *                      FFT use dsp_sine_float4_1024.
 */

void dsp_fft_float4_inverse (
    dsp_complex_float4_t pts[],
    const uint32_t  N,
    const float   sine[] );

#endif
