// Copyright 2018-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <xs1.h>
#include <xclib.h>

#include "dsp.h"
#include "dsp_fp.h"
#include "dsp_testing.h"
#include "fft.h"
#include "unity.h"

#ifdef __XC__
#define UNSAFE unsafe
#define ALIGNED [[aligned(8)]]
#else
#define UNSAFE
#define ALIGNED __attribute__ ((aligned (8)))
#endif //__XC_

static signed sext(unsigned a, unsigned b){
    asm("sext %0, %1": "=r"(a): "r"(b));
    return a;
}

#define MAX_PROC_FRAME_LENGTH_LOG2 10
#define MAX_PROC_FRAME_LENGTH (1<<MAX_PROC_FRAME_LENGTH_LOG2)

/*
 * Computes the discrete Fourier transform (DFT) of the given complex vector, storing the result back into the vector.
 * The vector can have any length. This is a wrapper function. Returns true if successful, false otherwise (out of memory).
 */
int Fft_transform(double real[], double imag[], size_t n);

/*
 * Computes the inverse discrete Fourier transform (IDFT) of the given complex vector, storing the result back into the vector.
 * The vector can have any length. This is a wrapper function. This transform does not perform scaling, so the inverse is not a true inverse.
 * Returns true if successful, false otherwise (out of memory).
 */
int Fft_inverseTransform(double real[], double imag[], size_t n);

#define LOOPS_LOG2 4
void test_complete_fft(){

    unsigned r = 1;
    UNSAFE {
        const int32_t * UNSAFE dsp_sine_lut[11] = {
                0,
                0,
                dsp_sine_4,
                dsp_sine_8,
                dsp_sine_16,
                dsp_sine_32,
                dsp_sine_64,
                dsp_sine_128,
                dsp_sine_256,
                dsp_sine_512,
                dsp_sine_1024,
        };
    for(unsigned l=2;l<MAX_PROC_FRAME_LENGTH_LOG2;l++){
        
        for(unsigned t=0;t<1<<LOOPS_LOG2;t++){
            
            unsigned proc_frame_length = (1<<l);
            double sine_lut[(MAX_PROC_FRAME_LENGTH/2) + 1];

            dsp_float_fft_make_sine_table(sine_lut, proc_frame_length);

            dsp_complex_int32_t ALIGNED a[MAX_PROC_FRAME_LENGTH];
            dsp_complex_float_t ALIGNED A[MAX_PROC_FRAME_LENGTH];
            double real[MAX_PROC_FRAME_LENGTH], imag[MAX_PROC_FRAME_LENGTH];

            int error = 0;
            int exponent = sext(dsp_pseudo_rand_int32(&r), 5);
            for(unsigned i=0;i<proc_frame_length;i++){
                a[i].re = dsp_pseudo_rand_int32(&r)>>1;
                a[i].im = dsp_pseudo_rand_int32(&r)>>1;
                A[i].re = dsp_conv_int32_to_float(a[i].re, exponent, &error);
                A[i].im = dsp_conv_int32_to_float(a[i].im, exponent, &error);
                real[i] = A[i].re;
                imag[i] = A[i].im;
            }
            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            Fft_transform(real, imag, proc_frame_length);

            dsp_float_fft_bit_reverse(A, proc_frame_length);
            dsp_float_fft_forward_fft(A, proc_frame_length, sine_lut);

            dsp_fft_bit_reverse(a, proc_frame_length);
            dsp_fft_forward(a, proc_frame_length, (const int32_t *)dsp_sine_lut[l]);
            exponent += l;

            unsigned diff = dsp_abs_diff_vect_complex_int32(a, exponent, A, proc_frame_length, &error);
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(l, diff, "Output delta is too large");
            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            for(unsigned i=0;i<proc_frame_length;i++){
                A[i].re = real[i];
                A[i].im = imag[i];
            }

            diff = dsp_abs_diff_vect_complex_int32(a, exponent, A, proc_frame_length, &error);
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(l, diff, "Output delta is too large");
            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
        }
    }
    }
}

