// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <platform.h>
#include <xs1.h>
#include <xclib.h>
#include <stdio.h>
#include "lib_dsp_transforms.h"

//#define DEBUG_PRINT
//#define SHORT_FFT_MORE_ACCURATE
//#define CONDITIONAL_SHIFT
//#define DEBUG_PRINT_STORES
//#define PRINT_FFT_INDEXING

inline int shiftdown(int sum) {
#ifdef SHORT_FFT_MORE_ACCURATE
#ifdef CONDITIONAL_SHIFT
   //This round-to-odd mode is more noisy: if ((sum & 3) == 3) return (sum >> 1) + 1;

   //this conditional rounding reduces the noice of the SHORT_FFT_MORE_ACCURATE
   //It takes 19% longer to compute but the noise is very similar to #ifndef SHORT_FFT_MORE_ACCURATE
   //Conclusion: Keeping one additional bit before the addtion int re_0 = shiftdown(tRe_0 + sRe2_0);
   //takes 19% longer with no improvement in accuracy.
   if ((sum & 3) == 1) return (sum >> 1) + 1;
   return sum >> 1;
#else
   // This mode causes considerably more noise in the spectrum.
   return sum >> 1;
#endif
#else
   return sum;  // don't shift, shift is done before the summation
#endif
}

#pragma unsafe arrays
void lib_dsp_fft_bit_reverse( lib_dsp_fft_complex_t pts[], int N )
{
    unsigned int shift = clz(N);
    for(unsigned int i = 1; i < N-1; i++) {
        unsigned int rev = bitrev(i) >> (shift + 1);
        if (rev > i) {
            unsigned im1, re1, im2, re2;
            asm("ldd %0,%1,%2[%3]" : "=r"(im1), "=r" (re1): "r"(pts), "r" (i));
            asm("ldd %0,%1,%2[%3]" : "=r"(im2), "=r" (re2): "r"(pts), "r" (rev));
            asm("std %0,%1,%2[%3]" :: "r"(im1), "r" (re1), "r"(pts), "r" (rev));
            asm("std %0,%1,%2[%3]" :: "r"(im2), "r" (re2), "r"(pts), "r" (i));
        }
    }
}

#pragma unsafe arrays
void lib_dsp_fft_bit_reverse_twoshort( lib_dsp_fft_complex_twoshort_t pts[], int N )
{
    // same functionality as lib_dsp_fft_bit_reverse!
    unsigned int shift = clz(N);
    for(unsigned int i = 1; i < N-1; i++) {
        unsigned int rev = bitrev(i) >> (shift + 1);
        if (rev > i) {
            unsigned im1, re1, im2, re2;
            asm("ldd %0,%1,%2[%3]" : "=r"(im1), "=r" (re1): "r"(pts), "r" (i));
            asm("ldd %0,%1,%2[%3]" : "=r"(im2), "=r" (re2) : "r"(pts), "r" (rev));
            asm("std %0,%1,%2[%3]" :: "r"(im1), "r" (re1), "r"(pts), "r" (rev));
            asm("std %0,%1,%2[%3]" :: "r"(im2), "r" (re2), "r"(pts), "r" (i));
        }
    }
}

#pragma unsafe arrays
void lib_dsp_fft_bit_reverse_short( lib_dsp_fft_complex_short_t pts[], int N )
{
    // same functionality as lib_dsp_fft_bit_reverse!
    unsigned int shift = clz(N);
    for(unsigned int i = 1; i < N-1; i++) {
        unsigned int rev = bitrev(i) >> (shift + 1);
        if (rev > i) {
            unsigned im_re_1, im_re_2;
            asm("ldw %0,%1[%2]" : "=r"(im_re_1): "r"(pts), "r" (i));
            asm("ldw %0,%1[%2]" : "=r"(im_re_2): "r"(pts), "r" (rev));
            asm("stw %0,%1[%2]" :: "r"(im_re_1), "r"(pts), "r" (rev));
            asm("stw %0,%1[%2]" :: "r"(im_re_2), "r"(pts), "r" (i));
        }
    }
}

#pragma unsafe arrays
void lib_dsp_fft_forward_tworeals( lib_dsp_fft_complex_t two_re[], lib_dsp_fft_complex_t two_im[], int N, const int sine[] )
{

    lib_dsp_fft_bit_reverse(two_re, N);
    lib_dsp_fft_forward_complex(two_re, N, sine);

    two_im[0].re = 0;
    two_im[0].im = 0;
    for(int i = N >> 1; i != 0; i--) {
        two_im[i].re = (two_re[i].im - two_re[N-i].im)>>1;
        two_im[N-i].re = -two_im[i].re;
        two_im[i].im = (two_re[N-i].re - two_re[i].re)>>1;
        two_im[N-i].im = -two_im[i].im;
    }
    for(int i = N >> 1; i != 0; i--) {
        two_re[i].re = (two_re[i].re + two_re[N-i].re)>>1;
        two_re[N-i].re = two_re[i].re;
        two_re[i].im = (two_re[i].im + two_re[N-i].im)>>1;
        two_re[N-i].im = two_re[i].im;
    }
}

#pragma unsafe arrays
void lib_dsp_fft_forward_tworeals_short( lib_dsp_fft_complex_short_t two_re[], lib_dsp_fft_complex_short_t two_im[], int N, const short int sine[] )
{

    lib_dsp_fft_bit_reverse_short(two_re, N);
    lib_dsp_fft_forward_complex_short(two_re, N, sine);

    two_im[0].re = 0;
    two_im[0].im = 0;
    for(int i = N >> 1; i != 0; i--) {
        two_im[i].re = (two_re[i].im - two_re[N-i].im)>>1;
        two_im[N-i].re = -two_im[i].re;
        two_im[i].im = (two_re[N-i].re - two_re[i].re)>>1;
        two_im[N-i].im = -two_im[i].im;
    }
    for(int i = N >> 1; i != 0; i--) {
        two_re[i].re = (two_re[i].re + two_re[N-i].re) >> 1;
        two_re[N-i].re = two_re[i].re;
        two_re[i].im = (two_re[i].im + two_re[N-i].im)>>1;
        two_re[N-i].im = two_re[i].im;
    }
}



#pragma unsafe arrays
void lib_dsp_fft_inverse_tworeals( lib_dsp_fft_complex_t two_re[], lib_dsp_fft_complex_t two_im[], int N, const int sine[] )
{
    for(int i = N >> 1; i != 0; i--) {
        two_re[i].re = two_re[i].re + two_im[N-i].im;
        two_re[N-i].re = two_re[N-i].re + two_im[i].im;
        two_re[i].im = two_re[i].im + two_im[i].re;
        two_re[N-i].im = two_re[N-i].im + two_im[N-i].re;
    }

    lib_dsp_fft_bit_reverse(two_re, N);
    lib_dsp_fft_inverse_complex(two_re, N, sine);

}

