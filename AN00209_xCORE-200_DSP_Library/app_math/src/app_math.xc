// Copyright (c) 2016, XMOS Ltd, All rights reserved
// XMOS DSP Library - Math Functions Test Program
// Uses Q24 format

// Include files
#include <stdio.h>
#include <xs1.h>
#include <lib_dsp.h>
#include <math.h>
#include <float.h>
#include <stdint.h>
#include <stdlib.h>

#define PRINT_CYCLE_COUNT 1
#define CHECK_RESULTS 1
#define PRINT_AND_ABORT_ON_ERROR 1
#define TEST_ALL_RAD 0

#if TEST_ALL_RAD
#define PRINT_INPUTS_AND_OUTPUTS 0
#define RAD_INCR 1
#else
#define PRINT_INPUTS_AND_OUTPUTS 1
#define RAD_INCR PI2_Q8_24/40
#endif

//#define EXPONENTIAL_INPUT

int main(void)
{
    int q_format; // location of the decimal point

    int start_time, end_time;
    unsigned overhead_time, cycles_taken; // absolute positive values
    timer tmr;
    tmr :> start_time;
    tmr :> end_time;
    overhead_time = end_time - start_time;

    int error_cnt_1 = 0, error_cnt_2 = 0;
    int maxerror = 0;
    int val_count = 0;


#if 0
    q_format = 24;
    printf("Test example for Math functions\n");
    printf("===============================\n");
    printf("Note: All calculations are done in Q8.24 format. That gives 7 digits of precion after the decimal point\n\n");

    // Multiply the square root of 128 (maximum double representation of Q8.24)
    printf ("Multiplication (11.31370850 x 11.31370850): %.7f\n\n", F24(lib_dsp_math_multiply(Q24(11.31370850), Q24(11.31370850), q_format)));;

    printf ("Multiplication (11.4 x 11.4). Will overflow!: %.7f\n", F24(lib_dsp_math_multiply(Q24(11.4), Q24(11.4), q_format)));;

    printf ("Saturated Multiplication (11.4 x 11.4): %.7f\n", F24(lib_dsp_math_multiply_sat(Q24(11.4), Q24(11.4), q_format)));;
    printf ("Note: Maximum double representation of Q8.24 format: %.7f\n\n", F24(0x7FFFFFFF));

    /*
    The result of 0.0005 x 0.0005 is 0.00000025. But this number is not representable as a binary.
    The closest representation in Q8.24 format is (4/2^24) = 0.000000238418579
    printf rounds this to 0.0000002 because the formatting string to printf specifies 7 digits of precision after the decimal point.
    This is the maximum precision that can be achieved with the 24 fractional bits of the Q8.24 format.
    */
    printf ("Multiplication of small numbers (0.0005 x 0.0005): %.7f\n\n", F24(lib_dsp_math_multiply(Q24(0.0005), Q24(0.0005), q_format)));;

    printf ("Reciprocal (3.0) : %.7f\n\n", F24(lib_dsp_math_reciprocal (Q24(3.0), q_format)));;

    printf ("Inverse square root (2) : %.7f\n\n", F24(lib_dsp_math_invsqrroot (Q24(2.), q_format)));;

    printf ("Square Root (2) : %.7f\n\n", F24(lib_dsp_math_squareroot (Q24(2.), q_format)));;


    /*
     *** Testing trigonometric functions
     */

    /*
     * Testing lib_dsp_math_sin
     */
    printf ("Sine wave (one cycle from -Pi to +Pi) :\n");
    error_cnt_1=0;
    error_cnt_2=0;
    maxerror=0;
    val_count = 0;

    for(q8_24 rad = -PI_Q8_24; rad <= PI_Q8_24; rad += RAD_INCR) {
        tmr :> start_time;
        q8_24 sine = lib_dsp_math_sin(rad);
        tmr :> end_time;
        cycles_taken = end_time-start_time-overhead_time;

#ifdef PRINT_INPUTS_AND_OUTPUTS
        printf("sin(%.7f) = %.7f\n",F24(rad), F24(sine));
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
            printf("ERROR: absolute error >= 2 is a failure criteria. absolute error for rad 0x%x is 0x%x\n",rad,abs_diff);
            printf("lib_dsp_math_sin(%.7f) = %.7f\n",F24(rad), F24(sine));
            break;
#endif
        }
        if (abs_diff > maxerror) {
            maxerror = abs_diff;
        }

        val_count++;
#endif

    }
