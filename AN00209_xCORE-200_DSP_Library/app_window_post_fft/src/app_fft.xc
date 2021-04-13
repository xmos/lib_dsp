// Copyright 2015-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// XMOS DSP Library - Example to use FFT and inverse FFT

#include <stdio.h>
#include <stdlib.h>
#include <xs1.h>
#include <xclib.h>
#include <dsp.h>
#include <stdint.h>

#define N 16

int data[N] = {10, 4, 0, -5, 7, 8, 10, 1,
               10, 4, 3, -5, 7, 8, 10, 1};

int window[N] = {
0,
2494,
9597,
20228,
32767,
45307,
55938,
63041,
65536,
63041,
55938,
45307,
32768,
20228,
9597,
2494,
};
dsp_complex_t fftdata[N];
dsp_complex_t fftdata2[N];

#define f16(x)   ((x)*16/65536.0)

int main(void) {
    int errors = 0;
    for(int i = 0; i < 16; i++) {
        fftdata[i].re = data[i] << 16;
        fftdata[i].im = 0;
        fftdata2[i].re = data[i] * window[i];
        fftdata2[i].im = 0;
    }
    
    dsp_fft_bit_reverse(fftdata, N);
    dsp_fft_forward(fftdata, N, dsp_sine_16);
    dsp_fft_split_spectrum(fftdata, N);
    
    dsp_fft_bit_reverse(fftdata2, N);
    dsp_fft_forward(fftdata2, N, dsp_sine_16);
    dsp_fft_split_spectrum(fftdata2, N);

    dsp_complex_window_hanning_post_fft_half(fftdata, N/2);
    for(int32_t i=0; i<N/2; i++) {
        int diffre = fftdata[i].re-fftdata2[i].re;
        int diffim = fftdata[i].im-fftdata2[i].im;
        if (abs(diffim) > 3 || abs(diffre) > 3) {
            printf( "%2d: %8.4f, %8.4f    %8.4f %8.4f    %8.4f %8.4f\n", i, f16(fftdata[i].re), f16(fftdata[i].im), f16(fftdata2[i].re), f16(fftdata2[i].im), f16(diffre), f16(diffim));
            errors++;
        }
    }
    if (errors == 0) {
        printf("Pass\n");
    } else {
        printf("Fail\n");
    }
    return 0;
}