#pragma unsafe arrays
void lib_dsp_fft_inverse_tworeals_short( lib_dsp_fft_complex_short_t two_re[], lib_dsp_fft_complex_short_t two_im[], int N, const short int sine[] )
{
    for(int i = N >> 1; i != 0; i--) {
        // right shift (>> 1) is to convert from Q15 to Q14 format.
        // This is to avoid integer overflow in the lib_dsp_fft_inverse_complex_short
        two_re[i].re = (two_re[i].re + two_im[N-i].im) >> 1;
        two_re[N-i].re = (two_re[N-i].re + two_im[i].im) >> 1;
        two_re[i].im = (two_re[i].im + two_im[i].re) >> 1;
        two_re[N-i].im = (two_re[N-i].im + two_im[N-i].re) >> 1;
    }

    lib_dsp_fft_bit_reverse_short(two_re, N);
    lib_dsp_fft_inverse_complex_short(two_re, N, sine);

}

#pragma unsafe arrays
void lib_dsp_fft_forward_complex_xs1( lib_dsp_fft_complex_t pts[], int N, const int sine[] )
{
    unsigned int shift = 30-clz(N);
    for(unsigned int step = 2 ; step <= N; step = step * 2, shift--) {
        unsigned int step2 = step >> 1;
        unsigned int step4 = step2 >> 1;
        unsigned int k;
        for(k = 0; k < step4 + (step2&1); k++) {
            int rRe = sine[(N>>2)-(k<<shift)];
            int rIm = sine[k<<shift];
            for(int block = k+N-step; block >= 0; block-=step) {
                int tRe = pts[block].re;
                int tIm = pts[block].im;
                int tRe2 = pts[block + step2].re;
                int tIm2 = pts[block + step2].im;

                int h;
                unsigned l;
                int sRe2, sIm2;
                {h,l} = macs(tRe2, rRe, 0, 0x80000000);
                {h,l} = macs(tIm2, rIm, h, l);
                sRe2 = h;
                {h,l} = macs(tRe2, -rIm, 0, 0x80000000);
                {h,l} = macs(tIm2, rRe, h, l);
                sIm2 = h;
                tRe >>= 1;
                tIm >>= 1;
                pts[block].re = tRe + sRe2;
                pts[block].im = tIm + sIm2;
                pts[block+step2].re = tRe - sRe2;
                pts[block+step2].im = tIm - sIm2;

            }
        }
        for(k=(step2 & 1); k < step4; k++) {
            int rRe = -sine[k<<shift];
            int rIm = sine[(N>>2)-(k<<shift)];
            for(int block = k+step4+N-step; block >= 0; block-=step) {
                int tRe = pts[block].re;
                int tIm = pts[block].im;
                int tRe2 = pts[block + step2].re;
                int tIm2 = pts[block + step2].im;

                int h;
                unsigned l;
                int sRe2, sIm2;
                {h,l} = macs(tRe2, rRe, 0, 0x80000000);
                {h,l} = macs(tIm2, rIm, h, l);
                sRe2 = h;
                {h,l} = macs(tRe2, -rIm, 0, 0x80000000);
                {h,l} = macs(tIm2, rRe, h, l);
                sIm2 = h;
                tRe >>= 1;
                tIm >>= 1;
                pts[block].re = tRe + sRe2;
                pts[block].im = tIm + sIm2;
                pts[block+step2].re = tRe - sRe2;
                pts[block+step2].im = tIm - sIm2;

            }
        }
    }
}