void test_complete_ifft(){

    unsigned r = 1;

    UNSAFE {
        const int32_t * UNSAFE dsp_sine_lut[11] = {
                0,
                0,
                dsp_sine_4,
                dsp_sine_8,
                dsp_sine_16,
                dsp_sine_32,
                dsp_sine_64,
                dsp_sine_128,
                dsp_sine_256,
                dsp_sine_512,
                dsp_sine_1024,
        };

        for(unsigned l=2;l<MAX_PROC_FRAME_LENGTH_LOG2;l++){
            
            for(unsigned t=0;t<1<<LOOPS_LOG2;t++){
                
                unsigned proc_frame_length = (1<<l);
                double sine_lut[(MAX_PROC_FRAME_LENGTH/2) + 1];

                dsp_float_fft_make_sine_table(sine_lut, proc_frame_length);

                dsp_complex_int32_t ALIGNED a[MAX_PROC_FRAME_LENGTH];
                dsp_complex_float_t ALIGNED A[MAX_PROC_FRAME_LENGTH];
                double real[MAX_PROC_FRAME_LENGTH], imag[MAX_PROC_FRAME_LENGTH];

                int error = 0;
                int exponent = sext(dsp_pseudo_rand_int32(&r), 5);
                for(unsigned i=0;i<proc_frame_length;i++){
                    a[i].re = dsp_pseudo_rand_int32(&r)>>(1+l);
                    a[i].im = dsp_pseudo_rand_int32(&r)>>(1+l);
                    A[i].re = dsp_conv_int32_to_float(a[i].re, exponent, &error);
                    A[i].im = dsp_conv_int32_to_float(a[i].im, exponent, &error);
                    real[i] = A[i].re;
                    imag[i] = A[i].im;
                }
                TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

                dsp_float_fft_bit_reverse(A, proc_frame_length);
                dsp_float_fft_inverse_fft(A, proc_frame_length, sine_lut);

                dsp_fft_bit_reverse(a, proc_frame_length);
                dsp_fft_inverse(a, proc_frame_length, (const int32_t *)dsp_sine_lut[l]);
                exponent -= l;

                Fft_inverseTransform(real, imag, proc_frame_length);

                unsigned diff = dsp_abs_diff_vect_complex_int32(a, exponent, A, proc_frame_length, &error);
                TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(1<<l, diff, "Output delta is too large");
                TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

                for(unsigned i=0;i<proc_frame_length;i++){
                    A[i].re = real[i];
                    A[i].im = imag[i];
                }

                diff = dsp_abs_diff_vect_complex_int32(a, exponent, A, proc_frame_length, &error);
                TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(1<<l, diff, "Output delta is too large");
                TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
            }
        }
    }
}

void test_bit_reverse(){

    unsigned r = 1;
    UNSAFE {

    for(unsigned l=2;l<MAX_PROC_FRAME_LENGTH_LOG2;l++){
        
        for(unsigned t=0;t<1<<LOOPS_LOG2;t++){
            
            unsigned proc_frame_length = (1<<l);

            dsp_complex_int32_t ALIGNED a[MAX_PROC_FRAME_LENGTH];
            dsp_complex_float_t ALIGNED A[MAX_PROC_FRAME_LENGTH];

            int error = 0;
            int exponent = sext(dsp_pseudo_rand_int32(&r), 5);
            for(unsigned i=0;i<proc_frame_length;i++){
                a[i].re = dsp_pseudo_rand_int32(&r)>>1;
                a[i].im = dsp_pseudo_rand_int32(&r)>>1;
                A[i].re = dsp_conv_int32_to_float(a[i].re, exponent, &error);
                A[i].im = dsp_conv_int32_to_float(a[i].im, exponent, &error);
            }
            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");


            dsp_float_fft_bit_reverse(A, proc_frame_length);

            dsp_fft_bit_reverse(a, proc_frame_length);
//            exponent += l;

            unsigned diff = dsp_abs_diff_vect_complex_int32(a, exponent, A, proc_frame_length, &error);
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(l, diff, "Output delta is too large");
            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

        }
    }
    }


}

