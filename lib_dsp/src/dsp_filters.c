// Copyright (c) 2015-2016, XMOS Ltd, All rights reserved

#include <platform.h>
#include "dsp_qformat.h"
#include "dsp_math.h"
#include "dsp_vector.h"
#include "dsp_statistics.h"
#include "dsp_filters.h"



int32_t dsp_filters_fir
(
    int32_t        input_sample,
    const int32_t* filter_coeffs,
    int32_t*       state_data,
    const int32_t  num_taps,
    const int32_t  q_format
) {
    int32_t ah = 0, b0, b1, s0 = input_sample, s1, s2, s3;
    uint32_t al = 1 << (q_format-1);
    
    int32_t nt = num_taps;

    while( nt >= 20 )
    {
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[4]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[5]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[5]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[6]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[6]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[7]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[7]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[7]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[8]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[8]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[8]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[9]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[9]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[9]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));
        
        nt -= 20; filter_coeffs += 20; state_data += 20;
    }
    switch( nt )
    {
        case 19:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[4]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[5]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[5]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[6]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[6]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[7]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[7]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[7]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[8]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[8]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[8]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        state_data[18] = s2;
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(filter_coeffs[18]),"r"(s2),"0"(ah),"1"(al));
        break;

        case 18:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[4]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[5]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[5]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[6]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[6]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[7]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[7]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[7]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[8]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        s1 = state_data[16]; 
        asm("std %0,%1,%2[8]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        break;

        case 17:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[4]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[5]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[5]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[6]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[6]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[7]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[7]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[7]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        state_data[16] = s0;
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(filter_coeffs[16]),"r"(s0),"0"(ah),"1"(al));
        break;

        case 16:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[4]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[5]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[5]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[6]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[6]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[7]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[7]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[7]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));
        break;
        
        case 15:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[4]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[5]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[5]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[6]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[6]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        state_data[14] = s2;
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(filter_coeffs[14]),"r"(s2),"0"(ah),"1"(al));
        break;

        case 14:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[4]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[5]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[5]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        s1 = state_data[12]; 
        asm("std %0,%1,%2[6]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        break;

        case 13:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[4]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[5]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[5]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        state_data[12] = s0;
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(filter_coeffs[12]),"r"(s0),"0"(ah),"1"(al));
        break;

        case 12:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[4]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[5]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[5]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));
        break;
        
        case 11:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[4]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        state_data[10] = s2;
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(filter_coeffs[10]),"r"(s2),"0"(ah),"1"(al));
        break;

        case 10:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        s1 = state_data[8]; 
        asm("std %0,%1,%2[4]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        break;

        case 9:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        state_data[8] = s0;
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(filter_coeffs[8]),"r"(s0),"0"(ah),"1"(al));
        break;

        case 8:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));
        break;
        
        case 7:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        state_data[6] = s2;
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(filter_coeffs[6]),"r"(s2),"0"(ah),"1"(al));
        break;

        case 6:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        s1 = state_data[4]; 
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        break;

        case 5:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        state_data[4] = s0;
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(filter_coeffs[4]),"r"(s0),"0"(ah),"1"(al));
        break;

        case 4:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));
        break;
        
        case 3:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        state_data[2] = s2;
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(filter_coeffs[2]),"r"(s2),"0"(ah),"1"(al));
        break;

        case 2:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        s1 = state_data[0]; 
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        break;

        case 1:
        
        state_data[0] = s0;
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(filter_coeffs[0]),"r"(s0),"0"(ah),"1"(al));
        break;
    }
    asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
    asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
    return ah;
}

// FIR filter (even coeff array boundary, no state data shifting - for internal use only)

int32_t _dsp_filters_interpolate__fir_even
(
    const int32_t* coeff,
    const int32_t* state,
    int32_t        taps,
    int32_t        format
) {
    int32_t ah = 0, b0, b1, s0, s1;
    uint32_t al = 1 << (format-1);
    
    while( taps >= 8 )
    {
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[0]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[1]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[2]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[3]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        taps -= 8; coeff += 8; state += 8;
    }
    switch( taps )
    {        
        case 7:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[0]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[1]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[2]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(coeff[6]),"r"(state[6]),"0"(ah),"1"(al));
        break;

        case 6:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[0]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[1]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        break;

        case 5:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[0]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[1]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(coeff[4]),"r"(state[4]),"0"(ah),"1"(al));
        break;

        case 4:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[0]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[1]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        break;
        
        case 3:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[0]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(coeff[2]),"r"(state[2]),"0"(ah),"1"(al));
        break;

        case 2:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        break;

        case 1:        
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(coeff[0]),"r"(state[0]),"0"(ah),"1"(al));
        break;
    }
    asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(format),"0"(ah),"1"(al));
    asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(format));
    return ah;
}

