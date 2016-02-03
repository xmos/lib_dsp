// Copyright (c) 2016, XMOS Ltd, All rights reserved
// XMOS DSP Library - Example to use FFT and inverse FFT

#include <stdio.h>
#include <xs1.h>
#include <lib_dsp.h>

#define TRACE_VALUES
#ifdef TRACE_VALUES
#define PRINT_FFT_INPUT 1
#define PRINT_FFT_OUTPUT 1
#define PRINT_IFFT_OUTPUT 1
#define N_FFT_POINTS 32
#else
#define PRINT_FFT_INPUT 0
#define PRINT_FFT_OUTPUT 0
#define PRINT_IFFT_OUTPUT 0
#define N_FFT_POINTS 4096
#endif

#define PRINT_CYCLE_COUNT 0

#define INPUT_FREQ N_FFT_POINTS/8

// Note: Use these data types to guarantee 64 bit alignment
// and having re and im together in one double word for efficient ldd and std
#ifdef COMPLEX_FFT
lib_dsp_fft_complex_t  data[N_FFT_POINTS];
int do_complex_fft_and_ifft();
#else
lib_dsp_fft_complex_t  two_re[N_FFT_POINTS];
lib_dsp_fft_complex_t  two_im[N_FFT_POINTS];
int do_tworeals_fft_and_ifft();
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
    do_tworeals_fft_and_ifft();
#endif
    return 0;
};



#ifdef COMPLEX_FFT

