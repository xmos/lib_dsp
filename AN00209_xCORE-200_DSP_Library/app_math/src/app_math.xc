// Copyright (c) 2016, XMOS Ltd, All rights reserved
// XMOS DSP Library - Math Functions Test Program
// Uses Q24 format

// Include files
#include <stdio.h>
#include <xs1.h>
#include <lib_dsp.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

// Define constants

#define Q_M               8
#define Q_N               24

#define PRINT_CYCLE_COUNT 1

int main(void)
{
    //  printf ("Add With Saturate (2 + 4) : %d\n", F24(xmos_dsp_math_adds (2, 4)));;
    unsigned start_time, end_time, overhead_time, cycles_taken;
    timer tmr;
    tmr :> start_time;
    tmr :> end_time;
    overhead_time = end_time - start_time;

    printf ("Multiplication (2.1234567891 x 4.9876543219) : %.10f\n\n", F24(lib_dsp_math_multiply (Q24(2.1234567891), Q24(4.9876543219), Q_N)));;

    printf ("Reciprocal (3.0) : %.10f\n\n", F24(lib_dsp_math_reciprocal (Q24(3.0), Q_N)));;

    printf ("Inverse square root (2) : %.10f\n\n", F24(lib_dsp_math_invsqrroot (Q24(2.), Q_N)));;

    printf ("Square Root (2) : %.10f\n\n", F24(lib_dsp_math_squareroot (Q24(2.), Q_N)));;

    printf ("Sine wave (one cycle) :\n");
    for(int rad = 0; rad <= PI2; rad += PI2/40) {
        tmr :> start_time;
        int sine=lib_dsp_math_sin(rad, Q_N);
        tmr :> end_time;
        cycles_taken = end_time-start_time-overhead_time;
        printf("sin(%.10f) = %.10f\n",F24(rad), F24(sine));
    }

#if PRINT_CYCLE_COUNT
    printf("Cycles taken for lib_dsp_math_sin function: %d\n", cycles_taken);
    // just to measure cycles
    tmr :> start_time;
    float sine_float = sin(3.141592653589793/4);
    printf("math.h sin(%.10f) = %.10f\n",3.141592653589793/4, sine_float);
    tmr :> end_time;
    cycles_taken = end_time-start_time-overhead_time;
    printf("Cycles taken for math.h sine function: %d\n", cycles_taken);
#endif
    printf("\n");


    printf("Cosine wave (one cycle) :\n");
    for(int rad = 0; rad <= PI2; rad += PI2/40) {
        tmr :> start_time;
        int cosine=lib_dsp_math_cos(rad, Q_N);
        tmr :> end_time;
        cycles_taken = end_time-start_time-overhead_time;
        printf("cos(%.10f) = %.10f\n",F24(rad), F24(cosine));
    }
#if PRINT_CYCLE_COUNT
    printf("Cycles taken for cosine function: %d\n", cycles_taken);
    // just to measure cycles
    tmr :> start_time;
    float cosine_float = cos(3.141592653589793/4);
    printf("math.h cos(%.10f) = %.10f\n",3.141592653589793/4, cosine_float);
    tmr :> end_time;
    cycles_taken = end_time-start_time-overhead_time;
    printf("Cycles taken for math.h cosine function: %d\n", cycles_taken);
#endif


    return (0);
}

