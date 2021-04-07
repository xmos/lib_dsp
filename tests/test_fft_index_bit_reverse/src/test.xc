// Copyright 2016-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include <xs1.h>
#include <xclib.h>
#include <stdio.h>
#include <stdlib.h>

#include "dsp_fft.h"
#include "generated.h"

int random(unsigned &x){
    crc32(x, -1, 0xEB31D82E);
    return (int)x;
}

void test_index_bit_reverse(){
    dsp_complex_t f[FFT_LENGTH];
    for(unsigned i=0;i<FFT_LENGTH;i++){
       f[i].re = i;
       f[i].im = i+FFT_LENGTH;
    }

    dsp_fft_bit_reverse(f, FFT_LENGTH);

    for(unsigned i=0;i<FFT_LENGTH;i++){
        unsigned j = bitrev(i)>>(clz(FFT_LENGTH)+1);
       if(f[i].re !=  j){
           printf("Error: error in bit reverse (real)\n");
           _Exit(1);
       }
       if(f[i].im !=  j+FFT_LENGTH){
           printf("Error: error in bit reverse (imaginary)\n");
           _Exit(1);
       }
    }
    printf("Bit reverse: Pass.\n");
}

unsafe int main(){
    test_index_bit_reverse();
    _Exit(0);
    return 0;
}