// FIR filter (odd coeff array boundary, no state data shifting - for internal use only)

int32_t _dsp_filters_interpolate__fir_odd
(
    const int32_t* coeff,
    const int32_t* state,
    int32_t        taps,
    int32_t        format
) {
    int32_t ah = 0, b0, b1, s0, s1;
    uint32_t al = 1 << (format-1);
    
    asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(coeff[0]),"r"(state[0]),"0"(ah),"1"(al));
    --taps;

    while( taps >= 8 )
    {
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[0]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[1]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[2]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[3]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        taps -= 8; coeff += 8; state += 8;
    }
    switch( taps )
    {        
        case 7:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[0]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[1]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[2]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(coeff[6]),"r"(state[6]),"0"(ah),"1"(al));
        break;

        case 6:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[0]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[1]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        break;

        case 5:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[0]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[1]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(coeff[4]),"r"(state[4]),"0"(ah),"1"(al));
        break;

        case 4:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[0]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[1]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        break;
        
        case 3:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[0]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(coeff[2]),"r"(state[2]),"0"(ah),"1"(al));
        break;

        case 2:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        break;

        case 1:        
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(coeff[0]),"r"(state[0]),"0"(ah),"1"(al));
        break;
    }
    asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(format),"0"(ah),"1"(al));
    asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(format));
    return ah;
}



void dsp_filters_interpolate
(
    int32_t       input,
    const int32_t coeff[],
    int32_t       state[],
    const int32_t num_taps,
    const int32_t interp_factor,
    int32_t       output_samples[],
    const int32_t q_format
) {
    int32_t s0 = input, s1, s2, s3;
    int32_t odd = 0, length = num_taps / interp_factor, len;
    int32_t* ss = state;

    /*
    L = 5, N = 3
    
           b0 b1 b2 b3 b4 b5 b6 b7 b8 b9 bA bB bC bD bE
    y0 <-- x0  0  0  0  0 x1  0  0  0  0 x2  0  0  0  0
    y1 <--  0 x0  0  0  0  0 x1  0  0  0  0 x2  0  0  0
    y2 <--  0  0 x0  0  0  0  0 x1  0  0  0  0 x2  0  0
    y3 <--  0  0  0 x0  0  0  0  0 x1  0  0  0  0 x2  0
    y4 <--  0  0  0  0 x0  0  0  0  0 x1  0  0  0  0 x2

           b0 b5 bA b1 b6 bB b2 b7 bC b3 b8 bD b4 b9 bE
    y0 <-- x0 x1 x2  0  0  0  0  0  0  0  0  0  0  0  0
    y1 <--  0  0  0 x0 x1 x2  0  0  0  0  0  0  0  0  0
    y2 <--  0  0  0  0  0  0 x0 x1 x2  0  0  0  0  0  0
    y3 <--  0  0  0  0  0  0  0  0  0 x0 x1 x2  0  0  0
    y4 <--  0  0  0  0  0  0  0  0  0  0  0  0 x0 x1 x2
    */
    
    len = length;
    while( len >= 8 )
    {
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(ss));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(ss));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(ss));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(ss));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(ss));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(ss));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(ss));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(ss));
        len -= 8; ss += 8;
    }
    switch( len )
    {
        case 7:
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(ss));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(ss));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(ss));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(ss));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(ss));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(ss));
        ss[6] = s2;
        break;

        case 6:
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(ss));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(ss));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(ss));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(ss));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(ss));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(ss));
        break;
    
        case 5:
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(ss));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(ss));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(ss));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(ss));
        ss[4] = s0;
        break;
    
        case 4:
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(ss));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(ss));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(ss));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(ss));
        break;
    
        case 3:
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(ss));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(ss));
        ss[2] = s2;
        break;
    
        case 2:
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(ss));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(ss));
        break;
    
        case 1: ss[0] = s0; break;
    }
    
    for( int32_t i = 0; i < interp_factor; ++i )
    {
        if( odd )
            output_samples[i] = _dsp_filters_interpolate__fir_odd( coeff, state, length, q_format );
        else
            output_samples[i] = _dsp_filters_interpolate__fir_even( coeff, state, length, q_format );
        coeff += length;
        odd ^= length & 1;
    }
}    



