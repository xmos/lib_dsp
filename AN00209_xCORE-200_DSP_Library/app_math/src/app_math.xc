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
#define PRINT_ERROR_TOO_BIG 1
#define EXIT_ON_ERROR_TOO_BIG 0
//#define TEST_ALL_INPUTS 1
#define NO_Q 0

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

static unsigned short square16LUT [256];

/** Scalar square root
 *
 *  This function computes the square root of the input value x
 *
 *  \param  x            Unsigned 32-bit input value for computation.
 *  \returns             Unsigned 16-bit output value.
 */
#pragma unsafe arrays
unsigned short sqrt32_lut_iter (const unsigned src)
{
   unsigned short      root;
   unsigned short      newBit = 0x80;
   unsigned            newWord;
   unsigned long long  square;
   const unsigned short *offset = square16LUT;

   unsigned i = src >> 16;

                                                   // Calculate upper 16 bits using look up table
   if (offset[128] <= i) offset += 128;
   if (offset[ 64] <= i) offset += 64;
   if (offset[ 32] <= i) offset += 32;
   if (offset[ 16] <= i) offset += 16;
   if (offset[  8] <= i) offset += 8;
   if (offset[  4] <= i) offset += 4;
   if (offset[  2] <= i) offset += 2;
   if (offset[  1] <= i) offset += 1;
   root = (offset - square16LUT) << 8;

   for (i = 0; i < 8; i++)                         // Iterate remaining 16 bits
   {
     newWord = root | newBit;                      // Add in new bit
     square = (unsigned long long) newWord * newWord;
     if (src >= square)
     {
       root = newWord;
     }
     newBit >>= 1;
   }
   return root;

}

// errors from -3..+3
#define ERROR_RANGE 7
typedef struct {
    int errors[ERROR_RANGE];
    int max_positive_error;
    int max_negative_error;
    int num_checked;
} error_s;

/*
 * Report Errors within the Error Range. Top and Bottom or range contain saturated values
 */
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
    printf("Maximum Positive Error: %d\n",e->max_positive_error);
    printf("Maximum Negative Error: %d\n",e->max_negative_error);
    printf("Number of values checked: %d\n", e->num_checked);
    printf("Percentage of 0 Error: %5.2f%%\n", 100.0*e->errors[half_range]/e->num_checked);
    return result;
}
void reset_errors(error_s *e) {
    for(int i=0; i<ERROR_RANGE; i++) {
        e->errors[i] = 0;
    }
    e->max_positive_error = 0;
    e->max_negative_error = 0;
    e->num_checked = 0;
}

/* Function to check a result against a expected result and store error statistics
 *
 * \param[in]      result
 * \param[in]      expected result
 * \param[in]      max absolute error
 * \param[in,out]  pointer to error struct object
 * \returns        true if check passed
*/
//#define CHECK_UNSIGNED_RESULT

int check_result(int result, int expected, unsigned max_abs_error, error_s *e) {
    static int half_range = ERROR_RANGE/2;
    unsigned error_found=0;

    int error = result - expected;

    // Save max positive and negative error
    if (error < e->max_negative_error) e->max_negative_error = error;
    if (error > e->max_positive_error) e->max_positive_error = error;

    if (error > (int) max_abs_error || error < -((int) max_abs_error)) {
        error_found=1;
    }

    if (error_found) {
    //if (error > max_error) {
#if PRINT_ERROR_TOO_BIG
        printf("ERROR: absolute error > %d is a failure criteria. Error found is %d\n",max_abs_error, error);
        printf("result is 0x%x, Expected is 0x%x\n",result, expected);
        printf("\n");
#if EXIT_ON_ERROR_TOO_BIG
        report_errors(max_abs_error, e);
        exit (0);
#endif
#endif
    }
    // saturate Errors
    if (error < -half_range) error = -half_range;
    if (error > half_range) error = half_range;

    e->errors[error+half_range]++; // increment the error counter

    e->num_checked++;

    return error_found;
}

unsigned overhead_time;
//#define TEST_LUT_SQRT  // LUT interpolation method
#define TEST_NR_SQRT   // Newton Raphson approximation method


//#define CHECK_SQRT_SQUARED

//Todo: Test if this performs as well as the conversion macros in lib_dsp_qformat.h
inline int qs(double d, const int q_format) {
  return (int)((signed long long)((d) * ((unsigned long long)1 << (q_format+20)) + (1<<19)) >> 20);
}

