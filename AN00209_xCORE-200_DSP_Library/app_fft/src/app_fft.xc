// Copyright 2015-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// XMOS DSP Library - Example to use FFT and inverse FFT

#include <stdio.h>
#include <xs1.h>
#include <xclib.h>
#include <dsp.h>
#include <stdint.h>

#define TRACE_VALUES 1
#if TRACE_VALUES
#define PRINT_FFT_INPUT 1
#define PRINT_FFT_OUTPUT 1
#define PRINT_IFFT_OUTPUT 1
#define N_FFT_POINTS 32
#define PRINT_CYCLE_COUNT 0
#else
#define PRINT_FFT_INPUT 0
#define PRINT_FFT_OUTPUT 0
#define PRINT_IFFT_OUTPUT 0
#define N_FFT_POINTS 4096
#define PRINT_CYCLE_COUNT 1
#endif

#define INPUT_FREQ N_FFT_POINTS/8

#ifndef INT16_BUFFERS
#define INT16_BUFFERS 0 // disabled by default. int32_t buffers is default
#endif

#ifndef TWOREALS
#define TWOREALS 0 // Processing two real signals with a single complex FFT. Disabled by default
#endif

#if TWOREALS
int32_t do_tworeals_fft_and_ifft();
#else
int32_t do_complex_fft_and_ifft();
#endif

// Array holding one complex signal or two real signals
#if INT16_BUFFERS
dsp_complex_short_t data[N_FFT_POINTS];
#else
dsp_complex_t  data[N_FFT_POINTS];
#endif


/**
 * Experiments with functions that generate sine and cosine signals with a defined number of points
 **/
// Macros to ease the use of the sin_N and cos_N functions
// Note: 31-clz(N) == log2(N) when N is power of two
#define SIN(M, N) sin_N(M, 31-clz(N), dsp_sine_ ## N)
#define COS(M, N) cos_N(M, 31-clz(N), dsp_sine_ ## N)

int32_t sin_N(int32_t x, int32_t log2_points_per_cycle, const int32_t sine[]);
int32_t cos_N(int32_t x, int32_t log2_points_per_cycle, const int32_t sine[]);

// generate sine signal with a configurable number of samples per cycle
#pragma unsafe arrays
int32_t sin_N(int32_t x, int32_t log2_points_per_cycle, const int32_t sine[]) {
    // size of sine[] must be equal to num_points!
    int32_t num_points = (1<<log2_points_per_cycle);
    int32_t half_num_points = num_points>>1;

    x = x & (num_points-1); // mask off the index

    switch (x >> (log2_points_per_cycle-2)) { // switch on upper two bits
       // upper two bits determine the quadrant.
       case 0: return sine[x];
       case 1: return sine[half_num_points-x];
       case 2: return -sine[x-half_num_points];
       case 3: return -sine[num_points-x];
    }
    return 0; // unreachable
}

// generate cosine signal with a configurable number of samples per cycle
#pragma unsafe arrays
int32_t cos_N(int32_t x, int32_t log2_points_per_cycle, const int32_t sine[]) {
    int32_t quarter_num_points = (1<<(log2_points_per_cycle-2));
    return sin_N(x+(quarter_num_points), log2_points_per_cycle, sine); // cos a = sin(a + 2pi/4)
}

int main( void )
{

#if TWOREALS
    do_tworeals_fft_and_ifft();
#else
    do_complex_fft_and_ifft();
#endif
    return 0;
};

#if INT16_BUFFERS
#define RIGHT_SHIFT 17  // shift down to Q14. Q15 would cause overflow.
#else
#define RIGHT_SHIFT 1   // shift down to Q30. Q31 would cause overflow
#endif

void print_data_array() {
#if INT16_BUFFERS
    printf("re,      im         \n");
    for(int32_t i=0; i<N_FFT_POINTS; i++) {
        printf( "%.5f, %.5f\n", F14(data[i].re), F14(data[i].im));
    }
#else
    printf("re,           im         \n");
    for(int32_t i=0; i<N_FFT_POINTS; i++) {
        printf( "%.10f, %.10f\n", F30(data[i].re), F30(data[i].im));
    }
#endif
}

