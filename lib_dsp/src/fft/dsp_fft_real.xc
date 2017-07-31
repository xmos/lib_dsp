// Copyright (c) 2017, XMOS Ltd, All rights reserved
#include "dsp.h"
#include <xclib.h>
#include <stdio.h>
#include "math.h"

static inline int32_t sin_1(int i, int N, const int32_t sine[]) {
    return sine[i];
}

static inline int32_t cos_1(int i, int N, const int32_t sine[]) {
    return sine[N/2-i];
}

extern  void dsp_fft_real_fix_forward_xs2(dsp_complex_t pts[], const uint32_t N, const int32_t sine[] );

extern  void dsp_fft_real_fix_inverse_xs2(dsp_complex_t pts[], const uint32_t N, const int32_t sine[] );

#pragma unsafe arrays
void dsp_fft_real_fix_forward(dsp_complex_t pts[], const uint32_t N, const int32_t sine[] ) {
    int shift = 32;
    int Xrk, Xik, XrNk, XiNk;
    asm("ldd %0, %1, %2[%3]" : "=r" (Xik), "=r" (Xrk) : "r" (pts), "r" (0));
    asm("std %0, %1, %2[%3]" :: "r" ((Xrk - Xik) >> 1), "r" ((Xik + Xrk) >> 1), "r" (pts), "r" (0));
    for(uint32_t k = 1; k <= N/2; k++) {
        asm("ldd %0, %1, %2[%3]" : "=r" (Xik), "=r" (Xrk) : "r" (pts), "r" (k));
        asm("ldd %0, %1, %2[%3]" : "=r" (XiNk), "=r" (XrNk) : "r" (pts), "r" (N-k));
        int si = sin_1(k, N, sine) >> 1;
        int32_t Ark = 0x40000000 - si;
        int32_t Bik = cos_1(k, N, sine) >> 1;
        int32_t Brk = 0x40000000 + si;
        int32_t nBik = -Bik;
        int32_t nBrk = -Brk;
        int32_t Grk = (Xrk*(int64_t)Ark + Xik*(int64_t)Bik +
                       XrNk*(int64_t)Brk + XiNk*(int64_t)Bik)>>shift;
        int32_t Gik = (Xik*(int64_t)Ark + Xrk*(int64_t)nBik +
                       XrNk*(int64_t)Bik + XiNk*(int64_t)nBrk)>>shift;
        asm("std %0, %1, %2[%3]" :: "r" (Gik), "r" (Grk), "r" (pts), "r" (k));
        int32_t GrNk = (XrNk*(int64_t)Ark + XiNk*(int64_t)nBik +
                        Xrk*(int64_t)Brk + Xik*(int64_t)nBik)>>shift;
        int32_t GiNk = (XiNk*(int64_t)Ark + XrNk*(int64_t)Bik +
                        Xrk*(int64_t)nBik + Xik*(int64_t)nBrk)>>shift;
        asm("std %0, %1, %2[%3]" :: "r" (GiNk), "r" (GrNk), "r" (pts), "r" (N-k));
    }
}

#pragma unsafe arrays
void dsp_fft_real_fix_inverse(dsp_complex_t pts[], const uint32_t N, const int32_t sine[] ) {
    int shift = 30;
    int Xrk, Xik, XrNk, XiNk;
    asm("ldd %0, %1, %2[%3]" : "=r" (Xik), "=r" (Xrk) : "r" (pts), "r" (0));
    asm("std %0, %1, %2[%3]" :: "r" ((Xrk - Xik) >> 0), "r" ((Xik + Xrk) >> 0), "r" (pts), "r" (0));
    for(uint32_t k = 1; k <= N/2; k++) {
        asm("ldd %0, %1, %2[%3]" : "=r" (Xik), "=r" (Xrk) : "r" (pts), "r" (k));
        asm("ldd %0, %1, %2[%3]" : "=r" (XiNk), "=r" (XrNk) : "r" (pts), "r" (N-k));
        int si = sin_1(k, N, sine) >> 1;
        int32_t Ark = 0x40000000 - si;
        int32_t Bik = -cos_1(k, N, sine) >> 1;
        int32_t Brk = 0x40000000 + si;
        int32_t nBik = -Bik;
        int32_t nBrk = -Brk;
        int32_t Grk = (Xrk*(int64_t)Ark + Xik*(int64_t)Bik +
                       XrNk*(int64_t)Brk + XiNk*(int64_t)Bik)>>shift;
        int32_t Gik = (Xik*(int64_t)Ark + Xrk*(int64_t)nBik +
                       XrNk*(int64_t)Bik + XiNk*(int64_t)nBrk)>>shift;
        asm("std %0, %1, %2[%3]" :: "r" (Gik), "r" (Grk), "r" (pts), "r" (k));
        int32_t GrNk = (XrNk*(int64_t)Ark + XiNk*(int64_t)nBik +
                        Xrk*(int64_t)Brk + Xik*(int64_t)nBik)>>shift;
        int32_t GiNk = (XiNk*(int64_t)Ark + XrNk*(int64_t)Bik +
                        Xrk*(int64_t)nBik + Xik*(int64_t)nBrk)>>shift;
        asm("std %0, %1, %2[%3]" :: "r" (GiNk), "r" (GrNk), "r" (pts), "r" (N-k));
    }
}

void dsp_fft_bit_reverse_and_forward_real (
    int32_t pts[],
    const uint32_t N,
    const int32_t sine[],
    const int32_t sin2[] ) {

    dsp_fft_bit_reverse((pts, dsp_complex_t[]), N>>1);
    dsp_fft_forward((pts, dsp_complex_t[]), N>>1, sine);
    dsp_fft_real_fix_forward_xs2((pts, dsp_complex_t[]), N>>1, sin2);
}

void dsp_fft_bit_reverse_and_inverse_real (
    int32_t pts[],
    const uint32_t N,
    const int32_t sine[],
    const int32_t sin2[] ) {

    dsp_fft_real_fix_inverse_xs2((pts, dsp_complex_t[]), N>>1, sin2);
    dsp_fft_bit_reverse((pts, dsp_complex_t[]), N>>1);
    dsp_fft_inverse((pts, dsp_complex_t[]), N>>1, sine);
}
