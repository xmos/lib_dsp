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

void test_short_long_conversion(){
    unsigned x=SEED;
    lib_dsp_fft_complex_t f[FFT_LENGTH];
    lib_dsp_fft_complex_t g[FFT_LENGTH];
    lib_dsp_fft_complex_short_t h[FFT_LENGTH];

    for(unsigned i=0; i < FFT_LENGTH; i++){
        f[i].re = random(x);
        f[i].im = random(x);
        g[i].re = f[i].re;
        g[i].im = f[i].im;
        h[i].re = 0;
        h[i].im = 0;
    }

    lib_dsp_fft_long_to_short(h, f, FFT_LENGTH);

    for(unsigned i=0; i < FFT_LENGTH; i++){
        if((f[i].re>>16) != h[i].re){
            debug_printf("Error in long to short(real)\n");
            _Exit(1);
        }
        if((f[i].im>>16) != h[i].im){
            debug_printf("Error in long to short(imaginary)\n");
            _Exit(1);
        }
    }
    debug_printf("Long to short conversion: Pass.\n");

    for(unsigned i=0; i < FFT_LENGTH; i++){
        f[i].re = 0;
        f[i].im = 0;
    }

    lib_dsp_fft_short_to_long(f, h, FFT_LENGTH);

    for(unsigned i=0; i < FFT_LENGTH; i++){
        if(f[i].re != (g[i].re&0xffff0000)){
            debug_printf("Error in long to short(real)\n");
            _Exit(1);
        }
        if(f[i].im != (g[i].im&0xffff0000)){
            debug_printf("Error in long to short(imaginary)\n");
            _Exit(1);
        }
    }

    debug_printf("Short to long conversion: Pass.\n");
}

unsafe int main(){
    test_short_long_conversion();
    _Exit(0);
    return 0;
}
