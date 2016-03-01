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

#define CHECK_RESULTS 1
#define PRINT_AND_ABORT_ON_ERROR 1
//#define TEST_ALL_INPUTS 1

#if TEST_ALL_INPUTS
#define PRINT_INPUTS_AND_OUTPUTS 0
#define PRINT_CYCLE_COUNT 0
#define RAD_INCR 1
#define X_INCR 1
#else
#define PRINT_CYCLE_COUNT 0
#define PRINT_INPUTS_AND_OUTPUTS 1
#define RAD_INCR PI2_Q8_24/100
#define X_INCR MAX_Q8_24/100
#endif

//#define EXPONENTIAL_INPUT
// errors from -3..+3
#define ERROR_RANGE 7
typedef struct {
    int errors[ERROR_RANGE];
    int max_error;
    int min_error;
    int num_checked;
} error_s;

int report_errors(unsigned max_abs_error, error_s *e) {
    int result = 1; // PASS
    int half_range = ERROR_RANGE/2;
    for(int i=0; i<ERROR_RANGE; i++) {
        int error = -half_range+i;
        if(i == 0) {
           printf("Cases Error <= %d: %d\n",error, e->errors[i]);
        } else if (i == ERROR_RANGE-1) {
           printf("Cases Error >= %d: %d\n",error, e->errors[i]);
        } else {
           printf("Cases Error == %d: %d\n",error, e->errors[i]);
        }
        if (error > max_abs_error || error < -max_abs_error) {
            result = 0;
        }
    }
    printf("Maximum Positive Error: %d\n",e->max_error);
    printf("Maximum Negative Error: %d\n",e->min_error);
    printf("Number of values checked: %d\n", e->num_checked);
    printf("Percentage of 0 Error: %5.2f%%\n", 100.0*e->errors[half_range]/e->num_checked);
    return result;
}
void reset_errors(error_s *e) {
    for(int i=0; i<ERROR_RANGE; i++) {
        e->errors[i] = 0;
    }
    e->max_error = 0;
    e->min_error = 0;
    e->num_checked = 0;
}

// Returns true if check passed
int check_result(int result, int expected, unsigned max_abs_error, error_s *e) {
    int error = result - expected;
    int half_range = ERROR_RANGE/2;
    // saturate Errors
    if (error < -half_range) error = -half_range;
    if (error > half_range) error = half_range;
    e->errors[error+half_range]++; // increment the error counter

    if (error < e->min_error) e->min_error = error;
    if (error > e->max_error) e->max_error = error;

    e->num_checked++;

    if (error > (int) max_abs_error || error < -((int) max_abs_error)) {
    //if (error > max_error) {
#if PRINT_AND_ABORT_ON_ERROR
        printf("ERROR: absolute error > 0x%x is a failure criteria. Error found is 0x%x\n",max_abs_error, error);
        printf("result is 0x%x, Expected is 0x%x\n",result, expected);
        report_errors(max_abs_error, e);
        exit (0);
#endif
        return 0;
    }
    return 1;
}

unsigned overhead_time;

void test_roots() {
    int q_format = 24; // location of the decimal point
    int start_time, end_time;
    unsigned cycles_taken; // absolute positive values
    timer tmr;
    error_s err;

    reset_errors(&err);

    printf("Test Roots\n");
    printf("----------\n");

    q8_24 result, expected;
    tmr :> start_time;
    result = lib_dsp_math_invsqrroot (Q24(2.), q_format);
    tmr :> end_time;
    cycles_taken = end_time-start_time-overhead_time;
    printf ("Inverse square root (2) : %.7f\n\n", F24(result));
    expected = Q24(1/sqrt(2));
    check_result(result, expected, 1, &err);

    result = lib_dsp_math_squareroot (Q24(2.), q_format);
    printf ("Square Root (2) : %.7f\n\n", F24(result));;
    expected =  Q24(sqrt(2));
    check_result(result, expected, 1, &err);

    printf("Error report from test_roots:\n");
    report_errors(1, &err);
    printf("\n");

}

