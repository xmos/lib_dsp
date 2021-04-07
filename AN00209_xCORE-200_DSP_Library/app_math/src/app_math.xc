// Copyright 2015-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// XMOS DSP Library - Math Functions Test Program
// Uses Q24 format

#include <stdio.h>
#include <print.h>
#include <xs1.h>
#include <dsp.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#define CHECK_RESULTS 1
#define PRINT_ERROR_TOO_BIG 1
#define EXIT_ON_ERROR_TOO_BIG 0
#define TEST_ALL_INPUTS 0

#define PRINT_CYCLE_COUNT 0

#if PRINT_CYCLE_COUNT
#define DIVIDE_STRESS 1 // execute divide on other cores to get worst case performance.
#else
#define DIVIDE_STRESS 0
#endif

#define STEPS 10

// errors from -15..+15
#define ERROR_RANGE 31
typedef struct {
    int32_t errors[ERROR_RANGE];
    int32_t smaller_than_min_error;
    int32_t greater_than_max_error;
    int32_t max_positive_error;
    int32_t max_negative_error;
    int32_t num_checked;
} error_s;

typedef enum {
  PV_OFF=0,
  PV_ON=1
} print_values_t;

typedef enum {
  PC_OFF=0,
  PC_ON=1
} print_cycles_t;

typedef enum {
  LINEAR=0,
  EXPONENTIAL=1
} stimulus_t;

/*
 * Report Errors within the Error Range. Top and Bottom or range contain saturated values
 */
