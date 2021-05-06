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

void test_bfp_clz_uint8(){
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(8, dsp_bfp_clz_uint8 ((uint8_t)0), "0");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(7, dsp_bfp_clz_uint8 ((uint8_t)1), "1");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_clz_uint8 ((uint8_t)UINT8_MAX), "UINT8_MAX");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, dsp_bfp_clz_uint8 ((uint8_t)UINT8_MAX>>1), "UINT8_MAX/2");
}

void test_bfp_clz_uint16(){
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(16, dsp_bfp_clz_uint16 ((uint16_t)0), "0");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(15, dsp_bfp_clz_uint16 ((uint16_t)1), "1");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_clz_uint16 ((uint16_t)UINT16_MAX), "UINT16_MAX");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, dsp_bfp_clz_uint16 ((uint16_t)UINT16_MAX>>1), "UINT16_MAX/2");
}

void test_bfp_clz_uint32(){
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(32, dsp_bfp_clz_uint32 ((uint32_t)0), "0");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(31, dsp_bfp_clz_uint32 ((uint16_t)1), "1");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_clz_uint32 ((uint32_t)UINT32_MAX), "UINT32_MAX");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, dsp_bfp_clz_uint32 ((uint32_t)UINT32_MAX>>1), "UINT32_MAX/2");
}

void test_bfp_clz_uint64(){
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(64, dsp_bfp_clz_uint64 ((uint64_t)0), "0");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(63, dsp_bfp_clz_uint64 ((uint64_t)1), "1");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_clz_uint64 ((uint64_t)UINT64_MAX), "UINT64_MAX");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, dsp_bfp_clz_uint64 ((uint64_t)UINT64_MAX>>1), "UINT64_MAX/2");
}

void test_bfp_cls_int8(){
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(7, dsp_bfp_cls_int8 ((int16_t)0), "0");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(6, dsp_bfp_cls_int8 ((int16_t)1), "1");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_int8 ((int16_t)INT8_MAX), "INT8_MAX");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, dsp_bfp_cls_int8 ((int16_t)INT8_MAX>>1), "INT8_MAX/2");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_int8 ((int16_t)INT8_MIN), "INT8_MIN");
}

void test_bfp_cls_int16(){
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(15, dsp_bfp_cls_int16 ((int16_t)0), "0");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(14, dsp_bfp_cls_int16 ((int16_t)1), "1");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_int16 ((int16_t)INT16_MAX), "INT16_MAX");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, dsp_bfp_cls_int16 ((int16_t)INT16_MAX>>1), "INT16_MAX/2");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_int16 ((int16_t)INT16_MIN), "INT16_MIN");
}

void test_bfp_cls_int32(){
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(31, dsp_bfp_cls_int32 ((int32_t)0), "0");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(30, dsp_bfp_cls_int32 ((int32_t)1), "1");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_int32 ((int32_t)INT32_MAX), "INT32_MAX");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, dsp_bfp_cls_int32 ((int32_t)INT32_MAX>>1), "INT32_MAX/2");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_int32 ((int32_t)INT32_MIN), "INT32_MIN");
}

void test_bfp_cls_int64(){
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(63, dsp_bfp_cls_int64 ((int64_t)0), "0");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(62, dsp_bfp_cls_int64 ((int64_t)1), "1");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_int64 ((int64_t)INT64_MAX), "INT64_MAX");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, dsp_bfp_cls_int64 ((int64_t)INT64_MAX>>1), "INT64_MAX/2");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_int64 ((int64_t)INT64_MIN), "INT64_MIN");
}

void test_bfp_cls_complex_int16(){
    dsp_complex_int16_t d;
    d.re = 0;
    d.im = 0;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(15, dsp_bfp_cls_complex_int16 (d), "0");
    d.re = 1;
    d.im = 1;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(14, dsp_bfp_cls_complex_int16 (d), "1");
    d.re = INT16_MAX;
    d.im = INT16_MAX;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_complex_int16 (d), "INT16_MAX");
    d.re = INT16_MAX/2;
    d.im = INT16_MAX/2;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, dsp_bfp_cls_complex_int16 (d), "INT16_MAX/2");
    d.re = INT16_MIN;
    d.im = INT16_MIN;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_complex_int16 (d), "INT16_MIN");
}

