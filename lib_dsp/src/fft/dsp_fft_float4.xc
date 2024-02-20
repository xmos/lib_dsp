// Copyright 2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <xs1.h>
#include <stdint.h>
#include "dsp_fft_float4.h"

extern void dsp_fft_float4_forward_xs3 (
        dsp_complex_float4_t pts[],
        uint32_t  N,
        const float   sine[]);

extern void dsp_fft_float4_inverse_xs3 (
        dsp_complex_float4_t pts[],
        uint32_t  N,
        const float   sine[]);

extern void dsp_fft_float4_split_spectrum_xs3( dsp_complex_float4_t pts[],
                                               uint32_t N, float m );

extern void dsp_fft_float4_merge_spectra_xs3( dsp_complex_float4_t pts[], uint32_t N );

void dsp_fft_float4_forward (
    dsp_complex_float4_t pts[],
    const uint32_t  N,
    const float   sine[] ){
#if defined(__XS3A__)
    dsp_fft_float4_forward_xs3 (pts, (uint32_t) N, sine);
#else
    asm volatile("ecallf %0" :: "r" (0));
#endif
}

void dsp_fft_float4_inverse (
    dsp_complex_float4_t pts[],
    const uint32_t  N,
    const float   sine[] ){
#if defined(__XS3A__)
    dsp_fft_float4_inverse_xs3 (pts, (uint32_t) N, sine);
#else
    asm volatile("ecallf %0" :: "r" (0));
#endif
}

void dsp_fft_float4_split_spectrum( dsp_complex_float4_t pts[], const
                                    uint32_t N, float m ){
#if defined(__XS3A__)
    dsp_fft_float4_split_spectrum_xs3(pts, (uint32_t) N, m);
#else
    asm volatile("ecallf %0" :: "r" (0));
#endif
}

void dsp_fft_float4_merge_spectra( dsp_complex_float4_t pts[], const uint32_t N ){
#if defined(__XS3A__)
    dsp_fft_float4_merge_spectra_xs3(pts, (uint32_t) N);
#else
    asm volatile("ecallf %0" :: "r" (0));
#endif
}