#if TWOREALS
void generate_tworeals_test_signal(int32_t N, int32_t test) {
    switch(test) {
     case 0: {
         printf("++++ Test %d: %d point FFT of two real signals:: re0: %d Hz cosine, re1: %d Hz cosine\n"
                 ,test,N,INPUT_FREQ,INPUT_FREQ);
         for(int32_t i=0; i<N; i++) {
             data[i].re = COS(i, 8) >> RIGHT_SHIFT;
             data[i].im = COS(i, 8) >> RIGHT_SHIFT;
         }
         break;
     }
     case 1: {
         printf("++++ Test %d: %d point FFT of two real signals:: re0: %d Hz sine, re1: %d Hz sine\n"
                 ,test,N,INPUT_FREQ,INPUT_FREQ);
         for(int32_t i=0; i<N; i++) {
             data[i].re = SIN(i, 8) >> RIGHT_SHIFT;
             data[i].im = SIN(i, 8) >> RIGHT_SHIFT;
         }
         break;
     }
     case 2: {
         printf("++++ Test %d: %d point FFT of two real signals:: re0: %d Hz sine, re1: %d Hz cosine\n"
                 ,test,N,INPUT_FREQ,INPUT_FREQ);
         for(int32_t i=0; i<N; i++) {
             data[i].re = SIN(i, 8) >> RIGHT_SHIFT;
             data[i].im = COS(i, 8) >> RIGHT_SHIFT;
         }
         break;
     }
     case 3: {
         printf("++++ Test %d: %d point FFT of two real signals:: re0: %d Hz cosine, re1: %d Hz sine\n"
                 ,test,N,INPUT_FREQ,INPUT_FREQ);
         for(int32_t i=0; i<N; i++) {
             data[i].re = COS(i, 8) >> RIGHT_SHIFT;
             data[i].im = SIN(i, 8) >> RIGHT_SHIFT;
         }
         break;
     }
     }
#if PRINT_FFT_INPUT
    printf("Generated Two Real Input Signals:\n");
    print_data_array();
#endif
}

int32_t do_tworeals_fft_and_ifft() {
    timer tmr;
    uint32_t start_time, end_time, overhead_time, cycles_taken;
    tmr :> start_time;
    tmr :> end_time;
    overhead_time = end_time - start_time;
#if INT16_BUFFERS
    printf("FFT/iFFT of two real signals of type int16_t\n");
#else
    printf("FFT/iFFT of two real signals of type int32_t\n");
#endif
    printf("============================================\n");

    for(int32_t test=0; test<4; test++) {
        generate_tworeals_test_signal(N_FFT_POINTS, test);

        tmr :> start_time;
#if INT16_BUFFERS
        dsp_complex_t tmp_data[N_FFT_POINTS]; // tmp buffer to enable 32-bit FFT/iFFT
        dsp_fft_short_to_long(data, tmp_data, N_FFT_POINTS); // convert into tmp buffer
        dsp_fft_bit_reverse(tmp_data, N_FFT_POINTS);
        dsp_fft_forward(tmp_data, N_FFT_POINTS, FFT_SINE(N_FFT_POINTS));
        dsp_fft_split_spectrum(tmp_data, N_FFT_POINTS);
        dsp_fft_long_to_short(tmp_data, data, N_FFT_POINTS); // convert from tmp buffer
#else
        dsp_fft_bit_reverse(data, N_FFT_POINTS);
        dsp_fft_forward(data, N_FFT_POINTS, FFT_SINE(N_FFT_POINTS));
        dsp_fft_split_spectrum(data, N_FFT_POINTS);
#endif
        tmr :> end_time;
        cycles_taken = end_time-start_time-overhead_time;
#if PRINT_CYCLE_COUNT
        printf("Cycles taken for %d point FFT of two purely real signals: %d\n", N_FFT_POINTS, cycles_taken);
#endif

#if PRINT_FFT_OUTPUT
        // Print forward complex FFT results
        //printf( "First half of Complex FFT output spectrum of Real signal 0 (cosine):\n");
        printf( "FFT output of two half spectra. Second half could be discarded due to symmetry without loosing information\n");
        printf( "spectrum of first signal in data[0..N/2-1]. spectrum of second signa in data[N/2..N-1]:\n");

        print_data_array();
#if 0
        printf( "First half of Complex FFT output spectrum of Real signal 1 (sine):\n");
        printf("re,           im         \n");
        for(int32_t i=0; i<N_FFT_POINTS; i++) {
            printf( "%.10f, %.10f\n", F30(data[i].re), F30(data[i].im));
        }
#endif
#endif

        tmr :> start_time;
#if INT16_BUFFERS
        dsp_fft_short_to_long(data, tmp_data, N_FFT_POINTS); // convert into tmp buffer
        dsp_fft_merge_spectra(tmp_data, N_FFT_POINTS);
        dsp_fft_bit_reverse(tmp_data, N_FFT_POINTS);
        dsp_fft_inverse(tmp_data, N_FFT_POINTS, FFT_SINE(N_FFT_POINTS));
        dsp_fft_long_to_short(tmp_data, data, N_FFT_POINTS); // convert from tmp buffer
#else
        dsp_fft_merge_spectra(data, N_FFT_POINTS);
        dsp_fft_bit_reverse(data, N_FFT_POINTS);
        dsp_fft_inverse(data, N_FFT_POINTS, FFT_SINE(N_FFT_POINTS));
#endif

        tmr :> end_time;
        cycles_taken = end_time-start_time-overhead_time;
#if PRINT_CYCLE_COUNT
        printf("Cycles taken for iFFT: %d\n", cycles_taken);
#endif

#if PRINT_IFFT_OUTPUT
        printf( "////// Time domain signal after dsp_fft_inverse\n");
        print_data_array();
#endif
        printf("\n" ); // Test delimiter
    }

    printf( "DONE.\n" );
    return 0;
}
# else  // Complex Signals
void generate_complex_test_signal(int32_t N, int32_t test) {
    switch(test) {
    case 0: {
        printf("++++ Test 0: %d point FFT/iFFT of complex signal:: Real: %d Hz cosine, Imag: 0\n"
                ,N_FFT_POINTS,N_FFT_POINTS/8);
        for(int32_t i=0; i<N; i++) {
            data[i].re = COS(i, 8) >> RIGHT_SHIFT;
            data[i].im = 0;
        }
        break;
    }
    case 1: {
        printf("++++ Test 1: %d point FFT/iFFT of complex signal:: Real: %d Hz sine, Imag: 0\n"
                ,N_FFT_POINTS,N_FFT_POINTS/8);
        for(int32_t i=0; i<N; i++) {
            data[i].re = SIN(i, 8) >> RIGHT_SHIFT;
            data[i].im = 0;
        }
        break;
    }
    case 2: {
        printf("++++ Test 2: %d point FFT/iFFT of complex signal: Real: 0, Imag: %d Hz cosine\n"
                ,N_FFT_POINTS,N_FFT_POINTS/8);
        for(int32_t i=0; i<N; i++) {
            data[i].re = 0;
            data[i].im = COS(i, 8) >> RIGHT_SHIFT;
        }
        break;
    }
    case 3: {
        printf("++++ Test 3: %d point FFT/iFFT of complex signal: Real: 0, Imag: %d Hz sine\n"
                ,N_FFT_POINTS,N_FFT_POINTS/8);
        for(int32_t i=0; i<N; i++) {
            data[i].re = 0;
            data[i].im = SIN(i, 8) >> RIGHT_SHIFT;
        }
        break;
    }
    }
#if PRINT_FFT_INPUT
    printf("Generated Signal:\n");
    print_data_array();
#endif
}