void test_bfp_cls_complex_int32(){
    dsp_complex_int32_t d;
    d.re = 0;
    d.im = 0;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(31, dsp_bfp_cls_complex_int32 (d), "0");
    d.re = 1;
    d.im = 1;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(30, dsp_bfp_cls_complex_int32 (d), "1");
    d.re = INT32_MAX;
    d.im = INT32_MAX;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_complex_int32 (d), "INT32_MAX");
    d.re = INT32_MAX/2;
    d.im = INT32_MAX/2;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, dsp_bfp_cls_complex_int32 (d), "INT32_MAX/2");
    d.re = INT32_MIN;
    d.im = INT32_MIN;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_complex_int32 (d), "INT32_MIN");
}


void test_bfp_cls_ch_pair_int16(){
    dsp_ch_pair_int16_t d;
    d.ch_a = 0;
    d.ch_b = 0;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(15, dsp_bfp_cls_ch_pair_int16 (d, 0), "0");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(15, dsp_bfp_cls_ch_pair_int16 (d, 1), "0");
    d.ch_a = 1;
    d.ch_b = 1;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(14, dsp_bfp_cls_ch_pair_int16 (d, 0), "1");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(14, dsp_bfp_cls_ch_pair_int16 (d, 1), "1");
    d.ch_a = INT16_MAX;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_ch_pair_int16 (d, 0), "INT16_MAX");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(14, dsp_bfp_cls_ch_pair_int16 (d, 1), "1");
    d.ch_b = INT16_MAX;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_ch_pair_int16 (d, 0), "INT16_MAX");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_ch_pair_int16 (d, 1), "INT16_MAX");
    d.ch_b = INT16_MAX/2;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_ch_pair_int16 (d, 0), "INT16_MAX");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, dsp_bfp_cls_ch_pair_int16 (d, 1), "INT16_MAX/2");
    d.ch_a = INT16_MAX/2;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, dsp_bfp_cls_ch_pair_int16 (d, 0), "INT16_MAX/2");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, dsp_bfp_cls_ch_pair_int16 (d, 1), "INT16_MAX/2");
    d.ch_a = INT16_MIN;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_ch_pair_int16 (d, 0), "INT16_MIN");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, dsp_bfp_cls_ch_pair_int16 (d, 1), "INT16_MAX/2");
    d.ch_b = INT16_MIN;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_ch_pair_int16 (d, 0), "INT16_MIN");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_ch_pair_int16 (d, 1), "INT16_MIN");
}

void test_bfp_cls_ch_pair_int32(){
    dsp_ch_pair_int32_t d;
    d.ch_a = 1;
    d.ch_b = 1;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(30, dsp_bfp_cls_ch_pair_int32 (d, 0), "1");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(30, dsp_bfp_cls_ch_pair_int32 (d, 1), "1");
    d.ch_a = 0;
    d.ch_b = 0;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(31, dsp_bfp_cls_ch_pair_int32 (d, 0), "0");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(31, dsp_bfp_cls_ch_pair_int32 (d, 1), "0");
    d.ch_a = INT32_MAX;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_ch_pair_int32 (d, 0), "INT32_MAX");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(31, dsp_bfp_cls_ch_pair_int32 (d, 1), "0");
    d.ch_b = INT32_MAX;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_ch_pair_int32 (d, 0), "INT32_MAX");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_ch_pair_int32 (d, 1), "INT32_MAX");
    d.ch_b = INT32_MAX/2;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_ch_pair_int32 (d, 0), "INT32_MAX");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, dsp_bfp_cls_ch_pair_int32 (d, 1), "INT32_MAX/2");
    d.ch_a = INT32_MAX/2;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, dsp_bfp_cls_ch_pair_int32 (d, 0), "INT32_MAX/2");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, dsp_bfp_cls_ch_pair_int32 (d, 1), "INT32_MAX/2");
    d.ch_a = INT32_MIN;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_ch_pair_int32 (d, 0), "INT32_MIN");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, dsp_bfp_cls_ch_pair_int32 (d, 1), "INT32_MAX/2");
    d.ch_b = INT32_MIN;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_ch_pair_int32 (d, 0), "INT32_MIN");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_ch_pair_int32 (d, 1), "INT32_MIN");
}