void check_overflow(int h) {
    if(!(h==0xffffffff || h==0)) {
        printf("ERROR: For short values, expecting upper word of macs result to be all one (negative) or all 0 but got %x\n",h);
    }
}
#ifdef SHORT_FFT_MORE_ACCURATE
#define SHORT_FFT_BASE 0x8000
#define SHORT_FFT_RESULT_RIGHT_SHIFT 1
#else
#define SHORT_FFT_BASE 0x8000
#define SHORT_FFT_RESULT_RIGHT_SHIFT 0
#endif
#pragma unsafe arrays
void lib_dsp_fft_forward_complex_twoshort( lib_dsp_fft_complex_twoshort_t pts[], int N, const short sine[] )
{
    unsigned int shift = 30-clz(N);
    for(unsigned int step = 2 ; step <= N; step = step * 2, shift--) {
        unsigned int step2 = step >> 1;
        unsigned int step4 = step2 >> 1;
        unsigned int k;
        for(k = 0; k < step4 + (step2&1); k++) {
            short rRe = sine[(N>>2)-(k<<shift)];
            short rIm = sine[k<<shift];
            for(int block = k+N-step; block >= 0; block-=step) {
                unsigned tRe; // = pts[block].re;
                unsigned tIm; // = pts[block].im;
                asm volatile("ldd %0,%1,%2[%3]" : "=r"(tIm), "=r" (tRe): "r"(pts), "r" (block));
                short tRe_1 = (short) (tRe >> 16);
                short tRe_0 = (short) (tRe);
                short tIm_1 = (short) (tIm >> 16);
                short tIm_0 = (short) (tIm);

                unsigned tRe2; //= pts[block + step2].re;
                unsigned tIm2; //= pts[block + step2].im;
                asm volatile("ldd %0,%1,%2[%3]" : "=r"(tIm2), "=r" (tRe2): "r"(pts), "r" (block+step2));
                short tRe2_1 = (short) (tRe2 >> 16);
                short tRe2_0 = (short) (tRe2);
                short tIm2_1 = (short) (tIm2 >> 16);
                short tIm2_0 = (short) (tIm2);

#ifdef DEBUG_PRINT
                printf("---- first index=%d, second index=%d\n" ,block, block+step2);
                printf("tRe2_1: %x, tRe2_0: %x, tIm2_1: %x, tIm2_0: %x\n"
                        ,tRe2_1, tRe2_0, tIm2_1, tIm2_0);
#endif

                int h;
                unsigned l;
#ifdef SHORT_FFT_MORE_ACCURATE
                int sRe2_0, sRe2_1, sIm2_0, sIm2_1;
#else
                short sRe2_0, sRe2_1, sIm2_0, sIm2_1;
#endif
                // macs operates on int so short has to be cast to int
                {h,l} = macs((int)tRe2_0, (int)rRe, 0, SHORT_FFT_BASE);
                {h,l} = macs((int)tIm2_0, (int)rIm, h, l);
#ifdef SHORT_FFT_MORE_ACCURATE
                sRe2_0 =  (int) (l>>15);
#else
                sRe2_0 =  (short) (l>>16);
#endif

                {h,l} = macs((int)tRe2_0, (int)-rIm, 0, SHORT_FFT_BASE);
                {h,l} = macs((int)tIm2_0, (int)rRe, h, l);
#ifdef SHORT_FFT_MORE_ACCURATE
                sIm2_0 =  (int) (l>>15);
#else
                sIm2_0 =  (short) (l>>16);
#endif

#ifdef DEBUG_PRINT_MACCS
                printf("macs output: sRe2_0: %x, sIm2_0: %x\n"
                        ,sRe2_0, sIm2_0);
#endif
                {h,l} = macs((int)tRe2_1, (int)rRe, 0, SHORT_FFT_BASE);
                {h,l} = macs((int)tIm2_1, (int)rIm, h, l);
#ifdef SHORT_FFT_MORE_ACCURATE
                sRe2_1 =  (int) (l>>15);
#else
                sRe2_1 =  (short) (l>>16);
#endif
                {h,l} = macs((int)tRe2_1, (int)-rIm, 0, SHORT_FFT_BASE);
                {h,l} = macs((int)tIm2_1, (int)rRe, h, l);
#ifdef SHORT_FFT_MORE_ACCURATE
                sIm2_1 =  (int) (l>>15);
#else
                sIm2_1 =  (short) (l>>16);
#endif

#ifdef DEBUG_PRINT_MACCS
                if(sRe2_0 != sRe2_1) {
                    printf("ERROR: sRe2_0 %08x is not equal to sRe2_1 %08x\n",sRe2_0, sRe2_1);
                }
                if(sIm2_0 != sIm2_1) {
                    printf("ERROR: sIm2_0 %08x is not equal to sIm2_1 %08x \n",sIm2_0, sIm2_1);
                }
#endif

#ifndef SHORT_FFT_MORE_ACCURATE
                tRe_0 >>= 1;
                tIm_0 >>= 1;
                tRe_1 >>= 1;
                tIm_1 >>= 1;
#endif

                int re_0 = shiftdown(tRe_0 + sRe2_0);
                int re_1 = shiftdown(tRe_1 + sRe2_1);

                int im_0 = shiftdown(tIm_0 + sIm2_0);
                int im_1 = shiftdown(tIm_1 + sIm2_1);

                int re_0_step = shiftdown(tRe_0 - sRe2_0);
                int re_1_step = shiftdown(tRe_1 - sRe2_1);

                int im_0_step = shiftdown(tIm_0 - sIm2_0);
                int im_1_step = shiftdown(tIm_1 - sIm2_1);

                // pack 16 bit values into 32 bit words before storing to memory
                re_0 &= 0xffff; // lower 16 bit
                re_0 |= (re_1 << 16); // or in upper 16 bit

                re_0_step &= 0xffff; // lower 16 bit
                re_0_step |= (re_1_step << 16); // or in upper 16 bit

                im_0 &= 0xffff; // lower 16 bit
                im_0 |= (im_1 << 16); // or in upper 16 bit

                im_0_step &= 0xffff; // lower 16 bit
                im_0_step |= (im_1_step << 16); // or in upper 16 bit


                asm("std %0,%1,%2[%3]" :: "r"(im_0), "r" (re_0), "r"(pts), "r" (block));
                asm("std %0,%1,%2[%3]" :: "r"(im_0_step), "r" (re_0_step), "r"(pts), "r" (block+step2));
#ifdef DEBUG_PRINT_STORES
                printf("stored at first  index=%d: re: %08x, im: %08x\n"
                        ,block ,re_0, im_0);
                printf("stored at second index=%d: re: %08x, im: %08x\n"
                        ,block+step2,re_0_step, im_0_step);
#endif

            }
        }
        for(k=(step2 & 1); k < step4; k++) {
            short rRe = -sine[k<<shift];
            short rIm = sine[(N>>2)-(k<<shift)];
            for(int block = k+step4+N-step; block >= 0; block-=step) {
                unsigned tRe; // = pts[block].re;
                unsigned tIm; // = pts[block].im;
                asm volatile("ldd %0,%1,%2[%3]" : "=r"(tIm), "=r" (tRe): "r"(pts), "r" (block));
                short tRe_1 = (short) (tRe >> 16);
                short tRe_0 = (short) (tRe);
                short tIm_1 = (short) (tIm >> 16);
                short tIm_0 = (short) (tIm);

                unsigned tRe2; //= pts[block + step2].re;
                unsigned tIm2; //= pts[block + step2].im;
                asm volatile("ldd %0,%1,%2[%3]" : "=r"(tIm2), "=r" (tRe2): "r"(pts), "r" (block+step2));
                short tRe2_1 = (short) (tRe2 >> 16);
                short tRe2_0 = (short) (tRe2);
                short tIm2_1 = (short) (tIm2 >> 16);
                short tIm2_0 = (short) (tIm2);

                int h;
                unsigned l;

#ifdef SHORT_FFT_MORE_ACCURATE
                int sRe2_0, sRe2_1, sIm2_0, sIm2_1;
#else
                short sRe2_0, sRe2_1, sIm2_0, sIm2_1;
#endif
                // macs operates on int so short has to be cast to int
                {h,l} = macs((int)tRe2_0, (int)rRe, 0, SHORT_FFT_BASE);
                {h,l} = macs((int)tIm2_0, (int)rIm, h, l);
#ifdef SHORT_FFT_MORE_ACCURATE
                sRe2_0 =  (int) (l>>15);
#else
                sRe2_0 =  (short) (l>>16);
#endif

                {h,l} = macs((int)tRe2_0, (int)-rIm, 0, SHORT_FFT_BASE);
                {h,l} = macs((int)tIm2_0, (int)rRe, h, l);
#ifdef SHORT_FFT_MORE_ACCURATE
                sIm2_0 =  (int) (l>>15);
#else
                sIm2_0 =  (short) (l>>16);
#endif

#ifdef DEBUG_PRINT_MACCS
                printf("macs output: sRe2_0: %x, sIm2_0: %x\n"
                        ,sRe2_0, sIm2_0);
#endif
                {h,l} = macs((int)tRe2_1, (int)rRe, 0, SHORT_FFT_BASE);
                {h,l} = macs((int)tIm2_1, (int)rIm, h, l);
#ifdef SHORT_FFT_MORE_ACCURATE
                sRe2_1 =  (int) (l>>15);
#else
                sRe2_1 =  (short) (l>>16);
#endif
                {h,l} = macs((int)tRe2_1, (int)-rIm, 0, SHORT_FFT_BASE);
                {h,l} = macs((int)tIm2_1, (int)rRe, h, l);
#ifdef SHORT_FFT_MORE_ACCURATE
                sIm2_1 =  (int) (l>>15);
#else
                sIm2_1 =  (short) (l>>16);
#endif

#ifdef DEBUG_PRINT_MACCS
                if(sRe2_0 != sRe2_1) {
                    printf("ERROR: sRe2_0 %08x is not equal to sRe2_1 %08x\n",sRe2_0, sRe2_1);
                }
                if(sIm2_0 != sIm2_1) {
                    printf("ERROR: sIm2_0 %08x is not equal to sIm2_1 %08x \n",sIm2_0, sIm2_1);
                }
#endif

#ifndef SHORT_FFT_MORE_ACCURATE
                tRe_0 >>= 1;
                tIm_0 >>= 1;
                tRe_1 >>= 1;
                tIm_1 >>= 1;
#endif

                int re_0 = shiftdown(tRe_0 + sRe2_0);
                int re_1 = shiftdown(tRe_1 + sRe2_1);

                int im_0 = shiftdown(tIm_0 + sIm2_0);
                int im_1 = shiftdown(tIm_1 + sIm2_1);

                int re_0_step = shiftdown(tRe_0 - sRe2_0);
                int re_1_step = shiftdown(tRe_1 - sRe2_1);

                int im_0_step = shiftdown(tIm_0 - sIm2_0);
                int im_1_step = shiftdown(tIm_1 - sIm2_1);

                // pack 16 bit values into 32 bit words before storing to memory
                re_0 &= 0xffff; // lower 16 bit
                re_0 |= (re_1 << 16); // or in upper 16 bit

                re_0_step &= 0xffff; // lower 16 bit
                re_0_step |= (re_1_step << 16); // or in upper 16 bit

                im_0 &= 0xffff; // lower 16 bit
                im_0 |= (im_1 << 16); // or in upper 16 bit

                im_0_step &= 0xffff; // lower 16 bit
                im_0_step |= (im_1_step << 16); // or in upper 16 bit


                asm("std %0,%1,%2[%3]" :: "r"(im_0), "r" (re_0), "r"(pts), "r" (block));
                asm("std %0,%1,%2[%3]" :: "r"(im_0_step), "r" (re_0_step), "r"(pts), "r" (block+step2));
#ifdef DEBUG_PRINT_STORES
                printf("stored at first  index=%d: re: %08x, im: %08x\n"
                        ,block ,re_0, im_0);
                printf("stored at second index=%d: re: %08x, im: %08x\n"
                        ,block+step2,re_0_step, im_0_step);
#endif

            }
        }
    }
}

