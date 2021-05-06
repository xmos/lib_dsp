// Copyright 2019-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include "unity.h"

#include <xs1.h>
#include <string.h>
#include <xclib.h>

#include <dsp.h>
#include <dsp_fp.h>
#include <dsp_testing.h>
#include <math.h>
#include <limits.h>

static signed sext(unsigned a, unsigned b){
    asm("sext %0, %1": "=r"(a): "r"(b));
    return a;
}

dsp_s32_float_t random_float_s32(unsigned *r){
    dsp_s32_float_t s;
    s.e = sext(dsp_pseudo_rand_int32(r), 4) - 31;
    s.m = dsp_pseudo_rand_int32(r);
    return s;
}

dsp_u32_float_t random_float_u32(unsigned *r){
    dsp_u32_float_t s;
    s.e = sext(dsp_pseudo_rand_int32(r), 4) - 32;
    s.m = dsp_pseudo_rand_uint32(r);
    return s;
}

dsp_s64_float_t random_float_s64(unsigned *r){
    dsp_s64_float_t s;
    s.e = sext(dsp_pseudo_rand_int32(r), 4) - 31;
    s.m = dsp_pseudo_rand_int64(r);
    return s;
}

dsp_u64_float_t random_float_u64(unsigned *r){
    dsp_u64_float_t s;
    s.e = sext(dsp_pseudo_rand_int32(r), 4) - 32;
    s.m = dsp_pseudo_rand_uint64(r);
    return s;
}

double u32_to_double(dsp_u32_float_t r){
    return ldexp(r.m, r.e);
}

double s32_to_double(dsp_s32_float_t r){
    return ldexp(r.m, r.e);
}

double u64_to_double(dsp_u64_float_t r){
    return ldexp(r.m, r.e);
}

double s64_to_double(dsp_s64_float_t r){
    return ldexp(r.m, r.e);
}

#define TEST_COUNT (1<<14)

void test_normalise_s32(){
    unsigned rand_seed = 2;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_s32_float_t a = random_float_s32(&rand_seed);
        double Before = s32_to_double(a);
        dsp_normalise_s32(&a);
        double After = s32_to_double(a);

        TEST_ASSERT_EQUAL_DOUBLE(Before, After);
    }
}

void test_normalise_s32_zero(){
  unsigned rand_seed = 3;

  for(unsigned i=0;i<TEST_COUNT;i++){
      dsp_s32_float_t a = random_float_s32(&rand_seed);
      a.m = 0;
      double Before = s32_to_double(a);

      dsp_normalise_s32(&a);
      TEST_ASSERT_EQUAL_INT(a.e, DSP_FLOAT_ZERO_EXP);

      double After = s32_to_double(a);
      TEST_ASSERT_EQUAL_DOUBLE(Before, After);
  }
}

void test_normalise_u32(){
    unsigned rand_seed = 4;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_u32_float_t a = random_float_u32(&rand_seed);
        double Before = u32_to_double(a);
        dsp_normalise_u32(&a);
        double After = u32_to_double(a);

        TEST_ASSERT_EQUAL_DOUBLE(Before, After);
    }
}

void test_normalise_u32_zero(){
    unsigned rand_seed = 5;

    for(unsigned i=0;i<TEST_COUNT;i++){
        dsp_u32_float_t a = random_float_u32(&rand_seed);
        a.m = 0;
        double Before = u32_to_double(a);

        dsp_normalise_u32(&a);
        TEST_ASSERT_EQUAL_INT(a.e, DSP_FLOAT_ZERO_EXP);

        double After = u32_to_double(a);
        TEST_ASSERT_EQUAL_DOUBLE(Before, After);
    }
}

void test_normalise_s64(){
    unsigned rand_seed = 2;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_s64_float_t a = random_float_s64(&rand_seed);
        double Before = s64_to_double(a);
        dsp_normalise_s64(&a);
        double After = s64_to_double(a);

        TEST_ASSERT_EQUAL_DOUBLE(Before, After);
    }
}

void test_normalise_u64(){
    unsigned rand_seed = 6;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_u64_float_t a = random_float_u64(&rand_seed);
        double Before = u64_to_double(a);
        dsp_normalise_u64(&a);
        double After = u64_to_double(a);

        TEST_ASSERT_EQUAL_DOUBLE(Before, After);
    }
}

void test_u32_to_s32(){
    unsigned rand_seed = 7;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_u32_float_t a = random_float_u32(&rand_seed);
        dsp_normalise_u32(&a);

        double Before = u32_to_double(a);

        dsp_s32_float_t b = dsp_u32_to_s32(a);

        double After = s32_to_double(b);
        int error = 0;
        uint32_t before = dsp_conv_float_to_uint32(Before, a.e, &error);//FIXME what does this mean?
        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

        uint32_t after = dsp_conv_float_to_uint32(After, a.e, &error);
        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

        TEST_ASSERT_UINT32_WITHIN_MESSAGE(1, before, after, "Output delta is too large");
        TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_s32(&b), "Output is not properly normalised");
    }
}

