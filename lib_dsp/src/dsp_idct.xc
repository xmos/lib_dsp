// Copyright (c) 2015-2016, XMOS Ltd, All rights reserved

#include "dsp_dct.h"

/* This is a very limited inverse DCT implementation.
 */

static inline int32_t mulcos(int32_t x, int32_t cos) {
    long long r = cos * (long long) x;
    return r >> 31;
}


void dsp_dct_inverse4(int32_t output[4], int32_t input[4]) {
    int32_t z = input[0] >> 1;
    int32_t y1 = mulcos(input[1], 1984016189) ;
    int32_t y3 = mulcos(input[3], 1984016189);
    int32_t y1_ = mulcos(input[1], 821806413);
    int32_t y3_ = mulcos(input[3], 821806413);
    int32_t y2 = mulcos(input[2], 1518500250);
    output[0] = z + y1  + y2 + y3_;
    output[1] = z + y1_ - y2 - y3;
    output[2] = z - y1_ - y2 + y3;
    output[3] = z - y1  + y2 - y3_;
}

void dsp_dct_inverse3(int32_t output[3], int32_t input[3]) {
    int32_t z = input[0] >> 1;
    int32_t y = mulcos(input[1], 1859775393);
    int32_t x = mulcos(input[2], 1073741824);
    output[0] = y + x + z;
    output[1] = z-input[2];
    output[2] = -y + x + z;
}

void dsp_dct_inverse2(int32_t output[2], int32_t input[2]) {
    int32_t z = input[0] >> 1;
    int32_t s = mulcos(input[1], 1518500250);
    output[0] = z + s;
    output[1] = z - s;
}

void dsp_dct_inverse1(int32_t output[1], int32_t input[1]) {
    output[0] = input[0];
}

#ifdef INCLUDE_REFERENCE_DCT
#include <math.h>

void referenceIDCT(int32_t output[], int32_t input[], int32_t N) {
    for(int32_t k = 0; k < N; k++) {
        double sum = input[0]/2.0;
        for(int32_t i = 1; i < N; i++) {
            double z = input[i] * cos(M_PI*i*(2*k+1)/(2*N));
            sum += z;
        }
        output[k] = sum;
    }
}

#endif