#if CHECK_RESULTS
    printf("num calculations: %8d; Errors >=1: %8d (%5.2f%%); Errors >=2: %5d (%5.2f%%)\n", val_count, error_cnt_1, error_cnt_1*100.0/val_count, error_cnt_2, error_cnt_2*100.0/val_count);
    printf("Max absolute error: %d\n",maxerror);
#endif

#if PRINT_CYCLE_COUNT
    printf("Cycles taken for lib_dsp_math_sin function: %d\n", cycles_taken);
    // just to measure cycles
    tmr :> start_time;
    double sine_float = sin(3.141592653589793/4);
    tmr :> end_time;
    cycles_taken = end_time-start_time-overhead_time;
    printf("math.h sin(%.7f) = %.7f\n",3.141592653589793/4, sine_float);
    printf("Cycles taken for math.h sine function: %d\n", cycles_taken);
#endif
    printf("\n");

    /*
     * Testing lib_dsp_math_sin
     */
    printf("Cosine wave (one cycle from -Pi to +Pi) :\n");

    error_cnt_1=0;
    error_cnt_2=0;
    maxerror=0;
    val_count = 0;

    for(q8_24 rad = -PI_Q8_24; rad <= PI_Q8_24; rad += RAD_INCR) {
        tmr :> start_time;
        int cosine=lib_dsp_math_cos(rad);
        tmr :> end_time;
        cycles_taken = end_time-start_time-overhead_time;
#ifdef PRINT_INPUTS_AND_OUTPUTS
        printf("cos(%.7f) = %.7f\n",F24(rad), F24(cosine));
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
            printf("ERROR: absolute error >= 2 is a failure criteria. absolute error for rad 0x%x is 0x%x\n",rad,abs_diff);
            printf("lib_dsp_math_cos(%.7f) = %.7f\n",F24(rad), F24(cosine));
            break;
#endif
        }
        if (abs_diff > maxerror) {
            maxerror = abs_diff;
        }
#endif
        val_count++;

    }

#if CHECK_RESULTS
    printf("num calculations: %8d; Errors >=1: %8d (%5.2f%%); Errors >=2: %5d (%5.2f%%)\n", val_count, error_cnt_1, error_cnt_1*100.0/val_count, error_cnt_2, error_cnt_2*100.0/val_count);
    printf("Max absolute error: %d\n",maxerror);
#endif


#if PRINT_CYCLE_COUNT
    printf("Cycles taken for cosine function: %d\n", cycles_taken);
    // just to measure cycles
    tmr :> start_time;
    double cosine_float = cos(3.141592653589793/4);
    tmr :> end_time;
    cycles_taken = end_time-start_time-overhead_time;
    printf("math.h cos(%.7f) = %.7f\n",3.141592653589793/4, cosine_float);
    printf("Cycles taken for math.h cosine function: %d\n", cycles_taken);
#endif

#endif
    /*
     * Testing lib_dsp_math_atan
     */
    printf("Calculate and check atan for input values from TBD\n");

    error_cnt_1=0;
    error_cnt_2=0;
    maxerror=0;
    val_count = 0;
    int worst_cycles=0;
    int worst_cycles_input;

#if 0
    printf("p0 = %d\n", Q24(-0.136887688941919269e2));
    printf("p1 = %d\n", Q24(-0.205058551958616520e2));
    printf("p2 = %d\n", Q24(-0.849462403513206835e1));
    printf("q2 = %d\n", Q24( 0.595784361425973445e2));

    //Just to get a trace of the funtion
    lib_dsp_math_atan(Q24(1.0111983));
    return;
