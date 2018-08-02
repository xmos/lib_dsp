// Copyright (c) 2016-2018, XMOS Ltd, All rights reserved
#include <xs1.h>
#include <xclib.h>
#include <stdio.h>
#include <stdlib.h>

#include "dsp_vector.h"

#ifndef SEED
#define SEED 255
#endif

#define DATA_SHIFT 1

#define MAX_VECTOR_LENGTH 64
#define RSHIFT 20

int random(unsigned &x){
    crc32(x, -1, 0xEB31D82E);
    return (int)x;
}

void generate_vector(int32_t vec[], unsigned length, unsigned &seed,
                     unsigned rshift) {
    for (int i=0; i<length; i++) {
        vec[i] = random(seed) >> rshift;
    }
}

void print_vector(int32_t vec[], unsigned length) {
    for (int i=0; i<length; ++i) {
        printf("%d,", vec[i]);
    }
    printf("\n");
}

void copy_vector(int32_t src[], int32_t dst[], unsigned length) {
    for (int i=0; i<length; ++i) {
        dst[i] = src[i];
    }
}

void test_vector(){
    unsigned x=SEED;
    unsigned rshift=RSHIFT;

    unsigned num_repeats = 2;

    int32_t vec1[MAX_VECTOR_LENGTH];
    int32_t vec2[MAX_VECTOR_LENGTH];
    int32_t vec3[MAX_VECTOR_LENGTH];
    int32_t vec_result[MAX_VECTOR_LENGTH];

    int32_t vec1_re[MAX_VECTOR_LENGTH];
    int32_t vec1_im[MAX_VECTOR_LENGTH];
    int32_t vec2_re[MAX_VECTOR_LENGTH];
    int32_t vec2_im[MAX_VECTOR_LENGTH];
    int32_t vec_result_re[MAX_VECTOR_LENGTH];
    int32_t vec_result_im[MAX_VECTOR_LENGTH];

    printf("MAX_VECTOR_LENGTH=%d\n", MAX_VECTOR_LENGTH);
    printf("RSHIFT=%d\n", rshift);

    for (unsigned i=0; i<num_repeats; i++) {

        if (i == 15) {
            rshift = 0;
            printf("RSHIFT=%d\n", rshift);
        }

        ///////////////////////////////////////////////////
        //     Generate all values for this test run     //
        ///////////////////////////////////////////////////
        printf("SEED=%d\n", x);
        printf("GENERATE\n");
        unsigned length = ((unsigned) random(x)) % MAX_VECTOR_LENGTH;
        if (length == 0) length = 1;
        if (i < 10) {
            length = i+1;
        }
        int32_t scalar = random(x) >> rshift;
        generate_vector(vec1, length, x, rshift);
        generate_vector(vec2, length, x, rshift);
        generate_vector(vec3, length, x, rshift);
        generate_vector(vec1_re, length, x, rshift);
        generate_vector(vec1_im, length, x, rshift);
        generate_vector(vec2_re, length, x, rshift);
        generate_vector(vec2_im, length, x, rshift);

        ///////////////////////////////
        //       Execute tests       //
        ///////////////////////////////

        // dsp_vector_minimum
        int min_index = dsp_vector_minimum(vec1, length);
        printf("min\n");
        printf("%d\n", min_index);

        // dsp_vector_maximum
        int max_index = dsp_vector_maximum(vec1, length);
        printf("max\n");
        printf("%d\n", max_index);

        // dsp_vector_negate
        dsp_vector_negate(vec1, vec_result, length);
        printf("negate\n");
        print_vector(vec_result, length);

        // dsp_vector_abs
        dsp_vector_abs(vec1, vec_result, length);
        printf("abs\n");
        print_vector(vec_result, length);

        // dsp_vector_adds
        dsp_vector_adds(vec1, scalar, vec_result, length);
        printf("adds\n");
        print_vector(vec_result, length);

        // dsp_vector_muls
        dsp_vector_muls(vec1, scalar, vec_result, length, 24);
        printf("muls\n");
        print_vector(vec_result, length);

        // dsp_vector_addv
        dsp_vector_addv(vec1, vec2, vec_result, length);
        printf("addv\n");
        print_vector(vec_result, length);

        // dsp_vector_minv
        copy_vector(vec1, vec_result, length);
        dsp_vector_minv((uint32_t*) vec_result, (uint32_t*) vec2, length);
        printf("minv\n");
        print_vector(vec_result, length);

        // dsp_vector_subv
        dsp_vector_subv(vec1, vec2, vec_result, length);
        printf("subv\n");
        print_vector(vec_result, length);

        // dsp_vector_mulv
        dsp_vector_mulv(vec1, vec2, vec_result, length, 24);
        printf("mulv\n");
        print_vector(vec_result, length);

        // dsp_vector_mulv_adds
        dsp_vector_mulv_adds(vec1, vec2, scalar, vec_result, length, 24);
        printf("mulv_adds\n");
        print_vector(vec_result, length);

        // dsp_vector_muls_addv
        dsp_vector_muls_addv(vec1, scalar, vec2, vec_result, length, 24);
        printf("muls_addv\n");
        print_vector(vec_result, length);

        // dsp_vector_muls_subv
        dsp_vector_muls_subv(vec1, scalar, vec2, vec_result, length, 24);
        printf("muls_subv\n");
        print_vector(vec_result, length);

        // dsp_vector_mulv_addv
        dsp_vector_mulv_addv(vec1, vec2, vec3, vec_result, length, 24);
        printf("mulv_addv\n");
        print_vector(vec_result, length);

        // dsp_vector_mulv_subv
        dsp_vector_mulv_subv(vec1, vec2, vec3, vec_result, length, 24);
        printf("mulv_subv\n");
        print_vector(vec_result, length);

        // dsp_vector_mulv_complex
        dsp_vector_mulv_complex(vec1_re, vec1_im, vec2_re, vec2_im,
                                vec_result_re, vec_result_im, length, 24);
        printf("mulv_complex\n");
        print_vector(vec_result_re, length);
        print_vector(vec_result_im, length);

    }

    printf("Vector Test: Pass\n");
}

unsafe int main(){
    test_vector();
    _Exit(0);
    return 0;
}


