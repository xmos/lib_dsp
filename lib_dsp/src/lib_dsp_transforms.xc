// Copyright (c) 2015, XMOS Ltd, All rights reserved

#include <platform.h>
#include <xs1.h>
#include <xclib.h>

#include "lib_dsp_transforms.h"

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
