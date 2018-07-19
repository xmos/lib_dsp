// Copyright (c) 2015-2016, XMOS Ltd, All rights reserved
// XMOS DSP Library - DCT Functions test program

#include <stdio.h>
#include <xs1.h>
#include <dsp_dct.h>

int32_t data[24] = {
    240000000, 230000000, 220000000, 210000000, 200000000, 190000000, 180000000, 170000000, 160000000, 150000000,
    140000000, 130000000, 120000000, 110000000, 100000000,  90000000,  80000000,  70000000,  60000000,  50000000,
    40000000,  30000000,  20000000,  10000000
};

int32_t data2[24] = {
    240000, 230000, 220000, 210000, 200000, 190000, 180000, 170000, 160000, 150000,
    140000, 130000, 120000, 110000, 100000,  90000,  80000,  70000,  60000,  50000,
    40000,  30000,  20000,  10000
};

int main( void )
{
    int32_t dcted[24];

    dsp_dct_forward24(dcted, data);
    printf("BEGIN\n");
    for(int32_t i = 0; i < 24; i++) {
        printf("%5d %6d\n", data[i], dcted[i]);
    }
    printf("END\n");

    dsp_dct_forward12(dcted, data2);
    printf("BEGIN\n");
    for(int32_t i = 0; i < 12; i++) {
        printf("%5d %6d\n", data2[i], dcted[i]);
    }
    printf("END\n");
    return 0;
}