//Vectors

void test_bfp_clz_vect_uint8(){
    uint8_t d[VECT_LEN];
    for(unsigned i=0;i<VECT_LEN;i++) d[i] = 0;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(8, dsp_bfp_clz_vect_uint8 (d, VECT_LEN), "0");
    for(unsigned i=0;i<VECT_LEN;i++) d[i] = 1;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(7, dsp_bfp_clz_vect_uint8 (d, VECT_LEN), "1");
    for(unsigned i=0;i<VECT_LEN;i++) d[i] = UINT8_MAX;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_clz_vect_uint8 (d, VECT_LEN), "UINT8_MAX");
    for(unsigned i=0;i<VECT_LEN;i++) d[i] = UINT8_MAX/2;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, dsp_bfp_clz_vect_uint8 (d, VECT_LEN), "UINT8_MAX/2");
}

void test_bfp_clz_vect_uint16(){
    uint16_t d[VECT_LEN];
    for(unsigned i=0;i<VECT_LEN;i++) d[i] = 0;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(16, dsp_bfp_clz_vect_uint16 (d, VECT_LEN), "0");
    for(unsigned i=0;i<VECT_LEN;i++) d[i] = 1;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(15, dsp_bfp_clz_vect_uint16 (d, VECT_LEN), "1");
    for(unsigned i=0;i<VECT_LEN;i++) d[i] = UINT16_MAX;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_clz_vect_uint16 (d, VECT_LEN), "UINT16_MAX");
    for(unsigned i=0;i<VECT_LEN;i++) d[i] = UINT16_MAX/2;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, dsp_bfp_clz_vect_uint16 (d, VECT_LEN), "UINT16_MAX/2");
}

void test_bfp_clz_vect_uint32(){
    uint32_t d[VECT_LEN];
    for(unsigned i=0;i<VECT_LEN;i++) d[i] = 0;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(32, dsp_bfp_clz_vect_uint32 (d, VECT_LEN), "0");
    for(unsigned i=0;i<VECT_LEN;i++) d[i] = 1;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(31, dsp_bfp_clz_vect_uint32 (d, VECT_LEN), "1");
    for(unsigned i=0;i<VECT_LEN;i++) d[i] = UINT32_MAX;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_clz_vect_uint32 (d, VECT_LEN), "UINT32_MAX");
    for(unsigned i=0;i<VECT_LEN;i++) d[i] = UINT32_MAX/2;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, dsp_bfp_clz_vect_uint32 (d, VECT_LEN), "UINT32_MAX/2");
}

void test_bfp_clz_vect_uint64(){
    uint64_t d[VECT_LEN];
    for(unsigned i=0;i<VECT_LEN;i++) d[i] = 0;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(64, dsp_bfp_clz_vect_uint64 (d, VECT_LEN), "0");
    for(unsigned i=0;i<VECT_LEN;i++) d[i] = 1;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(63, dsp_bfp_clz_vect_uint64 (d, VECT_LEN), "1");
    for(unsigned i=0;i<VECT_LEN;i++) d[i] = UINT64_MAX;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_clz_vect_uint64 (d, VECT_LEN), "UINT64_MAX");
    for(unsigned i=0;i<VECT_LEN;i++) d[i] = UINT64_MAX/2;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, dsp_bfp_clz_vect_uint64 (d, VECT_LEN), "UINT64_MAX/2");
}