void test_u64_to_u32(){
    unsigned rand_seed = 8;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_u64_float_t a = random_float_u64(&rand_seed);
        dsp_normalise_u64(&a);

        double Before = u64_to_double(a);
        dsp_u32_float_t b = dsp_u64_to_u32(a);
        double After = u32_to_double(b);

        TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_u32(&b), "Output is not properly normalised");

        int error = 0;
        uint32_t va = dsp_conv_float_to_uint32(After, b.e, &error);
        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
        uint32_t vb = dsp_conv_float_to_uint32(Before, b.e, &error);
        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

        TEST_ASSERT_EQUAL_UINT32(va, vb);
    }
}

void test_s64_to_s32(){
    unsigned rand_seed = 9;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_s64_float_t a = random_float_s64(&rand_seed);
        dsp_normalise_s64(&a);
        double Before = s64_to_double(a);
        dsp_s32_float_t b = dsp_s64_to_s32(a);
        double After = s32_to_double(b);

        int error = 0;
        int32_t va = dsp_conv_float_to_int32(After, b.e, &error);
        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
        int32_t vb = dsp_conv_float_to_int32(Before, b.e, &error);
        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");


        TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_s32(&b), "Output is not properly normalised");
        TEST_ASSERT_UINT32_WITHIN_MESSAGE(1, va, vb, "Output delta is too large");
    }
}


void test_denormalise_and_saturate_u32(){
    unsigned rand_seed = 10;

    for(unsigned i=0;i<TEST_COUNT;i++){

        int exponent = sext(dsp_pseudo_rand_int32(&rand_seed), 4);

        int error = 0;
        double T = dsp_conv_uint32_to_float(UINT_MAX, exponent, &error);
        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
        uint32_t r = dsp_pseudo_rand_uint32(&rand_seed);
        dsp_u32_float_t a = {r, exponent + 1};
        dsp_normalise_u32(&a);
        double A = u32_to_double(a);

        int32_t denorm = dsp_denormalise_and_saturate_u32(a, exponent);

        double expected;
        if(A > T){
            expected = T;
        } else {
            expected = A;
        }

        int32_t res = dsp_conv_float_to_uint32(expected, exponent, &error);
        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
        TEST_ASSERT_UINT32_WITHIN_MESSAGE(1, res, denorm, "Output delta is too large");

    }
}

void test_denormalise_and_saturate_s32(){
    unsigned rand_seed = 11;

    for(unsigned i=0;i<TEST_COUNT;i++){

        int exponent = sext(dsp_pseudo_rand_int32(&rand_seed), 4);

        int error = 0;
        double T = dsp_conv_int32_to_float(INT_MAX, exponent, &error);
        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
        int32_t r = dsp_pseudo_rand_int32(&rand_seed);
        dsp_s32_float_t a = {r, exponent + 1};
        dsp_normalise_s32(&a);
        double A = s32_to_double(a);

        int32_t denorm = dsp_denormalise_and_saturate_s32(a, exponent);

        double expected;
        if(A > T){
            expected = T;
        } else if(A < -T){
            expected = -T;
        } else {
            expected = A;
        }

        int32_t res = dsp_conv_float_to_int32(expected, exponent, &error);

        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
        TEST_ASSERT_INT32_WITHIN_MESSAGE(1, res, denorm, "Output delta is too large");

    }
}




void test_abs_s32_to_s32(){
    unsigned rand_seed = 12;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_s32_float_t a = random_float_s32(&rand_seed);
        dsp_normalise_s32(&a);

        double A = s32_to_double(a);

        a = dsp_abs_s32_to_s32(a);

        A = fabs(A);

        int error = 0;
        int32_t v = dsp_conv_float_to_int32(A, a.e, &error);

        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

        TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_s32(&a), "Output is not properly normalised");
        TEST_ASSERT_UINT32_WITHIN_MESSAGE(1, v, a.m, "Output delta is too large");
    }
}

void test_abs_s32_to_s32_zero(){
    unsigned rand_seed = 13;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_s32_float_t a = random_float_s32(&rand_seed);
        a.m = 0;
        dsp_normalise_s32(&a);

        double A = s32_to_double(a);

        a = dsp_abs_s32_to_s32(a);

        A = fabs(A);

        int error = 0;
        int32_t v = dsp_conv_float_to_int32(A, a.e, &error);

        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

        TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_s32(&a), "Output is not properly normalised");
        TEST_ASSERT_UINT32_WITHIN_MESSAGE(1, v, a.m, "Output delta is too large");
    }
}

