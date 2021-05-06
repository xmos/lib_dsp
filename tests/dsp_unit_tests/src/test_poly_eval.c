// Copyright 2019-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include "unity.h"

#include <xs1.h>
#include <string.h>
#include <xclib.h>

#include <stdio.h>
#include <dsp.h>
#include <dsp_fp.h>
#include <dsp_testing.h>
#include <math.h>
#include <limits.h>

#define MAX_LEN 32
void test_poly_eval(){
    unsigned rand_seed = 11;

    const unsigned n_coefs_7 = 8;
    const int coefs_exp = -31;
    const int32_t coefs_7_rev[] = {2147483647, 1488522224,  515882292, 119192660, 20649056,
                  2851237,  316192,  23243, };
    double coefs_7_rev_fp[MAX_LEN];

    for(unsigned i=0;i<n_coefs_7;i++){
        coefs_7_rev_fp[i] = ldexp(coefs_7_rev[i], coefs_exp);
    }

    int exp = -31;

    for(unsigned i=0;i<1<<14;i++){
        int32_t x = dsp_pseudo_rand_int32(&rand_seed);
        if (x>0) x=-x;

        int error = 0;
        double x_fp = dsp_conv_int32_to_float(x, exp, &error);
        int32_t y = dsp_poly_eval(x, coefs_7_rev, n_coefs_7);

        double y_fp = dsp_poly_eval_fp(x_fp, coefs_7_rev_fp, n_coefs_7);

        int32_t expected = dsp_conv_float_to_int32(y_fp, coefs_exp, &error);

        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

        TEST_ASSERT_INT32_WITHIN_MESSAGE(4, expected, y, "Output not close enough");
    }

}

