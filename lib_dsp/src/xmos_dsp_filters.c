#include <platform.h>
#include "xmos_dsp_qformat.h"
#include "xmos_dsp_math.h"
#include "xmos_dsp_vector.h"
#include "xmos_dsp_statistics.h"
#include "xmos_dsp_filters.h"

// Nth order FIR filter
//
// 'input_sample':  The new sample to be processed.
// 'filter_coeffs': Pointer to FIR coefficients arranged as [b0, b1, b2, ...bN-1].
// 'state_data':    Pointer to filter state data array of length N.
//                  Must be initialized at startup to all zero's.
// 'tap_count':     Filter tap count (N = tap_count = filter order + 1).
// 'q_format':      Fixed point format, the number of bits making up fractional part.
//
// return value:    Resulting filter output sample.

int xmos_dsp_fir
(
    int        input_sample,
    const int* filter_coeffs,
    int*       state_data,
    int        tap_count,
    int        q_format
)
{
    register int ah = 0, b0, b1, s0 = input_sample, s1, s2, s3;
    register unsigned int al = 1 << (q_format-1);
    
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
    asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
    return ah;
}

// Biquadratic IIR filter (direct form I)
//
// 'input_sample':  The new sample to be processed.
// 'filter_coeffs': Pointer to biquad coefficients arranged as [b0,b1,b2,a1,a2].
// 'state_data':    Pointer to filter state data (initialized at startup to zero's).
//                  The length of the state data array is 4.
// 'q_format':      Fixed point format, number of bits making up fractional part.
//
// return value:    Resulting filter output sample.

int xmos_dsp_iir_biquad
(
    int        input_sample,
    const int* filter_coeffs,
    int*       state_data,
    int        q_format
)
{
    register unsigned al; register int ah, c1,c2, s1,s2;
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
    asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));    
    asm("std %0,%1,%2[1]"::"r"(s1),"r"(ah),"r"(state_data));
    return ah;
}


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

int xmos_dsp_cascaded_iir_biquad( int input_sample, const int* filter_coeffs, int* state_data, int nn, int q_format )
{
    register unsigned al; register int ah, b0,b1, s1,s2;
    for( ;; )
    {
        switch( nn )
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
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
            asm("std %0,%1,%2[7]"::"r"(s1),"r"(ah),"r"(state_data));
            nn -= 4; filter_coeffs += 20; state_data += 16;
            input_sample = ah;
            break;
        }
    }
    return 0;
}
