// Copyright (c) 2016, XMOS Ltd, All rights reserved
#include <xs1.h>
#include <xclib.h>
#include <debug_print.h>
#include <stdlib.h>

#include "lib_dsp_fft.h"
#include "generated.h"

int random(unsigned &x){
    crc32(x, -1, 0xEB31D82E);
    return (int)x;
}

void test_index_bit_reverse(){
    lib_dsp_fft_complex_t f[FFT_LENGTH];
    for(unsigned i=0;i<FFT_LENGTH;i++){
       f[i].re = i;
       f[i].im = i+FFT_LENGTH;
    }

    lib_dsp_fft_bit_reverse(f, FFT_LENGTH);

    for(unsigned i=0;i<FFT_LENGTH;i++){
        unsigned j = bitrev(i)>>(clz(FFT_LENGTH)+1);
       if(f[i].re !=  j){
           debug_printf("Error: error in bit reverse (real)\n");
           _Exit(1);
       }
       if(f[i].im !=  j+FFT_LENGTH){
           debug_printf("Error: error in bit reverse (imaginary)\n");
           _Exit(1);
       }
    }
    debug_printf("Bit reverse: Pass.\n");
}

unsafe int main(){
    test_index_bit_reverse();
    _Exit(0);
    return 0;
}
