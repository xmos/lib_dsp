// Copyright 2017-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include "unity.h"

#include <xs1.h>
#include <string.h>
#include <xclib.h>

#include <stdio.h>
#include <dsp.h>
#include <dsp_fp.h>
#include <dsp_testing.h>

//void dsp_conv_vect_float_to_int16 (dsp_float_t *f, int16_t *d, int *d_exp, unsigned length, int *error)


#define VECT_LEN 8

void test_conv_int8_zeros(){
    dsp_float_t f[VECT_LEN];
    int8_t d[VECT_LEN];

    for(unsigned i=0;i<VECT_LEN;i++)
        f[i] = 0.0;
    int d_exp, error = 0;
    dsp_conv_vect_float_to_int8 (f, d, &d_exp, VECT_LEN, &error);

    TEST_ASSERT_EQUAL_INT32_MESSAGE(DSP_BFP_ZERO_EXP, d_exp, "bad exponent");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, error, "bad error");
    for(unsigned i=0;i<VECT_LEN;i++){
        TEST_ASSERT_EQUAL_INT8_MESSAGE(0, d[i], "output is not zero");
    }
}

void test_conv_int8_mostly_zeros(){
    dsp_float_t f[VECT_LEN];
    int8_t d[VECT_LEN];

    f[0] = 1.0;
    for(unsigned i=1;i<VECT_LEN;i++)
        f[i] = 0.0;
    int d_exp, error = 0;
    dsp_conv_vect_float_to_int8 (f, d, &d_exp, VECT_LEN, &error);

    TEST_ASSERT_EQUAL_INT32_MESSAGE(64, d[0], "mantissa is wrong");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-6, d_exp, "exponent is wrong");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, error, "bad error");
    for(unsigned i=1;i<VECT_LEN;i++){
        TEST_ASSERT_EQUAL_INT8_MESSAGE(0, d[i], "output is not zero");
    }
}

void test_conv_int16_zeros(){
    dsp_float_t f[VECT_LEN];
    int16_t d[VECT_LEN];

    for(unsigned i=0;i<VECT_LEN;i++)
        f[i] = 0.0;
    int d_exp, error = 0;
    dsp_conv_vect_float_to_int16 (f, d, &d_exp, VECT_LEN, &error);

    TEST_ASSERT_EQUAL_INT32_MESSAGE(DSP_BFP_ZERO_EXP, d_exp, "bad exponent");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, error, "bad error");
    for(unsigned i=0;i<VECT_LEN;i++){
        TEST_ASSERT_EQUAL_INT16_MESSAGE(0, d[i], "output is not zero");
    }
}

void test_conv_int16_mostly_zeros(){
    dsp_float_t f[VECT_LEN];
    int16_t d[VECT_LEN];

    f[0] = 1.0;
    for(unsigned i=1;i<VECT_LEN;i++)
        f[i] = 0.0;
    int d_exp, error = 0;
    dsp_conv_vect_float_to_int16 (f, d, &d_exp, VECT_LEN, &error);

    TEST_ASSERT_EQUAL_INT32_MESSAGE(16384, d[0], "mantissa is wrong");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-14, d_exp, "exponent is wrong");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, error, "bad error");
    for(unsigned i=1;i<VECT_LEN;i++){
        TEST_ASSERT_EQUAL_INT16_MESSAGE(0, d[i], "output is not zero");
    }
}

void test_conv_uint16_zeros(){
    dsp_float_t f[VECT_LEN];
    uint16_t d[VECT_LEN];

    for(unsigned i=0;i<VECT_LEN;i++)
        f[i] = 0.0;
    int d_exp, error = 0;
    dsp_conv_vect_float_to_uint16 (f, d, &d_exp, VECT_LEN, &error);

    TEST_ASSERT_EQUAL_INT32_MESSAGE(DSP_BFP_ZERO_EXP, d_exp, "bad exponent");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, error, "bad error");
    for(unsigned i=0;i<VECT_LEN;i++){
        TEST_ASSERT_EQUAL_UINT16_MESSAGE(0, d[i], "output is not zero");
    }
}

