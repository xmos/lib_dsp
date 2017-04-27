// Copyright (c) 2016-2017, XMOS Ltd, All rights reserved

#ifndef DSP_TRANSFORMS_H_
#define DSP_TRANSFORMS_H_

#include <stdint.h>

extern const int32_t dsp_sine_8[];
extern const int32_t dsp_sine_16[];
extern const int32_t dsp_sine_32[];
extern const int32_t dsp_sine_64[];
extern const int32_t dsp_sine_128[];
extern const int32_t dsp_sine_256[];
extern const int32_t dsp_sine_512[];
extern const int32_t dsp_sine_1024[];
extern const int32_t dsp_sine_2048[];
extern const int32_t dsp_sine_4096[];
extern const int32_t dsp_sine_8192[];
extern const int32_t dsp_sine_16384[];

#define FFT_SINE0(N) dsp_sine_ ## N
#define FFT_SINE(N) FFT_SINE0(N)

typedef struct
{
    int32_t re;
    int32_t im;
}
dsp_complex_t;

typedef struct
{
    int16_t re;
    int16_t im;
}
dsp_complex_short_t;

/** This function splits the spectrum of the FFT of two real sequences. Takes
 * the result of a double-packed dsp_complex_t array that has undergone
 * an FFT. This function splits the result into two arrays, one for each real
 * sequence, of length N/2.
 * It is expected that the output will be cast by:
 *   dsp_complex_t (* restrict w)[2] = (dsp_complex_t (*)[2])pts;
 * or a C equlivent. The 2 dimensional array w[2][N/2] can now be used to access
 * the frequency information of the two real sequences independently, with the
 * first index denoting the corresponding real sequence and the second index denoting
 * the FFT frequency bin.
 * Note that the DC component of the imaginary output spectrum (index zero) will
 * contain the real component for the Nyquest rate.
 *
 * \param[in,out] pts   Array of dsp_complex_t elements.
 * \param[in]     N     Number of points. Must be a power of two.
 */
void dsp_fft_split_spectrum( dsp_complex_t pts[], const uint32_t N );

/** This function merges two split spectra. It is the exact inverse operation of
 * dsp_fft_split_spectrum.
 *
 * \param[in,out] pts   Array of dsp_complex_t elements.
 * \param[in]     N     Number of points. Must be a power of two.
 */
void dsp_fft_merge_spectra( dsp_complex_t pts[], const uint32_t N );

/** This function copies an array of dsp_complex_short_t elements to an array of an equal
 * number of dsp_complex_t elements.
 *
 * \param[out]    l   Array of dsp_complex_t elements.
 * \param[in]     s   Array of dsp_complex_short_t elements.
 * \param[in]     N   Number of points.
 */
void dsp_fft_short_to_long( const dsp_complex_short_t s[], dsp_complex_t l[], const uint32_t N );

/** This function copies an array of dsp_complex_t elements to an array of an equal
 * number of dsp_complex_short_t elements.
 *
 * \param[out]    s   Array of dsp_complex_short_t elements.
 * \param[in]     l   Array of dsp_complex_t elements.
 * \param[in]     N   Number of points.
 */
void dsp_fft_long_to_short( const dsp_complex_t l[], dsp_complex_short_t s[], const uint32_t N );

/** This function preforms index bit reversing on the the arrays around prior to computing an FFT. A
 * calling sequence for a forward FFT involves dsp_fft_bit_reverse() followed by
 * dsp_fft_forward(), and for an inverse FFT it involves dsp_fft_bit_reverse() followed
 * by dsp_fft_inverse(). In some cases bit reversal can be avoided, for example
 * when computing a convolution.
 *
 * \param[in,out] pts   Array of dsp_complex_t elements.
 * \param[in]     N     Number of points. Must be a power of two.
 */
void dsp_fft_bit_reverse( dsp_complex_t pts[], const uint32_t N );

/** This function computes a forward FFT. The complex input signal is
 * supplied in an array of real and imaginary fixed-point values.
 * The same array is also used to store the output.
 * The magnitude of the FFT output is right shifted log2(N) times which corresponds to
 * division by N as shown in EQUATION 31-5 of ``http://www.dspguide.com/CH31.PDF``.
 * The number of points must be a power of 2, and the array of sine values should contain a quarter sine-wave.
 * Use one of the dsp_sine_N tables. The function does not perform bit reversed indexing of the input data.
 * If required then dsp_fft_bit_reverse() should be called beforehand.
 *
 * \param[in,out] pts   Array of dsp_complex_t elements.
 * \param[in]     N     Number of points. Must be a power of two.
 * \param[in]     sine  Array of N/4+1 sine values, each represented as a sign bit,
 *                      and a 31 bit fraction. 1 should be represented as 0x7fffffff.
 *                      Arrays are provided in dsp_tables.c; for example, for a 1024 point
 *                      FFT use dsp_sine_1024.
 */
void dsp_fft_forward (
    dsp_complex_t pts[],
    const uint32_t        N,
    const int32_t         sine[] );

/** This function computes an inverse FFT. The complex input array is
 * supplied as two arrays of integers, with numbers represented as
 * fixed-point values. Max input range is -0x3fffffff..0x3fffffff. 
 * Integer overflow can occur with inputs outside of this range.
 * The number of points must be a power of 2, and the
 * array of sine values should contain a quarter sine-wave. Use one of the
 * dsp_sine_N tables. The function does not perform bit reversed indexing of the input data.
 * if required then dsp_fft_bit_reverse() should be called beforehand.
 *
 * \param[in,out] pts   Array of dsp_complex_t elements.
 * \param[in]     N     Number of points. Must be a power of two.
 *
 * \param[in]     sine  Array of N/4+1 sine values, each represented as a sign bit,
 *                      and a 31 bit fraction. 1 should be represented as 0x7fffffff.
 *                      Arrays are provided in dsp_tables.c; for example, for a 1024 point
 *                      FFT use dsp_sine_1024.
 */
void dsp_fft_inverse (
    dsp_complex_t pts[],
    const uint32_t        N,
    const int32_t         sine[] );

#endif