int32_t do_complex_fft_and_ifft() {
    timer tmr;
    uint32_t start_time, end_time, overhead_time, cycles_taken;
    tmr :> start_time;
    tmr :> end_time;
    overhead_time = end_time - start_time;

#if INT16_BUFFERS
    printf("FFT/iFFT of a complex signal of type int16_t\n");
#else
    printf("FFT/iFFT of a complex signal of type int32_t\n");
#endif
    printf("============================================\n");

    for(int32_t test=0; test<4; test++) {
        generate_complex_test_signal(N_FFT_POINTS, test);

        tmr :> start_time;

#if INT16_BUFFERS
        dsp_complex_t tmp_data[N_FFT_POINTS]; // tmp buffer to enable 32-bit FFT/iFFT
        // convert into int32_t temporary buffer
        dsp_fft_short_to_long(data, tmp_data, N_FFT_POINTS); 
        // 32 bit FFT
        dsp_fft_bit_reverse(tmp_data, N_FFT_POINTS);
        dsp_fft_forward(tmp_data, N_FFT_POINTS, FFT_SINE(N_FFT_POINTS));
        // convert back into int16_t buffer
        dsp_fft_long_to_short(tmp_data, data, N_FFT_POINTS); 
#else
        // 32 bit FFT
        dsp_fft_bit_reverse(data, N_FFT_POINTS);
        dsp_fft_forward(data, N_FFT_POINTS, FFT_SINE(N_FFT_POINTS));
#endif
        tmr :> end_time;
        cycles_taken = end_time-start_time-overhead_time;


#if PRINT_CYCLE_COUNT
        printf("Cycles taken for %d point FFT of complex signal: %d\n", N_FFT_POINTS, cycles_taken);
#endif


#if PRINT_FFT_OUTPUT
        printf( "FFT output:\n");
        print_data_array();
#endif

        tmr :> start_time;
#if INT16_BUFFERS
        // convert into int32_t temporary buffer
        dsp_fft_short_to_long(data, tmp_data, N_FFT_POINTS); 
        // 32 bit iFFT
        dsp_fft_bit_reverse(tmp_data, N_FFT_POINTS);
        dsp_fft_inverse(tmp_data, N_FFT_POINTS, FFT_SINE(N_FFT_POINTS));
        // convert back into int16_t buffer
        dsp_fft_long_to_short(tmp_data, data, N_FFT_POINTS); 
#else
        // 32 bit iFFT
        dsp_fft_bit_reverse(data, N_FFT_POINTS);
        dsp_fft_inverse(data, N_FFT_POINTS, FFT_SINE(N_FFT_POINTS));
#endif
        tmr :> end_time;
        cycles_taken = end_time-start_time-overhead_time;
#if PRINT_CYCLE_COUNT
        printf("Cycles taken for iFFT: %d\n", cycles_taken);
#endif

#if PRINT_IFFT_OUTPUT
        printf( "////// Time domain signal after dsp_fft_inverse\n");
        print_data_array();
#endif
        printf("\n" ); // Test delimiter
    }

    printf( "DONE.\n" );
    return 0;

}
#endif