#define SHORT_IFFT_BASE 0x4000
#pragma unsafe arrays
void lib_dsp_fft_inverse_complex_twoshort( lib_dsp_fft_complex_twoshort_t pts[], int N, const short sine[] )
{
    unsigned int shift = 30-clz(N);
    for(unsigned int step = 2 ; step <= N; step = step * 2, shift--) {
        unsigned int step2 = step >> 1;
        unsigned int step4 = step2 >> 1;
        unsigned int k;
        for(k = 0; k < step4 + (step2&1); k++) {
            int rRe = sine[(N>>2)-(k<<shift)];
            int rIm = sine[k<<shift];
            for(int block = k+N-step; block >= 0; block-=step) {
                unsigned tRe; // = pts[block].re;
                unsigned tIm; // = pts[block].im;
                asm volatile("ldd %0,%1,%2[%3]" : "=r"(tIm), "=r" (tRe): "r"(pts), "r" (block));
                short tRe_1 = (short) (tRe >> 16);
                short tRe_0 = (short) (tRe);
                short tIm_1 = (short) (tIm >> 16);
                short tIm_0 = (short) (tIm);

                unsigned tRe2; //= pts[block + step2].re;
                unsigned tIm2; //= pts[block + step2].im;
                asm volatile("ldd %0,%1,%2[%3]" : "=r"(tIm2), "=r" (tRe2): "r"(pts), "r" (block+step2));
                short tRe2_1 = (short) (tRe2 >> 16);
                short tRe2_0 = (short) (tRe2);
                short tIm2_1 = (short) (tIm2 >> 16);
                short tIm2_0 = (short) (tIm2);

#ifdef DEBUG_PRINT
                printf("---- first index=%d, second index=%d\n" ,block, block+step2);
                printf("tRe2_1: %x, tRe2_0: %x, tIm2_1: %x, tIm2_0: %x\n"
                        ,tRe2_1, tRe2_0, tIm2_1, tIm2_0);
#endif

                int h;
                unsigned l;
                short sRe2_0, sRe2_1, sIm2_0, sIm2_1;
                // macs operates on int so short has to be cast to int
                {h,l} = macs((int)tRe2_0, (int)rRe, 0, SHORT_IFFT_BASE);
                {h,l} = macs((int)tIm2_0, (int)-rIm, h, l);
                // >> 15 because additional bit precision is available
                sRe2_0 =  (short) (l>>15);;

                {h,l} = macs((int)tRe2_0, (int)rIm, 0, SHORT_IFFT_BASE);
                {h,l} = macs((int)tIm2_0, (int)rRe, h, l);
                sIm2_0 =  (short) (l>>15);;

#ifdef DEBUG_PRINT_MACCS
                printf("macs output: sRe2_0: %x, sIm2_0: %x\n"
                        ,sRe2_0, sIm2_0);
#endif
                {h,l} = macs((int)tRe2_1, (int)rRe, 0, SHORT_IFFT_BASE);
                {h,l} = macs((int)tIm2_1, (int)-rIm, h, l);
                sRe2_1 =  (short) (l>>15);; // add 16 bit result to 32 bit word

                {h,l} = macs((int)tRe2_1, (int)rIm, 0, SHORT_IFFT_BASE);
                {h,l} = macs((int)tIm2_1, (int)rRe, h, l);
                sIm2_1 =  (short) (l>>15);;

#ifdef DEBUG_PRINT_MACCS
                if(sRe2_0 != sRe2_1) {
                    printf("ERROR: sRe2_0 %08x is not equal to sRe2_1 %08x\n",sRe2_0, sRe2_1);
                }
                if(sIm2_0 != sIm2_1) {
                    printf("ERROR: sIm2_0 %08x is not equal to sIm2_1 %08x \n",sIm2_0, sIm2_1);
                }
#endif

                // pack 16 bit values into 32 bit words
                int re_0 = (tRe_0 + sRe2_0) ;
                int re_1 = (tRe_1 + sRe2_1) ;

                int im_0 = (tIm_0 + sIm2_0) ;
                int im_1 = (tIm_1 + sIm2_1) ;

                int re_0_step = (tRe_0 - sRe2_0) ;
                int re_1_step = (tRe_1 - sRe2_1) ;

                int im_0_step = (tIm_0 - sIm2_0) ;
                int im_1_step = (tIm_1 - sIm2_1) ;

                // pack 16 bit values into 32 bit words before storing to memory
                re_0 &= 0xffff; // lower 16 bit
                re_0 |= (re_1 << 16); // or in upper 16 bit

                re_0_step &= 0xffff; // lower 16 bit
                re_0_step |= (re_1_step << 16); // or in upper 16 bit

                im_0 &= 0xffff; // lower 16 bit
                im_0 |= (im_1 << 16); // or in upper 16 bit

                im_0_step &= 0xffff; // lower 16 bit
                im_0_step |= (im_1_step << 16); // or in upper 16 bit


                asm("std %0,%1,%2[%3]" :: "r"(im_0), "r" (re_0), "r"(pts), "r" (block));
                asm("std %0,%1,%2[%3]" :: "r"(im_0_step), "r" (re_0_step), "r"(pts), "r" (block+step2));
#ifdef DEBUG_PRINT_STORES
                printf("stored at first  index=%d: re: %08x, im: %08x\n"
                        ,block ,re_0, im_0);
                printf("stored at second index=%d: re: %08x, im: %08x\n"
                        ,block+step2,re_0_step, im_0_step);
#endif

            }
        }
        for(k=(step2 & 1); k < step2-step4; k++) {
            short rRe = -sine[k<<shift];
            short rIm = sine[(N>>2)-(k<<shift)];
            for(int block = k+step4; block < k+step4+N; block+=step) {
                unsigned tRe; // = pts[block].re;
                unsigned tIm; // = pts[block].im;
                asm volatile("ldd %0,%1,%2[%3]" : "=r"(tIm), "=r" (tRe): "r"(pts), "r" (block));
                short tRe_1 = (short) (tRe >> 16);
                short tRe_0 = (short) (tRe);
                short tIm_1 = (short) (tIm >> 16);
                short tIm_0 = (short) (tIm);

                unsigned tRe2; //= pts[block + step2].re;
                unsigned tIm2; //= pts[block + step2].im;
                asm volatile("ldd %0,%1,%2[%3]" : "=r"(tIm2), "=r" (tRe2): "r"(pts), "r" (block+step2));
                short tRe2_1 = (short) (tRe2 >> 16);
                short tRe2_0 = (short) (tRe2);
                short tIm2_1 = (short) (tIm2 >> 16);
                short tIm2_0 = (short) (tIm2);

                int h;
                unsigned l;

                short sRe2_0, sRe2_1, sIm2_0, sIm2_1;
                // macs operates on int so short has to be cast to int
                {h,l} = macs((int)tRe2_0, (int)rRe, 0, SHORT_IFFT_BASE);
                {h,l} = macs((int)tIm2_0, (int)-rIm, h, l);
                sRe2_0 = (short) (l>>15);
                {h,l} = macs((int)tRe2_0, (int)rIm, 0, SHORT_IFFT_BASE);
                {h,l} = macs((int)tIm2_0, (int)rRe, h, l);
                sIm2_0 = (short) (l>>15);

                {h,l} = macs((int)tRe2_1, (int)rRe, 0, SHORT_IFFT_BASE);
                {h,l} = macs((int)tIm2_1, (int)-rIm, h, l);
                sRe2_1 = (short) (l>>15);
                {h,l} = macs((int)tRe2_1, (int)rIm, 0, SHORT_IFFT_BASE);
                {h,l} = macs((int)tIm2_1, (int)rRe, h, l);
                sIm2_1 = (short) (l>>15);

                int re_0 = (tRe_0 + sRe2_0) ;
                int re_1 = (tRe_1 + sRe2_1) ;

                int im_0 = (tIm_0 + sIm2_0) ;
                int im_1 = (tIm_1 + sIm2_1) ;

                int re_0_step = (tRe_0 - sRe2_0) ;
                int re_1_step = (tRe_1 - sRe2_1) ;

                int im_0_step = (tIm_0 - sIm2_0) ;
                int im_1_step = (tIm_1 - sIm2_1) ;

                // pack 16 bit values into 32 bit words
                re_0 &= 0xffff; // lower 16 bit
                re_0 |= (re_1 << 16); // or in upper 16 bit

                re_0_step &= 0xffff; // lower 16 bit
                re_0_step |= (re_1_step << 16); // or in upper 16 bit

                im_0 &= 0xffff; // lower 16 bit
                im_0 |= (im_1 << 16); // or in upper 16 bit

                im_0_step &= 0xffff; // lower 16 bit
                im_0_step |= (im_1_step << 16); // or in upper 16 bit

                asm("std %0,%1,%2[%3]" :: "r"(im_0), "r" (re_0), "r"(pts), "r" (block));
                asm("std %0,%1,%2[%3]" :: "r"(im_0_step), "r" (re_0_step), "r"(pts), "r" (block+step2));

            }
        }
    }
}