void test_forward_fft(){

    unsigned r = 1;
    UNSAFE {
        const int32_t * UNSAFE dsp_sine_lut[11] = {
                0,
                0,
                dsp_sine_4,
                dsp_sine_8,
                dsp_sine_16,
                dsp_sine_32,
                dsp_sine_64,
                dsp_sine_128,
                dsp_sine_256,
                dsp_sine_512,
                dsp_sine_1024,
        };
    for(unsigned l=2;l<MAX_PROC_FRAME_LENGTH_LOG2;l++){
        
        for(unsigned t=0;t<1<<LOOPS_LOG2;t++){
            
            unsigned proc_frame_length = (1<<l);
            double sine_lut[(MAX_PROC_FRAME_LENGTH/2) + 1];

            dsp_float_fft_make_sine_table(sine_lut, proc_frame_length);

            dsp_complex_int32_t ALIGNED a[MAX_PROC_FRAME_LENGTH];
            dsp_complex_float_t ALIGNED A[MAX_PROC_FRAME_LENGTH];

            int error = 0;
            int exponent = sext(dsp_pseudo_rand_int32(&r), 5);
            for(unsigned i=0;i<proc_frame_length;i++){
                a[i].re = dsp_pseudo_rand_int32(&r)>>1;
                a[i].im = dsp_pseudo_rand_int32(&r)>>1;
                A[i].re = dsp_conv_int32_to_float(a[i].re, exponent, &error);
                A[i].im = dsp_conv_int32_to_float(a[i].im, exponent, &error);
            }
            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            dsp_float_fft_forward_fft(A, proc_frame_length, sine_lut);

            dsp_fft_forward(a, proc_frame_length, (const int32_t *)dsp_sine_lut[l]);
            exponent += l;

            unsigned diff = dsp_abs_diff_vect_complex_int32(a, exponent, A, proc_frame_length, &error);
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(l, diff, "Output delta is too large");
            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
        }
    }
    }
}

void test_inverse_fft(){

    unsigned r = 1;

    UNSAFE {
        const int32_t * UNSAFE dsp_sine_lut[11] = {
                0,
                0,
                dsp_sine_4,
                dsp_sine_8,
                dsp_sine_16,
                dsp_sine_32,
                dsp_sine_64,
                dsp_sine_128,
                dsp_sine_256,
                dsp_sine_512,
                dsp_sine_1024,
        };

        for(unsigned l=2;l<MAX_PROC_FRAME_LENGTH_LOG2;l++){
            
            for(unsigned t=0;t<1<<LOOPS_LOG2;t++){
                
                unsigned proc_frame_length = (1<<l);
                double sine_lut[(MAX_PROC_FRAME_LENGTH/2) + 1];

                dsp_float_fft_make_sine_table(sine_lut, proc_frame_length);

                dsp_complex_int32_t ALIGNED a[MAX_PROC_FRAME_LENGTH];
                dsp_complex_float_t ALIGNED A[MAX_PROC_FRAME_LENGTH];

                int error = 0;
                int exponent = sext(dsp_pseudo_rand_int32(&r), 5);
                for(unsigned i=0;i<proc_frame_length;i++){
                    a[i].re = dsp_pseudo_rand_int32(&r)>>(1+l);
                    a[i].im = dsp_pseudo_rand_int32(&r)>>(1+l);
                    A[i].re = dsp_conv_int32_to_float(a[i].re, exponent, &error);
                    A[i].im = dsp_conv_int32_to_float(a[i].im, exponent, &error);
                }
                TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

                dsp_float_fft_inverse_fft(A, proc_frame_length, sine_lut);

                dsp_fft_inverse(a, proc_frame_length, (const int32_t *)dsp_sine_lut[l]);
                exponent -= l;

                unsigned diff = dsp_abs_diff_vect_complex_int32(a, exponent, A, proc_frame_length, &error);
                TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(1<<l, diff, "Output delta is too large");
                TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
            }
        }
    }
}

