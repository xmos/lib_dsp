// Copyright (c) 2015, XMOS Ltd, All rights reserved
// XMOS DSP Library - Example to use FFT and inverse FFT

#include <stdio.h>
#include <xs1.h>
#include <lib_dsp.h>

#define PRINT_FFT_INPUT 1
#define PRINT_FFT_OUTPUT 1
#define PRINT_IFFT_OUTPUT 1

#define N_FFT_POINTS 32
#define INPUT_FREQ N_FFT_POINTS/8

//Select Complex FFT or Real FFT of two signals
//#define COMPLEX_FFT

// Note: Use these data types to guarantee 64 bit alignment
// and having re and im together in one double word for efficient ldd and std
#ifdef COMPLEX_FFT
lib_dsp_fft_complex_t  data[N_FFT_POINTS];
int do_complex_fft_and_ifft();
#else
lib_dsp_fft_complex_t  two_re[N_FFT_POINTS];
lib_dsp_fft_complex_t  two_im[N_FFT_POINTS];
int do_tworeal_fft_and_ifft();
#endif

// sine signal with 8 samples per cycle
int sin8(int x) {
    x = x & (8-1); // mask off 3 bits
    switch (x >> 1) {
       // upper two bits determine the quadrant.
       case 0: return lib_dsp_sine_8[x];
       case 1: return lib_dsp_sine_8[4-x];
       case 2: return -lib_dsp_sine_8[x-4];
       case 3: return -lib_dsp_sine_8[8-x];
    }
}
int cos8(int x) {
    return sin8(x+(8/4)); // cos a = sin(a + 2pi/4)
}

int main( void )
{

#ifdef COMPLEX_FFT
    do_complex_fft_and_ifft();
#else
    do_tworeal_fft_and_ifft();
#endif
    return 0;
};