// Note: for an extra bit of precision change the 8 lines that use 0x80000000 or l>>31.
#pragma unsafe arrays
void lib_dsp_fft_inverse_complex_xs1( lib_dsp_fft_complex_t pts[], int N, const int sine[] )
{
    unsigned int shift = 30-clz(N);
    for(unsigned int step = 2 ; step <= N; step = step * 2, shift--) {
        unsigned int step2 = step >> 1;
        unsigned int step4 = step2 >> 1;
        unsigned int k;
        for(k = 0; k < step4 + (step2&1); k++) {
            int rRe = sine[(N>>2)-(k<<shift)];
            int rIm = sine[k<<shift];
            for(int block = k; block < k+N; block+=step) {
                int tRe = pts[block].re;
                int tIm = pts[block].im;
                int tRe2 = pts[block + step2].re;
                int tIm2 = pts[block + step2].im;
                int h;
                unsigned l;
                int sRe2, sIm2;
                {h,l} = macs(tRe2, rRe, 0, 0x80000000);  // Make this 0x40000000
                {h,l} = macs(tIm2, -rIm, h, l);
                sRe2 = h << 1;// | l >> 31;              // And include this part
                {h,l} = macs(tRe2, rIm, 0, 0x80000000);
                {h,l} = macs(tIm2, rRe, h, l);
                sIm2 = h << 1;// | l >> 31;
                pts[block].re = tRe + sRe2;
                pts[block].im = tIm + sIm2;
                pts[block+step2].re = tRe - sRe2;
                pts[block+step2].im = tIm - sIm2;
            }
        }
        for(k=(step2 & 1); k < step2-step4; k++) {
            int rRe = -sine[k<<shift];
            int rIm = sine[(N>>2)-(k<<shift)];
            for(int block = k+step4; block < k+step4+N; block+=step) {
                int tRe = pts[block].re;
                int tIm = pts[block].im;
                int tRe2 = pts[block + step2].re;
                int tIm2 = pts[block + step2].im;
                int h;
                unsigned l;
                int sRe2, sIm2;
                {h,l} = macs(tRe2, rRe, 0, 0x80000000);
                {h,l} = macs(tIm2, -rIm, h, l);
                sRe2 = h << 1;// | l >> 31;
                {h,l} = macs(tRe2, rIm, 0, 0x80000000);
                {h,l} = macs(tIm2, rRe, h, l);
                sIm2 = h << 1;// | l >> 31;
                pts[block].re = tRe + sRe2;
                pts[block].im = tIm + sIm2;
                pts[block+step2].re = tRe - sRe2;
                pts[block+step2].im = tIm - sIm2;
            }
        }
    }
}

//#define LDST16
#ifdef LDST16
#warning("LDST16 actually performs worse than ldw, shift and and")
#endif

