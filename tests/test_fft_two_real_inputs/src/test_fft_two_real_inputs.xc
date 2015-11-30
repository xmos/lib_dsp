// Copyright (c) 2015, XMOS Ltd, All rights reserved
#include "lib_dsp_transforms.h"
#include <xs1.h>
#include <stdio.h>
#include <stdlib.h>

#define N (1<<8)

int random(unsigned &x){
    crc32(x, -1, 0xEB31D82E);
    return (int)x;
}

void reorder_two_real_inputs(lib_dsp_fft_complex_t f[], unsigned n) {

    for(unsigned i=1;i<N/2;i++){
        int a_re = (f[i].re + f[N-i].re)/2;
        int a_im = (f[i].im - f[N-i].im)/2;
        int b_re = (f[i].im + f[N-i].im)/2;
        int b_im = (-f[i].re + f[N-i].re)/2;

        f[i].re = a_re;
        f[i].im = a_im;
        f[N-i].re = b_re;
        f[N-i].im = b_im;
    }

    f[0].re = f[0].re;
    f[n/2].re = f[0].im;
    f[n/2].im = 0;
    f[0].im = 0;


    for(unsigned i=1;i<N/4;i++){
        lib_dsp_fft_complex_t a = f[n/2 + i];
        lib_dsp_fft_complex_t b = f[N - i];
        f[n/2 + i] = b;
        f[N - i]  = a;
    }
}

int main(){

    unsigned x=0x52435674;

    while(1){
        random(x);

        //generate a frame
        lib_dsp_fft_complex_t f[N];
        lib_dsp_fft_complex_t a[N];
        lib_dsp_fft_complex_t b[N];

        for(unsigned i=0;i<N;i++){
            int d_a = random(x);
            int d_b = random(x);

            f[i].re = d_a;
            f[i].im = d_b;

            a[i].re = d_a;
            a[i].im = 0;

            b[i].re = d_b;
            b[i].im = 0;

        }

        lib_dsp_fft_bit_reverse(f, N);
        lib_dsp_fft_bit_reverse(a, N);
        lib_dsp_fft_bit_reverse(b, N);

        lib_dsp_fft_forward_complex(f, N, lib_dsp_sine_256);
        lib_dsp_fft_forward_complex(a, N, lib_dsp_sine_256);
        lib_dsp_fft_forward_complex(b, N, lib_dsp_sine_256);

        lib_dsp_fft_reorder_two_real_inputs(f, N);

 /*

        for(unsigned i=0;i<N/2;i++){
            printf("%3d %11d %11d %11d %11d * %11d %11d %11d %11d\n",i,
                    a[i].re, a[i].im,
                    f[i].re, f[i].im,
                    b[i].re, b[i].im,
                    f[N/2 + i].re, f[N/2 + i].im);
        }
        _Exit(1);

*/
        {
            lib_dsp_fft_complex_t (* restrict fft)[N/2] = (lib_dsp_fft_complex_t (* restrict )[N/2])f;

            for(unsigned i=0;i<N/2;i++){
                int diff_a_re = a[i].re - fft[0][i].re;
                int diff_a_im = a[i].im - fft[0][i].im;
                int diff_b_re = b[i].re - fft[1][i].re;
                int diff_b_im = b[i].im - fft[1][i].im;

                diff_a_re *= diff_a_re;
                diff_a_im *= diff_a_im;
                diff_b_re *= diff_b_re;
                diff_b_im *= diff_b_im;

                unsigned max_allowed_diff = 4*4;

                if( (diff_a_re > max_allowed_diff) ||
                    (diff_a_im > max_allowed_diff) ||
                    (diff_b_re > max_allowed_diff) ||
                    (diff_b_im > max_allowed_diff)   ){
                    printf("error %d\n", i);
                }

            }
        }

    }
    return 0;
}