void test_bfp_cls_vect_int16(){
    int16_t d[VECT_LEN];
    for(unsigned i=0;i<VECT_LEN;i++) d[i] = 0;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(15, dsp_bfp_cls_vect_int16 (d, VECT_LEN), "0");
    for(unsigned i=0;i<VECT_LEN;i++) d[i] = 1;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(14, dsp_bfp_cls_vect_int16 (d, VECT_LEN), "1");
    for(unsigned i=0;i<VECT_LEN;i++) d[i] = INT16_MAX;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_vect_int16 (d, VECT_LEN), "INT32_MAX");
    for(unsigned i=0;i<VECT_LEN;i++) d[i] = INT16_MAX/2;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, dsp_bfp_cls_vect_int16 (d, VECT_LEN), "INT32_MAX/2");
    for(unsigned i=0;i<VECT_LEN;i++) d[i] = INT16_MIN;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_vect_int16 (d, VECT_LEN), "INT32_MIN");
}

void test_bfp_cls_vect_int32(){
    int32_t d[VECT_LEN];
    for(unsigned i=0;i<VECT_LEN;i++) d[i] = 0;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(31, dsp_bfp_cls_vect_int32 (d, VECT_LEN), "0");
    for(unsigned i=0;i<VECT_LEN;i++) d[i] = 1;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(30, dsp_bfp_cls_vect_int32 (d, VECT_LEN), "1");
    for(unsigned i=0;i<VECT_LEN;i++) d[i] = INT32_MAX;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_vect_int32 (d, VECT_LEN), "INT32_MAX");
    for(unsigned i=0;i<VECT_LEN;i++) d[i] = INT32_MAX/2;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, dsp_bfp_cls_vect_int32 (d, VECT_LEN), "INT32_MAX/2");
    for(unsigned i=0;i<VECT_LEN;i++) d[i] = INT32_MIN;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_vect_int32 (d, VECT_LEN), "INT32_MIN");
}

void test_bfp_cls_vect_complex_int16(){
    dsp_complex_int16_t d[VECT_LEN];
    for(unsigned i=0;i<VECT_LEN;i++) {
        d[i].re = 0;
        d[i].im = 0;
    }
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(15, dsp_bfp_cls_vect_complex_int16 (d, VECT_LEN), "0");
    for(unsigned i=0;i<VECT_LEN;i++) {
        d[i].re = 1;
        d[i].im = 1;
    }
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(14, dsp_bfp_cls_vect_complex_int16 (d, VECT_LEN), "1");
    for(unsigned i=0;i<VECT_LEN;i++) {
        d[i].re = INT16_MAX;
        d[i].im = INT16_MAX;
    }
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_vect_complex_int16 (d, VECT_LEN), "INT32_MAX");
    for(unsigned i=0;i<VECT_LEN;i++) {
        d[i].re = INT16_MAX/2;
        d[i].im = INT16_MAX/2;
    }
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, dsp_bfp_cls_vect_complex_int16 (d, VECT_LEN), "INT32_MAX/2");
    for(unsigned i=0;i<VECT_LEN;i++) {
        d[i].re = INT16_MIN;
        d[i].im = INT16_MIN;
    }
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_vect_complex_int16 (d, VECT_LEN), "INT32_MIN");

}

void test_bfp_cls_vect_complex_int32(){
    dsp_complex_int32_t d[VECT_LEN];
    for(unsigned i=0;i<VECT_LEN;i++) {
        d[i].re = 0;
        d[i].im = 0;
    }
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(31, dsp_bfp_cls_vect_complex_int32 (d, VECT_LEN), "0");
    for(unsigned i=0;i<VECT_LEN;i++) {
        d[i].re = 1;
        d[i].im = 1;
    }
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(30, dsp_bfp_cls_vect_complex_int32 (d, VECT_LEN), "1");
    for(unsigned i=0;i<VECT_LEN;i++) {
        d[i].re = INT32_MAX;
        d[i].im = INT32_MAX;
    }
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_vect_complex_int32 (d, VECT_LEN), "INT32_MAX");
    for(unsigned i=0;i<VECT_LEN;i++) {
        d[i].re = INT32_MAX/2;
        d[i].im = INT32_MAX/2;
    }
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, dsp_bfp_cls_vect_complex_int32 (d, VECT_LEN), "INT32_MAX/2");
    for(unsigned i=0;i<VECT_LEN;i++) {
        d[i].re = INT32_MIN;
        d[i].im = INT32_MIN;
    }
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_vect_complex_int32 (d, VECT_LEN), "INT32_MIN");
}

