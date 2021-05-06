// Copyright 2017-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include "unity.h"

#include <xs1.h>
#include <string.h>
#include <math.h>

#include <xclib.h>
#include <stdio.h>
#include <stdlib.h>

#include "dsp.h"
#include "dsp_testing.h"
#include "dsp_fp.h"

#define MAX_LEN 64
#define ITT 1024

//TODO should we support saturation?



//these can be anything
#define Q_EXP_32 (-30)
#define Q_EXP_16 (-14)
#define Q_EXP_8 (-6)

void test_add_bfp_vect_int32(){

    int32_t a[MAX_LEN], b[MAX_LEN], c[MAX_LEN];
    dsp_float_t A[MAX_LEN], B[MAX_LEN], C[MAX_LEN];

    unsigned rand_seed = 5;

    for(unsigned len=1;len<MAX_LEN;len++){
        for(unsigned itt=0;itt<ITT;itt++){


            int b_exp = -31 + (3 + (dsp_pseudo_rand_int32(&rand_seed)%6));
            int c_exp = -31 + (3 + (dsp_pseudo_rand_int32(&rand_seed)%6));

            unsigned b_hr = dsp_pseudo_rand_uint32(&rand_seed)%6;
            unsigned c_hr = dsp_pseudo_rand_uint32(&rand_seed)%6;

            for(unsigned i=0;i<len;i++){
                b[i] = dsp_pseudo_rand_int32(&rand_seed)>>b_hr;
                c[i] = dsp_pseudo_rand_int32(&rand_seed)>>c_hr;
            }

            b_hr = dsp_bfp_cls_vect_int32(b, len);
            c_hr = dsp_bfp_cls_vect_int32(c, len);

            int error = 0;
            dsp_conv_vect_int32_to_float(b, b_exp, B, len, &error);
            dsp_conv_vect_int32_to_float(c, c_exp, C, len, &error);
            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            int a_exp;
            unsigned a_hr;
            dsp_add_bfp_vect_int32(
                a, &a_exp, &a_hr, 
                b, b_exp, b_hr, 
                c, c_exp, c_hr, 
                len);
            dsp_add_vect_float(A, B, C, len);

            unsigned diff = dsp_abs_diff_vect_int32(a, a_exp, A, len, &error);
            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            unsigned actual_a_hr = dsp_bfp_cls_vect_int32(a, len);

            TEST_ASSERT_LESS_THAN_UINT32_MESSAGE(3, diff, "error is too high");
            TEST_ASSERT_EQUAL_UINT32_MESSAGE(actual_a_hr, a_hr, "error hr is wrong");
        }
    }

    return;
}


void test_add_bfp_vect_int16(){

    int16_t a[MAX_LEN], b[MAX_LEN], c[MAX_LEN];
    dsp_float_t A[MAX_LEN], B[MAX_LEN], C[MAX_LEN];

    unsigned rand_seed = 5;

    for(unsigned len=1;len<MAX_LEN;len++){
        for(unsigned itt=0;itt<ITT;itt++){
            int b_exp = -31 + (3 + (dsp_pseudo_rand_int32(&rand_seed)%6));
            int c_exp = -31 + (3 + (dsp_pseudo_rand_int32(&rand_seed)%6));

            unsigned b_hr = dsp_pseudo_rand_uint32(&rand_seed)%6;
            unsigned c_hr = dsp_pseudo_rand_uint32(&rand_seed)%6;
            for(unsigned i=0;i<len;i++){
                b[i] = dsp_pseudo_rand_int16(&rand_seed)>>b_hr;
                c[i] = dsp_pseudo_rand_int16(&rand_seed)>>c_hr;
            }

            b_hr = dsp_bfp_cls_vect_int16(b, len);
            c_hr = dsp_bfp_cls_vect_int16(c, len);

            int error = 0;
            dsp_conv_vect_int16_to_float(b, b_exp, B, len, &error);
            dsp_conv_vect_int16_to_float(c, c_exp, C, len, &error);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            int a_exp;
            unsigned a_hr;
            dsp_add_bfp_vect_int16(
                a, &a_exp, &a_hr, 
                b, b_exp, b_hr, 
                c, c_exp, c_hr, 
                len);
            dsp_add_vect_float(A, B, C, len);

            unsigned diff = dsp_abs_diff_vect_int16(a, a_exp, A, len, &error);

            unsigned actual_a_hr = dsp_bfp_cls_vect_int16(a, len);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
            TEST_ASSERT_LESS_THAN_UINT32_MESSAGE(3, diff, "error is too high");
            TEST_ASSERT_EQUAL_UINT32_MESSAGE(actual_a_hr, a_hr, "error hr is wrong");
        }
    }
    return;
}

void test_add_bfp_vect_int8(){

    int8_t a[MAX_LEN], b[MAX_LEN], c[MAX_LEN];
    dsp_float_t A[MAX_LEN], B[MAX_LEN], C[MAX_LEN];

    unsigned rand_seed = 5;

    for(unsigned len=1;len<MAX_LEN;len++){

        for(unsigned itt=0;itt<ITT;itt++){
            int b_exp = -31 + (3 + (dsp_pseudo_rand_int32(&rand_seed)%6));
            int c_exp = -31 + (3 + (dsp_pseudo_rand_int32(&rand_seed)%6));

            unsigned b_hr = dsp_pseudo_rand_uint32(&rand_seed)%6;
            unsigned c_hr = dsp_pseudo_rand_uint32(&rand_seed)%6;
            for(unsigned i=0;i<len;i++){
                b[i] = dsp_pseudo_rand_int8(&rand_seed)>>b_hr;
                c[i] = dsp_pseudo_rand_int8(&rand_seed)>>c_hr;
            }

            b_hr = dsp_bfp_cls_vect_int8(b, len);
            c_hr = dsp_bfp_cls_vect_int8(c, len);

            int error = 0;

            dsp_conv_vect_int8_to_float(b, b_exp, B, len, &error);
            dsp_conv_vect_int8_to_float(c, c_exp, C, len, &error);

            if(error)
                continue;

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            int a_exp;
            unsigned a_hr;
            dsp_add_bfp_vect_int8(
                a, &a_exp, &a_hr, 
                b, b_exp, b_hr, 
                c, c_exp, c_hr, 
                len);
            dsp_add_vect_float(A, B, C, len);

            unsigned diff = UINT_MAX;
            diff = dsp_abs_diff_vect_int8(a, a_exp, A, len, &error);
            
            unsigned actual_a_hr = dsp_bfp_cls_vect_int8(a, len);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error 2");
            TEST_ASSERT_LESS_THAN_UINT32_MESSAGE(3, diff, "error is too high");
            TEST_ASSERT_EQUAL_UINT32_MESSAGE(actual_a_hr, a_hr, "error hr is wrong");
        }
    }
    return;
}