#ifdef COMPLEX_FFT
int do_complex_fft_and_ifft() {
    timer tmr;
    unsigned start_time, end_time, overhead_time, cycles_taken;
    tmr :> start_time;
    tmr :> end_time;
    overhead_time = end_time - start_time;

    printf("Generating a %d Hz sine and cosine signal for input to the Complex FFT:\n\n",N_FFT_POINTS/8);
    for(int i=0; i<N_FFT_POINTS; i++) {
        // generate a N_FFT_POINTS/8 Hz sing and cosine signals
        // 4 oscillations in 32 points -> Use sin8 to get 8 points per oscillation
        data[i].re = cos8(i);
        data[i].im = sin8(i);
    }

#if PRINT_FFT_INPUT
    printf( "real data (%d cosine cycles)\n",INPUT_FREQ);
    for(int i=0; i<N_FFT_POINTS; i++) {
        printf( "%08x\n",data[i].re);
    }
    printf( "\n" );
    printf( "imaginary data (%d sine cycles)\n",INPUT_FREQ);
    for(int i=0; i<N_FFT_POINTS; i++) {
        printf( "%08x\n",data[i].im);
    }
    printf( "\n" );
#endif

    tmr :> start_time;
    lib_dsp_fft_bit_reverse(data, N_FFT_POINTS);
    lib_dsp_fft_forward_complex(data, N_FFT_POINTS, FFT_SINE(N_FFT_POINTS));
    tmr :> end_time;
    cycles_taken = end_time-start_time-overhead_time;
    printf("Cycles taken for FFT: %d\n", cycles_taken);

#if PRINT_FFT_OUTPUT
    // Print forward complex FFT results
    printf( "FFT output spectrum:\n");
    for(int i=0; i<N_FFT_POINTS; i++) {
        printf( "re: %08x im: %08x\n", data[i].re, data[i].im);
    }
    printf( "\n" );
#endif

    tmr :> start_time;
    lib_dsp_fft_bit_reverse(data, N_FFT_POINTS);
    lib_dsp_fft_inverse_complex(data, N_FFT_POINTS, FFT_SINE(N_FFT_POINTS));
    tmr :> end_time;
    cycles_taken = end_time-start_time-overhead_time;
    printf("Cycles taken for iFFT: %d\n", cycles_taken);

#if PRINT_IFFT_OUTPUT
    printf( "////// After lib_dsp_fft_inverse_complex\n");
    printf( "real data (%d cosine cycles)\n", INPUT_FREQ);
    for(int i=0; i<N_FFT_POINTS; i++) {
        printf( "%08x\n",data[i].re);
    }
    printf( "\n" );
    printf( "imaginary data (%d sine cycles)\n",INPUT_FREQ);
    for(int i=0; i<N_FFT_POINTS; i++) {
        printf( "%08x\n",data[i].im);
    }
    printf( "\n" );
#endif
}
# else
int do_tworeal_fft_and_ifft() {
    timer tmr;
    unsigned start_time, end_time, overhead_time, cycles_taken;
    tmr :> start_time;
    tmr :> end_time;
    overhead_time = end_time - start_time;

    printf("Generating a %d Hz sine and cosine signal for input to the Forward 2xReal FFT\n\n",N_FFT_POINTS/8);
    for(int i=0; i<N_FFT_POINTS; i++) {
        // generate a N_FFT_POINTS/8 Hz sing and cosine signals
        // 4 oscillations in 32 points -> Use sin8 to get 8 points per oscillation
        two_re[i].re = cos8(i);
        two_re[i].im = sin8(i);
        two_im[i].re = 0;
        two_im[i].im = 0;
    }

#if PRINT_FFT_INPUT
    printf( "real1 data (%d cosine cycles)\n",INPUT_FREQ);
    for(int i=0; i<N_FFT_POINTS; i++) {
        printf( "%08x\n",two_re[i].re);
    }
    printf( "\n" );
    printf( "real2 data (%d sine cycles)\n",INPUT_FREQ);
    for(int i=0; i<N_FFT_POINTS; i++) {
        printf( "%08x\n",two_re[i].im);
    }
    printf( "\n" );
#endif

    printf("Forward 2xReal FFT, Size = %05u\n", N_FFT_POINTS );
    tmr :> start_time;
    lib_dsp_fft_forward_tworeals( two_re, two_im, N_FFT_POINTS, FFT_SINE(N_FFT_POINTS) );
    tmr :> end_time;

    cycles_taken = end_time-start_time-overhead_time;
    printf("Cycles taken for FFT: %d\n", cycles_taken);

#if PRINT_FFT_OUTPUT
    // Print forward complex FFT results
    printf( "FFT output spectrum of real1:\n");
    for(int i=0; i<N_FFT_POINTS; i++) {
        printf( "re: %08x im: %08x\n", two_re[i].re, two_im[i].re);
    }
    printf( "\n" );
    printf( "FFT output spectrum of real2:\n");
    for(int i=0; i<N_FFT_POINTS; i++) {
        printf( "re: %08x im: %08x\n", two_re[i].im, two_im[i].im);
    }
#endif
        
    printf( "Inverse 2xReal FFT, Size = %05u\n", N_FFT_POINTS );
    tmr :> start_time;
    lib_dsp_fft_inverse_tworeals( two_re, two_im, N_FFT_POINTS, FFT_SINE(N_FFT_POINTS) );
    tmr :> end_time;

    cycles_taken = end_time-start_time-overhead_time;
    printf("Cycles taken for FFT: %d\n", cycles_taken);

#if PRINT_IFFT_OUTPUT
    printf( "////// After lib_dsp_fft_inverse_tworeals\n");
    printf( "real1 data (%d cosine cycles)\n", INPUT_FREQ);
    for(int i=0; i<N_FFT_POINTS; i++) {
        printf( "%08x\n",two_re[i].re);
    }
    printf( "\n" );
    printf( "real2 data (%d sine cycles)\n",INPUT_FREQ);
    for(int i=0; i<N_FFT_POINTS; i++) {
        printf( "%08x\n",two_re[i].im);
    }
    printf( "\n" );
#endif

    printf( "DONE.\n" );
    return 0;
}
#endif

