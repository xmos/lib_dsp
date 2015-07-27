// ================================================================================================

#include <platform.h>
#include "xmos_dsp_qformat.h"
#include "xmos_dsp_math.h"
#include "xmos_dsp_vector.h"
#include "xmos_dsp_statistics.h"
#include "xmos_dsp_filters.h"

// ================================================================================================

// FIR filter
//
// 'input_sample':  The new sample to be processed.
// 'filter_coeffs': Pointer to FIR coefficients array arranged as [h0, h1, h2, ...hN-1].
// 'state_data':    Pointer to filter state data array of length N.
//                  Must be initialized at startup to all zero's.
// 'tap_count':     Filter tap count (N = tap_count = filter order + 1).
// 'q_format':      Fixed point format, the number of bits making up fractional part.
//
// return value:    Resulting filter output sample.

int xmos_dsp_filters_fir
(
    int        input_sample,
    const int* filter_coeffs,
    int*       state_data,
    int        tap_count,
    int        q_format
) {
    int ah = 0, b0, b1, s0 = input_sample, s1, s2, s3;
    unsigned int al = 1 << (q_format-1);
    
    while( tap_count >= 20 )
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
        
        tap_count -= 20; filter_coeffs += 20; state_data += 20;
    }
    switch( tap_count )
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

// ================================================================================================

// FIR filter (even coeff array boundary, no state data shifting - for internal use only)

int _xmos_dsp_filters_interpolate__fir_even
(
    const int* coeff,
    const int* state,
    int        taps,
    int        format
) {
    int ah = 0, b0, b1, s0, s1;
    unsigned int al = 1 << (format-1);
    
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

int _xmos_dsp_filters_interpolate__fir_odd
(
    const int* coeff,
    const int* state,
    int        taps,
    int        format
) {
    int ah = 0, b0, b1, s0, s1;
    unsigned int al = 1 << (format-1);
    
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

// Interpolating FIR filter
//
// 'output_samples': The resulting interpolated samples.
// 'input_sample':   The new sample to be processed.
// 'filter_coeffs':  Pointer to FIR coefficients array arranged as:
//                      h0,h(1L+0),h(2L+0),...h((N-1)L+0),
//                      h1,h(1L+1),h(2L+1),...h((N-1)L+1),
//                      ...
//                      hM,h(1L+M),h(2L+M),...h((N-1)L+M),
//                   where M = N-1
// 'state_data':     Pointer to filter state data array of length N.
//                   Must be initialized at startup to all zero's.
// 'phase_length':   Phase length (N), FIR filter tap count = N * interp_factor.
// 'interp_factor':  The interpolation factor/index (i.e. the up-sampling ratio).
//                   The interpolation factor/index can range from 2 to 16.
// 'q_format':       Fixed point format, the number of bits making up fractional part.

void xmos_dsp_filters_interpolate
(
    int       output[],
    int       input,
    const int coeff[],
    int       state[],
    int       taps,
    int       L,
    int       format
) {
    int s0 = input, s1, s2, s3;
    int odd = 0, length = taps / L, len;
    int* ss = state;

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
    
    for( int i = 0; i < L; ++i )
    {
        if( odd )
            output[i] = _xmos_dsp_filters_interpolate__fir_odd( coeff, state, length, format );
        else
            output[i] = _xmos_dsp_filters_interpolate__fir_even( coeff, state, length, format );
        coeff += length;
        odd ^= length & 1;
    }
}    

// ================================================================================================

// Decimating FIR filter
//
// 'input_samples': The new samples to be decimated.
// 'filter_coeffs': Pointer to FIR coefficients array arranged as [b0,b1,b2, ...,bN-1].
// 'state_data':    Pointer to filter state data array of length N.
//                  Must be initialized at startup to all zero's.
// 'tap_count':     Filter tap count (N = tap_count = filter order + 1).
// 'decim_factor':  The decimation factor/index (i.e. the down-sampling ratio).
// 'q_format':      Fixed point format, the number of bits making up fractional part.
//
// return value:    The resulting decimated sample.

int xmos_dsp_filters_decimate
(
    int       input_samples[],
    const int filter_coeffs[],
    int       state_data[],
    int       tap_count,
    int       decim_factor,
    int       q_format
) {
    int  output;
    int* dst = state_data + tap_count - 1;
    int* src = dst - (decim_factor-1);
    
    /*
                                    b0 b1 b2 b3 b4 b5 b6 b7 b8 b9 bA bB
    b0 b1 b2 b3 b4 b5 b6 b7 b8 b9 * x0 x1 x2 x3 x4 x5 x6 x7 x8 x9 xA xB -> y0
    b0 b1 b2 b3 b4 b5 b6 b7 b8 b9 *  ?  ?  ?  ?  ? x0 x1 x2 x3 x4 x5 x6 -> y0
    b0 b1 b2 b3 b4 b5 b6 b7 b8 b9 *  ?  ?  ?  ?  ?  ?  ?  ?  ?  ? x0 x1 -> y0
    */

    output = xmos_dsp_filters_fir( input_samples[0], filter_coeffs, state_data, tap_count, q_format );
    for( int i = 0; i < tap_count - (decim_factor-1); ++i ) *dst-- = *src--;
    for( int i = 0; i < decim_factor-1; ++i ) state_data[i] = input_samples[i+1];
    return output;    
}

// ================================================================================================

// Biquadratic IIR filter (direct form I)
//
// 'input_sample':  The new sample to be processed.
// 'filter_coeffs': Pointer to biquad coefficients array arranged as [b0,b1,b2,a1,a2].
// 'state_data':    Pointer to filter state data array (initialized at startup to zero's).
//                  The length of the state data array is 4.
// 'q_format':      Fixed point format, number of bits making up fractional part.
//
// return value:    Resulting filter output sample.

int xmos_dsp_filters_biquad
(
    int        input_sample,
    const int* filter_coeffs,
    int*       state_data,
    int        q_format
) {
    unsigned al; int ah, c1,c2, s1,s2;
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

// ================================================================================================

// Cascaded biquadratic IIR filters (direct form I)
//
// 'input_sample':  The new sample to be processed
// 'filter_coeffs': Pointer to biquad coefficients for all BiQuad sections
//                  arranged as [section 1: b0,b1,b2,a1,a2, ... section N: b0,b1,b2,a1,a2]
// 'state_data':    Pointer to filter state data (initialized at startup to zero's)
//                  The length of the state data array is sections * 4
// 'num_sections':  Number of BiQuad sections
// 'q_format':      Fixed point format, the number of bits making up fractional part
//
// return value:    Resulting filter output sample

int xmos_dsp_filters_biquads
(
    int        input_sample,
    const int* filter_coeffs,
    int*       state_data,
    int        num_sections,
    int        q_format
) {
    unsigned al; int ah, b0,b1, s1,s2;
    for( ;; )
    {
        switch( num_sections )
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
            num_sections -= 4; filter_coeffs += 20; state_data += 16;
            input_sample = ah;
            break;
        }
    }
    return 0;
}

// ================================================================================================
