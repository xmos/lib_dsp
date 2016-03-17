// Copyright (c) 2016, XMOS Ltd, All rights reserved
#include "lib_dsp_transforms.h"
#include <xs1.h>
#include <stdio.h>
#include <stdlib.h>

#define N (1<<11)

int random(unsigned &x){
    crc32(x, -1, 0xEB31D82E);
    return (int)x;
}

//reference implementation
void rebuild_one_real_input(lib_dsp_fft_complex_t f[], unsigned n){
    for(unsigned i=1;i<n/2;i++){
        f[n-i].re = f[i].re;
        f[n-i].im = -f[i].im;
    }
    f[n/2].re = f[0].im;
    f[0].im = 0;
    f[n/2].im = 0;
}

//reference implementation
void reorder_two_real_inputs(lib_dsp_fft_complex_t f[], unsigned n) {

    for(unsigned i=1;i<n/2;i++){
        int a_re = (f[i].re + f[n-i].re)/2;
        int a_im = (f[i].im - f[n-i].im)/2;
        int b_re = (f[i].im + f[n-i].im)/2;
        int b_im = (-f[i].re + f[n-i].re)/2;

        f[i].re = a_re;
        f[i].im = a_im;
        f[n-i].re = b_re;
        f[n-i].im = b_im;
    }

    int re = f[n/2].re;
    int im = f[n/2].im;

    f[0].re = f[0].re;
    f[n/2].re = f[0].im;
    f[n/2].im = im;
    f[0].im = re;


    for(unsigned i=1;i<n/4;i++){
        lib_dsp_fft_complex_t a = f[n/2 + i];
        lib_dsp_fft_complex_t b = f[n - i];
        f[n/2 + i] = b;
        f[n - i]  = a;
    }
}
/*

FFT length: 8
    Max delta of two reals going forwards:             2
    Max delta of one real going forwards and reverse:  16
    Max delta of two reals going forwards and reverse: 8
FFT length: 16
    Max delta of two reals going forwards:             3
    Max delta of one real going forwards and reverse:  26
    Max delta of two reals going forwards and reverse: 22
FFT length: 32
    Max delta of two reals going forwards:             3
    Max delta of one real going forwards and reverse:  41
    Max delta of two reals going forwards and reverse: 42
FFT length: 64
    Max delta of two reals going forwards:             4
    Max delta of one real going forwards and reverse:  67
    Max delta of two reals going forwards and reverse: 72
FFT length: 128
    Max delta of two reals going forwards:             4
    Max delta of one real going forwards and reverse:  108
    Max delta of two reals going forwards and reverse: 124
FFT length: 256
    Max delta of two reals going forwards:             4
    Max delta of one real going forwards and reverse:  183
    Max delta of two reals going forwards and reverse: 228
FFT length: 512
    Max delta of two reals going forwards:             4
    Max delta of one real going forwards and reverse:  324
    Max delta of two reals going forwards and reverse: 376
FFT length: 1024
    Max delta of two reals going forwards:             5
    Max delta of one real going forwards and reverse:  595
    Max delta of two reals going forwards and reverse: 682

 */