#endif

    //Test result in terms of Errors:
    //num calculations:    10001; Errors >=1:     8538 (85.37%); Errors >=2:  3641 (36.41%)
    //Max absolute error: 176

    //Using lib_dsp_math_multiply (does rounding) improves the result:
    //num calculations: 10001; Errors >=1: 6341 (63.40%); Errors >=2: 1373 (13.73%)
    //Max absolute error: 176

#ifdef EXPONENTIAL_INPUT
    unsigned x=0;
    while(x < MAX_Q8_24) {
#else
    for(unsigned x=0; x <= MAX_Q8_24; x+= MAX_Q8_24/10000) {
#endif
        tmr :> start_time;
        //double x_ = F24(x);
        q8_24 arctan=lib_dsp_math_atan(x);
        //double arctan_=lib_dsp_math_atan(x_);
        tmr :> end_time;
        cycles_taken = end_time-start_time-overhead_time;

        if(cycles_taken>worst_cycles) {
            worst_cycles = cycles_taken;
            worst_cycles_input = x;
        }
#if PRINT_CYCLE_COUNT
        printf("Cycles taken for lib_dsp_math_atan function: %d\n", cycles_taken);
#endif
#ifdef PRINT_INPUTS_AND_OUTPUTS
        printf("x is 0x%x\n",x);
        printf("atan(%.7f) = %.7f\n",F24(x),F24(arctan));
#endif
#if CHECK_RESULTS
        //double d_x = F24(x);
        double d_x = F24(x);
        double d_arctan_ref = atan(d_x);
        q8_24 arctan_ref = Q24(d_arctan_ref);
        //q8_24 arctan = Q24(arctan_);
        q8_24 abs_diff = abs(arctan - arctan_ref);
        if (abs_diff >= 1) {
            error_cnt_1++;
        }
        if (abs_diff >= 2) {
            error_cnt_2++;
#if PRINT_AND_ABORT_ON_ERROR
            //printf("ERROR: absolute error >= 2 is a failure criteria. absolute error for rad 0x%x is 0x%x\n",x,abs_diff);
            printf("ERROR: absolute error >= 2 is a failure criteria. absolute error is 0x%x (%5.2f%%)\n",abs_diff, abs_diff*100.0/arctan_ref);
            //printf("lib_dsp_math_atan(%.7f) = %.7f\n",F24(x), F24(arctan));
            //printf("Expected: atan(%.7f) = %.7f\n",F24(x), F24(arctan_ref));
            printf("lib_dsp_math_atan(%.7f) = %.7f\n",F24(x), F24(arctan));
            printf("Expected: atan(%.7f) = %.7f\n",d_x, d_arctan_ref);
            printf("\n");
            //break;
#endif
        }
        if (abs_diff > maxerror) {
            maxerror = abs_diff;
        }
#endif
        val_count++;

#ifdef EXPONENTIAL_INPUT
        // this generates input values 0, x = 2^y
        if(x == 0) {
          x = 1; // second smallest value in q4_28
        } else {
          x *= 2;
        }
#endif

    }

#if CHECK_RESULTS
    printf("num calculations: %8d; Errors >=1: %8d (%5.2f%%); Errors >=2: %5d (%5.2f%%)\n", val_count, error_cnt_1, error_cnt_1*100.0/val_count, error_cnt_2, error_cnt_2*100.0/val_count);
    printf("Max absolute error: %d\n",maxerror);
#endif

#if PRINT_CYCLE_COUNT
    printf("max cyles taken for lib_dsp_math_atan function: %d, input value was %.7f\n", worst_cycles, F24(worst_cycles_input));
    // just to measure cycles
    double d_x = F24(worst_cycles_input);
    tmr :> start_time;
    double d_arctan = atan(d_x);
    tmr :> end_time;
    cycles_taken = end_time-start_time-overhead_time;
    printf("math.h atan(%.7f) = %.7f\n",d_x, d_arctan);
    printf("Cycles taken for math.h atan function: %u\n", cycles_taken);
#endif

    return (0);
}

