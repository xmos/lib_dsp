// Copyright (c) 2015-2016, XMOS Ltd, All rights reserved

#include "dsp_dct.h"

/* This is a simple DCT implementation. It can be optimised for memory and
 * speed at a later time. 
 *
 * dct4(), dct3(), and dct2() are implemented natively
 * dct6() comprises two calls two dct3();
 * dct8() comprises two calls two dct4();
 * dct12() comprises two calls two dct6();
 * dct16() comprises two calls two dct8();
 * dct24() comprises two calls two dct12();
 * dct32() comprises two calls two dct16();
 */

static const int32_t costable8[4] = {
    2106220352,
    1785567396,
    1193077991,
    418953276,
};

static const int32_t costable16[8] = {
    2137142927,
    2055013723,
    1893911494,
    1660027308,
    1362349204,
    1012316784,
    623381598,
    210490206,
};

static const int32_t costable32[16] = {
    2144896910,
    2124240380,
    2083126254,
    2021950484,
    1941302225,
    1841958164,
    1724875040,
    1591180426,
    1442161874,
    1279254516,
    1104027237,
    918167572,
    723465451,
    521795963,
    315101295,
    105372028,
};

static const int32_t costable6[3] = {
    2074309917,
    1518500250,
    555809667,
};

static const int32_t costable12[6] = {
    2129111628,
    1984016189,
    1703713325,
    1307305214,
    821806413,
    280302863,
};

static const int32_t costable24[12] = {
    2142885721,
    2106220352,
    2033516969,
    1926019547,
    1785567396,
    1614563692,
    1415934356,
    1193077991,
    949807730,
    690285996,
    418953276,
    140452151,
};

static inline int32_t mulcos(int32_t x, int32_t cos) {
    long long r = cos * (long long) x;
    return r >> 31;
}

#define DCT(N,M)                                \
void dsp_dct_forward##N(int32_t output[N], int32_t input[N]) { \
    int32_t temp[N/2], temp2[N/2]; \
    for(int32_t i = 0; i < N/2; i++) { \
        temp[i] = input[i] + input[N-1-i]; \
    } \
    dsp_dct_forward##M(temp2, temp); \
    for(int32_t i = 0; i < N/2; i++) { \
        output[2*i] = temp2[i]; \
    } \
    for(int32_t i = 0; i < N/2; i++) { \
        int32_t z = input[i] - input[N-1-i]; \
        temp[i] = mulcos(z, costable##N[i]); \
    } \
    dsp_dct_forward##M(temp2, temp); \
    int32_t last = temp2[0]; \
    output[1] = last; \
    for(int32_t i = 1; i < N/2; i++) { \
        last = temp2[i]*2 - last; \
        output[2*i+1] = last; \
    } \
}

void dsp_dct_forward4(int32_t output[4], int32_t input[4]) {
    int32_t i03 = input[0] + input[3];
    int32_t i12 = input[1] + input[2];
    int32_t i03_ = input[0] - input[3];
    int32_t i12_ = input[1] - input[2];
    output[0] = i03 + i12;
    output[2] = mulcos(i03 - i12, 1518500250);
    output[1] =  mulcos(i03_, 1984016189) +
        mulcos(i12_, 821806413);
    output[3] = mulcos(i03_, 821806413) + 
        mulcos(i12_, -1984016189);
}

void dsp_dct_forward3(int32_t output[3], int32_t input[3]) {
    output[0] = input[0] + input[1] + input[2];
    output[1] = mulcos(input[0] - input[2], 1859775393);
    output[2] = ((input[0]+input[2])>>1) - input[1];
}

void dsp_dct_forward2(int32_t output[2], int32_t input[2]) {
    output[0] = input[0] + input[1];
    int32_t z = input[0] - input[1];
    output[1] = mulcos(z, 1518500250);
}

void dsp_dct_forward1(int32_t output[1], int32_t input[1]) {
    output[0] = input[0];
}


DCT(6,3)
DCT(8,4)
DCT(12,6)
DCT(16,8)
DCT(24,12)
DCT(32,16)

#ifdef INCLUDE_REFERENCE_DCT
#include <math.h>

void referenceDCT(int32_t output[], int32_t input[], int32_t N) {
    for(int32_t k = 0; k < N; k++) {
        double sum = 0;
        for(int32_t i = 0; i < N; i++) {
            double z = input[i] * cos(M_PI*(2*i+1)*k/(2*N));
            sum += z;
        }
        output[k] = sum;
    }
}
#endif