void test_conv_uint16_mostly_zeros(){
    dsp_float_t f[VECT_LEN];
    uint16_t d[VECT_LEN];

    f[0] = 1.0;
    for(unsigned i=1;i<VECT_LEN;i++)
        f[i] = 0.0;
    int d_exp, error = 0;
    dsp_conv_vect_float_to_uint16 (f, d, &d_exp, VECT_LEN, &error);

    TEST_ASSERT_EQUAL_UINT32_MESSAGE(32768, d[0], "mantissa is wrong");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-15, d_exp, "exponent is wrong");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, error, "bad error");
    for(unsigned i=1;i<VECT_LEN;i++){
        TEST_ASSERT_EQUAL_UINT16_MESSAGE(0, d[i], "output is not zero");
    }
}

void test_conv_uint16_error(){
    dsp_float_t f[VECT_LEN];
    uint16_t d[VECT_LEN];

    f[0] = -1.0;
    for(unsigned i=1;i<VECT_LEN;i++)
        f[i] = 0.0;
    int d_exp, error = 0;
    dsp_conv_vect_float_to_uint16 (f, d, &d_exp, VECT_LEN, &error);

    TEST_ASSERT_EQUAL_UINT32_MESSAGE(NEGATIVE_TO_UNSIGNED, error, "bad error");
}

void test_conv_int32_zeros(){
    dsp_float_t f[VECT_LEN];
    int32_t d[VECT_LEN];

    for(unsigned i=0;i<VECT_LEN;i++)
        f[i] = 0.0;
    int d_exp, error = 0;
    dsp_conv_vect_float_to_int32 (f, d, &d_exp, VECT_LEN, &error);

    TEST_ASSERT_EQUAL_INT32_MESSAGE(DSP_BFP_ZERO_EXP, d_exp, "bad exponent");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, error, "bad error");
    for(unsigned i=0;i<VECT_LEN;i++){
        TEST_ASSERT_EQUAL_INT32_MESSAGE(0, d[i], "output is not zero");
    }
}

void test_conv_int32_mostly_zeros(){
    dsp_float_t f[VECT_LEN];
    int32_t d[VECT_LEN];

    f[0] = 1.0;
    for(unsigned i=1;i<VECT_LEN;i++)
        f[i] = 0.0;
    int d_exp, error = 0;
    dsp_conv_vect_float_to_int32 (f, d, &d_exp, VECT_LEN, &error);

    TEST_ASSERT_EQUAL_INT32_MESSAGE(1073741824, d[0], "mantissa is wrong");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-30, d_exp, "exponent is wrong");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, error, "bad error");
    for(unsigned i=1;i<VECT_LEN;i++){
        TEST_ASSERT_EQUAL_INT32_MESSAGE(0, d[i], "output is not zero");
    }
}


void test_conv_uint32_zeros(){
    dsp_float_t f[VECT_LEN];
    uint32_t d[VECT_LEN];

    for(unsigned i=0;i<VECT_LEN;i++)
        f[i] = 0.0;
    int d_exp, error = 0;
    dsp_conv_vect_float_to_uint32 (f, d, &d_exp, VECT_LEN, &error);

    TEST_ASSERT_EQUAL_INT32_MESSAGE(DSP_BFP_ZERO_EXP, d_exp, "bad exponent");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, error, "bad error");
    for(unsigned i=0;i<VECT_LEN;i++){
        TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, d[i], "output is not zero");
    }
}

void test_conv_uint32_mostly_zeros(){
    dsp_float_t f[VECT_LEN];
    uint32_t d[VECT_LEN];

    f[0] = 1.0;
    for(unsigned i=1;i<VECT_LEN;i++)
        f[i] = 0.0;
    int d_exp, error = 0;
    dsp_conv_vect_float_to_uint32 (f, d, &d_exp, VECT_LEN, &error);

    TEST_ASSERT_EQUAL_UINT32_MESSAGE(2147483648, d[0], "mantissa is wrong");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-31, d_exp, "exponent is wrong");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, error, "bad error");
    for(unsigned i=1;i<VECT_LEN;i++){
        TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, d[i], "output is not zero");
    }
}