void test_abs_s32_to_u32(){
    unsigned rand_seed = 14;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_s32_float_t a = random_float_s32(&rand_seed);
        dsp_normalise_s32(&a);

        double A = s32_to_double(a);

        dsp_u32_float_t b = dsp_abs_s32_to_u32(a);

        double abs_A = fabs(A);

        int error = 0;
        uint32_t v = dsp_conv_float_to_uint32(abs_A, b.e, &error);

        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

        TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_u32(&b), "Output is not properly normalised");
        TEST_ASSERT_UINT32_WITHIN_MESSAGE(1, v, b.m, "Output delta is too large");
    }
}

void test_abs_s32_to_u32_zero(){
    unsigned rand_seed = 15;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_s32_float_t a = random_float_s32(&rand_seed);
        a.m = 0;
        dsp_normalise_s32(&a);

        double A = s32_to_double(a);

        dsp_u32_float_t b = dsp_abs_s32_to_u32(a);

        double abs_A = fabs(A);

        int error = 0;
        uint32_t v = dsp_conv_float_to_int32(abs_A, b.e, &error);

        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
        TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_u32(&b), "Output is not properly normalised");
        TEST_ASSERT_UINT32_WITHIN_MESSAGE(1, v, b.m, "Output delta is too large");
    }
}

void test_neg_s32(){
    unsigned rand_seed = 16;

    for(unsigned i=0;i<TEST_COUNT;i++){
        dsp_s32_float_t a = random_float_s32(&rand_seed);
        dsp_normalise_s32(&a);

        double A = s32_to_double(a);

        a = dsp_neg_s32(a);

        A = -A;

        TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_s32(&a), "Output is not properly normalised");
        int error = 0;
        int32_t v = dsp_conv_float_to_int32(A, a.e, &error);
        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
        TEST_ASSERT_UINT32_WITHIN_MESSAGE(1, v, a.m, "Output delta is too large");
    }
}

void test_neg_s32_zero(){
    unsigned rand_seed = 17;

    for(unsigned i=0;i<TEST_COUNT;i++){
        dsp_s32_float_t a = random_float_s32(&rand_seed);
        a.m = 0;
        dsp_normalise_s32(&a);

        double A = s32_to_double(a);

        a = dsp_neg_s32(a);

        A = -A;

        TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_s32(&a), "Output is not properly normalised");
        int error = 0;
        int32_t v = dsp_conv_float_to_int32(A, a.e, &error);
        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
        TEST_ASSERT_UINT32_WITHIN_MESSAGE(1, v, a.m, "Output delta is too large");
    }
}

void test_neg_s32_INT_MIN(){
    unsigned rand_seed = 18;

    dsp_s32_float_t a = random_float_s32(&rand_seed);
    a.m = INT_MIN;


    dsp_s32_float_t result = dsp_neg_s32(a);
    TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_s32(&result), "Output is not properly normalised");

    TEST_ASSERT_EQUAL_MESSAGE(INT_MAX, result.m, "Output delta is too large");
}

void test_gte_u32_u32(){
    unsigned rand_seed = 19;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_u32_float_t a = random_float_u32(&rand_seed);
        dsp_u32_float_t b = random_float_u32(&rand_seed);

        dsp_normalise_u32(&a);
        dsp_normalise_u32(&b);

        double A = u32_to_double(a);
        double B = u32_to_double(b);

        TEST_ASSERT_EQUAL_INT(dsp_gte_u32_u32(a, b), (A>=B));

    }
}

void test_gte_u32_u32_zero(){
    unsigned rand_seed = 20;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_u32_float_t a = random_float_u32(&rand_seed);
        a.m = 0;
        dsp_u32_float_t b = random_float_u32(&rand_seed);

        dsp_normalise_u32(&a);
        dsp_normalise_u32(&b);

        double A = u32_to_double(a);
        double B = u32_to_double(b);

        TEST_ASSERT_EQUAL_INT(dsp_gte_u32_u32(a, b), (A>=B));

    }

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_u32_float_t a = random_float_u32(&rand_seed);
        dsp_u32_float_t b = random_float_u32(&rand_seed);
        b.m = 0;

        dsp_normalise_u32(&a);
        dsp_normalise_u32(&b);

        double A = u32_to_double(a);
        double B = u32_to_double(b);

        TEST_ASSERT_EQUAL_INT(dsp_gte_u32_u32(a, b), (A>=B));

    }
}

void test_gte_s32_s32(){
    unsigned rand_seed = 21;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_s32_float_t a = random_float_s32(&rand_seed);
        dsp_s32_float_t b = random_float_s32(&rand_seed);

        dsp_normalise_s32(&a);
        dsp_normalise_s32(&b);

        double A = s32_to_double(a);
        double B = s32_to_double(b);
        TEST_ASSERT_EQUAL_INT(dsp_gte_s32_s32(a, b), (A>=B));
    }
}