void test_split_spectrum(){

    unsigned r = 1;
    UNSAFE {

    for(unsigned l=3;l<MAX_PROC_FRAME_LENGTH_LOG2;l++){
        
        for(unsigned t=0;t<1<<LOOPS_LOG2;t++){
            
            unsigned proc_frame_length = (1<<l);

            dsp_complex_int32_t ALIGNED a[MAX_PROC_FRAME_LENGTH];
            dsp_complex_float_t ALIGNED A[MAX_PROC_FRAME_LENGTH];

            int error = 0;
            int exponent = sext(dsp_pseudo_rand_int32(&r), 5);
            for(unsigned i=0;i<proc_frame_length;i++){
                a[i].re = dsp_pseudo_rand_int32(&r)>>1;
                a[i].im = dsp_pseudo_rand_int32(&r)>>1;
                A[i].re = dsp_conv_int32_to_float(a[i].re, exponent, &error);
                A[i].im = dsp_conv_int32_to_float(a[i].im, exponent, &error);
            }
            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            dsp_float_fft_split_spectrum(A, proc_frame_length);
            dsp_fft_split_spectrum(a, proc_frame_length);

            unsigned diff = dsp_abs_diff_vect_complex_int32(a, exponent, A, proc_frame_length, &error);
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(l, diff, "Output delta is too large");
            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

        }
    }
    }
}

void test_merge_spectra(){

    unsigned r = 1;
    UNSAFE {

    for(unsigned l=3;l<MAX_PROC_FRAME_LENGTH_LOG2;l++){
        
        for(unsigned t=0;t<1<<LOOPS_LOG2;t++){
            
            unsigned proc_frame_length = (1<<l);

            dsp_complex_int32_t ALIGNED a[MAX_PROC_FRAME_LENGTH];
            dsp_complex_float_t ALIGNED A[MAX_PROC_FRAME_LENGTH];

            int error = 0;
            int exponent = sext(dsp_pseudo_rand_int32(&r), 5);
            for(unsigned i=0;i<proc_frame_length;i++){
                a[i].re = dsp_pseudo_rand_int32(&r)>>1;
                a[i].im = dsp_pseudo_rand_int32(&r)>>1;
                A[i].re = dsp_conv_int32_to_float(a[i].re, exponent, &error);
                A[i].im = dsp_conv_int32_to_float(a[i].im, exponent, &error);
            }
            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            dsp_float_fft_merge_spectra(A, proc_frame_length);
            dsp_fft_merge_spectra(a, proc_frame_length);

            unsigned diff = dsp_abs_diff_vect_complex_int32(a, exponent, A, proc_frame_length, &error);
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(l, diff, "Output delta is too large");
            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

        }
    }
    }
}

void test_bit_reverse_and_forward_real(){


    unsigned r = 1;
    UNSAFE {
    for(unsigned l=3;l<MAX_PROC_FRAME_LENGTH_LOG2;l++){
        
        for(unsigned t=0;t<1<<LOOPS_LOG2;t++){
            
            unsigned proc_frame_length = (1<<l);
            double sine_lut[(MAX_PROC_FRAME_LENGTH/2) + 1];

            dsp_float_fft_make_sine_table(sine_lut, proc_frame_length);

            int32_t ALIGNED a[MAX_PROC_FRAME_LENGTH];
            dsp_complex_float_t ALIGNED A[MAX_PROC_FRAME_LENGTH];

            int error = 0;
            int exponent = sext(dsp_pseudo_rand_int32(&r), 5);
            for(unsigned i=0;i<proc_frame_length;i++){
                a[i] = dsp_pseudo_rand_int32(&r)>>1;
                A[i].re = dsp_conv_int32_to_float(a[i], exponent, &error);
                A[i].im = 0.0;
            }
            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");


            dsp_float_fft_bit_reverse(A, proc_frame_length);
            dsp_float_fft_forward_fft(A, proc_frame_length, sine_lut);
            dsp_float_fft_split_spectrum(A, proc_frame_length);

            switch(l){
                case 3: dsp_fft_bit_reverse_and_forward_real(a, proc_frame_length, dsp_sine_4, dsp_sine_8); break;
                case 4: dsp_fft_bit_reverse_and_forward_real(a, proc_frame_length, dsp_sine_8, dsp_sine_16); break;
                case 5: dsp_fft_bit_reverse_and_forward_real(a, proc_frame_length, dsp_sine_16, dsp_sine_32); break;
                case 6: dsp_fft_bit_reverse_and_forward_real(a, proc_frame_length, dsp_sine_32, dsp_sine_64); break;
                case 7: dsp_fft_bit_reverse_and_forward_real(a, proc_frame_length, dsp_sine_64, dsp_sine_128); break;
                case 8: dsp_fft_bit_reverse_and_forward_real(a, proc_frame_length, dsp_sine_128, dsp_sine_256); break;
                case 9: dsp_fft_bit_reverse_and_forward_real(a, proc_frame_length, dsp_sine_256, dsp_sine_512); break;
                case 10: dsp_fft_bit_reverse_and_forward_real(a, proc_frame_length, dsp_sine_512, dsp_sine_1024); break;
            }
            exponent += l;
            dsp_complex_int32_t *b = (dsp_complex_int32_t *)a;

            unsigned diff = dsp_abs_diff_vect_complex_int32(b, exponent, A, proc_frame_length/2, &error);
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(l, diff, "Output delta is too large");
            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

        }
    }
    }

}