#pragma unsafe arrays
void lib_dsp_fft_forward_complex_short_xs1( lib_dsp_fft_complex_short_t pts[], int N, const short sine[] )
{
    unsigned int shift = 30-clz(N);
    for(unsigned int step = 2 ; step <= N; step = step * 2, shift--) {
        unsigned int step2 = step >> 1;
        unsigned int step4 = step2 >> 1;
        unsigned int k;
        for(k = 0; k < step4 + (step2&1); k++) {
            short rRe = sine[(N>>2)-(k<<shift)];
            short rIm = sine[k<<shift];
#ifdef PRINT_FFT_INDEXING
            printf("111111 First k loop. k = %d\n",k);
            printf("step2       = %d\n",step2);
#endif
            for(int block = k+N-step; block >= 0; block-=step) {
                short tRe;
                short tIm;
                short tRe2;
                short tIm2;

#ifdef PRINT_FFT_INDEXING
                printf("   block       = %d\n",block);
                printf("   block+step2 = %d\n",block+step2);
#endif

#ifdef LDST16
                tRe = pts[block].re; // (tImRe >> 16);
                tIm = pts[block].im; //(tImRe);
                tRe2 = pts[block+step2].re;
                tIm2 = pts[block+step2].im;
#else
                unsigned tImRe;
                asm volatile("ldw %0,%1[%2]" : "=r"(tImRe): "r"(pts), "r" (block));
                tIm = (short) (tImRe >> 16);
                tRe = (short) (tImRe);

                unsigned tImRe2; //= pts[block + step2].re;
                asm volatile("ldw %0,%1[%2]" : "=r"(tImRe2): "r"(pts), "r" (block+step2));
                tIm2 = (short) (tImRe2 >> 16);
                tRe2 = (short) (tImRe2);
#endif
                //printf("tRe: 0x%x, tIm: 0x%x\n",tRe, tIm);
                //printf("tRe2: 0x%x, tIm2: 0x%x\n",tRe2, tIm2);

                int h;
                unsigned l;

#ifdef SHORT_FFT_MORE_ACCURATE
                int sRe2, sIm2;
#else
                short sRe2, sIm2;
#endif
                // macs operates on int so short has to be cast to int
                {h,l} = macs((int)tRe2, (int)rRe, 0, SHORT_FFT_BASE);
                {h,l} = macs((int)tIm2, (int)rIm, h, l);
#ifdef SHORT_FFT_MORE_ACCURATE
                sRe2 =  (int) (l>>15);
#else
                sRe2 =  (short) (l>>16);
#endif

                {h,l} = macs((int)tRe2, (int)-rIm, 0, SHORT_FFT_BASE);
                {h,l} = macs((int)tIm2, (int)rRe, h, l);
#ifdef SHORT_FFT_MORE_ACCURATE
                sIm2 =  (int) (l>>15);
#else
                sIm2 =  (short) (l>>16);
#endif


#ifndef SHORT_FFT_MORE_ACCURATE
                tRe >>= 1;
                tIm >>= 1;
#endif

                int re = shiftdown(tRe + sRe2);
                int im = shiftdown(tIm + sIm2);
                int re_step = shiftdown(tRe - sRe2);
                int im_step = shiftdown(tIm - sIm2);

#ifdef LDST16
                pts[block].re = (short) re;
                pts[block].im = (short) im;

                pts[block+step2].re = (short) re_step;
                pts[block+step2].im = (short) im_step;
#else
                // pack 16 bit values into 32 bit words before storing to memory
                re &= 0xffff; // lower 16 bit
                re |= (im << 16); // or in upper 16 bit (real part)

                re_step &= 0xffff; // lower 16 bit
                re_step |= (im_step << 16); // or in upper 16 bit (real part)

                asm("stw %0,%1[%2]" :: "r"(re), "r"(pts), "r" (block));
                asm("stw %0,%1[%2]" :: "r"(re_step), "r"(pts), "r" (block+step2));
#endif

#ifdef DEBUG_PRINT_STORES
                printf("stored at index block=%d:      im|re: %08x\n"
                        ,block ,re);
                printf("stored at index block+step=%d: im|re: %08x\n"
                        ,block+step2,re_step);
#endif

            }
        }
        for(k=(step2 & 1); k < step4; k++) {
            short rRe = -sine[k<<shift];
            short rIm = sine[(N>>2)-(k<<shift)];
#ifdef PRINT_FFT_INDEXING
            printf("222222 Second k loop. k = %d\n",k);
            printf("step2       = %d\n",step2);
#endif
            for(int block = k+step4+N-step; block >= 0; block-=step) {
                short tRe;
                short tIm;
                short tRe2;
                short tIm2;
#ifdef PRINT_FFT_INDEXING
                printf("   block       = %d\n",block);
                printf("   block+step2 = %d\n",block+step2);
#endif

#ifdef LDST16
                tRe = pts[block].re; // (tImRe >> 16);
                tIm = pts[block].im; //(tImRe);
                tRe2 = pts[block+step2].re;
                tIm2 = pts[block+step2].im;
#else
                unsigned tImRe;
                asm volatile("ldw %0,%1[%2]" : "=r"(tImRe): "r"(pts), "r" (block));
                tIm = (short) (tImRe >> 16);
                tRe = (short) (tImRe);

                unsigned tImRe2; //= pts[block + step2].re;
                asm volatile("ldw %0,%1[%2]" : "=r"(tImRe2): "r"(pts), "r" (block+step2));
                tIm2 = (short) (tImRe2 >> 16);
                tRe2 = (short) (tImRe2);
#endif

                int h;
                unsigned l;

#ifdef SHORT_FFT_MORE_ACCURATE
                int sRe2, sIm2;
#else
                short sRe2, sIm2;
#endif
                // macs operates on int so short has to be cast to int
                {h,l} = macs((int)tRe2, (int)rRe, 0, SHORT_FFT_BASE);
                {h,l} = macs((int)tIm2, (int)rIm, h, l);
#ifdef SHORT_FFT_MORE_ACCURATE
                sRe2 =  (int) (l>>15);
#else
                sRe2 =  (short) (l>>16);
#endif

                {h,l} = macs((int)tRe2, (int)-rIm, 0, SHORT_FFT_BASE);
                {h,l} = macs((int)tIm2, (int)rRe, h, l);
#ifdef SHORT_FFT_MORE_ACCURATE
                sIm2 =  (int) (l>>15);
#else
                sIm2 =  (short) (l>>16);
#endif


#ifndef SHORT_FFT_MORE_ACCURATE
                tRe >>= 1;
                tIm >>= 1;
#endif

                int re = shiftdown(tRe + sRe2);
                int im = shiftdown(tIm + sIm2);
                int re_step = shiftdown(tRe - sRe2);
                int im_step = shiftdown(tIm - sIm2);

#ifdef LDST16
                pts[block].re = (short) re;
                pts[block].im = (short) im;

                pts[block+step2].re = (short) re_step;
                pts[block+step2].im = (short) im_step;
#else
                // pack 16 bit values into 32 bit words before storing to memory
                re &= 0xffff; // lower 16 bit
                re |= (im << 16); // or in upper 16 bit (real part)

                re_step &= 0xffff; // lower 16 bit
                re_step |= (im_step << 16); // or in upper 16 bit (real part)

                asm("stw %0,%1[%2]" :: "r"(re), "r"(pts), "r" (block));
                asm("stw %0,%1[%2]" :: "r"(re_step), "r"(pts), "r" (block+step2));
#endif

#ifdef DEBUG_PRINT_STORES
                printf("stored at index block=%d:      im|re: %08x\n"
                        ,block ,re);
                printf("stored at index block+step=%d: im|re: %08x\n"
                        ,block+step2,re_step);
#endif

            }

        }
    }
}