void test_gte_s32_s32_zero(){
    unsigned rand_seed = 22;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_s32_float_t a = random_float_s32(&rand_seed);
        a.m = 0;
        dsp_s32_float_t b = random_float_s32(&rand_seed);

        dsp_normalise_s32(&a);
        dsp_normalise_s32(&b);

        double A = s32_to_double(a);
        double B = s32_to_double(b);
        TEST_ASSERT_EQUAL_INT(dsp_gte_s32_s32(a, b), (A>=B));
    }

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_s32_float_t a = random_float_s32(&rand_seed);
        dsp_s32_float_t b = random_float_s32(&rand_seed);
        b.m = 0;
        dsp_normalise_s32(&a);
        dsp_normalise_s32(&b);

        double A = s32_to_double(a);
        double B = s32_to_double(b);
        TEST_ASSERT_EQUAL_INT(dsp_gte_s32_s32(a, b), (A>=B));
    }
}

void test_eq_u32_u32(){
    unsigned rand_seed = 23;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_u32_float_t a = random_float_u32(&rand_seed);
        dsp_u32_float_t b = random_float_u32(&rand_seed);

        dsp_normalise_u32(&a);
        dsp_normalise_u32(&b);

        double A = u32_to_double(a);
        double B = u32_to_double(b);
        TEST_ASSERT_EQUAL_INT(dsp_eq_u32_u32(a, b), (A==B));
    }
}

void test_eq_u32_u32_zero(){
    unsigned rand_seed = 24;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_u32_float_t a = random_float_u32(&rand_seed);
        a.m = 0;
        dsp_u32_float_t b = random_float_u32(&rand_seed);

        dsp_normalise_u32(&a);
        dsp_normalise_u32(&b);

        double A = u32_to_double(a);
        double B = u32_to_double(b);
        TEST_ASSERT_EQUAL_INT(dsp_eq_u32_u32(a, b), (A==B));
    }
    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_u32_float_t a = random_float_u32(&rand_seed);
        dsp_u32_float_t b = random_float_u32(&rand_seed);
        b.m = 0;

        dsp_normalise_u32(&a);
        dsp_normalise_u32(&b);

        double A = u32_to_double(a);
        double B = u32_to_double(b);
        TEST_ASSERT_EQUAL_INT(dsp_eq_u32_u32(a, b), (A==B));
    }
}

void test_eq_s32_s32(){
    unsigned rand_seed = 25;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_s32_float_t a = random_float_s32(&rand_seed);
        dsp_s32_float_t b = random_float_s32(&rand_seed);

        dsp_normalise_s32(&a);
        dsp_normalise_s32(&b);

        double A = s32_to_double(a);
        double B = s32_to_double(b);

        TEST_ASSERT_EQUAL_INT(dsp_eq_s32_s32(a, b), (A==B));
    }
}

void test_eq_s32_s32_zero(){
    unsigned rand_seed = 26;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_s32_float_t a = random_float_s32(&rand_seed);
        a.m = 0;
        dsp_s32_float_t b = random_float_s32(&rand_seed);

        dsp_normalise_s32(&a);
        dsp_normalise_s32(&b);

        double A = s32_to_double(a);
        double B = s32_to_double(b);

        TEST_ASSERT_EQUAL_INT(dsp_eq_s32_s32(a, b), (A==B));
    }
    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_s32_float_t a = random_float_s32(&rand_seed);
        dsp_s32_float_t b = random_float_s32(&rand_seed);
        b.m = 0;

        dsp_normalise_s32(&a);
        dsp_normalise_s32(&b);

        double A = s32_to_double(a);
        double B = s32_to_double(b);

        TEST_ASSERT_EQUAL_INT(dsp_eq_s32_s32(a, b), (A==B));
    }
}

void test_add_s32_s32(){
    unsigned rand_seed = 27;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_s32_float_t a = random_float_s32(&rand_seed);
        dsp_s32_float_t b = random_float_s32(&rand_seed);

        dsp_normalise_s32(&a);
        dsp_normalise_s32(&b);

        double A = s32_to_double(a);
        double B = s32_to_double(b);

        double R = A + B;

        dsp_s32_float_t r = dsp_add_s32_s32(a, b);

        TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_s32(&r), "Output is not properly normalised");

        int error = 0;
        int32_t v = dsp_conv_float_to_int32(R, r.e, &error);
        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
        unsigned tailroom = clz(bitrev(v|r.m));

        TEST_ASSERT_INT32_WITHIN_MESSAGE(16<<tailroom, v, r.m, "Output delta is too large");
    }
}