int32_t report_errors(int32_t min_error, int32_t max_error, error_s *e) {
    int32_t result = 1; // PASS
    int32_t half_range = ERROR_RANGE/2;
    for(int32_t i=0; i<ERROR_RANGE; i++) {
        int32_t error = -half_range+i;
        unsigned num_errors = e->errors[i];

        if(num_errors) {
            if (error > max_error || error < min_error) {
              printf("FAIL:");
            }
            printf("Cases Error == %d: %d, percentage: %5.2f%\n",error, num_errors, 100.0*num_errors/e->num_checked);
        }
    }
    if (e->smaller_than_min_error ) {
        printf("Errors smaller than min_error %d: %d\n",min_error,e->smaller_than_min_error);
        result = 0;
    }
    if (e->greater_than_max_error ) {
        printf("Errors smaller than min_error %d: %d\n",max_error,e->greater_than_max_error);
        result = 0;
    }
    printf("Maximum Positive Error: %d\n",e->max_positive_error);
    printf("Maximum Negative Error: %d\n",e->max_negative_error);
    printf("Number of values checked: %d\n", e->num_checked);

    return result;
}
void reset_errors(error_s *e) {
    for(int32_t i=0; i<ERROR_RANGE; i++) {
        e->errors[i] = 0;
    }
    e->smaller_than_min_error = 0;
    e->greater_than_max_error = 0;
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

int32_t check_result(int32_t result, int32_t expected, int32_t min_error, int32_t max_error, error_s *e) {
    static int32_t half_range = ERROR_RANGE/2;
    uint32_t error_found=0;

    int32_t error = result - expected;

    // Save max positive and negative error
    if (error < e->max_negative_error) e->max_negative_error = error;
    if (error > e->max_positive_error) e->max_positive_error = error;

    if (error > max_error) {
        e->greater_than_max_error++;
        printf("ERROR: error %d is greater than max_error %d\n",error, max_error);
        error_found = 1;
    } else if(error < min_error) {
        e->smaller_than_min_error++;
        printf("ERROR: error %d is smaller than min_error %d\n",error, min_error);
        error_found = 1;
    }

    if (error_found) {
    //if (error > max_error) {
#if PRINT_ERROR_TOO_BIG
        printf("result is 0x%x, Expected is 0x%x\n",result, expected);
        printf("\n");
#if EXIT_ON_ERROR_TOO_BIG
        report_errors(min_error, max_error, e);
        exit (0);
#endif
#endif
    }

    int bin_idx = error+half_range;
    if(bin_idx>=0 && bin_idx<ERROR_RANGE) {
       e->errors[bin_idx]++; // increment the error counter
    }

    e->num_checked++;

    return error_found;
}

int overhead_time;

//Todo: Test if this performs as well as the conversion macros in dsp_qformat.h
inline int32_t qs(double d, const int32_t q_format) {
  return (int32_t)((int64_t)((d) * ((uint64_t)1 << (q_format+20)) + (1<<19)) >> 20);
}

void test_multipliation_and_division() {
    const int32_t q_format = 24; // location of the decimal point. Gives 8 digits of precision after conversion to floating point.
    q8_24 result, expected;
    error_s err;
    reset_errors(&err);

    printf("Test Multiplication and Division\n");
    printf("--------------------------------\n");
    printf("Note: All calculations are done in Q8.24 format. That gives 7 digits of precision after the decimal point\n");
    printf("Note: Maximum double representation of Q8.24 format: %.8f\n\n", F24(0x7FFFFFFF));

    double f0, f1;
    f0 = 11.3137085;
    f1 = 11.3137085;
    // Multiply the square root of 128 (maximum double representation of Q8.24)
    printf ("Multiplication (%.8f x %.8f): %.8f\n\n",f0, f1, F24(dsp_math_multiply(Q24(f0), Q24(f1), q_format)));

    printf ("Multiplication (11.4 x 11.4). Will overflow!: %.8f\n\n", F24(dsp_math_multiply(Q24(11.4), Q24(11.4), q_format)));;

    printf ("Saturated Multiplication (11.4 x 11.4): %.8f\n\n", F24(dsp_math_multiply_sat(Q24(11.4), Q24(11.4), q_format)));;

    /*
    The result of 0.0005 x 0.0005 is 0.00000025. But this number is not representable as a binary.
    The closest representation in Q8.24 format is (4/2^24) = 0.000000238418579
    printf rounds this to 0.0000002 because the formatting string to printf specifies 8 digits of precision after the decimal point.
    This is the maximum precision that can be achieved with the 24 fractional bits of the Q8.24 format.
    */
    printf ("Multiplication of small numbers (0.0005 x 0.0005): %.8f\n\n", F24(dsp_math_multiply(Q24(0.0005), Q24(0.0005), q_format)));;


    double dividend, divisor;

    dividend = 1.123456; divisor = -128;
    result = dsp_math_divide(Q24(dividend), Q24(divisor), q_format);
    printf ("Signed Division %.8f / %.8f): %.8f\n\n",dividend, divisor, F24(result));
    expected = Q24(dividend/divisor);
    check_result(result, expected, -1, 1, &err);

    dividend = -1.123456; divisor = -128;
    result = dsp_math_divide(Q24(dividend), Q24(divisor), q_format);
    printf ("Signed Division %.8f / %.8f): %.8f\n\n",dividend, divisor, F24(result));
    expected = Q24(dividend/divisor);
    check_result(result, expected, -1, 1, &err);

    dividend = -1.123456; divisor = 127.9999999;
    result = dsp_math_divide(Q24(dividend), Q24(divisor), q_format);
    printf ("Signed Division %.8f / %.8f): %.8f\n\n",dividend, divisor, F24(result));
    expected = Q24(dividend/divisor);
    check_result(result, expected, -1, 1, &err);

    dividend = 1.123456; divisor = 127.9999999;
    result = dsp_math_divide(Q24(dividend), Q24(divisor), q_format);
    printf ("Signed Division %.8f / %.8f): %.8f\n\n",dividend, divisor, F24(result));
    expected = Q24(dividend/divisor);
    check_result(result, expected, -1, 1, &err);

    result = dsp_math_divide_unsigned(Q24(dividend), Q24(divisor), q_format);
    printf ("Division %.8f / %.8f): %.8f\n\n",dividend, divisor, F24(result));
    expected = Q24(dividend/divisor);
    check_result(result, expected, -1, 1, &err);

    result = dsp_math_divide(0x02000000, 0x02000001, 24);
    check_result(result, 0x01000000, -1, 1, &err);

    printf("Error report from test_multipliation_and_division:\n");
    report_errors(-1, 1, &err);
    printf("\n");

}


q8_24 execute(int func, q8_24 x, unsigned &cycles_taken) {
    int32_t start_time, end_time;
    timer tmr;
    // uint32_t cycles_taken; // absolute positive values
    q8_24 result;
    // even func index executes the fixed point functions.
    switch(func) {
    case  0: {TIME_FUNCTION(result = dsp_math_exp(x)); return result;}
    case  1: {TIME_FUNCTION(result = Q24(exp(F24(x)))); return result;}
    case  2: {TIME_FUNCTION(result = dsp_math_log(x)); return result;}
    case  3: {TIME_FUNCTION(result = Q24(log(F24(x)))); return result;}
    case  4: {TIME_FUNCTION(result = dsp_math_sqrt(x)); return result;}
    case  5: {TIME_FUNCTION(result = Q24(sqrt(F24(x)))); return result;}
    case  6: {TIME_FUNCTION(result = dsp_math_sin(x)); return result;}
    case  7: {TIME_FUNCTION(result = Q24(sin(F24(x)))); return result;}
    case  8: {TIME_FUNCTION(result = dsp_math_cos(x)); return result;}
    case  9: {TIME_FUNCTION(result = Q24(cos(F24(x)))); return result;}
    case  10: {TIME_FUNCTION(result = dsp_math_atan(x)); return result;}
    case  11: {TIME_FUNCTION(result = Q24(atan(F24(x)))); return result;}
    case  12: {TIME_FUNCTION(result = dsp_math_sinh(x)); return result;}
    case  13: {TIME_FUNCTION(result = Q24(sinh(F24(x)))); return result;}
    case  14: {TIME_FUNCTION(result = dsp_math_cosh(x)); return result;}
    case  15: {TIME_FUNCTION(result = Q24(cosh(F24(x)))); return result;}

    }
    return INT32_MIN;
}

int test_input_range(int func, char name[], int min, int max, stimulus_t exponential, int minerror, int maxerror, 
                     int permille, print_values_t print_values) {
    int fail = 0;
    int done=0;
    int done_after_next_iteration=0;
    int i=0;
    unsigned cycles_float, cycles_fixed;
    unsigned worst_cycles_fixed=0, worst_cycles_float=0;

    int32_t worst_cycles_input;

    error_s err; 
    reset_errors(&err);

    int64_t x = (int64_t) min;  // use double precision to handle overflow

    printf("- Testing %s\n",name);

    while(!done) {
        timer t;
        int z, zc;

        zc = execute(func|1,x, cycles_float);
        if (zc == INT32_MIN) {
            printf("%s returned INT32_MIN\n",name);
        }
        z = execute(func,x, cycles_fixed);
        if(print_values) {
          printf("%s(%.7f) == %.7f\n",name,F24(x), F24(z));
        }

        check_result(z, zc, minerror, maxerror, &err);

        // Performance
#if PRINT_EVERY_CYCLE_COUNT 
        perf_ratio = dsp_math_divide(cycles_float, cycles_fixed, 24);
        printf("Cycles taken to execute %s: %d\n", name, cycles_fixed);
        printf("%s is %.2f times faster than it's floating point equivalent\n", name, F24(perf_ratio));
#endif 

        if(cycles_fixed>worst_cycles_fixed) {
            worst_cycles_fixed = cycles_fixed;
            worst_cycles_input = x;
            worst_cycles_float = cycles_float;
        }

        i++;
        // update x
        if(exponential) {
            if(x==-1) {
                x = 0;   // transition to 0
            } else if(x==0) {
                x = 1;   // transition to positive
            } else if(x<0) {
                x >>= 1; //shift down towards -1
            } else { // positive values
                x <<= 1; // shift up towards max
            }
        } else {
            x = (int64_t) min + (((unsigned)(max - min))>>STEPS) * i;
        }
    
        // handle done
        if(done_after_next_iteration) done = 1;
        if(x >= (int64_t) max) {
            x = max;
            done_after_next_iteration = 1;
        }
    }

    report_errors(minerror, maxerror, &err);
    unsigned half_range = ERROR_RANGE/2;
    // number of errors -1, 0, +1
    unsigned small_errors = err.errors[half_range-1] + err.errors[half_range] + err.errors[half_range+1];
    // 
    unsigned achieved_permille = (1000 * small_errors) / err.num_checked;
    if(achieved_permille < permille) {
        printf("FAIL: only ");
    } 
    printf("%d per thousand in one bit error\n", achieved_permille);
    
#if PRINT_CYCLE_COUNT
        printf("Worst case cycles for executing %s was measured for input %.7f: %d\n", name, F24(worst_cycles_input), worst_cycles_fixed);
        perf_ratio = dsp_math_divide(worst_cycles_float, worst_cycles_fixed, 24);
        printf("%s is %.2f times faster than it's floating point equivalent\n", name, F24(perf_ratio));
#endif

    printstr("\n"); // Delimiter

    return fail;
}

/*
 * Test the valid input rn=anges
 */
void test_single_input_functions() {

    int fail=0;

    printf("Test Exponential and Logarithmic Functions\n");
    printf("------------------------------------------\n");
    fail += test_input_range(0,"dsp_math_exp", INT32_MIN, Q24(log(127)), LINEAR, -26,  5, 987, PV_OFF);
    fail += test_input_range(2,"dsp_math_log", 1,         INT32_MAX,     LINEAR, -2,  2, 926, PV_OFF);

    printf("Test Squareroot\n");
    printf("---------------\n");
    fail += test_input_range(4,"dsp_math_sqrt", 0,  INT32_MAX,     EXPONENTIAL, -1,  1, 1000, PV_OFF);

    printf("Test Trigonometric Functions\n");
    printf("----------------------------\n");
    fail += test_input_range(6,"dsp_math_sin", -PI_Q8_24, PI_Q8_24,      LINEAR, -1,  1, 1000, PV_OFF);
    fail += test_input_range(8,"dsp_math_cos", -PI_Q8_24, PI_Q8_24,      LINEAR, -1,  1, 1000, PV_OFF);
    fail += test_input_range(10,"dsp_math_atan", INT32_MIN,       INT32_MAX,     1, -1,  EXPONENTIAL, 984, PV_OFF);
    fail += test_input_range(12,"dsp_math_sinh", -11*ONE_Q8_24>>1, 11*ONE_Q8_24>>1, LINEAR, -40, 40, 726, PV_OFF);  // Should aim for -4 4 800
    fail += test_input_range(14,"dsp_math_cosh", -11*ONE_Q8_24>>1, 11*ONE_Q8_24>>1, LINEAR, -40, 40, 711, PV_OFF);  // Should aim for -4 4, 800

    if (fail != 0) {
        printf("Total failures %d\n", fail);
    }
}

static int atan2_fails = 0, atan2_tests = 0;

static void single_atan2_test(int y, int x, unsigned hypot, int angle, int precision, int angle_precision) {
    int hypot_precision = 24 - precision;
    int z[2];
    z[0] = x;
    z[1] = y;
    dsp_math_atan2_hypot(z, precision);
    unsigned hypotdiff = abs(((unsigned)z[0]) - hypot);
    unsigned anglediff = abs(((unsigned)z[1]) - angle);
    unsigned hypotlimit = (hypot >> (hypot_precision)) + ((hypot >> (hypot_precision-1))&1) - 1;
    unsigned anglelimit = (0x60000000 >> angle_precision);
    
    if (hypotdiff > hypotlimit) {
        printf("%08x %08x; Hypot %08x expected %08x\n", y, x, z[0], hypot);
        printf("atan2_hypot: Hypot diff %u limit is %d\n", hypotdiff,hypotlimit);
        atan2_fails++;
    }
    if (anglediff > anglelimit) {
        printf("%08x %08x Angle %08x expected %08x\n", y, x, z[1], angle);
        
        printf("atan2_angle: Angle diff %u limit is %d\n", anglediff, anglelimit);
        atan2_fails++;
    }
    atan2_tests++;
}


static const unsigned random_poly = 0xEDB88320; 
static unsigned random_val = 0x12345678; //seed
int get_random_number(void)
{
  crc32(random_val, -1, random_poly);
  return (int)random_val;
}

#define SQRT2(X) ((unsigned)(1.414213562*(X)))
void atan2_test(void) {
    single_atan2_test(1000,1000, SQRT2(1000), 0x10000000, 0, 25);
    single_atan2_test(0xFFFFFF,0xFFFFFF, SQRT2(0xFFFFFF), 0x10000000, 0, 25);
    single_atan2_test(0xFFFFFFF,0xFFFFFFF, SQRT2(0xFFFFFFF), 0x10000000, 0, 25);
    single_atan2_test(0x3FFFFFFF,0x3FFFFFFF, SQRT2(0x3FFFFFFF), 0x10000000, 0, 25);
    single_atan2_test(0x7FFFFFFF,0x7FFFFFFF, SQRT2(0x7FFFFFFF), 0x10000000, 0, 25);
    single_atan2_test(0x7FFFFFFF,-0x7FFFFFFF, SQRT2(0x7FFFFFFF), 0x30000000, 0, 25);
    single_atan2_test(-0x7FFFFFFF,0x7FFFFFFF, SQRT2(0x7FFFFFFF), -0x10000000, 0, 25);
    single_atan2_test(-0x7FFFFFFF,-0x7FFFFFFF, SQRT2(0x7FFFFFFF), -0x30000000, 0, 25);
    for(int i = 0; i < 23; i++) {
        single_atan2_test(1200000000, 1600000000, 2000000000, 0x0d1bfaf9, i, 24-i);
    }
    
    for(int i = 0; i < PI2_Q8_24; i += PI2_Q8_24/391) {
        int angle = (((long long) i) << 31) / PI2_Q8_24;
        if (angle >= (1 << 30)) {
            angle -= (1 << 31);
        }
        int s = dsp_math_sin(i);
        int c = dsp_math_cos(i);
        single_atan2_test(s*127, c*127, 127<<24, angle, 0, 24);
        single_atan2_test(s/13, c/13, (1<<24)/13, angle, 0, 22);
    }
    
    for(int i = 0; i < 100; i += 1){
        int x = get_random_number();    //Returns random number from INT_MIN to INT_MAX
        int y = get_random_number();
        if (x < -0x7FFFFFFF) x = -0x7FFFFFFF;   //Clip as arctan2 cannot take INT_MIN
        if (y < -0x7FFFFFFF) y = -0x7FFFFFFF;

        int hypot = (int)( sqrt( (double)x * (double)x + (double)y * (double)y ) );
        int angle = (int)( (atan2((double)y, (double)x)) * (0x40000000 / 3.1415926536f) );
        single_atan2_test(y, x, hypot, angle, 0, 24); //We cannot quite reach 25b on random points so set limit to 24b
    } 
    printf("Atan2: %d out of %d passes\n", atan2_tests-atan2_fails, atan2_tests);
}

void logistics_test() {
    int errors = 0;
    int one = 1 << 24;
    float fone = one;
    int max_good = 0;
    int max_fast = 0;
    for(int i = 8*one; i >= -8*one; i-=one/8) {
        int log1 = one / (exp(-i/fone) +1.0);
        int log2 = dsp_math_logistics_fast(i);
        int log3 = dsp_math_logistics(i);
        int err = abs(log2 - log1);
        if (err > max_fast) {
            max_fast = err;
        }
        if (abs(log2 - log1) > one * 7 / 100) {
            errors++;
            printf("logistics_fast() error; %f: %f %f (err = %f)\n", i/fone, log1/fone, log2/fone, err/fone);
        }
        err = abs(log3 - log1);
        if (err > max_good) {
            max_good = err;
        }
        if (err > 1) {
            errors++;
            printf("logistics() error: %f: %f %f (err = %d)\n", i/fone, log1/fone, log3/fone, err);
        }
    }
    printf("Logistics max error: %f (fast) %d (good)\n", max_fast/fone, max_good);
    if (errors == 0) {
        printf("Logistics test passed\n");
    } else {
        printf("Logistics test failed with %d errors\n", errors);
    }
}

void softplus_test() {
    int errors = 0;
    int one = 1 << 24;
    float fone = one;
    int max_good = 0;
    int max_fast = 0;
    for(int i = 4*one; i >= -4*one; i-=one/8) {
        int log1 = log(exp(i/fone) +1.0)*fone;
//        int log2 = dsp_math_softplus_fast(i);
        int log3 = dsp_math_softplus(i);
//        int err = abs(log2 - log1);
//        if (err > max_fast) {
//            max_fast = err;
//        }
//        if (abs(log2 - log1) > one * 7 / 100) {
//            errors++;
//            printf("softplus_fast() error; %f: %f %f (err = %f)\n", i/fone, log1/fone, log2/fone, err/fone);
//        }
        int err = abs(log3 - log1);
        if (err > max_good) {
            max_good = err;
        }
        if (err > 1) {
            errors++;
            printf("softplus() error: %f: %f %f (err = %d)\n", i/fone, log1/fone, log3/fone, err);
        }
    }
    printf("Softplus max error: %f (fast) %d (good)\n", max_fast/fone, max_good);
    if (errors == 0) {
        printf("Softplus test passed\n");
    } else {
        printf("Softplus test failed with %d errors\n", errors);
    }
}

void test_math(void)
{

    int32_t start_time, end_time;
    timer tmr;
    tmr :> start_time;
    tmr :> end_time;
    overhead_time = end_time - start_time;

    printf("Test example for Math functions\n");
    printf("===============================\n");

    softplus_test();
    logistics_test();

    test_multipliation_and_division();

    test_single_input_functions();

    atan2_test();
    
    exit (0);
}

void divide() {
    int32_t divisor = 3;
    int32_t result = 0x7FFFFFFF;;
    while(1) {
        result = dsp_math_divide(result, divisor, 24);
        if(result==0) result = 0x7FFFFFFF;
    }
}

void test_isqrt() {
    int errors = 0;
    for(int i = 0; i < 33; i++) {
        for(int k = 0; k < 20; k++) {
            unsigned r = get_random_number();
            unsigned mask = ((1 << i)-1);
            r &= mask;
            if (k & 1) {
                r |= ~mask;
            }
            unsigned rr = dsp_math_int_sqrt(r);
            if (r == 0 && rr == 0) continue;
            unsigned rrm = (rr - 1) * (rr - 1);
            unsigned rrp = rr >= 0xffff ? 0xffffffff : (rr + 1) * (rr + 1);
            if (r <= rrm || r > rrp) {
                errors++;
                printf("Error: sqrt(%08x) not %d\n", r, rr);
            }
        }
    }
    for(int i = 0; i < 65; i++) {
        for(int k = 0; k < 20; k++) {
            unsigned long long r = (unsigned) get_random_number() ;
            r = (r << 32) | (unsigned) get_random_number() ;
            unsigned long long mask = ((1ull << i)-1);
            r &= mask;
            if (k & 1) {
                r |= ~mask;
            }
            unsigned rr = dsp_math_int_sqrt64(r);
            if (r == 0 && rr == 0) continue;
            unsigned long long rrm = (rr - 1ull) * (rr-1ull);
            unsigned long long rrp = rr == 0xffffffff ? 0xffffffffffffffffull : (rr + 1ull)*(rr + 1ull);
            if (r <= rrm || r > rrp) {
                errors++;
                printf("Error: sqrt64(%08llx) not %u\n", r, rr);
            }
        }
    }
    if (errors == 0) {
        printf("isqrt() passed\n");
    } else {
        printf("isqrt() FAIL with %d errors\n", errors);
    }
}

int main(void) {
    test_isqrt();
    par {
        test_math();
#if DIVIDE_STRESS
        divide();
        divide();
        divide();
        divide();
#endif
    }
    return 0;
}