void test_bfp_cls_vect_ch_pair_int16(){
    dsp_ch_pair_int16_t d[VECT_LEN];
    for(unsigned i=0;i<VECT_LEN;i++) {
        d[i].ch_a = 1;
        d[i].ch_b = 1;
    }
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(14, dsp_bfp_cls_vect_ch_pair_int16 (d, VECT_LEN, 0), "1");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(14, dsp_bfp_cls_vect_ch_pair_int16 (d, VECT_LEN, 1), "1");
    for(unsigned i=0;i<VECT_LEN;i++) {
        d[i].ch_a = 0;
        d[i].ch_b = 0;
    }
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(15, dsp_bfp_cls_vect_ch_pair_int16 (d, VECT_LEN, 0), "0");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(15, dsp_bfp_cls_vect_ch_pair_int16 (d, VECT_LEN, 1), "0");
    for(unsigned i=0;i<VECT_LEN;i++)
        d[i].ch_a = INT16_MAX;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_vect_ch_pair_int16 (d, VECT_LEN, 0), "INT16_MAX");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(15, dsp_bfp_cls_vect_ch_pair_int16 (d, VECT_LEN, 1), "0");
    for(unsigned i=0;i<VECT_LEN;i++)
        d[i].ch_b = INT16_MAX;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_vect_ch_pair_int16 (d, VECT_LEN, 0), "INT16_MAX");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_vect_ch_pair_int16 (d, VECT_LEN, 1), "INT16_MAX");
    for(unsigned i=0;i<VECT_LEN;i++)
        d[i].ch_b = INT16_MAX/2;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_vect_ch_pair_int16 (d, VECT_LEN, 0), "INT16_MAX");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, dsp_bfp_cls_vect_ch_pair_int16 (d, VECT_LEN, 1), "INT16_MAX/2");
    for(unsigned i=0;i<VECT_LEN;i++)
        d[i].ch_a = INT16_MAX/2;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, dsp_bfp_cls_vect_ch_pair_int16 (d, VECT_LEN, 0), "INT16_MAX/2");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, dsp_bfp_cls_vect_ch_pair_int16 (d, VECT_LEN, 1), "INT16_MAX/2");
    for(unsigned i=0;i<VECT_LEN;i++)
        d[i].ch_a = INT16_MIN;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_vect_ch_pair_int16 (d, VECT_LEN, 0), "INT16_MIN");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, dsp_bfp_cls_vect_ch_pair_int16 (d, VECT_LEN, 1), "INT32_MAX/2");
    for(unsigned i=0;i<VECT_LEN;i++)
        d[i].ch_b = INT16_MIN;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_vect_ch_pair_int16 (d, VECT_LEN, 0), "INT16_MIN");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_vect_ch_pair_int16 (d, VECT_LEN, 1), "INT16_MIN");

}