void test_add_vect_int32(){

    int32_t a[MAX_LEN], b[MAX_LEN], c[MAX_LEN];
    dsp_float_t A[MAX_LEN], B[MAX_LEN], C[MAX_LEN];

    unsigned rand_seed = 5;

    for(unsigned len=8;len<MAX_LEN;len++){
        for(unsigned itt=0;itt<ITT;itt++){

            for(unsigned i=0;i<len;i++){
                b[i] = dsp_pseudo_rand_int32(&rand_seed)>>2;
                c[i] = dsp_pseudo_rand_int32(&rand_seed)>>2;
            }

            int error = 0;
            dsp_conv_vect_int32_to_float(b, Q_EXP_32, B, len, &error);
            dsp_conv_vect_int32_to_float(c, Q_EXP_32, C, len, &error);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            dsp_add_vect_int32(a, b, c, len);
            dsp_add_vect_float(A, B, C, len);

            unsigned diff = dsp_abs_diff_vect_int32(a, Q_EXP_32, A, len, &error);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
            TEST_ASSERT_LESS_THAN_UINT32_MESSAGE(3, diff, "error is too high");
        }
    }

    return;
}


void test_add_vect_int16(){

    int16_t a[MAX_LEN], b[MAX_LEN], c[MAX_LEN];
    dsp_float_t A[MAX_LEN], B[MAX_LEN], C[MAX_LEN];
    for(unsigned i=0;i<MAX_LEN;i++){
        b[i] = 0;
        c[i] = 0;
    }

    unsigned rand_seed = 5;

    for(unsigned len=1;len<MAX_LEN;len++){
        for(unsigned itt=0;itt<ITT;itt++){

            for(unsigned i=0;i<len;i++){
                b[i] = dsp_pseudo_rand_int16(&rand_seed)>>2;
                c[i] = dsp_pseudo_rand_int16(&rand_seed)>>2;
            }

            int error = 0;
            dsp_conv_vect_int16_to_float(b, Q_EXP_16, B, len, &error);
            dsp_conv_vect_int16_to_float(c, Q_EXP_16, C, len, &error);
            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            dsp_add_vect_int16(a, b, c, len);
            dsp_add_vect_float(A, B, C, len);

            unsigned diff = dsp_abs_diff_vect_int16(a, Q_EXP_16, A, len, &error);
            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
            TEST_ASSERT_LESS_THAN_UINT32_MESSAGE(3, diff, "error is too high");
        }
    }
    return;
}

void test_add_vect_int8(){

    int8_t a[MAX_LEN], b[MAX_LEN], c[MAX_LEN];
    dsp_float_t A[MAX_LEN], B[MAX_LEN], C[MAX_LEN];

    unsigned rand_seed = 6;

    for(unsigned len=1;len<MAX_LEN;len++){
        for(unsigned itt=0;itt<ITT;itt++){

            for(unsigned i=0;i<len;i++){
                b[i] = dsp_pseudo_rand_int8(&rand_seed)>>2;
                c[i] = dsp_pseudo_rand_int8(&rand_seed)>>2;
            }

            int error = 0;
            dsp_conv_vect_int8_to_float(b, Q_EXP_8, B, len, &error);
            dsp_conv_vect_int8_to_float(c, Q_EXP_8, C, len, &error);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            dsp_add_vect_int8(a, b, c, len);
            dsp_add_vect_float(A, B, C, len);

            unsigned diff = dsp_abs_diff_vect_int8(a, Q_EXP_8, A, len, &error);
            if(error){
                printf("%x %u %u/n", error, len, diff);
                for(unsigned i=0;i<len;i++){
                    printf("%d %d %d\n", a[i], b[i], c[i]);
                }
                
            }
            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            TEST_ASSERT_LESS_THAN_UINT32_MESSAGE(3, diff, "error is too high");
        }
    }
    return;
}



void test_mul_bfp_vect_complex32(){

    dsp_complex_int32_t a[MAX_LEN], b[MAX_LEN], c[MAX_LEN];
    dsp_complex_float_t A[MAX_LEN], B[MAX_LEN], C[MAX_LEN];

    unsigned rand_seed = 5;

    for(unsigned len=1;len<MAX_LEN;len++){
        for(unsigned itt=0;itt<ITT;itt++){

            int b_exp = -31 + (3 + (dsp_pseudo_rand_int32(&rand_seed)%6));
            int c_exp = -31 + (3 + (dsp_pseudo_rand_int32(&rand_seed)%6));

            unsigned b_hr = dsp_pseudo_rand_uint32(&rand_seed)%6;
            unsigned c_hr = dsp_pseudo_rand_uint32(&rand_seed)%6;

            for(unsigned i=0;i<len;i++){
                b[i].re = dsp_pseudo_rand_int32(&rand_seed)>>b_hr;
                b[i].im = dsp_pseudo_rand_int32(&rand_seed)>>b_hr;
                c[i].re = dsp_pseudo_rand_int32(&rand_seed)>>c_hr;
                c[i].im = dsp_pseudo_rand_int32(&rand_seed)>>c_hr;
            }

            int error = 0;
            dsp_conv_vect_complex_int32_to_complex_float(b, b_exp, B, len, &error);
            dsp_conv_vect_complex_int32_to_complex_float(c, c_exp, C, len, &error);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            int a_exp;
            unsigned a_hr;
            b_hr = dsp_bfp_cls_vect_complex_int32(b, len);
            c_hr = dsp_bfp_cls_vect_complex_int32(c, len);

            dsp_mul_bfp_vect_complex_int32(
                a, &a_exp, &a_hr, 
                b, b_exp, b_hr, 
                c, c_exp, c_hr, 
                len);
            dsp_mul_vect_complex_float(A, B, C, len);

            unsigned diff = dsp_abs_diff_vect_complex_int32(a, a_exp, A, len, &error);

            unsigned actual_a_hr = dsp_bfp_cls_vect_complex_int32(a, len);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
            TEST_ASSERT_LESS_THAN_UINT32_MESSAGE(4, diff, "error is too high");
            TEST_ASSERT_EQUAL_UINT32_MESSAGE(a_hr, actual_a_hr, "error hr is wrong");
        }
    }
    return;
}