void test_roots() {
    int start_time, end_time;
    unsigned cycles_taken; // absolute positive values
    timer tmr;
    error_s err;

    reset_errors(&err);

    for (int i = 0; i < 256; i++)                       // Initialize look up table
    {
      square16LUT[i] = i*i;
    }

    printf("Test Roots\n");
    printf("----------\n");

    unsigned result, expected;

    for(int i=1; i<=32; i++) { // exponent
        unsigned x = (unsigned long long) (1<<i)-1; // 2^x - 1 (x in 1..31)

        //printf ("x == 0x%x\n", x);
        //printf("\n");

        double d_sqrt;
#ifdef TEST_LUT_SQRT
        TIME_FUNCTION(result = sqrt32_lut_iter(x));
        printf ("Square Root LUT Method (%.8f) : %.5f\n", F24(x), F12(result));
        d_sqrt = sqrt(F24(x));
        expected =  qs(d_sqrt, 12);

        check_result(result, expected, 1, &err);
#if PRINT_CYCLE_COUNT
        printf("Cycles taken for sqrt32_lut_iter function: %d\n", cycles_taken);
#endif
#endif

#ifdef TEST_NR_SQRT
        TIME_FUNCTION(result = lib_dsp_math_squareroot(x););
#if PRINT_CYCLE_COUNT
    printf("Cycles taken for lib_dsp_math_squareroot function: %d\n", cycles_taken);
#endif
        printf ("Square Root (%.8f) : %.8f\n", F24(x), F24(result));
#ifdef CHECK_SQRT_SQUARED
        int squared_result = lib_dsp_math_multiply(result, result, 27); // sqrt(x) * sqrt(x) = x
        expected = x;
        printf("Result 0x%x, Squared_Result 0x%x, Expected 0x%x\n",result, squared_result, expected);
        check_result(squared_result, expected, 1, &err);
#else
        TIME_FUNCTION(d_sqrt = sqrt(F24(x)));
#if PRINT_CYCLE_COUNT
    printf("Cycles taken for sqrt function: %d\n", cycles_taken);
#endif
        expected =  Q24(d_sqrt);

        check_result(result, expected, 1, &err);
#endif

#endif


    }

    printf("Error report: lib_dsp_math_squareroot vs sqrt:\n");
    report_errors(1, &err);
    printf("\n");

}