void test_bfp_cls_vect_ch_pair_int32(){
    dsp_ch_pair_int32_t d[VECT_LEN];
    for(unsigned i=0;i<VECT_LEN;i++) {
        d[i].ch_a = 1;
        d[i].ch_b = 1;
    }
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(30, dsp_bfp_cls_vect_ch_pair_int32 (d, VECT_LEN, 0), "1");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(30, dsp_bfp_cls_vect_ch_pair_int32 (d, VECT_LEN, 1), "1");
    for(unsigned i=0;i<VECT_LEN;i++) {
        d[i].ch_a = 0;
        d[i].ch_b = 0;
    }
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(31, dsp_bfp_cls_vect_ch_pair_int32 (d, VECT_LEN, 0), "0");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(31, dsp_bfp_cls_vect_ch_pair_int32 (d, VECT_LEN, 1), "0");
    for(unsigned i=0;i<VECT_LEN;i++)
        d[i].ch_a = INT32_MAX;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_vect_ch_pair_int32 (d, VECT_LEN, 0), "INT32_MAX");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(31, dsp_bfp_cls_vect_ch_pair_int32 (d, VECT_LEN, 1), "0");
    for(unsigned i=0;i<VECT_LEN;i++)
        d[i].ch_b = INT32_MIN;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_vect_ch_pair_int32 (d, VECT_LEN, 0), "INT32_MAX");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_vect_ch_pair_int32 (d, VECT_LEN, 1), "INT32_MAX");
    for(unsigned i=0;i<VECT_LEN;i++)
        d[i].ch_b = INT32_MAX/2;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_vect_ch_pair_int32 (d, VECT_LEN, 0), "INT32_MAX");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, dsp_bfp_cls_vect_ch_pair_int32 (d, VECT_LEN, 1), "INT32_MAX/2");
    for(unsigned i=0;i<VECT_LEN;i++)
        d[i].ch_a = INT32_MAX/2;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, dsp_bfp_cls_vect_ch_pair_int32 (d, VECT_LEN, 0), "INT32_MAX/2");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, dsp_bfp_cls_vect_ch_pair_int32 (d, VECT_LEN, 1), "INT32_MAX/2");
    for(unsigned i=0;i<VECT_LEN;i++)
        d[i].ch_a = INT32_MIN;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_vect_ch_pair_int32 (d, VECT_LEN, 0), "INT32_MIN");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, dsp_bfp_cls_vect_ch_pair_int32 (d, VECT_LEN, 1), "INT32_MAX/2");
    for(unsigned i=0;i<VECT_LEN;i++)
        d[i].ch_b = INT32_MIN;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_vect_ch_pair_int32 (d, VECT_LEN, 0), "INT32_MIN");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, dsp_bfp_cls_vect_ch_pair_int32 (d, VECT_LEN, 1), "INT32_MIN");

}

void test_bfp_shl_vect_uint8(){
    uint8_t d[VECT_LEN];

    for(unsigned i=0;i<VECT_LEN;i++)
        d[i] = i;
    dsp_bfp_shl_vect_uint8(d, VECT_LEN, 0);
    for(unsigned i=0;i<VECT_LEN;i++)
        TEST_ASSERT_EQUAL_UINT8_MESSAGE(i, d[i], "0");


    dsp_bfp_shl_vect_uint8(d, VECT_LEN, 1);
    for(unsigned i=0;i<VECT_LEN;i++)
        TEST_ASSERT_EQUAL_UINT8_MESSAGE(i<<1, d[i], "1");


    dsp_bfp_shl_vect_uint8(d, VECT_LEN, -1);
    for(unsigned i=0;i<VECT_LEN;i++)
        TEST_ASSERT_EQUAL_UINT8_MESSAGE(i, d[i], "-1");
}

void test_bfp_shl_vect_uint16(){
    uint16_t d[VECT_LEN];

    for(unsigned i=0;i<VECT_LEN;i++)
        d[i] = i;
    dsp_bfp_shl_vect_uint16(d, VECT_LEN, 0);
    for(unsigned i=0;i<VECT_LEN;i++)
        TEST_ASSERT_EQUAL_UINT16_MESSAGE(i, d[i], "0");


    dsp_bfp_shl_vect_uint16(d, VECT_LEN, 1);
    for(unsigned i=0;i<VECT_LEN;i++)
        TEST_ASSERT_EQUAL_UINT16_MESSAGE(i<<1, d[i], "1");


    dsp_bfp_shl_vect_uint16(d, VECT_LEN, -1);
    for(unsigned i=0;i<VECT_LEN;i++)
        TEST_ASSERT_EQUAL_UINT16_MESSAGE(i, d[i], "-1");
}

void test_bfp_shl_vect_int16(){
    int16_t d[VECT_LEN];

    for(unsigned i=0;i<VECT_LEN;i++)
        d[i] = i;
    dsp_bfp_shl_vect_int16(d, VECT_LEN, 0);
    for(unsigned i=0;i<VECT_LEN;i++)
        TEST_ASSERT_EQUAL_INT16_MESSAGE(i, d[i], "0");


    dsp_bfp_shl_vect_int16(d, VECT_LEN, 1);
    for(unsigned i=0;i<VECT_LEN;i++)
        TEST_ASSERT_EQUAL_INT16_MESSAGE(i<<1, d[i], "1");


    dsp_bfp_shl_vect_int16(d, VECT_LEN, -1);
    for(unsigned i=0;i<VECT_LEN;i++)
        TEST_ASSERT_EQUAL_INT16_MESSAGE(i, d[i], "-1");
}