void test_bfp_vect_mul32(){

    int32_t a[MAX_LEN], b[MAX_LEN], c[MAX_LEN];
    dsp_float_t A[MAX_LEN], B[MAX_LEN], C[MAX_LEN];

    unsigned rand_seed = 6;

    for(unsigned len=1;len<MAX_LEN;len++){
        for(unsigned itt=0;itt<ITT;itt++){


            int b_exp = -31 + (3 + (dsp_pseudo_rand_int32(&rand_seed)%6));
            int c_exp = -31 + (3 + (dsp_pseudo_rand_int32(&rand_seed)%6));

            unsigned b_hr = dsp_pseudo_rand_uint32(&rand_seed)%6;
            unsigned c_hr = dsp_pseudo_rand_uint32(&rand_seed)%6;
            for(unsigned i=0;i<len;i++){
                b[i] = dsp_pseudo_rand_int32(&rand_seed)>>b_hr;
                c[i] = dsp_pseudo_rand_int32(&rand_seed)>>c_hr;
            }

            b_hr = dsp_bfp_cls_vect_int32(b, len);
            c_hr = dsp_bfp_cls_vect_int32(c, len);

            int error = 0;
            dsp_conv_vect_int32_to_float(b, b_exp, B, len, &error);
            dsp_conv_vect_int32_to_float(c, c_exp, C, len, &error);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            int a_exp;
            unsigned a_hr;
            dsp_mul_bfp_vect_int32(
                a, &a_exp, &a_hr, 
                b, b_exp, b_hr, 
                c, c_exp, c_hr, 
                len);
            dsp_mul_vect_float(A, B, C, len);


            unsigned diff = dsp_abs_diff_vect_int32(a, a_exp, A, len, &error);

            unsigned actual_a_hr = dsp_bfp_cls_vect_int32(a, len);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
            TEST_ASSERT_LESS_THAN_UINT32_MESSAGE(3, diff, "error is too high");
            TEST_ASSERT_EQUAL_UINT32_MESSAGE(a_hr, actual_a_hr, "error hr is wrong");
        }
    }

    return;
}


void test_bfp_vect_mul16(){

    int16_t a[MAX_LEN], b[MAX_LEN], c[MAX_LEN];
    dsp_float_t A[MAX_LEN], B[MAX_LEN], C[MAX_LEN];

    unsigned rand_seed = 5;

    for(unsigned len=1;len<MAX_LEN;len++){
        for(unsigned itt=0;itt<ITT;itt++){

            int b_exp = -31 + (3 + (dsp_pseudo_rand_int32(&rand_seed)%6));
            int c_exp = -31 + (3 + (dsp_pseudo_rand_int32(&rand_seed)%6));

            unsigned b_hr = dsp_pseudo_rand_uint32(&rand_seed)%6;
            unsigned c_hr = dsp_pseudo_rand_uint32(&rand_seed)%6;
            for(unsigned i=0;i<len;i++){
                b[i] = dsp_pseudo_rand_int16(&rand_seed)>>b_hr;
                c[i] = dsp_pseudo_rand_int16(&rand_seed)>>c_hr;
            }

            b_hr = dsp_bfp_cls_vect_int16(b, len);
            c_hr = dsp_bfp_cls_vect_int16(c, len);

            int error = 0;
            dsp_conv_vect_int16_to_float(b, b_exp, B, len, &error);
            dsp_conv_vect_int16_to_float(c, c_exp, C, len, &error);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            int a_exp;
            unsigned a_hr;
            dsp_mul_bfp_vect_int16(
                a, &a_exp, &a_hr, 
                b, b_exp, b_hr, 
                c, c_exp, c_hr, 
                len);
            dsp_mul_vect_float(A, B, C, len);

            unsigned diff = dsp_abs_diff_vect_int16(a, a_exp, A, len, &error);

            unsigned actual_a_hr = dsp_bfp_cls_vect_int16(a, len);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
            TEST_ASSERT_LESS_THAN_UINT32_MESSAGE(3, diff, "error is too high");
            TEST_ASSERT_EQUAL_UINT32_MESSAGE(a_hr, actual_a_hr, "error hr is wrong");
        }
    }
    return;
}

void test_bfp_vect_mul8(){

    int8_t a[MAX_LEN], b[MAX_LEN], c[MAX_LEN];
    dsp_float_t A[MAX_LEN], B[MAX_LEN], C[MAX_LEN];

    unsigned rand_seed = 5;

    for(unsigned len=1;len<MAX_LEN;len++){
        for(unsigned itt=0;itt<ITT;itt++){

            int b_exp = -31 + (3 + (dsp_pseudo_rand_int32(&rand_seed)%6));
            int c_exp = -31 + (3 + (dsp_pseudo_rand_int32(&rand_seed)%6));

            unsigned b_hr = dsp_pseudo_rand_uint32(&rand_seed)%6;
            unsigned c_hr = dsp_pseudo_rand_uint32(&rand_seed)%6;
            for(unsigned i=0;i<len;i++){
                b[i] = dsp_pseudo_rand_int16(&rand_seed)>>b_hr;
                c[i] = dsp_pseudo_rand_int16(&rand_seed)>>c_hr;
            }

            b_hr = dsp_bfp_cls_vect_int8(b, len);
            c_hr = dsp_bfp_cls_vect_int8(c, len);

            int error = 0;
            dsp_conv_vect_int8_to_float(b, b_exp, B, len, &error);
            dsp_conv_vect_int8_to_float(c, c_exp, C, len, &error);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            int a_exp;
            unsigned a_hr;
            dsp_mul_bfp_vect_int8(
                a, &a_exp, &a_hr, 
                b, b_exp, b_hr, 
                c, c_exp, c_hr, 
                len);
            dsp_mul_vect_float(A, B, C, len);

            unsigned diff = UINT_MAX;
            diff = dsp_abs_diff_vect_int8(a, a_exp, A, len, &error);

            unsigned actual_a_hr = dsp_bfp_cls_vect_int8(a, len);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
            TEST_ASSERT_LESS_THAN_UINT32_MESSAGE(3, diff, "error is too high");
            TEST_ASSERT_EQUAL_UINT32_MESSAGE(a_hr, actual_a_hr, "error hr is wrong");
        }
    }
    return;
}

