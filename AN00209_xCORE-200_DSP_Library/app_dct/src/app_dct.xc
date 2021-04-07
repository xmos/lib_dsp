// Copyright 2015-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// XMOS DSP Library - DCT Functions test program

#include <stdio.h>
#include <xs1.h>
#include <dsp_dct.h>

int32_t data[24] = {
    24000, 23000, 22000, 21000, 20000, 19000, 18000, 17000, 16000, 15000,
    14000, 13000, 12000, 11000, 10000,  9000,  8000,  7000,  6000,  5000,
    4000,  3000,  2000,  1000
};

int32_t correct[24] = {
    300000, 116633,
    0, 12883,
    0, 4581,
    0, 2291,
    0, 1345,
    0, 871,
    0, 577,
    0, 411,
    0, 263,
    0, 189,
    0, 95,
    0, 33
};

int main( void )
{
    int32_t dcted[24];
    
    dsp_dct_forward24(dcted, data);
    for(int32_t i = 0; i < 24; i++) {
        printf("%5d %6d%s\n", data[i], dcted[i], dcted[i] != correct[i] ? " Wrong":"");
    }
    return 0;
}

