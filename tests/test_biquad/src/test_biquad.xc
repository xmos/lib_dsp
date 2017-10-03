// Copyright (c) 2016-2017, XMOS Ltd, All rights reserved

#include <platform.h>
#include <xs1.h>
#include <stdio.h>

#include "dsp.h"

const int32_t filter_coeffs_A_weight[] = {
    125794161,  251588322,  125794161,  120558903,  -6768130,
    536870911,  -1073882321,  537011429,  1016763977,  -480585240,
    536870911,  -1073601322,  536730429,  1070850480,  -533983462,
};
const uint32_t num_sections_A_weight = 3;
const uint32_t q_format_A_weight = 29;
const unsigned impulse_resp_length_A_weight = 64;
const int32_t expected_ir_A_weight[64] = {
503176644,  1063234091,  572573514,  -88497353,  -211414247,  -213126630,  -196031518,  -177716803,
-160705023,  -145217365,  -131159536,  -118405744,  -106836488,  -96342594,  -86824861,  -78193220,
-70365902,  -63268672,  -56834137,  -51001115,  -45714063,  -40922558,  -36580822,  -32647300,
-29084264,  -25857466,  -22935814,  -20291083,  -17897650,  -15732253,  -13773777,  -12003051,
-10402676,  -8956853,  -7651243,  -6472829,  -5409794,  -4451412,  -3587946,  -2810557,
-2111223,  -1482661,  -918262,  -412026,  41492,  447236,  809693,  1132933,
1420651,  1676202,  1902630,  2102695,  2278904,  2433533,  2568644,  2686110,
2787631,  2874747,  2948857,  3011230,  3063016,  3105260,  3138906,  3164815,
};

int main(){

    int32_t state_data[3*DSP_NUM_STATES_PER_BIQUAD]={0};
    dsp_filters_biquads(INT_MAX, filter_coeffs_A_weight,
            state_data, num_sections_A_weight, q_format_A_weight);
    for(unsigned i=1;i<impulse_resp_length_A_weight;i++){

        int32_t v = dsp_filters_biquads(0, filter_coeffs_A_weight,
                state_data, num_sections_A_weight, q_format_A_weight);
        int diff = v - expected_ir_A_weight[i];
        if (diff < 0) diff = -diff;
        if(diff > (1<<12)){
            printf("Fail\n");
            return 1;
        }
    }
    printf("Pass\n");
    return 0;
}
