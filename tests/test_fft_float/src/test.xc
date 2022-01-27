// Copyright 2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <xs1.h>
#include <xclib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "dsp_fft_float4.h"
#include "dsp_bfp.h"

int random(unsigned &x){
    crc32(x, -1, 0xEB31D82E);
    return (int)x;
}

// Float FFTs are tested by going forwards and backwards.
// There is a small chance that you get the right answer
// when errors cancel each other out - that can be fixed at some time.

void test_float_fft(){
    unsigned x=1;

    float max_error = 0;
    float average_error = 0;
    unsigned test_count = 2;
    int fail = 0;
    for(unsigned t=0;t<test_count;t++){

        dsp_complex_float4_t f[FFT_LENGTH];
        dsp_complex_float4_t g[FFT_LENGTH];
        for(unsigned i=0;i<FFT_LENGTH;i++){
            float max = 0x7fffffff;
            g[i].re = f[i].re = random(x) / max;
            g[i].im = f[i].im = random(x) / max;
        }
        dsp_fft_float4_bit_reverse(f, FFT_LENGTH);
        dsp_fft_float4_forward(f, FFT_LENGTH, FFT_FLOAT_SINE(FFT_LENGTH));
        dsp_fft_float4_bit_reverse(f, FFT_LENGTH);
        dsp_fft_float4_inverse(f, FFT_LENGTH, FFT_FLOAT_SINE(FFT_LENGTH));

        max_error = average_error = 0;
        for(unsigned i=0;i<FFT_LENGTH;i++){
            float e = f[i].re / FFT_LENGTH - g[i].re;
            average_error += e;
            e = fabs(e);
            if(e > max_error)
                max_error = e;
            e = f[i].im / FFT_LENGTH - g[i].im;
            average_error += e;
            e = fabs(e);
            if(e > max_error)
                max_error = e;
        }
        average_error = average_error / FFT_LENGTH / 2;
        if (max_error >= 1e-6 || fabs(average_error) >= 1e-7) {
            fail = 1;
            printf("Forward/inverse FAIL %g %g\n", max_error, average_error);
        }
        for(unsigned i=0;i<FFT_LENGTH;i++){
            float max = 0x7fffffff;
            g[i].re = f[i].re = random(x) / max;
            g[i].im = f[i].im = random(x) / max;
        }

        dsp_fft_float4_bit_reverse(f, FFT_LENGTH);
        dsp_fft_float4_forward(f, FFT_LENGTH, FFT_FLOAT_SINE(FFT_LENGTH));
        dsp_fft_float4_split_spectrum(f, FFT_LENGTH, 0.5);
        dsp_fft_float4_merge_spectra(f, FFT_LENGTH);
        dsp_fft_float4_bit_reverse(f, FFT_LENGTH);
        dsp_fft_float4_inverse(f, FFT_LENGTH, FFT_FLOAT_SINE(FFT_LENGTH));

        max_error = average_error = 0;
        for(unsigned i=0;i<FFT_LENGTH;i++){
            float e = f[i].re / FFT_LENGTH - g[i].re;
            average_error += e;
            e = fabs(e);
            if(e > max_error)
                max_error = e;
            e = f[i].im / FFT_LENGTH - g[i].im;
            average_error += e;
            e = fabs(e);
            if(e > max_error)
                max_error = e;
        }
        average_error = average_error / FFT_LENGTH / 2;
        if (max_error >= 1e-6 || fabs(average_error) >= 1e-7) {
            fail = 1;
            printf("Real forward/inverse FAIL %g %g\n", max_error, average_error);
        }

        int exp = dsp_float_to_bfp((f, float[]), FFT_LENGTH);
        dsp_fft_bit_reverse((f, dsp_complex_t[]), FFT_LENGTH);
        dsp_fft_forward((f, dsp_complex_t[]), FFT_LENGTH, FFT_SINE(FFT_LENGTH));
        dsp_fft_bit_reverse((f, dsp_complex_t[]), FFT_LENGTH);
        dsp_fft_inverse((f, dsp_complex_t[]), FFT_LENGTH, FFT_SINE(FFT_LENGTH));
        dsp_bfp_to_float((f, float[]), FFT_LENGTH, exp);

        max_error = average_error = 0;
        for(unsigned i=0;i<FFT_LENGTH;i++){
            float e = f[i].re / FFT_LENGTH - g[i].re;
            average_error += e;
            e = fabs(e);
            if(e > max_error)
                max_error = e;
            e = f[i].im / FFT_LENGTH - g[i].im;
            average_error += e;
            e = fabs(e);
            if(e > max_error)
                max_error = e;
        }
        average_error = average_error / FFT_LENGTH / 2;
        if (max_error >= 1e-6 || fabs(average_error) >= 1e-7) {
            fail = 1;
            printf("Float to BSP forward/inverse FAIL %g %g\n", max_error, average_error);
        }
    }
    if (!fail) {
        printf("Float FFT: Pass.\n");
    }
}

unsafe int main(){
    test_float_fft();
    _Exit(0);
    return 0;
}