void test_fft_bit_reverse_and_inverse_real(){


    unsigned r = 1;
    UNSAFE {

    for(unsigned l=3;l<MAX_PROC_FRAME_LENGTH_LOG2;l++){
        
        for(unsigned t=0;t<1<<LOOPS_LOG2;t++){
            unsigned proc_frame_length = (1<<l);

            int32_t ALIGNED a[MAX_PROC_FRAME_LENGTH];
            int32_t ALIGNED b[MAX_PROC_FRAME_LENGTH];

            for(unsigned i=0;i<proc_frame_length;i++){
                a[i] = dsp_pseudo_rand_int32(&r)>>1;
                b[i] = a[i];
            }

            switch(l){
                case 3: dsp_fft_bit_reverse_and_forward_real(a, proc_frame_length, dsp_sine_4, dsp_sine_8); break;
                case 4: dsp_fft_bit_reverse_and_forward_real(a, proc_frame_length, dsp_sine_8, dsp_sine_16); break;
                case 5: dsp_fft_bit_reverse_and_forward_real(a, proc_frame_length, dsp_sine_16, dsp_sine_32); break;
                case 6: dsp_fft_bit_reverse_and_forward_real(a, proc_frame_length, dsp_sine_32, dsp_sine_64); break;
                case 7: dsp_fft_bit_reverse_and_forward_real(a, proc_frame_length, dsp_sine_64, dsp_sine_128); break;
                case 8: dsp_fft_bit_reverse_and_forward_real(a, proc_frame_length, dsp_sine_128, dsp_sine_256); break;
                case 9: dsp_fft_bit_reverse_and_forward_real(a, proc_frame_length, dsp_sine_256, dsp_sine_512); break;
                case 10: dsp_fft_bit_reverse_and_forward_real(a, proc_frame_length, dsp_sine_512, dsp_sine_1024); break;
            }
            switch(l){
                case 3: dsp_fft_bit_reverse_and_inverse_real(a, proc_frame_length, dsp_sine_4, dsp_sine_8); break;
                case 4: dsp_fft_bit_reverse_and_inverse_real(a, proc_frame_length, dsp_sine_8, dsp_sine_16); break;
                case 5: dsp_fft_bit_reverse_and_inverse_real(a, proc_frame_length, dsp_sine_16, dsp_sine_32); break;
                case 6: dsp_fft_bit_reverse_and_inverse_real(a, proc_frame_length, dsp_sine_32, dsp_sine_64); break;
                case 7: dsp_fft_bit_reverse_and_inverse_real(a, proc_frame_length, dsp_sine_64, dsp_sine_128); break;
                case 8: dsp_fft_bit_reverse_and_inverse_real(a, proc_frame_length, dsp_sine_128, dsp_sine_256); break;
                case 9: dsp_fft_bit_reverse_and_inverse_real(a, proc_frame_length, dsp_sine_256, dsp_sine_512); break;
                case 10: dsp_fft_bit_reverse_and_inverse_real(a, proc_frame_length, dsp_sine_512, dsp_sine_1024); break;
            }

            for(unsigned i=0;i<proc_frame_length;i++){
                TEST_ASSERT_INT32_WITHIN_MESSAGE(32*l, a[i], b[i], "Output delta is too large");
            }

        }
    }
    }

}
