// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "lib_dsp_dct.h"

/* This is a very limited inverse DCT implementation.
 */

static inline int mulcos(int x, int cos) {
    long long r = cos * (long long) x;
    return r >> 31;
}


void idct4(int output[4], int input[4]) {
    int z = input[0] >> 1;
    int y1 = mulcos(input[1], 1984016189) ;
    int y3 = mulcos(input[3], 1984016189);
    int y1_ = mulcos(input[1], 821806413);
    int y3_ = mulcos(input[3], 821806413);
    int y2 = mulcos(input[2], 1518500250);
    output[0] = z + y1  + y2 + y3_;
    output[1] = z + y1_ - y2 - y3;
    output[2] = z - y1_ - y2 + y3;
    output[3] = z - y1  + y2 - y3_;
}

void idct3(int output[3], int input[3]) {
    int z = input[0] >> 1;
    int y = mulcos(input[1], 1859775393);
    int x = mulcos(input[2], 1073741824);
    output[0] = y + x + z;
    output[1] = z-input[2];
    output[2] = -y + x + z;
}

void idct2(int output[2], int input[2]) {
    int z = input[0] >> 1;
    int s = mulcos(input[1], 1518500250);
    output[0] = z + s;
    output[1] = z - s;
}

void idct1(int output[1], int input[1]) {
    output[0] = input[0];
}

#ifdef INCLUDE_REFERENCE_DCT
#include <math.h>

void referenceIDCT(int output[], int input[], int N) {
    for(int k = 0; k < N; k++) {
        double sum = input[0]/2.0;
        for(int i = 1; i < N; i++) {
            double z = input[i] * cos(M_PI*i*(2*k+1)/(2*N));
            sum += z;
        }
        output[k] = sum;
    }
}

#endif