void test_multipliation_and_division() {
    int q_format = 24; // location of the decimal point
    q8_24 result, expected;
    error_s err;
    reset_errors(&err);

    printf("Test Multiplication and Division\n");
    printf("--------------------------------\n");
    printf("Note: All calculations are done in Q8.24 format. That gives 7 digits of precion after the decimal point\n");
    printf("Note: Maximum double representation of Q8.24 format: %.7f\n\n", F24(0x7FFFFFFF));

    double f0, f1;
    f0 = 11.3137085;
    f1 = 11.3137085;
    // Multiply the square root of 128 (maximum double representation of Q8.24)
    printf ("Multiplication (%.7f x %.7): %.7f\n\n",f0, f1, F24(lib_dsp_math_multiply(Q24(f0), Q24(f1), q_format)));;

    printf ("Multiplication (11.4 x 11.4). Will overflow!: %.7f\n\n", F24(lib_dsp_math_multiply(Q24(11.4), Q24(11.4), q_format)));;

    printf ("Saturated Multiplication (11.4 x 11.4): %.7f\n\n", F24(lib_dsp_math_multiply_sat(Q24(11.4), Q24(11.4), q_format)));;

    /*
    The result of 0.0005 x 0.0005 is 0.00000025. But this number is not representable as a binary.
    The closest representation in Q8.24 format is (4/2^24) = 0.000000238418579
    printf rounds this to 0.0000002 because the formatting string to printf specifies 7 digits of precision after the decimal point.
    This is the maximum precision that can be achieved with the 24 fractional bits of the Q8.24 format.
    */
    printf ("Multiplication of small numbers (0.0005 x 0.0005): %.7f\n\n", F24(lib_dsp_math_multiply(Q24(0.0005), Q24(0.0005), q_format)));;


    result = lib_dsp_math_reciprocal (Q24(3.0), q_format);
    printf ("Reciprocal (3.0) : %.7f\n\n", F24(result));
    expected = Q24(1.0/3.0);
    check_result(result, expected, 1, &err);

    double dividend, divisor;

    dividend = 1.123456; divisor = -128;
    result = lib_dsp_math_divide(Q24(dividend), Q24(divisor), q_format);
    printf ("Signed Division %.7f / %.7f): %.7f\n\n",dividend, divisor, F24(result));
    expected = Q24(dividend/divisor);
    check_result(result, expected, 1, &err);

    dividend = -1.123456; divisor = -128;
    result = lib_dsp_math_divide(Q24(dividend), Q24(divisor), q_format);
    printf ("Signed Division %.7f / %.7f): %.7f\n\n",dividend, divisor, F24(result));
    expected = Q24(dividend/divisor);
    check_result(result, expected, 1, &err);

    dividend = -1.123456; divisor = 127.9999999;
    result = lib_dsp_math_divide(Q24(dividend), Q24(divisor), q_format);
    printf ("Signed Division %.7f / %.7f): %.7f\n\n",dividend, divisor, F24(result));
    expected = Q24(dividend/divisor);
    check_result(result, expected, 1, &err);

    dividend = 1.123456; divisor = 127.9999999;
    result = lib_dsp_math_divide(Q24(dividend), Q24(divisor), q_format);
    printf ("Signed Division %.7f / %.7f): %.7f\n\n",dividend, divisor, F24(result));
    expected = Q24(dividend/divisor);
    check_result(result, expected, 1, &err);

    result = lib_dsp_math_divide_unsigned(Q24(dividend), Q24(divisor), q_format);
    printf ("Division %.7f / %.7f): %.7f\n\n",dividend, divisor, F24(result));
    expected = Q24(dividend/divisor);
    check_result(result, expected, 1, &err);

    printf("Error report from test_multipliation_and_division:\n");
    report_errors(1, &err);
    printf("\n");

}

#define EXPONENTIAL_INPUT