unsafe int main(){

    unsigned x=0x52435674;
    const int * unsafe sine_lut[9] = {
            lib_dsp_sine_8,
            lib_dsp_sine_16,
            lib_dsp_sine_32,
            lib_dsp_sine_64,
            lib_dsp_sine_128,
            lib_dsp_sine_256,
            lib_dsp_sine_512,
            lib_dsp_sine_1024,
            lib_dsp_sine_2048
    };

    for(unsigned n=3;n<=11;n++){
        const unsigned fft_length = 1<<n;
        printf("FFT length: %d\n", fft_length);
        int max_two_real_delta = 0;
        int max_forward_inverse_delta = 0;
        int max_forward_inverse_two_real_delta = 0;
        for(unsigned s=0;s<64*1024;s++){
            random(x);

            //generate a frame
            lib_dsp_fft_complex_t f[N];
            lib_dsp_fft_complex_t a[N];
            lib_dsp_fft_complex_t b[N];
            lib_dsp_fft_complex_t c[N];
            lib_dsp_fft_complex_t d[N];

            for(unsigned i=0;i<fft_length;i++){
                int d_a = random(x);
                int d_b = random(x);

                f[i].re = d_a;
                f[i].im = d_b;

                a[i].re = d_a;
                a[i].im = 0;

                b[i].re = d_b;
                b[i].im = 0;

                c[i].re = d_a;
                c[i].im = 0;

                d[i].re = d_b;
                d[i].im = 0;
            }

            lib_dsp_fft_bit_reverse(f, fft_length);
            lib_dsp_fft_bit_reverse(a, fft_length);
            lib_dsp_fft_bit_reverse(b, fft_length);

            lib_dsp_fft_forward_complex(f, fft_length, (int *)sine_lut[n-3]);
            lib_dsp_fft_forward_complex(a, fft_length, (int *)sine_lut[n-3]);
            lib_dsp_fft_forward_complex(b, fft_length, (int *)sine_lut[n-3]);

            lib_dsp_fft_reorder_two_real_inputs(f, fft_length);

            //Special case for first index as the imaginary part is the real part for index (N/2).

            int a_re_diff = a[0].re - f[0].re;
            int a_im_diff = a[fft_length/2].re - f[0].im;
            int b_re_diff = b[0].re - f[0+(fft_length/2)].re;
            int b_im_diff = b[fft_length/2].re - f[0+(fft_length/2)].im;

            if(a_re_diff<0) a_re_diff=-a_re_diff;
            if(a_im_diff<0) a_im_diff=-a_im_diff;
            if(b_re_diff<0) b_re_diff=-b_re_diff;
            if(b_im_diff<0) b_im_diff=-b_im_diff;

            if (a_re_diff>max_two_real_delta) max_two_real_delta = a_re_diff;
            if (a_im_diff>max_two_real_delta) max_two_real_delta = a_im_diff;
            if (b_re_diff>max_two_real_delta) max_two_real_delta = b_re_diff;
            if (b_im_diff>max_two_real_delta) max_two_real_delta = b_im_diff;

            unsigned max_allowed_diff = 4*4;

            if( (a_re_diff > max_allowed_diff) ||
                (a_im_diff > max_allowed_diff) ||
                (a_re_diff > max_allowed_diff) ||
                (a_re_diff > max_allowed_diff) ){
                printf("error\n");
                _Exit(1);
            }

            for(unsigned i=1;i<fft_length/2;i++){
                int a_re_diff = a[i].re - f[i].re;
                int a_im_diff = a[i].im - f[i].im;
                int b_re_diff = b[i].re - f[i+(fft_length/2)].re;
                int b_im_diff = b[i].im - f[i+(fft_length/2)].im;

                if(a_re_diff<0) a_re_diff=-a_re_diff;
                if(a_im_diff<0) a_im_diff=-a_im_diff;
                if(b_re_diff<0) b_re_diff=-b_re_diff;
                if(b_im_diff<0) b_im_diff=-b_im_diff;

                if (a_re_diff>max_two_real_delta) max_two_real_delta = a_re_diff;
                if (a_im_diff>max_two_real_delta) max_two_real_delta = a_im_diff;
                if (b_re_diff>max_two_real_delta) max_two_real_delta = b_re_diff;
                if (b_im_diff>max_two_real_delta) max_two_real_delta = b_im_diff;

                unsigned max_allowed_diff = 4*4;

                if( (a_re_diff > max_allowed_diff) ||
                    (a_im_diff > max_allowed_diff) ||
                    (a_re_diff > max_allowed_diff) ||
                    (a_re_diff > max_allowed_diff) ){
                    printf("error %d\n", i);
                    _Exit(1);
                }
            }

            lib_dsp_fft_bit_reverse(a, fft_length);
            lib_dsp_fft_bit_reverse(b, fft_length);

            lib_dsp_fft_inverse_complex(a, fft_length, (int *)sine_lut[n-3]);
            lib_dsp_fft_inverse_complex(b, fft_length, (int *)sine_lut[n-3]);

            for(unsigned i=0;i<fft_length;i++){
                int a_re_diff = a[i].re - c[i].re;
                int a_im_diff = a[i].im - c[i].im;

                int b_re_diff = b[i].re - d[i].re;
                int b_im_diff = b[i].im - d[i].im;

                if(a_re_diff<0) a_re_diff=-a_re_diff;
                if(a_im_diff<0) a_im_diff=-a_im_diff;
                if(b_re_diff<0) b_re_diff=-b_re_diff;
                if(b_im_diff<0) b_im_diff=-b_im_diff;


                if (a_re_diff>max_forward_inverse_delta) max_forward_inverse_delta = a_re_diff;
                if (a_im_diff>max_forward_inverse_delta) max_forward_inverse_delta = a_im_diff;
                if (b_re_diff>max_forward_inverse_delta) max_forward_inverse_delta = b_re_diff;
                if (b_im_diff>max_forward_inverse_delta) max_forward_inverse_delta = b_im_diff;

                unsigned max_allowed_diff = (64*64);

                if(        (a_re_diff>max_allowed_diff)
                        || (a_im_diff>max_allowed_diff)
                        || (b_re_diff>max_allowed_diff)
                        || (b_im_diff>max_allowed_diff) ){

                    printf("error %d\n", i);
                    _Exit(1);
                }
            }

            for(unsigned i=0;i<fft_length/2;i++){
                c[i] = f[i];
                d[i] = f[i+(fft_length/2)];
            }

            lib_dsp_fft_rebuild_one_real_input(c, fft_length);
            lib_dsp_fft_rebuild_one_real_input(d, fft_length);

            lib_dsp_fft_bit_reverse(c, fft_length);
            lib_dsp_fft_bit_reverse(d, fft_length);

            lib_dsp_fft_inverse_complex(c, fft_length, (int *)sine_lut[n-3]);
            lib_dsp_fft_inverse_complex(d, fft_length, (int *)sine_lut[n-3]);

            for(unsigned i=0;i<fft_length;i++){
                int a_re_diff = a[i].re - c[i].re;
                int a_im_diff = a[i].im - c[i].im;

                int b_re_diff = b[i].re - d[i].re;
                int b_im_diff = b[i].im - d[i].im;

                if(a_re_diff<0) a_re_diff=-a_re_diff;
                if(a_im_diff<0) a_im_diff=-a_im_diff;
                if(b_re_diff<0) b_re_diff=-b_re_diff;
                if(b_im_diff<0) b_im_diff=-b_im_diff;

                if (a_re_diff>max_forward_inverse_two_real_delta)
                    max_forward_inverse_two_real_delta = a_re_diff;
                if (a_im_diff>max_forward_inverse_two_real_delta)
                    max_forward_inverse_two_real_delta = a_im_diff;
                if (b_re_diff>max_forward_inverse_two_real_delta)
                    max_forward_inverse_two_real_delta = b_re_diff;
                if (b_im_diff>max_forward_inverse_two_real_delta)
                    max_forward_inverse_two_real_delta = b_im_diff;

                unsigned max_allowed_diff = (64*64);

                if(    (a_re_diff>max_allowed_diff)
                    || (a_im_diff>max_allowed_diff)
                    || (b_re_diff>max_allowed_diff)
                    || (b_im_diff>max_allowed_diff) ){

                    printf("error %d\n", i);
                    _Exit(1);
                }
            }
        }
        printf("\tMax delta of two reals going forwards:             %d\n", max_two_real_delta);
        printf("\tMax delta of one real going forwards and reverse:  %d\n", max_forward_inverse_delta);
        printf("\tMax delta of two reals going forwards and reverse: %d\n", max_forward_inverse_two_real_delta);

    }
    return 0;
}