#pragma unsafe arrays
void lib_dsp_fft_inverse_complex_short( lib_dsp_fft_complex_short_t pts[], int N, const short sine[] )
{
    unsigned int shift = 30-clz(N);
    for(unsigned int step = 2 ; step <= N; step = step * 2, shift--) {
        unsigned int step2 = step >> 1;
        unsigned int step4 = step2 >> 1;
        unsigned int k;
        for(k = 0; k < step4 + (step2&1); k++) {
            int rRe = sine[(N>>2)-(k<<shift)];
            int rIm = sine[k<<shift];
            for(int block = k+N-step; block >= 0; block-=step) {
                short tRe;
                short tIm;
                short tRe2;
                short tIm2;

#ifdef LDST16
                tRe = pts[block].re; // (tImRe >> 16);
                tIm = pts[block].im; //(tImRe);
                tRe2 = pts[block+step2].re;
                tIm2 = pts[block+step2].im;
#else
                unsigned tImRe;
                asm volatile("ldw %0,%1[%2]" : "=r"(tImRe): "r"(pts), "r" (block));
                tIm = (short) (tImRe >> 16);
                tRe = (short) (tImRe);

                unsigned tImRe2; //= pts[block + step2].re;
                asm volatile("ldw %0,%1[%2]" : "=r"(tImRe2): "r"(pts), "r" (block+step2));
                tIm2 = (short) (tImRe2 >> 16);
                tRe2 = (short) (tImRe2);
#endif

                int h;
                unsigned l;

                short sRe2, sIm2;
                // macs operates on int so short has to be cast to int
                {h,l} = macs((int)tRe2, (int)rRe, 0, SHORT_IFFT_BASE);
                {h,l} = macs((int)tIm2, (int)-rIm, h, l);
                // >> 15 because additional bit precision is available
                sRe2 =  (short) (l>>15);
                // TODO: Check why this works. l >> 15 means bit 16 is valid but that's not available in a short

                {h,l} = macs((int)tRe2, (int)rIm, 0, SHORT_IFFT_BASE);
                {h,l} = macs((int)tIm2, (int)rRe, h, l);
                sIm2 =  (short) (l>>15);;

                int re = (tRe + sRe2) ;
                int im = (tIm + sIm2) ;
                int re_step = (tRe - sRe2) ;
                int im_step = (tIm - sIm2) ;

#ifdef LDST16
                pts[block].re = (short) re;
                pts[block].im = (short) im;

                pts[block+step2].re = (short) re_step;
                pts[block+step2].im = (short) im_step;
#else
                // pack 16 bit values into 32 bit words before storing to memory
                re &= 0xffff; // lower 16 bit
                re |= (im << 16); // or in upper 16 bit (real part)

                re_step &= 0xffff; // lower 16 bit
                re_step |= (im_step << 16); // or in upper 16 bit (real part)

                asm("stw %0,%1[%2]" :: "r"(re), "r"(pts), "r" (block));
                asm("stw %0,%1[%2]" :: "r"(re_step), "r"(pts), "r" (block+step2));

#endif


            }
        }
        for(k=(step2 & 1); k < step2-step4; k++) {
            short rRe = -sine[k<<shift];
            short rIm = sine[(N>>2)-(k<<shift)];
            for(int block = k+step4; block < k+step4+N; block+=step) {
                short tRe;
                short tIm;
                short tRe2;
                short tIm2;

#ifdef LDST16
                tRe = pts[block].re; // (tImRe >> 16);
                tIm = pts[block].im; //(tImRe);
                tRe2 = pts[block+step2].re;
                tIm2 = pts[block+step2].im;
#else
                unsigned tImRe;
                asm volatile("ldw %0,%1[%2]" : "=r"(tImRe): "r"(pts), "r" (block));
                tIm = (short) (tImRe >> 16);
                tRe = (short) (tImRe);

                unsigned tImRe2; //= pts[block + step2].re;
                asm volatile("ldw %0,%1[%2]" : "=r"(tImRe2): "r"(pts), "r" (block+step2));
                tIm2 = (short) (tImRe2 >> 16);
                tRe2 = (short) (tImRe2);
#endif

                int h;
                unsigned l;

                short sRe2, sIm2;
                // macs operates on int so short has to be cast to int
                {h,l} = macs((int)tRe2, (int)rRe, 0, SHORT_IFFT_BASE);
                {h,l} = macs((int)tIm2, (int)-rIm, h, l);
                // >> 15 because additional bit precision is available
                sRe2 =  (short) (l>>15);
                // TODO: Check why this works. l >> 15 means bit 16 is valid but that's not available in a short

                {h,l} = macs((int)tRe2, (int)rIm, 0, SHORT_IFFT_BASE);
                {h,l} = macs((int)tIm2, (int)rRe, h, l);
                sIm2 =  (short) (l>>15);;

                int re = (tRe + sRe2) ;
                int im = (tIm + sIm2) ;
                int re_step = (tRe - sRe2) ;
                int im_step = (tIm - sIm2) ;

#ifdef LDST16
                pts[block].re = (short) re;
                pts[block].im = (short) im;

                pts[block+step2].re = (short) re_step;
                pts[block+step2].im = (short) im_step;
#else
                // pack 16 bit values into 32 bit words before storing to memory
                re &= 0xffff; // lower 16 bit
                re |= (im << 16); // or in upper 16 bit (real part)

                re_step &= 0xffff; // lower 16 bit
                re_step |= (im_step << 16); // or in upper 16 bit (real part)

                asm("stw %0,%1[%2]" :: "r"(re), "r"(pts), "r" (block));
                asm("stw %0,%1[%2]" :: "r"(re_step), "r"(pts), "r" (block+step2));
#endif
            }
        }
    }
}



#pragma unsafe arrays
void fftTwiddle(int re[], int im[], int N)
{
    unsigned int shift = clz(N);
    for(unsigned int i = 1; i < N-1; i++)
    {
        unsigned int rev = bitrev(i) >> (shift + 1);
        if (rev > i)
        {
            int t = re[i];
            re[i] = re[rev];
            re[rev] = t;
            t = im[i];
            im[i] = im[rev];
            im[rev] = t;
        }
    }
}