void test_multipliation_and_division() {
    int q_format = 24; // location of the decimal point. Gives 8 digits of precision after conversion to floating point.
    q8_24 result, expected;
    error_s err;
    reset_errors(&err);

    printf("Test Multiplication and Division\n");
    printf("--------------------------------\n");
    printf("Note: All calculations are done in Q8.24 format. That gives 7 digits of precion after the decimal point\n");
    printf("Note: Maximum double representation of Q8.24 format: %.8f\n\n", F24(0x7FFFFFFF));

    double f0, f1;
    f0 = 11.3137085;
    f1 = 11.3137085;
    // Multiply the square root of 128 (maximum double representation of Q8.24)
    printf ("Multiplication (%.8f x %.8f): %.8f\n\n",f0, f1, F24(lib_dsp_math_multiply(Q24(f0), Q24(f1), q_format)));

    printf ("Multiplication (11.4 x 11.4). Will overflow!: %.8f\n\n", F24(lib_dsp_math_multiply(Q24(11.4), Q24(11.4), q_format)));;

    printf ("Saturated Multiplication (11.4 x 11.4): %.8f\n\n", F24(lib_dsp_math_multiply_sat(Q24(11.4), Q24(11.4), q_format)));;

    /*
    The result of 0.0005 x 0.0005 is 0.00000025. But this number is not representable as a binary.
    The closest representation in Q8.24 format is (4/2^24) = 0.000000238418579
    printf rounds this to 0.0000002 because the formatting string to printf specifies 8 digits of precision after the decimal point.
    This is the maximum precision that can be achieved with the 24 fractional bits of the Q8.24 format.
    */
    printf ("Multiplication of small numbers (0.0005 x 0.0005): %.8f\n\n", F24(lib_dsp_math_multiply(Q24(0.0005), Q24(0.0005), q_format)));;


    double dividend, divisor;

    dividend = 1.123456; divisor = -128;
    result = lib_dsp_math_divide(Q24(dividend), Q24(divisor), q_format);
    printf ("Signed Division %.8f / %.8f): %.8f\n\n",dividend, divisor, F24(result));
    expected = Q24(dividend/divisor);
    check_result(result, expected, 1, &err);

    dividend = -1.123456; divisor = -128;
    result = lib_dsp_math_divide(Q24(dividend), Q24(divisor), q_format);
    printf ("Signed Division %.8f / %.8f): %.8f\n\n",dividend, divisor, F24(result));
    expected = Q24(dividend/divisor);
    check_result(result, expected, 1, &err);

    dividend = -1.123456; divisor = 127.9999999;
    result = lib_dsp_math_divide(Q24(dividend), Q24(divisor), q_format);
    printf ("Signed Division %.8f / %.8f): %.8f\n\n",dividend, divisor, F24(result));
    expected = Q24(dividend/divisor);
    check_result(result, expected, 1, &err);

    dividend = 1.123456; divisor = 127.9999999;
    result = lib_dsp_math_divide(Q24(dividend), Q24(divisor), q_format);
    printf ("Signed Division %.8f / %.8f): %.8f\n\n",dividend, divisor, F24(result));
    expected = Q24(dividend/divisor);
    check_result(result, expected, 1, &err);

    result = lib_dsp_math_divide_unsigned(Q24(dividend), Q24(divisor), q_format);
    printf ("Division %.8f / %.8f): %.8f\n\n",dividend, divisor, F24(result));
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
        q8_24 sine;
        TIME_FUNCTION(sine = lib_dsp_math_sin(rad));

#if PRINT_INPUTS_AND_OUTPUTS
        printf("sin(%.8f) = %.8f\n",F24(rad), F24(sine));
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
    printf("Error report: lib_dsp_math_sin vs sin:\n");
    report_errors(1, &err);
#endif

#if PRINT_CYCLE_COUNT
    printf("Cycles taken for lib_dsp_math_sin function: %d\n", cycles_taken);
    // just to measure cycles
    tmr :> start_time;
    double sine_float = sin(3.141592653589793/4);
    tmr :> end_time;
    cycles_taken = end_time-start_time-overhead_time;
    printf("math.h sin(%.8f) = %.8f\n",3.141592653589793/4, sine_float);
    printf("Cycles taken for math.h sine function: %d\n", cycles_taken);
#endif
    printf("\n");

    /*
     * Testing lib_dsp_math_sin
     */
    printf("Cosine wave (one cycle from -Pi to +Pi) :\n");
    reset_errors(&err);

    for(q8_24 rad = -PI_Q8_24; rad <= PI_Q8_24; rad += RAD_INCR) {
        q8_24 cosine;
        TIME_FUNCTION(cosine=lib_dsp_math_cos(rad));
#if PRINT_INPUTS_AND_OUTPUTS
        printf("cos(%.8f) = %.8f\n",F24(rad), F24(cosine));
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
    printf("Error report: lib_dsp_math_cos vs cos:\n");
    report_errors(1, &err);
#endif

#if PRINT_CYCLE_COUNT
    printf("Cycles taken for cosine function: %d\n", cycles_taken);
    // just to measure cycles
    tmr :> start_time;
    double cosine_float = cos(3.141592653589793/4);
    tmr :> end_time;
    cycles_taken = end_time-start_time-overhead_time;
    printf("math.h cos(%.8f) = %.8f\n",3.141592653589793/4, cosine_float);
    printf("Cycles taken for math.h cosine function: %d\n", cycles_taken);
#endif
    printf("\n");

    /*
     * Testing lib_dsp_math_atan
     */
    printf("Test lib_dsp_math_atan\n");
    reset_errors(&err);

    int worst_cycles=0;
    int worst_cycles_input;

    /*
    * Test result in terms of Errors:
    * num calculations:  1000226; Errors >=1:    44561 ( 4.46%); Errors >=2:     0 ( 0.00%)
    * Max absolute error: 1
    */

#ifdef EXPONENTIAL_INPUT
    for(int i=-31; i<=31; i++) {
        int x;
        if(i<0) {
            // create negative numbers
            //x = sext((1<<i), i+1); // -2^x (x in 31..1)
            x = -((1<<-i)-1); //-(2^x-1) (x in 31..1)
        } else if(i>0) {
            x = (1<<i)-1; // 2^x-1 (x in 1..31)
        } else {
            x = 0;
        }
#else
    for(unsigned x=0; x <= MAX_Q8_24; x+= X_INCR) {
#endif


        q8_24 arctan;
        TIME_FUNCTION(arctan=lib_dsp_math_atan(x));

        if(cycles_taken>worst_cycles) {
            worst_cycles = cycles_taken;
            worst_cycles_input = x;
        }
#if PRINT_CYCLE_COUNT
        printf("Cycles taken for lib_dsp_math_atan function: %d\n", cycles_taken);
#endif
#if PRINT_INPUTS_AND_OUTPUTS
        printf("atan(%.8f) = %.8f\n",F24(x),F24(arctan));
#endif
#if CHECK_RESULTS
        double d_x = F24(x);
        double d_arctan_ref = atan(d_x);
        q8_24 expected = Q24(d_arctan_ref);
        check_result(arctan, expected, 1, &err);
#endif
    }

#if CHECK_RESULTS
    printf("Error report from lib_dsp_math_atan:\n");
    report_errors(1, &err);
#endif

#if PRINT_CYCLE_COUNT
    printf("max cyles taken for lib_dsp_math_atan function: %d, input value was %.8f\n", worst_cycles, F24(worst_cycles_input));
    // just to measure cycles
    double d_x = F24(worst_cycles_input);
    tmr :> start_time;
    double d_arctan = atan(d_x);
    tmr :> end_time;
    cycles_taken = end_time-start_time-overhead_time;
    printf("math.h atan(%.8f) = %.8f\n",d_x, d_arctan);
    printf("Cycles taken for math.h atan function: %u\n", cycles_taken);
#endif
    printf("\n");
}

int test_math(void)
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

void divide() {
    int divisor = 3;
    int result = 0x7FFFFFFF;;
    while(1) {
        result = lib_dsp_math_divide(result, divisor, 24);
        if(result==0) result = 0x7FFFFFFF;
    }
}

#define DIVIDE_STRESS

int main(void) {
    par {
        test_math();
#ifdef DIVIDE_STRESS
        divide();
        divide();
        divide();
        divide();
#endif
    }
    return 0;
}