void test_add_s32_s32_zero(){
  unsigned rand_seed = 28;

  for(unsigned i=0;i<TEST_COUNT;i++){

      dsp_s32_float_t a = random_float_s32(&rand_seed);
      dsp_s32_float_t b = {0, 0};

      dsp_normalise_s32(&a);
      dsp_normalise_s32(&b);

      dsp_s32_float_t result = dsp_add_s32_s32(a, b);

      TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_s32(&result), "Output is not properly normalised");
      TEST_ASSERT_EQUAL_INT_MESSAGE(a.e, result.e, "Incorrect exponents");
      TEST_ASSERT_EQUAL_INT_MESSAGE(a.m, result.m, "Output delta is too large");
  }
  for(unsigned i=0;i<TEST_COUNT;i++){

      dsp_s32_float_t a = {0, 0};
      dsp_s32_float_t b = random_float_s32(&rand_seed);

      dsp_normalise_s32(&a);
      dsp_normalise_s32(&b);

      dsp_s32_float_t result = dsp_add_s32_s32(a, b);

      TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_s32(&result), "Output is not properly normalised");
      TEST_ASSERT_EQUAL_INT_MESSAGE(b.e, result.e, "Incorrect exponents");
      TEST_ASSERT_EQUAL_INT_MESSAGE(b.m, result.m, "Output delta is too large");
  }
}

void test_add_u32_u32(){
    unsigned rand_seed = 29;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_u32_float_t a = random_float_u32(&rand_seed);
        dsp_u32_float_t b = random_float_u32(&rand_seed);

        double R = u32_to_double(a) + u32_to_double(b);

        dsp_normalise_u32(&a);
        dsp_normalise_u32(&b);

        dsp_u32_float_t r = dsp_add_u32_u32(a, b);

        TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_u32(&r), "Output is not properly normalised");

        int error = 0;
        uint32_t v = dsp_conv_float_to_uint32(R, r.e, &error);
        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

        TEST_ASSERT_UINT32_WITHIN_MESSAGE(2, v, r.m, "Output delta is too large");
    }
}

void test_add_u32_u32_zero(){
  unsigned rand_seed = 30;

  for(unsigned i=0;i<TEST_COUNT;i++){

      dsp_u32_float_t a = random_float_u32(&rand_seed);
      dsp_u32_float_t b = {0, 0};

      dsp_normalise_u32(&a);
      dsp_normalise_u32(&b);
      dsp_u32_float_t result = dsp_add_u32_u32(a, b);

      TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_u32(&result), "Output is not properly normalised");
      TEST_ASSERT_EQUAL_INT_MESSAGE(a.e, result.e, "Incorrect exponents");
      TEST_ASSERT_EQUAL_UINT32_MESSAGE(a.m, result.m, "Output delta is too large");
  }
  for(unsigned i=0;i<TEST_COUNT;i++){

      dsp_u32_float_t a = {0, 0};
      dsp_u32_float_t b = random_float_u32(&rand_seed);

      dsp_normalise_u32(&a);
      dsp_normalise_u32(&b);
      dsp_u32_float_t result = dsp_add_u32_u32(a, b);

      TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_u32(&result), "Output is not properly normalised");
      TEST_ASSERT_EQUAL_INT_MESSAGE(b.e, result.e, "Incorrect exponents");
      TEST_ASSERT_EQUAL_UINT32_MESSAGE(b.m, result.m, "Output delta is too large");
  }
}

void test_sub_s32_s32(){
    unsigned rand_seed = 31;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_s32_float_t a = random_float_s32(&rand_seed);
        dsp_s32_float_t b = random_float_s32(&rand_seed);

        double A = s32_to_double(a);
        double B = s32_to_double(b);

        double R = A - B;

        dsp_normalise_s32(&a);
        dsp_normalise_s32(&b);

        dsp_s32_float_t r = dsp_sub_s32_s32(a, b);

        TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_s32(&r), "Output is not properly normalised");

        int error = 0;
        int32_t v = dsp_conv_float_to_int32(R, r.e, &error);
        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
        unsigned tailroom = clz(bitrev(v|r.m));

        TEST_ASSERT_INT32_WITHIN_MESSAGE(16<<tailroom, v, r.m, "Output delta is too large");
    }
}

void test_sub_s32_s32_zero(){
    unsigned rand_seed = 32;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_s32_float_t a = random_float_s32(&rand_seed);
        dsp_s32_float_t b = {0, 0};

        dsp_normalise_s32(&a);
        dsp_normalise_s32(&b);
        dsp_s32_float_t result = dsp_sub_s32_s32(a, b);

        TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_s32(&result), "Output is not properly normalised");
        TEST_ASSERT_EQUAL_INT_MESSAGE(a.e, result.e, "Incorrect exponents");
        TEST_ASSERT_EQUAL_INT_MESSAGE(a.m, result.m, "Output delta is too large");
    }
}