void generate_complex_test_signal(lib_dsp_fft_complex_t data[], int N, int test) {
    switch(test) {
    case 0: {
        printf("++++ Test 0: %d point FFT/iFFT of complex signal:: Real: %d Hz cosine, Imag: 0\n"
                ,N_FFT_POINTS,INPUT_FREQ);
        for(int i=0; i<N; i++) {
            data[i].re = cos8(i);
            data[i].im = 0;
        }
        break;
    }
    case 1: {
        printf("++++ Test 1: %d point FFT/iFFT of complex signal:: Real: %d Hz sine, Imag: 0\n"
                ,N_FFT_POINTS,INPUT_FREQ);
        for(int i=0; i<N; i++) {
            data[i].re = sin8(i);
            data[i].im = 0;
        }
        break;
    }
    case 2: {
        printf("++++ Test 2: %d point FFT/iFFT of complex signal: Real: 0, Imag: %d Hz cosine\n"
                ,N_FFT_POINTS,INPUT_FREQ);
        for(int i=0; i<N; i++) {
            data[i].re = 0;
            data[i].im = cos8(i);
        }
        break;
    }
    case 3: {
        printf("++++ Test 3: %d point FFT/iFFT of complex signal: Real: 0, Imag: %d Hz sine\n"
                ,N_FFT_POINTS,INPUT_FREQ);
        for(int i=0; i<N; i++) {
            data[i].re = 0;
            data[i].im = sin8(i);
        }
        break;
    }
    }
#if PRINT_FFT_INPUT
    printf("Generated Signal:\n");
    printf("re,           im         \n");
    for(int i=0; i<N; i++) {
        printf("%.10f, %.10f\n",F31(data[i].re), F31(data[i].im));
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
        generate_complex_test_signal(data, N_FFT_POINTS, test);

        tmr :> start_time;
        lib_dsp_fft_bit_reverse(data, N_FFT_POINTS);
        //lib_dsp_fft_forward_complex_xs1(data, N_FFT_POINTS, FFT_SINE(N_FFT_POINTS));
        lib_dsp_fft_forward_complex(data, N_FFT_POINTS, FFT_SINE(N_FFT_POINTS));
        tmr :> end_time;
        cycles_taken = end_time-start_time-overhead_time;
#if PRINT_CYCLE_COUNT
        printf("Cycles taken for %d point FFT of complex signal: %d\n", N_FFT_POINTS, cycles_taken);
#endif

#if PRINT_FFT_OUTPUT
        // Print forward complex FFT results
        printf( "FFT output. spectrum of signal:\n");
        printf("re,           im         \n");
        for(int i=0; i<N_FFT_POINTS; i++) {
            printf( "%.10f, %.10f\n", F31(data[i].re), F31(data[i].im));
        }
#endif

        tmr :> start_time;
        lib_dsp_fft_bit_reverse(data, N_FFT_POINTS);
        lib_dsp_fft_inverse_complex(data, N_FFT_POINTS, FFT_SINE(N_FFT_POINTS));
        tmr :> end_time;
        cycles_taken = end_time-start_time-overhead_time;
#if PRINT_CYCLE_COUNT
        printf("Cycles taken for iFFT: %d\n", cycles_taken);
#endif

#if PRINT_IFFT_OUTPUT
        printf( "////// Time domain signal after lib_dsp_fft_inverse_complex\n");
        printf("re,           im         \n");
        for(int i=0; i<N_FFT_POINTS; i++) {
            printf( "%.10f, %.10f\n", F31(data[i].re), F31(data[i].im));
        }
#endif
        printf("\n" ); // Test delimiter
    }

    printf( "DONE.\n" );
    return 0;

}
# else
void generate_tworeals_test_signal(int N, int test) {
    switch(test) {
    case 0: {
        printf("++++ Test %d: %d point FFT of two real signals:: re0: %d Hz cosine, re1: %d Hz cosine\n"
                ,test,N,INPUT_FREQ,INPUT_FREQ);
        for(int i=0; i<N; i++) {
            two_re[i].re = cos8(i);
            two_re[i].im = cos8(i);
            two_im[i].re = 0;
            two_im[i].im = 0;
        }
        break;
    }
    case 1: {
        printf("++++ Test %d: %d point FFT of two real signals:: re0: %d Hz sine, re1: %d Hz sine\n"
                 ,test,N,INPUT_FREQ,INPUT_FREQ);
        for(int i=0; i<N; i++) {
            two_re[i].re = sin8(i);
            two_re[i].im = sin8(i);
            two_im[i].re = 0;
            two_im[i].im = 0;
        }
        break;
    }
    case 2: {
        printf("++++ Test %d: %d point FFT of two real signals:: re0: %d Hz sine, re1: %d Hz cosine\n"
                 ,test,N,INPUT_FREQ,INPUT_FREQ);
        for(int i=0; i<N; i++) {
            two_re[i].re = sin8(i);;
            two_re[i].im = cos8(i);
            two_im[i].re = 0;
            two_im[i].im = 0;
        }
        break;
    }
    case 3: {
        printf("++++ Test %d: %d point FFT of two real signals:: re0: %d Hz cosine, re1: %d Hz sine\n"
                 ,test,N,INPUT_FREQ,INPUT_FREQ);
        for(int i=0; i<N; i++) {
            two_re[i].re = cos8(i);;
            two_re[i].im = sin8(i);
            two_im[i].re = 0;
            two_im[i].im = 0;
        }
        break;
    }
    }
#if PRINT_FFT_INPUT
    printf("Generated Two Real Input Signals:\n");
    printf("re0,          re1         \n");
    for(int i=0; i<N; i++) {
        printf("%.10f, %.10f\n",F31(two_re[i].re), F31(two_re[i].im));
    }
#endif
}
int do_tworeals_fft_and_ifft() {
    timer tmr;
    unsigned start_time, end_time, overhead_time, cycles_taken;
    tmr :> start_time;
    tmr :> end_time;
    overhead_time = end_time - start_time;

    for(int test=0; test<4; test++) {
        generate_tworeals_test_signal(N_FFT_POINTS, test);

        printf("Forward 2xReal FFT, Size = %05u\n", N_FFT_POINTS );
        tmr :> start_time;
        lib_dsp_fft_forward_tworeals( two_re, two_im, N_FFT_POINTS, FFT_SINE(N_FFT_POINTS) );
        tmr :> end_time;

        cycles_taken = end_time-start_time-overhead_time;
#if PRINT_CYCLE_COUNT
        printf("Cycles taken for %d point FFT of two purely real signals: %d\n", N_FFT_POINTS, cycles_taken);
#endif

#if PRINT_FFT_OUTPUT
        // Print forward complex FFT results
        printf( "Complex FFT output spectrum of Real signal 0 (cosine):\n");
        printf("re,           im         \n");
        for(int i=0; i<N_FFT_POINTS; i++) {
            printf( "%.10f, %.10f\n", F31(two_re[i].re), F31(two_im[i].re));
        }
        printf( "Complex FFT output spectrum of Real signal 1 (sine):\n");
        printf("re,           im         \n");
        for(int i=0; i<N_FFT_POINTS; i++) {
            printf( "%.10f, %.10f\n", F31(two_re[i].im), F31(two_im[i].im));
        }
#endif

        printf( "Inverse 2xReal FFT, Size = %05u\n", N_FFT_POINTS );
        tmr :> start_time;
        lib_dsp_fft_inverse_tworeals( two_re, two_im, N_FFT_POINTS, FFT_SINE(N_FFT_POINTS) );
        tmr :> end_time;

        cycles_taken = end_time-start_time-overhead_time;
#if PRINT_CYCLE_COUNT
        printf("Cycles taken for iFFT: %d\n", cycles_taken);
#endif

#if PRINT_IFFT_OUTPUT
        printf( "////// Time domain signal after lib_dsp_fft_inverse_tworeals\n");
        printf("Recovered Two Real Input Signals:\n");
        printf("re,           re         \n");
        for(int i=0; i<N_FFT_POINTS; i++) {
            printf( "%.10f, %.10f\n", F31(two_re[i].re), F31(two_re[i].im));
        }
#endif
        printf("\n" ); // Test delimiter
    }
    printf( "DONE.\n" );
    return 0;
}
#endif