void test_bfp_vect_muls32(){

    int32_t a[MAX_LEN], b[MAX_LEN], c;
    dsp_float_t A[MAX_LEN], B[MAX_LEN], C;

    unsigned rand_seed = 6;

    for(unsigned len=1;len<MAX_LEN;len++){
        for(unsigned itt=0;itt<ITT;itt++){


            int b_exp = -31 + (3 + (dsp_pseudo_rand_int32(&rand_seed)%6));
            int c_exp = -31 + (3 + (dsp_pseudo_rand_int32(&rand_seed)%6));

            unsigned b_hr = dsp_pseudo_rand_uint32(&rand_seed)%6;
            unsigned c_hr = dsp_pseudo_rand_uint32(&rand_seed)%6;
            for(unsigned i=0;i<len;i++){
                b[i] = dsp_pseudo_rand_int32(&rand_seed)>>b_hr;
            }
            c = dsp_pseudo_rand_int32(&rand_seed)>>c_hr;

            b_hr = dsp_bfp_cls_vect_int32(b, len);
            c_hr = dsp_bfp_cls_int32(c);

            int error = 0;
            dsp_conv_vect_int32_to_float(b, b_exp, B, len, &error);
            C = dsp_conv_int32_to_float(c, c_exp, &error);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            int a_exp;
            unsigned a_hr;
            dsp_muls_bfp_vect_int32(
                a, &a_exp, &a_hr, 
                b, b_exp, b_hr, 
                c, c_exp, c_hr, 
                len);

            for (int i=0; i<len; i++) {
                A[i] = B[i] * C;
            }

            unsigned diff = dsp_abs_diff_vect_int32(a, a_exp, A, len, &error);

            unsigned actual_a_hr = dsp_bfp_cls_vect_int32(a, len);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
            TEST_ASSERT_LESS_THAN_UINT32_MESSAGE(3, diff, "error is too high");
            TEST_ASSERT_EQUAL_UINT32_MESSAGE(a_hr, actual_a_hr, "error hr is wrong");
        }
    }

    return;
}

void test_bfp_vect_muls16(){

    int16_t a[MAX_LEN], b[MAX_LEN], c;
    dsp_float_t A[MAX_LEN], B[MAX_LEN], C;

    unsigned rand_seed = 5;

    for(unsigned len=1;len<MAX_LEN;len++){
        for(unsigned itt=0;itt<ITT;itt++){

            int b_exp = -31 + (3 + (dsp_pseudo_rand_int32(&rand_seed)%6));
            int c_exp = -31 + (3 + (dsp_pseudo_rand_int32(&rand_seed)%6));

            unsigned b_hr = dsp_pseudo_rand_uint32(&rand_seed)%6;
            unsigned c_hr = dsp_pseudo_rand_uint32(&rand_seed)%6;
            for(unsigned i=0;i<len;i++){
                b[i] = dsp_pseudo_rand_int16(&rand_seed)>>b_hr;
            }
            c = dsp_pseudo_rand_int16(&rand_seed)>>c_hr;

            b_hr = dsp_bfp_cls_vect_int16(b, len);
            c_hr = dsp_bfp_cls_int16(c);

            int error = 0;
            dsp_conv_vect_int16_to_float(b, b_exp, B, len, &error);
            C = dsp_conv_int16_to_float(c, c_exp, &error);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            int a_exp;
            unsigned a_hr;
            dsp_muls_bfp_vect_int16(
                a, &a_exp, &a_hr, 
                b, b_exp, b_hr, 
                c, c_exp, c_hr, 
                len);

            for (int i=0; i<len; i++) {
                A[i] = B[i] * C;
            }

            unsigned diff = dsp_abs_diff_vect_int16(a, a_exp, A, len, &error);

            unsigned actual_a_hr = dsp_bfp_cls_vect_int16(a, len);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
            TEST_ASSERT_LESS_THAN_UINT32_MESSAGE(3, diff, "error is too high");
            TEST_ASSERT_EQUAL_UINT32_MESSAGE(a_hr, actual_a_hr, "error hr is wrong");
        }
    }
    return;
}

void test_bfp_vect_muls8(){

    int8_t a[MAX_LEN], b[MAX_LEN], c;
    dsp_float_t A[MAX_LEN], B[MAX_LEN], C;

    unsigned rand_seed = 5;

    for(unsigned len=1;len<MAX_LEN;len++){
        for(unsigned itt=0;itt<ITT;itt++){

            int b_exp = -31 + (3 + (dsp_pseudo_rand_int32(&rand_seed)%6));
            int c_exp = -31 + (3 + (dsp_pseudo_rand_int32(&rand_seed)%6));

            unsigned b_hr = dsp_pseudo_rand_uint32(&rand_seed)%6;
            unsigned c_hr = dsp_pseudo_rand_uint32(&rand_seed)%6;
            for(unsigned i=0;i<len;i++){
                b[i] = dsp_pseudo_rand_int16(&rand_seed)>>b_hr;
            }
            c = dsp_pseudo_rand_int16(&rand_seed)>>c_hr;

            b_hr = dsp_bfp_cls_vect_int8(b, len);
            c_hr = dsp_bfp_cls_int8(c);

            int error = 0;
            dsp_conv_vect_int8_to_float(b, b_exp, B, len, &error);
            C = dsp_conv_int8_to_float(c, c_exp, &error);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            int a_exp;
            unsigned a_hr;
            dsp_muls_bfp_vect_int8(
                a, &a_exp, &a_hr, 
                b, b_exp, b_hr, 
                c, c_exp, c_hr, 
                len);

            for (int i=0; i<len; i++) {
                A[i] = B[i] * C;
            }

            unsigned diff = UINT_MAX;
            diff = dsp_abs_diff_vect_int8(a, a_exp, A, len, &error);

            unsigned actual_a_hr = dsp_bfp_cls_vect_int8(a, len);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
            TEST_ASSERT_LESS_THAN_UINT32_MESSAGE(3, diff, "error is too high");
            TEST_ASSERT_EQUAL_UINT32_MESSAGE(a_hr, actual_a_hr, "error hr is wrong");
        }
    }
    return;
}