void test_bfp_shl_vect_uint32(){
    uint32_t d[VECT_LEN];

    for(unsigned i=0;i<VECT_LEN;i++)
        d[i] = i;
    dsp_bfp_shl_vect_uint32(d, VECT_LEN, 0);
    for(unsigned i=0;i<VECT_LEN;i++)
        TEST_ASSERT_EQUAL_UINT32_MESSAGE(i, d[i], "0");


    dsp_bfp_shl_vect_uint32(d, VECT_LEN, 1);
    for(unsigned i=0;i<VECT_LEN;i++)
        TEST_ASSERT_EQUAL_UINT32_MESSAGE(i<<1, d[i], "1");


    dsp_bfp_shl_vect_uint32(d, VECT_LEN, -1);
    for(unsigned i=0;i<VECT_LEN;i++)
        TEST_ASSERT_EQUAL_UINT32_MESSAGE(i, d[i], "-1");
}

void test_bfp_shl_vect_int32(){
    int32_t d[VECT_LEN];

    for(unsigned i=0;i<VECT_LEN;i++)
        d[i] = i;
    dsp_bfp_shl_vect_int32(d, VECT_LEN, 0);
    for(unsigned i=0;i<VECT_LEN;i++)
        TEST_ASSERT_EQUAL_INT32_MESSAGE(i, d[i], "0");


    dsp_bfp_shl_vect_int32(d, VECT_LEN, 1);
    for(unsigned i=0;i<VECT_LEN;i++)
        TEST_ASSERT_EQUAL_INT32_MESSAGE(i<<1, d[i], "1");


    dsp_bfp_shl_vect_int32(d, VECT_LEN, -1);
    for(unsigned i=0;i<VECT_LEN;i++)
        TEST_ASSERT_EQUAL_INT32_MESSAGE(i, d[i], "-1");
}

void test_bfp_shl_vect_complex_int16(){
    dsp_complex_int16_t d[VECT_LEN];

    for(unsigned i=0;i<VECT_LEN;i++){
        d[i].re = i;
        d[i].im = i+VECT_LEN;
    }
    dsp_bfp_shl_vect_complex_int16(d, VECT_LEN, 0);
    for(unsigned i=0;i<VECT_LEN;i++){
        TEST_ASSERT_EQUAL_INT32_MESSAGE(i, d[i].re, "0");
        TEST_ASSERT_EQUAL_INT32_MESSAGE(i+VECT_LEN, d[i].im, "0");
    }

    dsp_bfp_shl_vect_complex_int16(d, VECT_LEN, 1);
    for(unsigned i=0;i<VECT_LEN;i++){
        TEST_ASSERT_EQUAL_INT32_MESSAGE(i<<1, d[i].re, "1");
        TEST_ASSERT_EQUAL_INT32_MESSAGE((i+VECT_LEN)<<1, d[i].im, "1");
    }

    dsp_bfp_shl_vect_complex_int16(d, VECT_LEN, -1);
    for(unsigned i=0;i<VECT_LEN;i++){
        TEST_ASSERT_EQUAL_INT32_MESSAGE(i, d[i].re, "-1");
        TEST_ASSERT_EQUAL_INT32_MESSAGE(i+VECT_LEN, d[i].im, "-1");
    }
}

