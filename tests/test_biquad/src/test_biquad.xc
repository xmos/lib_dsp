// Copyright (c) 2017-2019, XMOS Ltd, All rights reserved
#include "unity.h"

#include <xs1.h>
#include <string.h>
#include <math.h>

#include <xclib.h>
#include <dsp.h>
#include <dsp_fp.h>

#include "iir.h"

int32_t double_to_int32(double d, const int d_exp){
    int m_exp;
    double m = frexp (d, &m_exp);

    double r = ldexp(m, m_exp - d_exp);
    int output_exponent;
    frexp(r, &output_exponent);
    if(output_exponent>31){
//        printf("%d\n", output_exponent);
//        printf("exponent is too high to cast to an int32_t (%d)\n", output_exponent);
        return 0.0;
    }
    return r;
}

#define CRC_POLY (0xEB31D82E)
int32_t random_int32(unsigned &r){
    crc32(r, -1, CRC_POLY);
    return (int32_t)r;
}

{double, double} biquad_measure(unsigned order, const int q_format, int32_t * filter_coeffs){

    double filter_coeffs_fp[MAX_BIQUAD_ORDER*DSP_NUM_COEFFS_PER_BIQUAD];
    for(unsigned s=0;s<order*DSP_NUM_COEFFS_PER_BIQUAD;s++){
        filter_coeffs_fp[s] = ldexp(filter_coeffs[s], -q_format);
    }
    int32_t  [[aligned(8)]]state_data[MAX_BIQUAD_ORDER*DSP_NUM_STATES_PER_BIQUAD];
    memset(state_data, 0, sizeof(state_data));

    double state_data_fp[MAX_BIQUAD_ORDER*DSP_NUM_STATES_PER_BIQUAD];
    memset(state_data_fp, 0, sizeof(state_data_fp));

#define DATA_LEN (1<<16)
    int x_exp = -31;
    unsigned r = 0;

    double m = 0.0;
    double mean_error = 0;
    double mean_squared_error = 0;
    for(unsigned s=0;s<DATA_LEN;s++){

        int32_t x = random_int32(r)>>1;
        double x_fp = ldexp(x, x_exp);

        int32_t y = dsp_filters_biquads(x, filter_coeffs, state_data, order, q_format);
        double y_fp = dsp_filters_biquads_fp(x_fp, filter_coeffs_fp, state_data_fp, order);

        for(unsigned i=0;i<DSP_NUM_STATES_PER_BIQUAD*order;i++){
            m = fmax(m,state_data_fp[i]);
        }
        int32_t y_fp_normalised = double_to_int32(y_fp,x_exp);

        double diff = (double)(y-y_fp_normalised);
        mean_error += diff;
        mean_squared_error += (diff*diff);

    }
    printf("max: %f \t",m);
//    TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(1, mean_error/DATA_LEN, "mean error too big");
//    TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(2048, mean_squared_error/DATA_LEN, "mean squared error too big");


    return {mean_error/DATA_LEN, mean_squared_error/DATA_LEN};
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
    printf("%f %f\n",mean_error, mean_squared_error);
    TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(1, mean_error, "mean error too big");
    TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(2048, mean_squared_error, "mean squared error too big");
    return;
}


void test_biquads_auto_gen(){


    for(unsigned order_index = 0;order_index < IIR_ORDERS;order_index++){

        for(unsigned iir_index = 0; iir_index < IIRS_PER_ORDER; iir_index++){

            const int q_format = q_factors[order_index][iir_index];
            const int biquad_order = biquad_orders[order_index][iir_index];
            int32_t [[aligned(8)]] filter_coeffs[MAX_BIQUAD_ORDER*DSP_NUM_COEFFS_PER_BIQUAD];

            memcpy(filter_coeffs, filter[order_index][iir_index], biquad_order*DSP_NUM_COEFFS_PER_BIQUAD*sizeof(int32_t));

            uint32_t max_coef=0,min_coef=UINT_MAX;
                for(unsigned s=0;s<biquad_order*DSP_NUM_COEFFS_PER_BIQUAD;s++){
                    int32_t c= filter_coeffs[s];
                    if(c){
                        if(c<0)c=-c;
                        uint32_t d =c;
                        if(d>max_coef)max_coef = d;
                        if(d<min_coef)min_coef = d;
                    }
                }
                printf("%u %u %u\t", max_coef, min_coef, 32 - clz(max_coef/ min_coef));

//                printf("\n");
//                printf("order_index: %d\n", order_index);
//                printf("iir_index: %d\n", iir_index);
//            printf("q_format: %d\n", q_format);
//            printf("biquad_order: %d\n", biquad_order);
//            for(unsigned c=0;c<DSP_NUM_COEFFS_PER_BIQUAD*biquad_order;c++){
//
//                printf("%f\n", ldexp(filter_coeffs[c], -q_format));
//            }

            double mean_error = 0;
            double mean_squared_error = 0;
            {mean_error, mean_squared_error} = biquad_measure(biquad_order, q_format, filter_coeffs);
            printf("%f %f\n",mean_error, mean_squared_error);
//            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(1, mean_error, "mean error too big");
//            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(2048, mean_squared_error, "mean squared error too big");
        }
    }
    return;
}