void test_vect_mul32(){

    int32_t a[MAX_LEN], b[MAX_LEN], c[MAX_LEN];
    dsp_float_t A[MAX_LEN], B[MAX_LEN], C[MAX_LEN];

    unsigned rand_seed = 5;

    for(unsigned len=1;len<MAX_LEN;len++){
        for(unsigned itt=0;itt<ITT;itt++){
            for(unsigned i=0;i<MAX_LEN;i++){
                b[i] = 0;
                c[i] = 0;
            }
            for(unsigned i=0;i<len;i++){
                b[i] = dsp_pseudo_rand_int32(&rand_seed)>>2;
                c[i] = dsp_pseudo_rand_int32(&rand_seed)>>2;
            }

            int error = 0;
            dsp_conv_vect_int32_to_float(b, Q_EXP_32, B, len, &error);
            dsp_conv_vect_int32_to_float(c, Q_EXP_32, C, len, &error);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            dsp_mul_vect_int32(a, b, c, len);
            dsp_mul_vect_float(A, B, C, len);

            unsigned diff = dsp_abs_diff_vect_int32(a, Q_EXP_32, A, len, &error);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
            TEST_ASSERT_LESS_THAN_UINT32_MESSAGE(3, diff, "error is too high");
        }
    }

    return;
}


void test_vect_mul16(){

    int16_t a[MAX_LEN], b[MAX_LEN], c[MAX_LEN];
    dsp_float_t A[MAX_LEN], B[MAX_LEN], C[MAX_LEN];

    unsigned rand_seed = 5;

    for(unsigned len=1;len<MAX_LEN;len++){
        for(unsigned itt=0;itt<ITT;itt++){
            for(unsigned i=0;i<MAX_LEN;i++){
                b[i] = 0;
                c[i] = 0;
            }

            for(unsigned i=0;i<len;i++){
                b[i] = dsp_pseudo_rand_int16(&rand_seed)>>2;
                c[i] = dsp_pseudo_rand_int16(&rand_seed)>>2;
            }

            int error = 0;
            dsp_conv_vect_int16_to_float(b, Q_EXP_16, B, len, &error);
            dsp_conv_vect_int16_to_float(c, Q_EXP_16, C, len, &error);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            dsp_mul_vect_int16(a, b, c, len);
            dsp_mul_vect_float(A, B, C, len);

            unsigned diff = dsp_abs_diff_vect_int16(a, Q_EXP_16, A, len, &error);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
            TEST_ASSERT_LESS_THAN_UINT32_MESSAGE(3, diff, "error is too high");
        }
    }
    return;
}

void test_vect_mul8(){

    int8_t a[MAX_LEN], b[MAX_LEN], c[MAX_LEN];
    dsp_float_t A[MAX_LEN], B[MAX_LEN], C[MAX_LEN];

    unsigned rand_seed = 5;

    for(unsigned len=1;len<MAX_LEN;len++){
        for(unsigned itt=0;itt<ITT;itt++){
            for(unsigned i=0;i<MAX_LEN;i++){
                b[i] = 0;
                c[i] = 0;
            }

            for(unsigned i=0;i<len;i++){
                b[i] = dsp_pseudo_rand_int8(&rand_seed)>>2;
                c[i] = dsp_pseudo_rand_int8(&rand_seed)>>2;
            }

            int error = 0;
            dsp_conv_vect_int8_to_float(b, Q_EXP_8, B, len, &error);
            dsp_conv_vect_int8_to_float(c, Q_EXP_8, C, len, &error);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            dsp_mul_vect_int8(a, b, c, len);
            dsp_mul_vect_float(A, B, C, len);

            unsigned diff = dsp_abs_diff_vect_int8(a, Q_EXP_8, A, len, &error);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
            TEST_ASSERT_LESS_THAN_UINT32_MESSAGE(3, diff, "error is too high");
        }
    }
    return;
}


void test_bfp_vect_sub32(){

    int32_t a[MAX_LEN], b[MAX_LEN], c[MAX_LEN];
    dsp_float_t A[MAX_LEN], B[MAX_LEN], C[MAX_LEN];

    unsigned rand_seed = 5;

    for(unsigned len=1;len<MAX_LEN;len++){
        for(unsigned itt=0;itt<ITT;itt++){

            int b_exp = -31 + (3 + (dsp_pseudo_rand_int32(&rand_seed)%6));
            int c_exp = -31 + (3 + (dsp_pseudo_rand_int32(&rand_seed)%6));

            unsigned b_hr = dsp_pseudo_rand_uint32(&rand_seed)%6;
            unsigned c_hr = dsp_pseudo_rand_uint32(&rand_seed)%6;
            for(unsigned i=0;i<len;i++){
                b[i] = dsp_pseudo_rand_int32(&rand_seed)>>b_hr;
                c[i] = dsp_pseudo_rand_int32(&rand_seed)>>c_hr;
            }

            b_hr = dsp_bfp_cls_vect_int32(b, len);
            c_hr = dsp_bfp_cls_vect_int32(c, len);

            int error = 0;
            dsp_conv_vect_int32_to_float(b, b_exp, B, len, &error);
            dsp_conv_vect_int32_to_float(c, c_exp, C, len, &error);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            int a_exp;
            unsigned a_hr;
            dsp_sub_bfp_vect_int32(
                a, &a_exp, &a_hr, 
                b, b_exp, b_hr, 
                c, c_exp, c_hr, 
                len);
            dsp_sub_vect_float(A, B, C, len);


            unsigned diff = dsp_abs_diff_vect_int32(a, a_exp, A, len, &error);

            unsigned actual_a_hr = dsp_bfp_cls_vect_int32(a, len);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
            TEST_ASSERT_LESS_THAN_UINT32_MESSAGE(3, diff, "error is too high");
            TEST_ASSERT_EQUAL_UINT32_MESSAGE(a_hr, actual_a_hr, "error hr is wrong");
        }
    }

    return;
}


