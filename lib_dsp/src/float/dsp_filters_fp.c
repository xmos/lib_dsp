// Copyright 2019-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include <dsp_filters_fp.h>


double dsp_filters_biquad_fp
(
    double       input_sample,
    const double filter_coeffs[],
    double       state_data[]
){
    return dsp_filters_biquads_fp(input_sample, filter_coeffs, state_data, 1);
}

double dsp_filters_biquads_fp
(
    double       input_sample,
    const double filter_coeffs[],
    double       state_data[],
    const uint32_t num_sections
){
    double carry = input_sample;

    for(unsigned s=0;s<num_sections;s++){

        double r = carry * filter_coeffs[(s*DSP_NUM_COEFFS_PER_BIQUAD) + 0]
                + state_data[(s*DSP_NUM_STATES_PER_BIQUAD) + 1] * filter_coeffs[(s*DSP_NUM_COEFFS_PER_BIQUAD) + 1]
                + state_data[(s*DSP_NUM_STATES_PER_BIQUAD) + 0] * filter_coeffs[(s*DSP_NUM_COEFFS_PER_BIQUAD) + 2]
                + state_data[(s*DSP_NUM_STATES_PER_BIQUAD) + 3] * filter_coeffs[(s*DSP_NUM_COEFFS_PER_BIQUAD) + 3]
                + state_data[(s*DSP_NUM_STATES_PER_BIQUAD) + 2] * filter_coeffs[(s*DSP_NUM_COEFFS_PER_BIQUAD) + 4];

        state_data[(s*DSP_NUM_STATES_PER_BIQUAD) + 0] = state_data[(s*DSP_NUM_STATES_PER_BIQUAD) + 1];
        state_data[(s*DSP_NUM_STATES_PER_BIQUAD) + 1] = carry;
        state_data[(s*DSP_NUM_STATES_PER_BIQUAD) + 2] = state_data[(s*DSP_NUM_STATES_PER_BIQUAD) + 3];
        state_data[(s*DSP_NUM_STATES_PER_BIQUAD) + 3] = r;
        carry = r;
    }
    return carry;
}