void test_sub_u32_u32(){
    unsigned rand_seed = 33;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_u32_float_t a = random_float_u32(&rand_seed);
        dsp_u32_float_t b = random_float_u32(&rand_seed);

        double R = u32_to_double(a) - u32_to_double(b);

        dsp_normalise_u32(&a);
        dsp_normalise_u32(&b);

        dsp_s32_float_t r = dsp_sub_u32_u32(a, b);

        TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_s32(&r), "Output is not properly normalised");

        int error = 0;
        int32_t v = dsp_conv_float_to_int32(R, r.e, &error);
        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
        unsigned tailroom = clz(bitrev(v|r.m));

        TEST_ASSERT_INT32_WITHIN_MESSAGE(16<<tailroom, v, r.m, "Output delta is too large");
    }
}

void test_sub_u32_u32_zero(){
    unsigned rand_seed = 34;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_u32_float_t a = random_float_u32(&rand_seed);
        dsp_u32_float_t b = {0, 0};

        double actual = u32_to_double(a);

        dsp_normalise_u32(&a);
        dsp_normalise_u32(&b);

        dsp_s32_float_t result = dsp_sub_u32_u32(a, b);
        TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_s32(&result), "Output is not properly normalised");

        int error = 0;
        uint32_t actual_normalised = dsp_conv_float_to_int32(actual, result.e, &error);
        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
        TEST_ASSERT_EQUAL_INT_MESSAGE(actual_normalised, result.m, "Output delta is too large");
    }
}

void test_mul_s32_s32(){
    unsigned rand_seed = 2;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_s32_float_t a = random_float_s32(&rand_seed);
        dsp_s32_float_t b = random_float_s32(&rand_seed);

        double actual = s32_to_double(a) * s32_to_double(b);

        dsp_normalise_s32(&a);
        dsp_normalise_s32(&b);

        dsp_s32_float_t r = dsp_mul_s32_s32(a, b);
        TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_s32(&r), "Output is not properly normalised");

        int error = 0;
        int32_t v = dsp_conv_float_to_int32(actual, r.e, &error);
        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
        TEST_ASSERT_INT32_WITHIN_MESSAGE(2, v, r.m, "Output delta is too large");
    }
}

void test_mul_s32_s32_zero(){
    unsigned rand_seed = 35;

    for(unsigned i=0;i<TEST_COUNT;i++){
        dsp_s32_float_t a = random_float_s32(&rand_seed);
        dsp_s32_float_t b = {0, 0};

        dsp_normalise_s32(&a);
        dsp_normalise_s32(&b);

        // double actual = s32_to_double(b);

        dsp_s32_float_t result_normalised = dsp_mul_s32_s32(a, b);
        TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_s32(&result_normalised), "Output is not properly normalised");

        TEST_ASSERT_FALSE_MESSAGE(result_normalised.m, "Output is non-zero");
    }
    for(unsigned i=0;i<TEST_COUNT;i++){
        dsp_s32_float_t a = {0, 0};
        dsp_s32_float_t b = random_float_s32(&rand_seed);

        dsp_normalise_s32(&a);
        dsp_normalise_s32(&b);

        // double actual = s32_to_double(a);

        dsp_s32_float_t result_normalised = dsp_mul_s32_s32(a, b);
        TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_s32(&result_normalised), "Output is not properly normalised");

        TEST_ASSERT_FALSE_MESSAGE(result_normalised.m, "Output is non-zero");
    }
}

void test_mul_u32_u32(){
    unsigned rand_seed = 36;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_u32_float_t a = random_float_u32(&rand_seed);
        dsp_u32_float_t b = random_float_u32(&rand_seed);

        double actual = u32_to_double(a) * u32_to_double(b);

        dsp_normalise_u32(&a);
        dsp_normalise_u32(&b);

        dsp_u32_float_t r = dsp_mul_u32_u32(a, b);
        TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_u32(&r), "Output is not properly normalised");

        int error = 0;
        uint32_t v = dsp_conv_float_to_uint32(actual, r.e, &error);
        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
        TEST_ASSERT_UINT32_WITHIN_MESSAGE(1, v, r.m, "Output delta is too large");
    }
}

void test_mul_u32_u32_zero(){
    unsigned rand_seed = 37;

    for(unsigned i=0;i<TEST_COUNT;i++){
        dsp_u32_float_t a = random_float_u32(&rand_seed);
        dsp_u32_float_t b = {0, 0};

        dsp_normalise_u32(&a);
        dsp_normalise_u32(&b);

        // double actual = u32_to_double(b);

        dsp_u32_float_t result_normalised = dsp_mul_u32_u32(a, b);
        TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_u32(&result_normalised), "Output is not properly normalised");

        TEST_ASSERT_FALSE_MESSAGE(result_normalised.m, "Output is non-zero");
    }
    for(unsigned i=0;i<TEST_COUNT;i++){
        dsp_u32_float_t a = {0, 0};
        dsp_u32_float_t b = random_float_u32(&rand_seed);

        dsp_normalise_u32(&a);
        dsp_normalise_u32(&b);

        // double actual = u32_to_double(a);

        dsp_u32_float_t result_normalised = dsp_mul_u32_u32(a, b);
        TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_u32(&result_normalised), "Output is not properly normalised");

        TEST_ASSERT_FALSE_MESSAGE(result_normalised.m, "Output is non-zero");
    }
}

