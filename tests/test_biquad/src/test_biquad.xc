// Copyright 2017-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include "unity.h"

#include <xs1.h>
#include <string.h>

#include <xclib.h>
#include <dsp.h>
#include <dsp_fp.h>
#include <dsp_testing.h>

#include "iir.h"

{double, double} biquad_measure(unsigned order, const int q_format, int32_t * filter_coeffs){

unsafe {
    double filter_coeffs_fp[MAX_BIQUAD_ORDER*DSP_NUM_COEFFS_PER_BIQUAD];
    for(unsigned s=0;s<order*DSP_NUM_COEFFS_PER_BIQUAD;s++){
        filter_coeffs_fp[s] = ldexp(filter_coeffs[s], -q_format);
    }

#define STATE_OVER_LENGTH (MAX_BIQUAD_ORDER*DSP_NUM_STATES_PER_BIQUAD+8)

    int32_t  [[aligned(8)]]state_data[STATE_OVER_LENGTH];
    memset(state_data, 0, sizeof(state_data));

    double state_data_fp[STATE_OVER_LENGTH];
    memset(state_data_fp, 0, sizeof(state_data_fp));

#define DATA_LEN (1<<16)
    int x_exp = -31;
    unsigned r = 0;

    double m = 0.0;
    double mean_error = 0;
    double mean_squared_error = 0;
    for(unsigned s=0;s<DATA_LEN;s++){

        int32_t x = dsp_pseudo_rand_int32((unsigned * unsafe) &r)>>1;
        int error = 0;
        dsp_float_t x_fp = dsp_conv_int32_to_float(x, x_exp, (int * unsafe) &error);
        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

        int32_t y = dsp_filters_biquads(x, filter_coeffs, state_data, order, q_format);
        double y_fp = dsp_filters_biquads_fp(x_fp, filter_coeffs_fp, state_data_fp, order);

        for(unsigned i=0;i<DSP_NUM_STATES_PER_BIQUAD*order;i++){
            m = fmax(m,state_data_fp[i]);
        }

        int32_t y_fp_normalised = dsp_conv_float_to_int32(y_fp,x_exp, &error);
        TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
        double diff = (double)(y-y_fp_normalised);
        mean_error += diff;
        mean_squared_error += (diff*diff);

    }

    for(unsigned i=order*DSP_NUM_STATES_PER_BIQUAD;i<STATE_OVER_LENGTH;i++){
        TEST_ASSERT_EQUAL_INT32_MESSAGE(0, state_data[i], "state_data should be zero");
    }

    //check that no state that should be written to got written to.

    TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(1, mean_error/DATA_LEN, "mean error too big");
    TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(2048, mean_squared_error/DATA_LEN, "mean squared error too big");


    return {mean_error/DATA_LEN, mean_squared_error/DATA_LEN};
}
}

void test_biquads(){

    const int q_format = 30;
    int32_t [[aligned(8)]]filter_coeffs[3*DSP_NUM_COEFFS_PER_BIQUAD] = {
            Q30(0.9384318012763266), Q30(-0.9384318012763266), Q30(0.0), Q30(0.9614814515953285), Q30(0.0),
            Q30(1.0), Q30(-2.0), Q30(1.0), Q30(1.936937343346109), Q30(-0.938431804016836),
            Q30(1.0), Q30(-2.0), Q30(1.0), Q30(1.9745034942895083), Q30(-0.9760269394471773),
    };

    double mean_error = 0;
    double mean_squared_error = 0;
    {mean_error, mean_squared_error} = biquad_measure(3, q_format, filter_coeffs);
    TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(1, mean_error, "mean error too big");
    TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(2048, mean_squared_error, "mean squared error too big");
    return;
}
