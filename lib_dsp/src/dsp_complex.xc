// Copyright (c) 2017, XMOS Ltd, All rights reserved
#include <dsp_complex.h>
#include <dsp_math.h>

dsp_complex_t dsp_complex_add(dsp_complex_t a, dsp_complex_t b) {
    dsp_complex_t sum = { a.re + b.re, a.im + b.im };
    return sum;
}

dsp_complex_t dsp_complex_sub(dsp_complex_t a, dsp_complex_t b) {
    dsp_complex_t diff = { a.re - b.re, a.im - b.im };
    return diff;
}

dsp_complex_t dsp_complex_mul(dsp_complex_t a, dsp_complex_t b, uint32_t N) {
    int32_t re = (a.re * (int64_t) b.re - a.im * (int64_t) b.im) >> N;
    int32_t im = (a.re * (int64_t) b.im + a.im * (int64_t) b.re) >> N;
    dsp_complex_t sum = { re, im };
    return sum;
}

dsp_complex_t dsp_complex_mul_conjugate(dsp_complex_t a, dsp_complex_t b, uint32_t N) {
    int32_t re = ( a.re * (int64_t) b.re + a.im * (int64_t) b.im) >> N;
    int32_t im = (-a.re * (int64_t) b.im + a.im * (int64_t) b.re) >> N;
    dsp_complex_t sum = { re, im };
    return sum;
}

#if !defined(__XS2A__)
dsp_complex_t dsp_complex_fir(dsp_complex_t a[], dsp_complex_t b[],
                              uint32_t L, uint32_t off, uint32_t N) {
    int64_t re = 0;
    int64_t im = 0;
    for(unsigned i = 0; i < L; i++) {
        re += a[i+off].re * (int64_t) b[i].re - a[i+off].im * (int64_t) b[i].im;
        im += a[i+off].re * (int64_t) b[i].im + a[i+off].im * (int64_t) b[i].re;
    }
    dsp_complex_t sum = { re >> N, im >> N };
    return sum;
}
#endif

#if !defined(__XS2A__)
void dsp_complex_mul_vector(dsp_complex_t a[], dsp_complex_t b[],
                                     uint32_t L, uint32_t N) {
    for(unsigned i = 0; i < L; i++) {
        a[i] = dsp_complex_mul(a[i], b[i], N);
    }
}
#endif

#if !defined(__XS2A__)
void dsp_complex_mul_conjugate_vector(dsp_complex_t a[], dsp_complex_t b[],
                                     uint32_t L, uint32_t N) {
    for(unsigned i = 0; i < L; i++) {
        a[i] = dsp_complex_mul(a[i], b[i], N);
    }
}
#endif

void dsp_complex_magnitude_vector(uint32_t magnitude[],
                                  dsp_complex_t input[],
                                  uint32_t N, uint32_t P) {
    for(int i = N-1; i >= 0; i--) {
        int z[2];
        z[0] = input[i].re;
        z[1] = input[i].im;
        dsp_math_atan2_hypot(z, P);
        magnitude[i] = z[0];
    }
}