void test_mul_s32_u32(){
    unsigned rand_seed = 38;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_s32_float_t a = random_float_s32(&rand_seed);
        dsp_u32_float_t b = random_float_u32(&rand_seed);

        double actual = s32_to_double(a) * u32_to_double(b);

        dsp_normalise_s32(&a);
        dsp_normalise_u32(&b);

        dsp_s32_float_t r = dsp_mul_s32_u32(a, b);
        TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_s32(&r), "Output is not properly normalised");

        int error = 0;
        uint32_t v = dsp_conv_float_to_int32(actual, r.e, &error);
        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

        TEST_ASSERT_UINT32_WITHIN_MESSAGE(2, v, r.m, "Output delta is too large");
    }
}

void test_mul_s32_u32_zero(){
    unsigned rand_seed = 39;

    for(unsigned i=0;i<TEST_COUNT;i++){
        dsp_s32_float_t a = random_float_s32(&rand_seed);
        dsp_u32_float_t b = {0, 0};

        dsp_normalise_s32(&a);
        dsp_normalise_u32(&b);

        // double actual = u32_to_double(b);

        dsp_s32_float_t result_normalised = dsp_mul_s32_u32(a, b);
        TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_s32(&result_normalised), "Output is not properly normalised");

        TEST_ASSERT_FALSE_MESSAGE(result_normalised.m, "Output is non-zero");
    }
    for(unsigned i=0;i<TEST_COUNT;i++){
        dsp_s32_float_t a = {0, 0};
        dsp_u32_float_t b = random_float_u32(&rand_seed);

        dsp_normalise_s32(&a);
        dsp_normalise_u32(&b);

        // double actual = s32_to_double(a);

        dsp_s32_float_t result_normalised = dsp_mul_s32_u32(a, b);
        TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_s32(&result_normalised), "Output is not properly normalised");

        TEST_ASSERT_FALSE_MESSAGE(result_normalised.m, "Output is non-zero");
    }
}

void test_div_s32_s32(){
    unsigned rand_seed = 40;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_s32_float_t a = random_float_s32(&rand_seed);
        dsp_s32_float_t b = random_float_s32(&rand_seed);

        double A = s32_to_double(a);
        double B = s32_to_double(b);
        double actual = A / B;

        dsp_normalise_s32(&a);
        dsp_normalise_s32(&b);

        dsp_s32_float_t r = dsp_div_s32_s32(a, b);

        TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_s32(&r), "Output is not properly normalised");

        int error = 0;
        int32_t v = dsp_conv_float_to_int32(actual, r.e, &error);
        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

        TEST_ASSERT_INT32_WITHIN_MESSAGE(2, v, r.m, "Output delta is too large");
    }
}

void test_div_s32_s32_by_zero(){
    unsigned rand_seed = 41;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_s32_float_t a = random_float_s32(&rand_seed);
        dsp_s32_float_t b = {0, 0};

        dsp_normalise_s32(&a);
        dsp_normalise_s32(&b);

        dsp_s32_float_t result_normalised = dsp_div_s32_s32(a, b);
        TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_s32(&result_normalised), "Output is not properly normalised");

        double actual = 1.0;
        int error = 0;
        int32_t result = dsp_conv_float_to_int32(actual, result_normalised.e, &error);
        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
        TEST_ASSERT_EQUAL_INT32_MESSAGE(result_normalised.m, result, "Output delta is too large");
    }
}

void test_div_u32_u32(){
    unsigned rand_seed = 42;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_u32_float_t a = random_float_u32(&rand_seed);
        dsp_u32_float_t b = random_float_u32(&rand_seed);

        double actual = u32_to_double(a) / u32_to_double(b);

        dsp_normalise_u32(&a);
        dsp_normalise_u32(&b);

        dsp_u32_float_t r = dsp_div_u32_u32(a, b);

        TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_u32(&r), "Output is not properly normalised");

        int error = 0;
        uint32_t v = dsp_conv_float_to_uint32(actual, r.e, &error);
        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

        TEST_ASSERT_UINT32_WITHIN_MESSAGE(4, v, r.m, "Output delta is too large");
    }
}

void test_div_u32_u32_in_equals_out(){
    unsigned rand_seed = 43;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_u32_float_t a = random_float_u32(&rand_seed);
        dsp_u32_float_t b = a;

        double actual = 1.0;

        dsp_normalise_u32(&a);
        dsp_normalise_u32(&b);

        dsp_u32_float_t r = dsp_div_u32_u32(a, b);
        dsp_normalise_u32(&r);
        TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_u32(&r), "Output is not properly normalised");

        int error = 0;
        uint32_t v = dsp_conv_float_to_uint32(actual, r.e, &error);
        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

        TEST_ASSERT_UINT32_WITHIN_MESSAGE(2, r.m, v, "Output delta is too large");
    }
}