int32_t dsp_filters_decimate
(
    int32_t       input_samples[],
    const int32_t filter_coeffs[],
    int32_t       state_data[],
    const int32_t num_taps,
    const int32_t decim_factor,
    const int32_t q_format
) {
    int32_t  output;
    int32_t* dst = state_data + num_taps - 1;
    int32_t* src = dst - (decim_factor-1);
    
    /*
                                    b0 b1 b2 b3 b4 b5 b6 b7 b8 b9 bA bB
    b0 b1 b2 b3 b4 b5 b6 b7 b8 b9 * x0 x1 x2 x3 x4 x5 x6 x7 x8 x9 xA xB -> y0
    b0 b1 b2 b3 b4 b5 b6 b7 b8 b9 *  ?  ?  ?  ?  ? x0 x1 x2 x3 x4 x5 x6 -> y0
    b0 b1 b2 b3 b4 b5 b6 b7 b8 b9 *  ?  ?  ?  ?  ?  ?  ?  ?  ?  ? x0 x1 -> y0
    */

    output = dsp_filters_fir( input_samples[0], filter_coeffs, state_data, num_taps, q_format );
    for( int32_t i = 0; i < num_taps - (decim_factor-1); ++i ) *dst-- = *src--;
    for( int32_t i = 0; i < decim_factor-1; ++i ) state_data[i] = input_samples[i+1];
    return output;    
}



int32_t dsp_filters_biquad
(
    int32_t        input_sample,
    const int32_t* filter_coeffs,
    int32_t*       state_data,
    const int32_t q_format
) {
    uint32_t al; int32_t ah, c1,c2, s1,s2;
    asm("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(filter_coeffs));
    asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(input_sample),"r"(c1),"0"(0),"1"(1<<(q_format-1)));
    asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
    asm("std %0,%1,%2[0]"::"r"(s1),"r"(input_sample),"r"(state_data));
    asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(c2),"0"(ah),"1"(al));
    asm("ldd %0,%1,%2[1]":"=r"(c2),"=r"(c1):"r"(filter_coeffs));
    asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(c1),"0"(ah),"1"(al));
    asm("ldd %0,%1,%2[1]":"=r"(s2),"=r"(s1):"r"(state_data));
    asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(c2),"0"(ah),"1"(al));
    asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(filter_coeffs[4]),"0"(ah),"1"(al));
    asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
    asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));    
    asm("std %0,%1,%2[1]"::"r"(s1),"r"(ah),"r"(state_data));
    return ah;
}



