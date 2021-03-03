// Copyright (c) 2018-2021, XMOS Ltd, All rights reserved
// This software is available under the terms provided in LICENSE.txt.
#include "dsp_math.h"

void dsp_sqrt_calc_exp(const int exp, const  unsigned hr, int * shl, int * e){
    int even_exp = exp - hr;
    if(even_exp & 1){
        even_exp += 1;
        if(hr){
            *shl = hr - 1;
            *e = even_exp/2 -14;
        } else {
            *shl = -1;
            *e = even_exp/2 -14;
        }
    } else {
        *shl = hr;
        *e = even_exp/2 -14;
    }
}