void test_bfp_vect_sub16(){

    int16_t a[MAX_LEN], b[MAX_LEN], c[MAX_LEN];
    dsp_float_t A[MAX_LEN], B[MAX_LEN], C[MAX_LEN];

    unsigned rand_seed = 5;

    for(unsigned len=1;len<MAX_LEN;len++){
        for(unsigned itt=0;itt<ITT;itt++){

            int b_exp = -31 + (3 + (dsp_pseudo_rand_int32(&rand_seed)%6));
            int c_exp = -31 + (3 + (dsp_pseudo_rand_int32(&rand_seed)%6));

            unsigned b_hr = dsp_pseudo_rand_uint32(&rand_seed)%6;
            unsigned c_hr = dsp_pseudo_rand_uint32(&rand_seed)%6;
            for(unsigned i=0;i<len;i++){
                b[i] = dsp_pseudo_rand_int16(&rand_seed)>>b_hr;
                c[i] = dsp_pseudo_rand_int16(&rand_seed)>>c_hr;
            }

            b_hr = dsp_bfp_cls_vect_int16(b, len);
            c_hr = dsp_bfp_cls_vect_int16(c, len);

            int error = 0;
            dsp_conv_vect_int16_to_float(b, b_exp, B, len, &error);
            dsp_conv_vect_int16_to_float(c, c_exp, C, len, &error);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            int a_exp;
            unsigned a_hr;
            dsp_sub_bfp_vect_int16(
                a, &a_exp, &a_hr, 
                b, b_exp, b_hr, 
                c, c_exp, c_hr, 
                len);
            dsp_sub_vect_float(A, B, C, len);

            unsigned diff = dsp_abs_diff_vect_int16(a, a_exp, A, len, &error);

            unsigned actual_a_hr = dsp_bfp_cls_vect_int16(a, len);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
            TEST_ASSERT_LESS_THAN_UINT32_MESSAGE(3, diff, "error is too high");
            TEST_ASSERT_EQUAL_UINT32_MESSAGE(a_hr, actual_a_hr, "error hr is wrong");
        }
    }
    return;
}

void test_bfp_vect_sub8(){

    int8_t a[MAX_LEN], b[MAX_LEN], c[MAX_LEN];
    dsp_float_t A[MAX_LEN], B[MAX_LEN], C[MAX_LEN];

    unsigned rand_seed = 5;

    for(unsigned len=1;len<MAX_LEN;len++){
        for(unsigned itt=0;itt<ITT;itt++){

            int b_exp = -31 + (3 + (dsp_pseudo_rand_int32(&rand_seed)%6));
            int c_exp = -31 + (3 + (dsp_pseudo_rand_int32(&rand_seed)%6));

            unsigned b_hr = dsp_pseudo_rand_uint32(&rand_seed)%6;
            unsigned c_hr = dsp_pseudo_rand_uint32(&rand_seed)%6;
            for(unsigned i=0;i<len;i++){
                b[i] = dsp_pseudo_rand_int16(&rand_seed)>>b_hr;
                c[i] = dsp_pseudo_rand_int16(&rand_seed)>>c_hr;
            }

            b_hr = dsp_bfp_cls_vect_int8(b, len);
            c_hr = dsp_bfp_cls_vect_int8(c, len);

            int error = 0;
            dsp_conv_vect_int8_to_float(b, b_exp, B, len, &error);
            dsp_conv_vect_int8_to_float(c, c_exp, C, len, &error);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            int a_exp;
            unsigned a_hr;
            dsp_sub_bfp_vect_int8(
                a, &a_exp, &a_hr, 
                b, b_exp, b_hr, 
                c, c_exp, c_hr, 
                len);
            dsp_sub_vect_float(A, B, C, len);

            unsigned diff = UINT_MAX;
            diff = dsp_abs_diff_vect_int8(a, a_exp, A, len, &error);

            unsigned actual_a_hr = dsp_bfp_cls_vect_int8(a, len);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
            TEST_ASSERT_LESS_THAN_UINT32_MESSAGE(3, diff, "error is too high");
            TEST_ASSERT_EQUAL_UINT32_MESSAGE(a_hr, actual_a_hr, "error hr is wrong");
        }
    }
    return;
}


void test_vect_sub32(){

    int32_t a[MAX_LEN], b[MAX_LEN], c[MAX_LEN];
    dsp_float_t A[MAX_LEN], B[MAX_LEN], C[MAX_LEN];

    unsigned rand_seed = 5;

    for(unsigned len=1;len<MAX_LEN;len++){
        for(unsigned itt=0;itt<ITT;itt++){
            for(unsigned i=0;i<MAX_LEN;i++){
                b[i] = 0;
                c[i] = 0;
            }
            for(unsigned i=0;i<len;i++){
                b[i] = dsp_pseudo_rand_int32(&rand_seed)>>2;
                c[i] = dsp_pseudo_rand_int32(&rand_seed)>>2;
            }

            int error = 0;
            dsp_conv_vect_int32_to_float(b, Q_EXP_32, B, len, &error);
            dsp_conv_vect_int32_to_float(c, Q_EXP_32, C, len, &error);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            dsp_sub_vect_int32(a, b, c, len);
            dsp_sub_vect_float(A, B, C, len);

            unsigned diff = dsp_abs_diff_vect_int32(a, Q_EXP_32, A, len, &error);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
            TEST_ASSERT_LESS_THAN_UINT32_MESSAGE(3, diff, "error is too high");
        }
    }

    return;
}


void test_vect_sub16(){

    int16_t a[MAX_LEN], b[MAX_LEN], c[MAX_LEN];
    dsp_float_t A[MAX_LEN], B[MAX_LEN], C[MAX_LEN];

    unsigned rand_seed = 5;

    for(unsigned len=1;len<MAX_LEN;len++){
        for(unsigned itt=0;itt<ITT;itt++){
            for(unsigned i=0;i<MAX_LEN;i++){
                b[i] = 0;
                c[i] = 0;
            }
            // printf("----\n");

            for(unsigned i=0;i<len;i++){
                b[i] = dsp_pseudo_rand_int16(&rand_seed)>>2;
                c[i] = dsp_pseudo_rand_int16(&rand_seed)>>2;
            }

            int error = 0;
            dsp_conv_vect_int16_to_float(b, Q_EXP_16, B, len, &error);
            dsp_conv_vect_int16_to_float(c, Q_EXP_16, C, len, &error);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            dsp_sub_vect_int16(a, b, c, len);
            dsp_sub_vect_float(A, B, C, len);

            unsigned diff = dsp_abs_diff_vect_int16(a, Q_EXP_16, A, len, &error);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
            TEST_ASSERT_LESS_THAN_UINT32_MESSAGE(3, diff, "error is too high");
        }
    }
    return;
}