int32_t dsp_filters_biquads
(
    int32_t        input_sample,
    const int32_t* filter_coeffs,
    int32_t*       state_data,
    const int32_t  num_sections,
    const int32_t  q_format
) {
    uint32_t al; int32_t ah, b0,b1, s1,s2;

    int32_t ns = num_sections;  
    for( ;; )
    {
        switch( ns )
        {
            case 4:
            asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(input_sample),"r"(b0),"0"(0),"1"(1<<(q_format-1)));
            asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[0]"::"r"(s1),"r"(input_sample),"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[1]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al));
            asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
            asm("std %0,%1,%2[1]"::"r"(s1),"r"(ah),"r"(state_data));
            input_sample = ah;
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(ah),"r"(b1),"0"(0),"1"(1<<(q_format-1)));
            asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[2]"::"r"(s1),"r"(input_sample),"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b0),"0"(ah),"1"(al));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("ldd %0,%1,%2[3]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b0),"0"(ah),"1"(al));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b1),"0"(ah),"1"(al));
            asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
            asm("std %0,%1,%2[3]"::"r"(s1),"r"(ah),"r"(state_data));
            input_sample = ah;
            asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(input_sample),"r"(b0),"0"(0),"1"(1<<(q_format-1)));
            asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[4]"::"r"(s1),"r"(input_sample),"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[5]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[7]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al));
            asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
            asm("std %0,%1,%2[5]"::"r"(s1),"r"(ah),"r"(state_data));
            input_sample = ah;
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(ah),"r"(b1),"0"(0),"1"(1<<(q_format-1)));
            asm("ldd %0,%1,%2[8]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("ldd %0,%1,%2[6]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[6]"::"r"(s1),"r"(input_sample),"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b0),"0"(ah),"1"(al));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[9]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("ldd %0,%1,%2[7]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b0),"0"(ah),"1"(al));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b1),"0"(ah),"1"(al));
            asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
            asm("std %0,%1,%2[7]"::"r"(s1),"r"(ah),"r"(state_data));
            return ah;

            case 3:
            asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(input_sample),"r"(b0),"0"(0),"1"(1<<(q_format-1)));
            asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[0]"::"r"(s1),"r"(input_sample),"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[1]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al));
            asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
            asm("std %0,%1,%2[1]"::"r"(s1),"r"(ah),"r"(state_data));
            input_sample = ah;
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(ah),"r"(b1),"0"(0),"1"(1<<(q_format-1)));
            asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[2]"::"r"(s1),"r"(input_sample),"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b0),"0"(ah),"1"(al));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("ldd %0,%1,%2[3]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b0),"0"(ah),"1"(al));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b1),"0"(ah),"1"(al));
            asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
            asm("std %0,%1,%2[3]"::"r"(s1),"r"(ah),"r"(state_data));
            input_sample = ah;
            asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(input_sample),"r"(b0),"0"(0),"1"(1<<(q_format-1)));
            asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[4]"::"r"(s1),"r"(input_sample),"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[5]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(filter_coeffs[14]),"0"(ah),"1"(al));
            asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
            asm("std %0,%1,%2[5]"::"r"(s1),"r"(ah),"r"(state_data));
            return ah;

            case 2:
            asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(input_sample),"r"(b0),"0"(0),"1"(1<<(q_format-1)));
            asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[0]"::"r"(s1),"r"(input_sample),"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[1]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al));
            asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
            asm("std %0,%1,%2[1]"::"r"(s1),"r"(ah),"r"(state_data));
            input_sample = ah;
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(ah),"r"(b1),"0"(0),"1"(1<<(q_format-1)));
            asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[2]"::"r"(s1),"r"(input_sample),"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b0),"0"(ah),"1"(al));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("ldd %0,%1,%2[3]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b0),"0"(ah),"1"(al));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b1),"0"(ah),"1"(al));
            asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
            asm("std %0,%1,%2[3]"::"r"(s1),"r"(ah),"r"(state_data));
            return ah;
            
            case 1:
            asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(input_sample),"r"(b0),"0"(0),"1"(1<<(q_format-1)));
            asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[0]"::"r"(s1),"r"(input_sample),"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[1]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(filter_coeffs[4]),"0"(ah),"1"(al));
            asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));    
            asm("std %0,%1,%2[1]"::"r"(s1),"r"(ah),"r"(state_data));
            return ah;

            default:
            asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(input_sample),"r"(b0),"0"(0),"1"(1<<(q_format-1)));
            asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[0]"::"r"(s1),"r"(input_sample),"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[1]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al));
            asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
            asm("std %0,%1,%2[1]"::"r"(s1),"r"(ah),"r"(state_data));
            input_sample = ah;
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(ah),"r"(b1),"0"(0),"1"(1<<(q_format-1)));
            asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[2]"::"r"(s1),"r"(input_sample),"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b0),"0"(ah),"1"(al));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("ldd %0,%1,%2[3]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b0),"0"(ah),"1"(al));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b1),"0"(ah),"1"(al));
            asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
            asm("std %0,%1,%2[3]"::"r"(s1),"r"(ah),"r"(state_data));
            input_sample = ah;
            asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(input_sample),"r"(b0),"0"(0),"1"(1<<(q_format-1)));
            asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[4]"::"r"(s1),"r"(input_sample),"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[5]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[7]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al));
            asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
            asm("std %0,%1,%2[5]"::"r"(s1),"r"(ah),"r"(state_data));
            input_sample = ah;
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(ah),"r"(b1),"0"(0),"1"(1<<(q_format-1)));
            asm("ldd %0,%1,%2[8]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("ldd %0,%1,%2[6]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[6]"::"r"(s1),"r"(input_sample),"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b0),"0"(ah),"1"(al));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[9]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("ldd %0,%1,%2[7]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b0),"0"(ah),"1"(al));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b1),"0"(ah),"1"(al));
            asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
            asm("std %0,%1,%2[7]"::"r"(s1),"r"(ah),"r"(state_data));
            ns -= 4; filter_coeffs += 20; state_data += 16;
            input_sample = ah;
            break;
        }
    }
    return 0;
}