void test_bfp_shl_vect_complex_int32(){
    dsp_complex_int32_t d[VECT_LEN];

    for(unsigned i=0;i<VECT_LEN;i++){
        d[i].re = i;
        d[i].im = i+VECT_LEN;
    }
    dsp_bfp_shl_vect_complex_int32(d, VECT_LEN, 0);
    for(unsigned i=0;i<VECT_LEN;i++){
        TEST_ASSERT_EQUAL_INT32_MESSAGE(i, d[i].re, "0");
        TEST_ASSERT_EQUAL_INT32_MESSAGE(i+VECT_LEN, d[i].im, "0");
    }

    dsp_bfp_shl_vect_complex_int32(d, VECT_LEN, 1);
    for(unsigned i=0;i<VECT_LEN;i++){
        TEST_ASSERT_EQUAL_INT32_MESSAGE(i<<1, d[i].re, "1");
        TEST_ASSERT_EQUAL_INT32_MESSAGE((i+VECT_LEN)<<1, d[i].im, "1");
    }

    dsp_bfp_shl_vect_complex_int32(d, VECT_LEN, -1);
    for(unsigned i=0;i<VECT_LEN;i++){
        TEST_ASSERT_EQUAL_INT32_MESSAGE(i, d[i].re, "-1");
        TEST_ASSERT_EQUAL_INT32_MESSAGE(i+VECT_LEN, d[i].im, "-1");
    }
}

void test_bfp_shl_vect_ch_pair_int16(){
    dsp_ch_pair_int16_t d[VECT_LEN];

    for(unsigned i=0;i<VECT_LEN;i++){
        d[i].ch_a = i;
        d[i].ch_b = i+VECT_LEN;
    }
    dsp_bfp_shl_vect_ch_pair_int16(d, VECT_LEN, 0, 0);
    for(unsigned i=0;i<VECT_LEN;i++){
        TEST_ASSERT_EQUAL_INT16_MESSAGE(i, d[i].ch_a, "0");
        TEST_ASSERT_EQUAL_INT16_MESSAGE(i+VECT_LEN, d[i].ch_b, "0");
    }

    dsp_bfp_shl_vect_ch_pair_int16(d, VECT_LEN, 1, 2);
    for(unsigned i=0;i<VECT_LEN;i++){
        TEST_ASSERT_EQUAL_INT16_MESSAGE(i<<1, d[i].ch_a, "1");
        TEST_ASSERT_EQUAL_INT16_MESSAGE((i+VECT_LEN)<<2, d[i].ch_b, "1");
    }

    dsp_bfp_shl_vect_ch_pair_int16(d, VECT_LEN, -1, -2);
    for(unsigned i=0;i<VECT_LEN;i++){
        TEST_ASSERT_EQUAL_INT16_MESSAGE(i, d[i].ch_a, "-1");
        TEST_ASSERT_EQUAL_INT16_MESSAGE(i+VECT_LEN, d[i].ch_b, "-1");
    }
}

void test_bfp_slh_vect_ch_pair_int32(){
    dsp_ch_pair_int32_t d[VECT_LEN];

    for(unsigned i=0;i<VECT_LEN;i++){
        d[i].ch_a = i;
        d[i].ch_b = i+VECT_LEN;
    }
    dsp_bfp_shl_vect_ch_pair_int32(d, VECT_LEN, 0, 0);
    for(unsigned i=0;i<VECT_LEN;i++){
        TEST_ASSERT_EQUAL_INT32_MESSAGE(i, d[i].ch_a, "0");
        TEST_ASSERT_EQUAL_INT32_MESSAGE(i+VECT_LEN, d[i].ch_b, "0");
    }

    dsp_bfp_shl_vect_ch_pair_int32(d, VECT_LEN, 1, 2);
    for(unsigned i=0;i<VECT_LEN;i++){
        TEST_ASSERT_EQUAL_INT32_MESSAGE(i<<1, d[i].ch_a, "1");
        TEST_ASSERT_EQUAL_INT32_MESSAGE((i+VECT_LEN)<<2, d[i].ch_b, "1");
    }

    dsp_bfp_shl_vect_ch_pair_int32(d, VECT_LEN, -1, -2);
    for(unsigned i=0;i<VECT_LEN;i++){
        TEST_ASSERT_EQUAL_INT32_MESSAGE(i, d[i].ch_a, "-1");
        TEST_ASSERT_EQUAL_INT32_MESSAGE(i+VECT_LEN, d[i].ch_b, "-1");
    }

}