void test_conv_uint32_error(){
    dsp_float_t f[VECT_LEN];
    uint32_t d[VECT_LEN];

    f[0] = -1.0;
    for(unsigned i=1;i<VECT_LEN;i++)
        f[i] = 0.0;
    int d_exp, error = 0;
    dsp_conv_vect_float_to_uint32 (f, d, &d_exp, VECT_LEN, &error);

    TEST_ASSERT_EQUAL_UINT32_MESSAGE(NEGATIVE_TO_UNSIGNED, error, "bad error");
}


void test_conv_int64_zeros(){
    dsp_float_t f[VECT_LEN];
    int64_t d[VECT_LEN];

    for(unsigned i=0;i<VECT_LEN;i++)
        f[i] = 0.0;
    int d_exp, error = 0;
    dsp_conv_vect_float_to_int64 (f, d, &d_exp, VECT_LEN, &error);

    TEST_ASSERT_EQUAL_INT32_MESSAGE(DSP_BFP_ZERO_EXP, d_exp, "bad exponent");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, error, "bad error");
    for(unsigned i=0;i<VECT_LEN;i++){
        TEST_ASSERT_EQUAL_INT64_MESSAGE(0, d[i], "output is not zero");
    }
}

void test_conv_int64_mostly_zeros(){
    dsp_float_t f[VECT_LEN];
    int64_t d[VECT_LEN];

    f[0] = 1.0;
    for(unsigned i=1;i<VECT_LEN;i++)
        f[i] = 0.0;
    int d_exp, error = 0;
    dsp_conv_vect_float_to_int64 (f, d, &d_exp, VECT_LEN, &error);

    TEST_ASSERT_EQUAL_INT64_MESSAGE(0x4000000000000000LL, d[0], "mantissa is wrong");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-62, d_exp, "exponent is wrong");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, error, "bad error");
    for(unsigned i=1;i<VECT_LEN;i++){
        TEST_ASSERT_EQUAL_INT64_MESSAGE(0, d[i], "output is not zero");
    }
}

void test_conv_uint64_zeros(){
    dsp_float_t f[VECT_LEN];
    uint64_t d[VECT_LEN];

    for(unsigned i=0;i<VECT_LEN;i++)
        f[i] = 0.0;
    int d_exp, error = 0;
    dsp_conv_vect_float_to_uint64 (f, d, &d_exp, VECT_LEN, &error);

    TEST_ASSERT_EQUAL_INT32_MESSAGE(DSP_BFP_ZERO_EXP, d_exp, "bad exponent");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, error, "bad error");
    for(unsigned i=0;i<VECT_LEN;i++){
        TEST_ASSERT_EQUAL_UINT64_MESSAGE(0, d[i], "output is not zero");
    }
}

void test_conv_uint64_mostly_zeros(){
    dsp_float_t f[VECT_LEN];
    uint64_t d[VECT_LEN];

    f[0] = 1.0;
    for(unsigned i=1;i<VECT_LEN;i++)
        f[i] = 0.0;
    int d_exp, error = 0;
    dsp_conv_vect_float_to_uint64 (f, d, &d_exp, VECT_LEN, &error);

    TEST_ASSERT_EQUAL_UINT64_MESSAGE(0x8000000000000000LLU, d[0], "mantissa is wrong");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-63, d_exp, "exponent is wrong");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, error, "bad error");
    for(unsigned i=1;i<VECT_LEN;i++){
        TEST_ASSERT_EQUAL_UINT64_MESSAGE(0, d[i], "output is not zero");
    }
}

void test_conv_uint64_error(){
    dsp_float_t f[VECT_LEN];
    uint64_t d[VECT_LEN];

    f[0] = -1.0;
    for(unsigned i=1;i<VECT_LEN;i++)
        f[i] = 0.0;
    int d_exp, error = 0;
    dsp_conv_vect_float_to_uint64 (f, d, &d_exp, VECT_LEN, &error);

    TEST_ASSERT_EQUAL_UINT32_MESSAGE(NEGATIVE_TO_UNSIGNED, error, "bad error");
}