void test_trigonometric() {
    int start_time, end_time;
    unsigned cycles_taken; // absolute positive values
    timer tmr;

    error_s err;
    reset_errors(&err);

    printf("Test Trigonometric Functions\n");
    printf("----------------------------\n");

    /*
     * Testing lib_dsp_math_sin
     */
    printf ("Sine wave (one cycle from -Pi to +Pi) :\n");

    for(q8_24 rad = -PI_Q8_24; rad <= PI_Q8_24; rad += RAD_INCR) {
        tmr :> start_time;
        q8_24 sine = lib_dsp_math_sin(rad);
        tmr :> end_time;
        cycles_taken = end_time-start_time-overhead_time;

#if PRINT_INPUTS_AND_OUTPUTS
        printf("sin(%.7f) = %.7f\n",F24(rad), F24(sine));
#endif

#if CHECK_RESULTS
        // check the fixed point result vs the floating point result from math.h
        double d_rad = F24(rad);
        double d_sine_ref = sin(d_rad);
        q8_24 expected = Q24(d_sine_ref);
        check_result(sine, expected, 1, &err);
#endif

    }
#if CHECK_RESULTS
    printf("Error report from lib_dsp_math_sin:\n");
    report_errors(1, &err);
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
    reset_errors(&err);

    for(q8_24 rad = -PI_Q8_24; rad <= PI_Q8_24; rad += RAD_INCR) {
        tmr :> start_time;
        int cosine=lib_dsp_math_cos(rad);
        tmr :> end_time;
        cycles_taken = end_time-start_time-overhead_time;
#if PRINT_INPUTS_AND_OUTPUTS
        printf("cos(%.7f) = %.7f\n",F24(rad), F24(cosine));
#endif
#if CHECK_RESULTS
        // check the fixed point result vs the floating point result from math.h
        double d_rad = F24(rad);
        double d_cosine_ref = cos(d_rad);
        q8_24 expected = Q24(d_cosine_ref);
        check_result(cosine, expected, 1, &err);
#endif
    }

#if CHECK_RESULTS
    printf("Error report from lib_dsp_math_cos:\n");
    report_errors(1, &err);
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
    printf("\n");

    /*
     * Testing lib_dsp_math_atan
     */
    printf("Test lib_dsp_math_atan\n");
    report_errors(1, &err);

    int worst_cycles=0;
    int worst_cycles_input;

    /*
    * Test result in terms of Errors:
    * num calculations:  1000226; Errors >=1:    44561 ( 4.46%); Errors >=2:     0 ( 0.00%)
    * Max absolute error: 1
    */

#ifdef EXPONENTIAL_INPUT
    unsigned x=0;
    while(x < MAX_Q8_24) {
#else
    for(unsigned x=0; x <= MAX_Q8_24; x+= X_INCR) {
#endif
        tmr :> start_time;
        q8_24 arctan=lib_dsp_math_atan(x);

        tmr :> end_time;
        cycles_taken = end_time-start_time-overhead_time;

        if(cycles_taken>worst_cycles) {
            worst_cycles = cycles_taken;
            worst_cycles_input = x;
        }
#if PRINT_CYCLE_COUNT
        printf("Cycles taken for lib_dsp_math_atan function: %d\n", cycles_taken);
#endif
#if PRINT_INPUTS_AND_OUTPUTS
        printf("atan(%.7f) = %.7f\n",F24(x),F24(arctan));
#endif
#if CHECK_RESULTS
        double d_x = F24(x);
        double d_arctan_ref = atan(d_x);
        q8_24 expected = Q24(d_arctan_ref);
        check_result(arctan, expected, 1, &err);
#endif

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
    printf("Error report from lib_dsp_math_atan:\n");
    report_errors(1, &err);
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
    printf("\n");
}

int main(void)
{

    int start_time, end_time;
    timer tmr;
    tmr :> start_time;
    tmr :> end_time;
    overhead_time = end_time - start_time;

    printf("Test example for Math functions\n");
    printf("===============================\n");

    test_multipliation_and_division();

    test_roots();

    test_trigonometric();

    return (0);
}