void test_div_u32_u32_by_zero(){
    unsigned rand_seed = 44;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_u32_float_t a = random_float_u32(&rand_seed);
        dsp_u32_float_t b = {0, 0};

        dsp_normalise_u32(&a);
        dsp_normalise_u32(&b);
        dsp_u32_float_t result_normalised = dsp_div_u32_u32(a, b);
        TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_u32(&result_normalised), "Output is not properly normalised");

        double actual = 1.0;
        int error = 0;
        int32_t result = dsp_conv_float_to_uint32(actual, result_normalised.e, &error);
        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
        TEST_ASSERT_EQUAL_UINT32_MESSAGE(result_normalised.m, result, "Output delta is too large");
    }
}

void test_div_s32_u32(){
    unsigned rand_seed = 45;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_s32_float_t a = random_float_s32(&rand_seed);
        dsp_u32_float_t b = random_float_u32(&rand_seed);

        double actual = s32_to_double(a) / u32_to_double(b);

        dsp_normalise_s32(&a);
        dsp_normalise_u32(&b);

        dsp_s32_float_t r = dsp_div_s32_u32(a, b);

        TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_s32(&r), "Output is not properly normalised");

        int error = 0;
        uint32_t v = dsp_conv_float_to_int32(actual, r.e, &error);
        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

        TEST_ASSERT_UINT32_WITHIN_MESSAGE(2, v, r.m, "Output delta is too large");
    }
}

void test_div_s32_u32_by_zero(){
    unsigned rand_seed = 46;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_s32_float_t a = random_float_s32(&rand_seed);
        dsp_u32_float_t b = {0, 0};

        dsp_normalise_s32(&a);
        dsp_normalise_u32(&b);
        dsp_s32_float_t result_normalised = dsp_div_s32_u32(a, b);
        TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_s32(&result_normalised), "Output is not properly normalised");

        double actual = 1.0;
        int error = 0;
        int32_t result = dsp_conv_float_to_int32(actual, result_normalised.e, &error);
        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
        TEST_ASSERT_EQUAL_INT32_MESSAGE(result_normalised.m, result, "Output delta is too large");
    }
}

void test_sqrt_u32(){
    unsigned rand_seed = 47;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_u32_float_t a = random_float_u32(&rand_seed);
        dsp_normalise_u32(&a);

        double A = u32_to_double(a);

        dsp_u32_float_t r = dsp_sqrt_u32(a);

        double R = sqrt(A);

        int error = 0;
        uint32_t v = dsp_conv_float_to_uint32(R, r.e, &error);
        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

        TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_u32(&r), "Output is not properly normalised");

        TEST_ASSERT_UINT32_WITHIN_MESSAGE(5, v, r.m, "Output delta is too large");
    }
}
void test_sqrt_u32_zero(){
    unsigned rand_seed = 48;

    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_u32_float_t a = random_float_u32(&rand_seed);
        a.m = 0;
        dsp_normalise_u32(&a);

        double A = u32_to_double(a);

        dsp_u32_float_t r = dsp_sqrt_u32(a);

        double R = sqrt(A);

        int error = 0;
        uint32_t v = dsp_conv_float_to_int32(R, r.e, &error);
        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

        TEST_ASSERT_FALSE_MESSAGE(dsp_normalise_u32(&r), "Output is not properly normalised");

        TEST_ASSERT_UINT32_WITHIN_MESSAGE(5, v, r.m, "Output delta is too large");
    }
}


void test_exponential_average_u32(){
    unsigned rand_seed = 49;

    double X = 0.0;
    dsp_u32_float_t x = {0, DSP_FLOAT_ZERO_EXP};

    unsigned average_error = 0;
    for(unsigned i=0;i<TEST_COUNT;i++){

        dsp_u32_float_t new_sample = random_float_u32(&rand_seed);
        dsp_normalise_u32(&new_sample);

        double New_sample = u32_to_double(new_sample);

        uint32_t alpha = dsp_pseudo_rand_uint32(&rand_seed);

        int error = 0;
        double Alpha = dsp_conv_uint32_to_float(alpha, -32, &error);
        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

        X = (X*Alpha) + ((1.0 - Alpha)*New_sample);

        dsp_exponential_average_u32(&x, new_sample, alpha);

        uint32_t r = dsp_conv_float_to_uint32(X, x.e, &error);
        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
        int diff = r - x.m;
        if(diff <0) diff = -diff;

        average_error += diff;
        TEST_ASSERT_INT32_WITHIN_MESSAGE(8192, r, x.m, "Output delta is too large");

    }
    TEST_ASSERT_LESS_THAN_UINT32_MESSAGE(16, average_error/TEST_COUNT, "average error is too high");
}