void test_vect_sub8(){

    int8_t a[MAX_LEN], b[MAX_LEN], c[MAX_LEN];
    dsp_float_t A[MAX_LEN], B[MAX_LEN], C[MAX_LEN];

    unsigned rand_seed = 5;

    for(unsigned len=1;len<MAX_LEN;len++){
        for(unsigned itt=0;itt<ITT;itt++){
            for(unsigned i=0;i<MAX_LEN;i++){
                b[i] = 0;
                c[i] = 0;
            }

            for(unsigned i=0;i<len;i++){
                b[i] = dsp_pseudo_rand_int8(&rand_seed)>>2;
                c[i] = dsp_pseudo_rand_int8(&rand_seed)>>2;
            }

            int error = 0;
            dsp_conv_vect_int8_to_float(b, Q_EXP_8, B, len, &error);
            dsp_conv_vect_int8_to_float(c, Q_EXP_8, C, len, &error);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            dsp_sub_vect_int8(a, b, c, len);
            dsp_sub_vect_float(A, B, C, len);

            unsigned diff = dsp_abs_diff_vect_int8(a, Q_EXP_8, A, len, &error);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
            TEST_ASSERT_LESS_THAN_UINT32_MESSAGE(3, diff, "error is too high");
        }
    }
    return;
}

void test_bfp_vect_div32() {

    int32_t a[MAX_LEN], b[MAX_LEN], c[MAX_LEN];
    dsp_float_t A[MAX_LEN], B[MAX_LEN], C[MAX_LEN];

    unsigned rand_seed = 6;

    for(unsigned len=1;len<MAX_LEN;len++){
        len = MAX_LEN;
        for(unsigned itt=0;itt<ITT;itt++){

            int b_exp = dsp_pseudo_rand_int32(&rand_seed) % 15;
            int c_exp = dsp_pseudo_rand_int32(&rand_seed) % 15;

            unsigned b_hr = 5 + dsp_pseudo_rand_uint32(&rand_seed)%6;
            unsigned c_hr = 5 + dsp_pseudo_rand_uint32(&rand_seed)%6;
            for(unsigned i=0;i<len;i++){
                b[i] = dsp_pseudo_rand_int32(&rand_seed)>>b_hr;
                c[i] = dsp_pseudo_rand_int32(&rand_seed)>>c_hr;
            }

            b_hr = dsp_bfp_cls_vect_int32(b, len);
            c_hr = dsp_bfp_cls_vect_int32(c, len);

            int error = 0;
            dsp_conv_vect_int32_to_float(b, b_exp, B, len, &error);
            dsp_conv_vect_int32_to_float(c, c_exp, C, len, &error);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            int a_exp;
            unsigned a_hr;
            dsp_div_bfp_vect_int32(
                    a, &a_exp, &a_hr,
                    b, b_exp, b_hr,
                    c, c_exp, c_hr,
                    len);

            int zero_div[MAX_LEN] = {0};
            for (int i=0; i<len; i++) {
                if (c[i] == 0) {
                    zero_div[i] = 1;
                } else {
                    A[i] = B[i] / C[i];
                }
            }

            unsigned actual_a_hr = dsp_bfp_cls_vect_int32(a, len);

            // Set A[i] == a[i] for zero-divs (diff doesn't like INT_MAX -> float)
            for (int i=0; i<len; i++) {
                if (zero_div[i]) {
                    A[i] = (dsp_float_t) ldexp(1, a_exp);
                    a[i] = 1;
                }
            }
            unsigned diff = dsp_abs_diff_vect_int32(a, a_exp, A, len, &error);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
            TEST_ASSERT_LESS_THAN_UINT32_MESSAGE(3, diff, "error is too high");
            TEST_ASSERT_EQUAL_UINT32_MESSAGE(a_hr, actual_a_hr, "error hr is wrong");
        }
    }

    return;
}


void test_bfp_vect_div_complex_32() {

    dsp_complex_t a[MAX_LEN], b[MAX_LEN];
    int32_t c[MAX_LEN];
    dsp_complex_float_t A[MAX_LEN], B[MAX_LEN];
    dsp_float_t C[MAX_LEN];

    unsigned rand_seed = 6;

    for(unsigned len=1;len<MAX_LEN;len++){
        len = MAX_LEN;
        for(unsigned itt=0;itt<ITT;itt++){

            int b_exp = dsp_pseudo_rand_int32(&rand_seed) % 5;
            int c_exp = dsp_pseudo_rand_int32(&rand_seed) % 5;

            unsigned b_hr = 5 + dsp_pseudo_rand_uint32(&rand_seed)%6;
            unsigned c_hr = 5 + dsp_pseudo_rand_uint32(&rand_seed)%6;
            for(unsigned i=0;i<len;i++){
                b[i].re = dsp_pseudo_rand_int32(&rand_seed)>>b_hr;
                b[i].im = dsp_pseudo_rand_int32(&rand_seed)>>b_hr;
                c[i] = dsp_pseudo_rand_int32(&rand_seed)>>c_hr;
            }

            b_hr = dsp_bfp_cls_vect_complex_int32(b, len);
            c_hr = dsp_bfp_cls_vect_int32(c, len);

            int error = 0;
            dsp_conv_vect_complex_int32_to_complex_float(b, b_exp, B, len, &error);
            dsp_conv_vect_int32_to_float(c, c_exp, C, len, &error);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            int a_exp;
            unsigned a_hr;
            dsp_div_bfp_vect_complex_int32(
                    a, &a_exp, &a_hr,
                    b, b_exp, b_hr,
                    c, c_exp, c_hr,
                    len);

            int zero_div[MAX_LEN] = {0};
            for (int i=0; i<len; i++) {
                if (c[i] == 0) {
                    zero_div[i] = 1;
                } else {
                    A[i].re = B[i].re / C[i];
                    A[i].im = B[i].im / C[i];
                }
            }

            unsigned actual_a_hr = dsp_bfp_cls_vect_complex_int32(a, len);

            // Set A[i] == a[i] for zero-divs (diff doesn't like INT_MAX -> float)
            for (int i=0; i<len; i++) {
                if (zero_div[i]) {
                    A[i].re = (dsp_float_t) ldexp(1, a_exp);
                    A[i].im = (dsp_float_t) ldexp(1, a_exp);
                    a[i].re = 1;
                    a[i].im = 1;
                }
            }
            unsigned diff = dsp_abs_diff_vect_complex_int32(a, a_exp, A, len, &error);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
            TEST_ASSERT_LESS_THAN_UINT32_MESSAGE(3, diff, "error is too high");
            TEST_ASSERT_EQUAL_UINT32_MESSAGE(a_hr, actual_a_hr, "error hr is wrong");
        }
    }

    return;
}

