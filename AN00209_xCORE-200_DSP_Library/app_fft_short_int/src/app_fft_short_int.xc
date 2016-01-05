// Copyright (c) 2016, XMOS Ltd, All rights reserved
// XMOS DSP Library - Example to use FFT and inverse FFT

#include <stdio.h>
#include <xs1.h>
#include <lib_dsp.h>

#define PRINT_FFT_INPUT 1
#define PRINT_FFT_OUTPUT 1
#define PRINT_IFFT_OUTPUT 1
#define PRINT_CYCLE_COUNT 0

#define N_FFT_POINTS 32
#define INPUT_FREQ N_FFT_POINTS/8

#define COMPLEX_FFT  // tworeal isn't supported yet for short

// Note: Use these data types to guarantee 64 bit alignment
// and having re and im together in one double word for efficient ldd and std
#ifdef COMPLEX_FFT
lib_dsp_fft_complex_short_t data[N_FFT_POINTS];
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
    return 0; // unreachable
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
void generate_two_short_complex_test_signals(lib_dsp_fft_complex_short_t data[], int N, int test) {
    switch(test) {
    case 0: {
        printf("++++ Test 0: %d point FFT of two complex short int signals:: Real: %d Hz cosine, Imag: 0\n"
                ,N_FFT_POINTS,INPUT_FREQ);
        for(int i=0; i<N; i++) {
            data[i].re_0 = cos8(i)>>16;
            data[i].re_1 = cos8(i)>>16;
            data[i].im_0 = 0;
            data[i].im_1 = 0;
        }
        break;
    }
    case 1: {
        printf("++++ Test 1: %d point FFT of two complex short int signals:: Real: %d Hz sine, Imag: 0\n"
                ,N_FFT_POINTS,INPUT_FREQ);
        for(int i=0; i<N; i++) {
            data[i].re_0 = sin8(i)>>16;
            data[i].re_1 = sin8(i)>>16;
            data[i].im_0 = 0;
            data[i].im_1 = 0;
        }
        break;
    }
    case 2: {
        printf("++++ Test 2: %d point FFT of two complex short int signals: Real: 0, Imag: %d Hz cosine\n"
                ,N_FFT_POINTS,INPUT_FREQ);
        for(int i=0; i<N; i++) {
            data[i].re_0 = 0;
            data[i].re_1 = 0;
            data[i].im_0 = cos8(i)>>16;
            data[i].im_1 = cos8(i)>>16;
        }
        break;
    }
    case 3: {
        printf("++++ Test 3: %d point FFT of two complex short int signals: Real: 0, Imag: %d Hz sine\n"
                ,N_FFT_POINTS,INPUT_FREQ);
        for(int i=0; i<N; i++) {
            data[i].re_0 = 0;
            data[i].re_1 = 0;
            data[i].im_0 = sin8(i)>>16;
            data[i].im_1 = sin8(i)>>16;
        }
        break;
    }
    }

#if PRINT_FFT_INPUT
    printf("Generated Input Signal 0:\n");
    printf("re,      im         \n");
    for(int i=0; i<N; i++) {
        printf("%.5f, %.5f\n",F15(data[i].re_0), F15(data[i].im_0));
    }
    printf("Generated Input Signal 1:\n");
    printf("re,      im         \n");
    for(int i=0; i<N; i++) {
        printf("%.5f, %.5f\n",F15(data[i].re_1), F15(data[i].im_1));
    }
#endif
}

int do_complex_fft_and_ifft() {
    timer tmr;
    unsigned start_time, end_time, overhead_time, cycles_taken;
    tmr :> start_time;
    tmr :> end_time;
    overhead_time = end_time - start_time;

    for(int test=0; test<4; test++) {
        generate_two_short_complex_test_signals(data, N_FFT_POINTS, test);

        tmr :> start_time;
        lib_dsp_fft_bit_reverse_short(data, N_FFT_POINTS);
        lib_dsp_fft_forward_complex_short(data, N_FFT_POINTS, FFT_SINE_SHORT(N_FFT_POINTS));
        tmr :> end_time;
        cycles_taken = end_time-start_time-overhead_time;
#if PRINT_CYCLE_COUNT
        printf("Cycles taken for %d point complex FFT of two complex short signals: %d\n", N_FFT_POINTS, cycles_taken);
#endif

#if PRINT_FFT_OUTPUT
        // Print forward complex FFT results
        printf("FFT output. Spectrum of complex signal 0:\n");
        printf("re,      im         \n");
        for(int i=0; i<N_FFT_POINTS; i++) {
            printf( "%.5f, %.5f\n", F15(data[i].re_0), F15(data[i].im_0)); // 16 bit gives 5 decimal points of Precision
        }
        printf("FFT output. Spectrum of complex signal 1:\n");
        printf("re,      im         \n");
        for(int i=0; i<N_FFT_POINTS; i++) {
            printf( "%.5f, %.5f\n", F15(data[i].re_1), F15(data[i].im_1));
        }
#endif

        tmr :> start_time;
        lib_dsp_fft_bit_reverse_short(data, N_FFT_POINTS);
        lib_dsp_fft_inverse_complex_short(data, N_FFT_POINTS, FFT_SINE_SHORT(N_FFT_POINTS));
        tmr :> end_time;
        cycles_taken = end_time-start_time-overhead_time;
#if PRINT_CYCLE_COUNT
        printf("Cycles taken for iFFT: %d\n", cycles_taken);
#endif

#if PRINT_IFFT_OUTPUT
        printf( "////// Time domain signals after lib_dsp_fft_inverse_complex_short\n");
        printf("Complex signal 0:\n");
        printf("re,      im         \n");
        for(int i=0; i<N_FFT_POINTS; i++) {
            printf("%.5f, %.5f\n",F15(data[i].re_0), F15(data[i].im_0));
        }
        printf("Complex signal 1:\n");
        printf("re,      im         \n");
        for(int i=0; i<N_FFT_POINTS; i++) {
            printf("%.5f, %.5f\n",F15(data[i].re_1), F15(data[i].im_1));
        }
#endif

        printf("\n" ); // Test delimiter
    }

    printf( "DONE.\n" );
    return 0;

}
# else
int do_tworeal_fft_and_ifft() {
    timer tmr;
    unsigned start_time, end_time, overhead_time, cycles_taken;
    tmr :> start_time;
    tmr :> end_time;
    overhead_time = end_time - start_time;

    printf("Generating a %d Hz sine and cosine signals for input to the Forward 2xReal FFT\n\n",N_FFT_POINTS/8);
    for(int i=0; i<N_FFT_POINTS; i++) {
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
    lib_dsp_fft_forward_tworeals( two_re, two_im, N_FFT_POINTS, FFT_SINE_SHORT(N_FFT_POINTS) );
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
    lib_dsp_fft_inverse_tworeals( two_re, two_im, N_FFT_POINTS, FFT_SINE_SHORT(N_FFT_POINTS) );
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

