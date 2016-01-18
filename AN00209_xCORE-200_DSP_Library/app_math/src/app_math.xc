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

#define PRINT_CYCLE_COUNT 1
#define CHECK_RESULTS 1
#define TEST_ITERATIONS 40
#define PRINT_AND_ABORT_ON_ERROR 1
#define PRINT_INPUTS_AND_OUTPUTS 1

// max input to sine and cosine is 2*Pi which is < 7.
// That means 3 data bits before the decimal point are enough. Plus one sign bit.
#define SINE_Q 24

int main(void)
{
    int q_format; // location of the decimal point

    unsigned start_time, end_time, overhead_time, cycles_taken;
    timer tmr;
    tmr :> start_time;
    tmr :> end_time;
    overhead_time = end_time - start_time;

    q_format = 24;
    printf ("Multiplication (2.1234567891 x 4.9876543219) : %.10f\n\n", F24(lib_dsp_math_multiply (Q24(2.1234567891), Q24(4.9876543219), q_format)));;

    printf ("Reciprocal (3.0) : %.10f\n\n", F24(lib_dsp_math_reciprocal (Q24(3.0), q_format)));;

    printf ("Inverse square root (2) : %.10f\n\n", F24(lib_dsp_math_invsqrroot (Q24(2.), q_format)));;

    printf ("Square Root (2) : %.10f\n\n", F24(lib_dsp_math_squareroot (Q24(2.), q_format)));;

    int error_cnt_1 = 0, error_cnt_2 = 0;
    int maxerror = 0;
    int val_count = 0;

    printf ("Sine wave (one cycle from -Pi to +Pi) :\n");
    error_cnt_1=0;
    error_cnt_2=0;
    maxerror=0;
    val_count = 0;
    int error_cnt = 0;
    for(q8_24 rad = -PI; rad <= PI; rad += PI2/TEST_ITERATIONS) {
        tmr :> start_time;
        q8_24 sine = lib_dsp_math_sin(rad);
        tmr :> end_time;
        cycles_taken = end_time-start_time-overhead_time;

#ifdef PRINT_INPUTS_AND_OUTPUTS
        printf("sin(%.10f) = %.10f\n",F24(rad), F24(sine));
#endif

#if CHECK_RESULTS
        // check the fixed point result vs the floating point result from math.h
        //
        double d_rad = F24(rad);
        double d_sine_ref = sin(d_rad);
        q8_24 sine_ref = Q24(d_sine_ref);

        q8_24 abs_diff = abs(sine - sine_ref);

        if (abs_diff >= 1) {
            error_cnt_1++;
        }
        if (abs_diff >= 2) {
            error_cnt_2++;
#if PRINT_AND_ABORT_ON_ERROR
            printf("ERROR: absolute error >= 2 is a failure criteria. absolute error is 0x%d\n",abs_diff);
            printf("lib_dsp_math_sin(%.10f) = %.10f\n",F24(rad), F24(sine));
            break;
#endif
        }
        if (abs_diff > maxerror) {
            maxerror = abs_diff;
        }

        /*
        if (abs(abs_diff) >= CHECK_MIN_VALUE) {
            error_cnt++;
            //printf("ERROR: More than three bit difference (%.10f) between expected float result %.10f (reference) and fixed point result %.10f\n",F24(abs_diff),F24(sine_ref), F24(sine));
            printf("ERROR: More than %d bit difference (0x%x) between expected float result 0x%x (reference) and fixed point result 0x%x\n"
                    ,CHECK_RESULTS_BIT_ACCURACY,abs_diff,sine_ref, sine);
        }
        */

        val_count++;
#endif

    }
#if CHECK_RESULTS
    printf("num calculations: %8d; Errors >=1: %8d (%5.2f%%); Errors >=2: %5d (%5.2f%%)\n", val_count, error_cnt_1, error_cnt_1*100.0/val_count, error_cnt_2, error_cnt_2*100.0/val_count);
    printf("Max absolute error: %d\n",maxerror);
    //printf("Number of Errors >= %d from lib_dsp_math_sin: %d\n",CHECK_MIN_VALUE, error_cnt);
#endif

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


    printf("Cosine wave (one cycle from -Pi to +Pi) :\n");
    error_cnt=0;
    error_cnt_1=0;
    error_cnt_2=0;
    maxerror=0;
    val_count = 0;

    //for(int rad = -coeffs.pi; rad <= coeffs.pi; rad += coeffs.two_pi/40) {
    for(q8_24 rad = -PI; rad <= PI; rad += PI2/TEST_ITERATIONS) {
        tmr :> start_time;
        int cosine=lib_dsp_math_cos(rad);
        tmr :> end_time;
        cycles_taken = end_time-start_time-overhead_time;
#ifdef PRINT_INPUTS_AND_OUTPUTS
        printf("cos(%.10f) = %.10f\n",F24(rad), F24(cosine));
#endif
#if CHECK_RESULTS
        // check the fixed point result vs the floating point result from math.h
        double d_rad = F24(rad);
        double d_cosine_ref = cos(d_rad);
        q8_24 cosine_ref = Q24(d_cosine_ref);

        q8_24 abs_diff = abs(cosine - cosine_ref);
        if (abs_diff >= 1) {
            error_cnt_1++;
        }
        if (abs_diff >= 2) {
            error_cnt_2++;
#if PRINT_AND_ABORT_ON_ERROR
            printf("ERROR: absolute error >= 2 is a failure criteria. absolute error is 0x%d\n",abs_diff);
            printf("lib_dsp_math_cos(%.10f) = %.10f\n",F24(rad), F24(cosine));
            break;
#endif
        }
        if (abs_diff > maxerror) {
            maxerror = abs_diff;
        }

        /*
        if (abs(abs_diff) >= CHECK_MIN_VALUE) {
            error_cnt++;
            //printf("ERROR: More than three bit difference (%.10f) between expected float result %.10f (reference) and fixed point result %.10f\n",F24(abs_diff),F24(cosine_ref), F24(sine));
            //printf("ERROR: More than %d bit difference (0x%x) between expected float result 0x%x (reference) and fixed point result 0x%x\n"
            //        ,CHECK_RESULTS_BIT_ACCURACY,abs_diff,cosine_ref, cosine);
        }
        */
#endif
        val_count++;

    }

#if CHECK_RESULTS
    printf("num calculations: %8d; Errors >=1: %8d (%5.2f%%); Errors >=2: %5d (%5.2f%%)\n", val_count, error_cnt_1, error_cnt_1*100.0/val_count, error_cnt_2, error_cnt_2*100.0/val_count);
    printf("Max absolute error: %d\n",maxerror);
    //printf("Number of Errors >= %d from lib_dsp_math_cos: %d\n",CHECK_MIN_VALUE, error_cnt);
#endif


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