void test_bfp_vect_div16() {

    int16_t a[MAX_LEN], b[MAX_LEN], c[MAX_LEN];
    dsp_float_t A[MAX_LEN], B[MAX_LEN], C[MAX_LEN];

    unsigned rand_seed = 6;

    for(unsigned len=1;len<MAX_LEN;len++){
        len = MAX_LEN;
        for(unsigned itt=0;itt<ITT;itt++){

            int b_exp = dsp_pseudo_rand_int16(&rand_seed) % 15;
            int c_exp = dsp_pseudo_rand_int16(&rand_seed) % 15;

            unsigned b_hr = 5 + dsp_pseudo_rand_uint16(&rand_seed)%6;
            unsigned c_hr = 5 + dsp_pseudo_rand_uint16(&rand_seed)%6;
            for(unsigned i=0;i<len;i++){
                b[i] = dsp_pseudo_rand_int16(&rand_seed)>>b_hr;
                c[i] = dsp_pseudo_rand_int16(&rand_seed)>>c_hr;
            }

            b_hr = dsp_bfp_cls_vect_int16(b, len);
            c_hr = dsp_bfp_cls_vect_int16(c, len);

            int error = 0;
            dsp_conv_vect_int16_to_float(b, b_exp, B, len, &error);
            dsp_conv_vect_int16_to_float(c, c_exp, C, len, &error);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error 1");

            int a_exp;
            unsigned a_hr;
            dsp_div_bfp_vect_int16(
                    a, &a_exp, &a_hr,
                    b, b_exp, b_hr,
                    c, c_exp, c_hr,
                    len);

            int zero_div[MAX_LEN] = {0};
            for (int i=0; i<len; i++) {
                if (c[i] == 0) {
                    zero_div[i] = 1;
                } else {
                    A[i] = B[i] / C[i];
                }
            }


            unsigned actual_a_hr = dsp_bfp_cls_vect_int16(a, len);

            // Set A[i] == a[i] for zero-divs (diff doesn't like INT_MAX -> float)
            for (int i=0; i<len; i++) {
                if (zero_div[i]) {
                    A[i] = (dsp_float_t) ldexp(1, a_exp);
                    a[i] = 1;
                }
            }
            unsigned diff = dsp_abs_diff_vect_int16(a, a_exp, A, len, &error);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error 2");
            TEST_ASSERT_LESS_THAN_UINT16_MESSAGE(3, diff, "error is too high");
            TEST_ASSERT_EQUAL_UINT16_MESSAGE(a_hr, actual_a_hr, "error hr is wrong");
        }
    }

    return;
}

void test_bfp_vect_div8() {

    int8_t a[MAX_LEN], b[MAX_LEN], c[MAX_LEN];
    dsp_float_t A[MAX_LEN], B[MAX_LEN], C[MAX_LEN];

    unsigned rand_seed = 6;

    for(unsigned len=1;len<MAX_LEN;len++){
        len = MAX_LEN;
        for(unsigned itt=0;itt<ITT;itt++){

            int b_exp = dsp_pseudo_rand_int8(&rand_seed) % 15;
            int c_exp = dsp_pseudo_rand_int8(&rand_seed) % 15;

            unsigned b_hr = 5 + dsp_pseudo_rand_uint8(&rand_seed)%6;
            unsigned c_hr = 5 + dsp_pseudo_rand_uint8(&rand_seed)%6;
            for(unsigned i=0;i<len;i++){
                b[i] = dsp_pseudo_rand_int8(&rand_seed)>>b_hr;
                c[i] = dsp_pseudo_rand_int8(&rand_seed)>>c_hr;
            }

            b_hr = dsp_bfp_cls_vect_int8(b, len);
            c_hr = dsp_bfp_cls_vect_int8(c, len);

            int error = 0;
            dsp_conv_vect_int8_to_float(b, b_exp, B, len, &error);
            dsp_conv_vect_int8_to_float(c, c_exp, C, len, &error);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            int a_exp;
            unsigned a_hr;
            dsp_div_bfp_vect_int8(
                    a, &a_exp, &a_hr,
                    b, b_exp, b_hr,
                    c, c_exp, c_hr,
                    len);

            int zero_div[MAX_LEN] = {0};
            for (int i=0; i<len; i++) {
                if (C[i] == 0) {
                    zero_div[i] = 1;
                } else {
                    A[i] = B[i] / C[i];
                }
            }

            unsigned actual_a_hr = dsp_bfp_cls_vect_int8(a, len);

            // Set A[i] == a[i] for zero-divs (diff doesn't like INT_MAX -> float)
            for (int i=0; i<len; i++) {
                if (zero_div[i]) {
                    A[i] = (dsp_float_t) ldexp(1, a_exp);
                    a[i] = 1;
                }
            }
            unsigned diff = dsp_abs_diff_vect_int8(a, a_exp, A, len, &error);

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
            TEST_ASSERT_LESS_THAN_UINT8_MESSAGE(3, diff, "error is too high");
            TEST_ASSERT_EQUAL_UINT8_MESSAGE(a_hr, actual_a_hr, "error hr is wrong");
        }
    }

    return;
}