#pragma unsafe arrays
void fftForward(int re[], int im[], int N, const int sine[])
{
    unsigned int shift = 30-clz(N);
    for(unsigned int step = 2 ; step <= N; step = step * 2, shift--)
    {
        unsigned int step2 = step >> 1;
        unsigned int step4 = step2 >> 1;
        unsigned int k;
        for(k = 0; k < step4 + (step2&1); k++) {
            int rRe = sine[(N>>2)-(k<<shift)];
            int rIm = sine[k<<shift];
            for(int block = k; block < k+N; block+=step)
            {
                int tRe = re[block];
                int tIm = im[block];
                int tRe2 = re[block + step2];
                int tIm2 = im[block + step2];
                int h;
                unsigned l;
                int sRe2, sIm2;
                {h,l} = macs(tRe2, rRe, 0, 0x80000000);
                {h,l} = macs(tIm2, rIm, h, l);
                sRe2 = h;
                {h,l} = macs(tRe2, -rIm, 0, 0x80000000);
                {h,l} = macs(tIm2, rRe, h, l);
                sIm2 = h;
                tRe >>= 1;
                tIm >>= 1;
                re[block] = tRe + sRe2;
                im[block] = tIm + sIm2;
                re[block+step2] = tRe - sRe2;
                im[block+step2] = tIm - sIm2;
            }
        }
        for(k=(step2 & 1); k < step2-step4; k++)
        {
            int rRe = -sine[k<<shift];
            int rIm = sine[(N>>2)-(k<<shift)];
            for(int block = k+step4; block < k+step4+N; block+=step)
            {
                int tRe = re[block];
                int tIm = im[block];
                int tRe2 = re[block + step2];
                int tIm2 = im[block + step2];
                int h;
                unsigned l;
                int sRe2, sIm2;
                {h,l} = macs(tRe2, rRe, 0, 0x80000000);
                {h,l} = macs(tIm2, rIm, h, l);
                sRe2 = h;
                {h,l} = macs(tRe2, -rIm, 0, 0x80000000);
                {h,l} = macs(tIm2, rRe, h, l);
                sIm2 = h;
                tRe >>= 1;
                tIm >>= 1;
                re[block] = tRe + sRe2;
                im[block] = tIm + sIm2;
                re[block+step2] = tRe - sRe2;
                im[block+step2] = tIm - sIm2;
            }
        }
    }
}

// Note: for an extra bit of precision change the 8 lines that use 0x80000000 or l>>31.
// TODO: Use lexctract instead! Will yield the same performance and add one bit of precision
#pragma unsafe arrays
void fftInverse(int re[], int im[], int N, const int sine[])
{
    unsigned int shift = 30-clz(N);
    for(unsigned int step = 2 ; step <= N; step = step * 2, shift--)
    {
        unsigned int step2 = step >> 1;
        unsigned int step4 = step2 >> 1;
        unsigned int k;
        for(k = 0; k < step4 + (step2&1); k++)
        {
            int rRe = sine[(N>>2)-(k<<shift)];
            int rIm = sine[k<<shift];
            for(int block = k; block < k+N; block+=step) 
            {
                int tRe = re[block];
                int tIm = im[block];
                int tRe2 = re[block + step2];
                int tIm2 = im[block + step2];
                int h;
                unsigned l;
                int sRe2, sIm2;
                {h,l} = macs(tRe2, rRe, 0, 0x80000000);  // Make this 0x40000000
                {h,l} = macs(tIm2, -rIm, h, l);
                sRe2 = h << 1;// | l >> 31;              // And include this part
                {h,l} = macs(tRe2, rIm, 0, 0x80000000);
                {h,l} = macs(tIm2, rRe, h, l);
                sIm2 = h << 1;// | l >> 31;
                re[block] = tRe + sRe2;
                im[block] = tIm + sIm2;
                re[block+step2] = tRe - sRe2;
                im[block+step2] = tIm - sIm2;
            }
        }
        for(k=(step2 & 1); k < step2-step4; k++)
        {
            int rRe = -sine[k<<shift];
            int rIm = sine[(N>>2)-(k<<shift)];
            for(int block = k+step4; block < k+step4+N; block+=step)
            {
                int tRe = re[block];
                int tIm = im[block];
                int tRe2 = re[block + step2];
                int tIm2 = im[block + step2];
                int h;
                unsigned l;
                int sRe2, sIm2;
                {h,l} = macs(tRe2, rRe, 0, 0x80000000);
                {h,l} = macs(tIm2, -rIm, h, l);
                sRe2 = h << 1;// | l >> 31;
                {h,l} = macs(tRe2, rIm, 0, 0x80000000);
                {h,l} = macs(tIm2, rRe, h, l);
                sIm2 = h << 1;// | l >> 31;
                re[block] = tRe + sRe2;
                im[block] = tIm + sIm2;
                re[block+step2] = tRe - sRe2;
                im[block+step2] = tIm - sIm2;
            }
        }
    }
}

#pragma unsafe arrays
void lib_dsp_fft_forward_tworeals_optimised( lib_dsp_fft_complex_t two_re[], lib_dsp_fft_complex_t two_im[], int N, const int sine[] )
{
    int re1, re2, re1_ni, re2_ni;
    int im1, im2, im1_ni, im2_ni;

    lib_dsp_fft_bit_reverse(two_re, N);
    lib_dsp_fft_forward_complex(two_re, N, sine);

    two_im[0].re = 0;
    two_im[0].im = 0;

    for(int i = N >> 1; i != 0; i--) {

        asm("ldd %0,%1,%2[%3]" : "=r"(re2), "=r" (re1): "r"(two_re), "r" (i));
        asm("ldd %0,%1,%2[%3]" : "=r"(re2_ni), "=r" (re1_ni): "r"(two_re), "r" (N-i));

        im1 = (re2 - re2_ni) >> 1;
        im1_ni = -im1;
        im2 = (re1_ni - re1) >> 1;
        im2_ni = -im2;

        asm("std %0,%1,%2[%3]" :: "r"(im1), "r" (im2), "r"(two_im), "r" (i));
        asm("std %0,%1,%2[%3]" :: "r"(im1_ni), "r" (im2_ni), "r"(two_im), "r" (N-i));
        //im1[i] = (re2[i] - re2[N-i])>>1;
        //im1[N-i] = -im1[i];
        //im2[i] = (re1[N-i] - re1[i])>>1;
        //im2[N-i] = -im2[i];

        //two_im[i].re = (two_re[i].im - two_re[N-i].im)>>1;
        //two_im[N-i].re = -two_im[i].re;
        //two_im[i].im = (two_re[N-i].re - two_re[i].re)>>1;
        //two_im[N-i].im = -two_im[i].im;
    }
    for(int i = N >> 1; i != 0; i--) {
        two_re[i].re = (two_re[i].re + two_re[N-i].re) >> 1;
        two_re[N-i].re = two_re[i].re;
        two_re[i].im = (two_re[i].im + two_re[N-i].im)>>1;
        two_re[N-i].im = two_re[i].im;
    }
}
