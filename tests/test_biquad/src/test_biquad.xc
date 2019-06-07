// Copyright (c) 2019, XMOS Ltd, All rights reserved
#include "unity.h"

#include <xs1.h>
#include <string.h>
#include <math.h>

#include <xclib.h>
#include <dsp.h>
#include <dsp_fp.h>

int32_t double_to_int32(double d, const int d_exp){
    int m_exp;
    double m = frexp (d, &m_exp);

    double r = ldexp(m, m_exp - d_exp);
    int output_exponent;
    frexp(r, &output_exponent);
    if(output_exponent>31){
        printf("exponent is too high to cast to an int32_t (%d)\n", output_exponent);
    }
    return r;
}

#define CRC_POLY (0xEB31D82E)
int32_t random_int32(unsigned &r){
    crc32(r, -1, CRC_POLY);
    return (int32_t)r;
}

void test_biquads(){

    const int q_format = 30;
    int32_t [[aligned(8)]]filter_coeffs[3*DSP_NUM_COEFFS_PER_BIQUAD] = {
            Q30(0.9384318012763266), Q30(-0.9384318012763266), Q30(0.0), Q30(0.9614814515953285), Q30(0.0),
            Q30(1.0), Q30(-2.0), Q30(1.0), Q30(1.936937343346109), Q30(-0.938431804016836),
            Q30(1.0), Q30(-2.0), Q30(1.0), Q30(1.9745034942895083), Q30(-0.9760269394471773),
    };

    double filter_coeffs_fp[3*DSP_NUM_COEFFS_PER_BIQUAD];
    for(unsigned s=0;s<3*DSP_NUM_COEFFS_PER_BIQUAD;s++){
        filter_coeffs_fp[s] = ldexp(filter_coeffs[s], -q_format);
    }

    int32_t  [[aligned(8)]]state_data[3*DSP_NUM_STATES_PER_BIQUAD];
    memset(state_data, 0, sizeof(state_data));

    double state_data_fp[3*DSP_NUM_STATES_PER_BIQUAD];
    memset(state_data_fp, 0, sizeof(state_data_fp));

#define DATA_LEN (1<<16)
    int x_exp = -31;
    unsigned r = 0;

    int64_t mean_error = 0;
    uint64_t mean_squared_error = 0;
    for(unsigned s=0;s<DATA_LEN;s++){

        int32_t x = random_int32(r)>>1;
        double x_fp = ldexp(x, x_exp);

        int32_t y = dsp_filters_biquads(x, filter_coeffs, state_data, 3, q_format);
        double y_fp = dsp_filters_biquads_fp(x_fp, filter_coeffs_fp, state_data_fp, 3);
        int32_t y_fp_normalised = double_to_int32(y_fp,x_exp);

        int diff = y-y_fp_normalised;
        mean_error += diff;
        mean_squared_error += (diff*diff);

    }
//    printf("%lld %llu\n",mean_error/DATA_LEN, mean_squared_error/DATA_LEN);
    TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(1, mean_error/DATA_LEN, "mean error too big");
    TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(2048, mean_squared_error/DATA_LEN, "mean squared error too big");


    return;
}
