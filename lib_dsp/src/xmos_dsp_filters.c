#include "xmos_dsp_filters.h"

#define XS2_LOADD( aa, ii, x0, x1 ) asm("ldd %0,%1,%2[ii]":"=r"(x1),"=r"(x0):"r"(aa))
#define XS2_STORD( aa, ii, x0, x1 ) asm("std %0,%1,%2[ii]"::"r"(x1),"r"(x0),"r"(aa))
#define XS2_LEXTR( rr, ah, al, ff ) asm("lextract %0,%1,%2,%3,32":"=r"(rr):"r"(ah),"r"(al),"r"(ff))

// Nth order FIR filter
//
// 'input_sample':  The new sample to be processed.
// 'filter_coeffs': Pointer to FIR coefficients arranged as [b0, b1, b2, ...bN-1].
// 'state_data':    Pointer to filter state data array of length N-1.
//                  Must be initialized at startup to all zero's.
// 'tap_count':     Filter tap count (N = filter order = tap count - 1).
// 'q_format':      Fixed point format, the number of bits making up fractional part.
//
// return value:    Resulting filter output sample.

float _xmos_dsp_fir__float
(
    float        input_sample,
    const float* filter_coeffs,
    float*       state_data,
    int          tap_count
)
{
    register float accum = 0; int i;
    for( i = tap_count-1; i > 0; --i ) state_data[i] = state_data[i-1];
	state_data[0] = input_sample;
	accum += filter_coeffs[0] * input_sample;
    for( i = 1; i < tap_count; ++i ) accum += filter_coeffs[i] * state_data[i];
    return accum;
}

int _xmos_dsp_fir__fixed
(
    int        input_sample,
    const int* filter_coeffs,
    int*       state_data,
    int        tap_count,
    int        q_format
)
{
    register long long accum = 1 << (q_format - 1); int i;
    for( i = tap_count-1; i > 0; --i ) state_data[i] = state_data[i-1];
	state_data[0] = input_sample;
	accum += (long long)filter_coeffs[0] * input_sample;
    for( i = 1; i < tap_count; ++i ) accum += (long long)filter_coeffs[i] * state_data[i];
    return (int) (accum >> q_format);
}


int xmos_dsp_fir_new
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
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        
        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        
        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[4]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[5]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[5]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        
        asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[6]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[6]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[7]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[7]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[7]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        
        asm("ldd %0,%1,%2[8]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[8]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[8]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[9]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[9]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[9]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        
        tap_count -= 20; filter_coeffs += 20; state_data += 20;
    }
    switch( tap_count )
    {
        case 19:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );

        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[4]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[5]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[5]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );

        asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[6]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[6]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[7]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[7]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[7]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );

        asm("ldd %0,%1,%2[8]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[8]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[8]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        state_data[18] = s2;
        {ah,al} = macs( filter_coeffs[18], s2, ah, al );
        break;

        case 18:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );

        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[4]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[5]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[5]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );

        asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[6]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[6]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[7]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[7]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[7]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );

        asm("ldd %0,%1,%2[8]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        s1 = state_data[16]; 
        asm("std %0,%1,%2[8]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        break;

        case 17:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );

        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[4]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[5]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[5]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );

        asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[6]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[6]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[7]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[7]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[7]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );

        state_data[16] = s0;
        {ah,al} = macs( filter_coeffs[16], s0, ah, al );
        break;

        case 16:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );

        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[4]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[5]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[5]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );

        asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[6]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[6]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[7]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[7]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[7]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        break;
        
        case 15:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );

        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[4]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[5]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[5]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );

        asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[6]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[6]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        state_data[14] = s2;
        {ah,al} = macs( filter_coeffs[14], s2, ah, al );
        break;

        case 14:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );

        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[4]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[5]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[5]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );

        asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        s1 = state_data[12]; 
        asm("std %0,%1,%2[6]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        break;

        case 13:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );

        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[4]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[5]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[5]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );

        state_data[12] = s0;
        {ah,al} = macs( filter_coeffs[12], s0, ah, al );
        break;

        case 12:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );

        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[4]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[5]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[5]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        break;
        
        case 11:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );

        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[4]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        state_data[10] = s2;
        {ah,al} = macs( filter_coeffs[10], s2, ah, al );
        break;

        case 10:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );

        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        s1 = state_data[8]; 
        asm("std %0,%1,%2[4]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        break;

        case 9:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );

        state_data[8] = s0;
        {ah,al} = macs( filter_coeffs[8], s0, ah, al );
        break;

        case 8:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        break;
        
        case 7:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        state_data[6] = s2;
        {ah,al} = macs( filter_coeffs[6], s2, ah, al );
        break;

        case 6:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        s1 = state_data[4]; 
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        break;

        case 5:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );

        state_data[4] = s0;
        {ah,al} = macs( filter_coeffs[4], s0, ah, al );
        break;

        case 4:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        break;
        
        case 3:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        state_data[2] = s2;
        {ah,al} = macs( filter_coeffs[2], s2, ah, al );
        break;

        case 2:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        s1 = state_data[0]; 
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        break;

        case 1:
        
        state_data[0] = s0;
        {ah,al} = macs( filter_coeffs[0], s0, ah, al );
        break;
    }
    asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
    return ah;
}

int xmos_dsp_fir
(
    int        input_sample,
    const int* filter_coeffs,
    int*       state_data,
    int        tap_count,
    int        q_format
)
{
    register int ah=0, b0, b1, s0=input_sample, s1, s2, s3;
    register unsigned int al = 1 << (q_format-1);
    
    state_data[tap_count-1] = state_data[tap_count-2];
	
	while( tap_count >= 20 )
    {
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[0]"::"r"(s1),"r"(s0),"r"(state_data));
    
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[1]"::"r"(s3),"r"(s2),"r"(state_data));
            
        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[2]"::"r"(s1),"r"(s0),"r"(state_data));
    
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[3]"::"r"(s3),"r"(s2),"r"(state_data));
            
        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[4]"::"r"(s1),"r"(s0),"r"(state_data));
    
        asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[5]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[5]"::"r"(s3),"r"(s2),"r"(state_data));
            
        asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[6]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[6]"::"r"(s1),"r"(s0),"r"(state_data));
    
        asm("ldd %0,%1,%2[7]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[7]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[7]"::"r"(s3),"r"(s2),"r"(state_data));
            
        asm("ldd %0,%1,%2[8]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[8]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[8]"::"r"(s1),"r"(s0),"r"(state_data));
    
        asm("ldd %0,%1,%2[9]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[9]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[9]"::"r"(s3),"r"(s2),"r"(state_data));
            
        tap_count -= 20; filter_coeffs += 20; state_data += 20;
    }
    switch( tap_count )
    {    
        case 19:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[0]"::"r"(s1),"r"(s0),"r"(state_data));
    
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[1]"::"r"(s3),"r"(s2),"r"(state_data));

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[2]"::"r"(s1),"r"(s0),"r"(state_data));
    
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[3]"::"r"(s3),"r"(s2),"r"(state_data));

        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[4]"::"r"(s1),"r"(s0),"r"(state_data));
    
        asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[5]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[5]"::"r"(s3),"r"(s2),"r"(state_data));
            
        asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[6]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[6]"::"r"(s1),"r"(s0),"r"(state_data));
        
		asm("ldd %0,%1,%2[7]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[7]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[7]"::"r"(s3),"r"(s2),"r"(state_data));
        
        asm("ldd %0,%1,%2[8]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[8]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[8]"::"r"(s1),"r"(s0),"r"(state_data));

        {ah,al} = macs( filter_coeffs[18], s2, ah, al );
		break;

        case 18:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[0]"::"r"(s1),"r"(s0),"r"(state_data));
    
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[1]"::"r"(s3),"r"(s2),"r"(state_data));

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[2]"::"r"(s1),"r"(s0),"r"(state_data));
    
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[3]"::"r"(s3),"r"(s2),"r"(state_data));

        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[4]"::"r"(s1),"r"(s0),"r"(state_data));
    
        asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[5]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[5]"::"r"(s3),"r"(s2),"r"(state_data));
            
        asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[6]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[6]"::"r"(s1),"r"(s0),"r"(state_data));
        
		asm("ldd %0,%1,%2[7]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[7]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[7]"::"r"(s3),"r"(s2),"r"(state_data));
        
        asm("ldd %0,%1,%2[8]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[8]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[8]"::"r"(s1),"r"(s0),"r"(state_data));
		break;

        case 17:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[0]"::"r"(s1),"r"(s0),"r"(state_data));
    
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[1]"::"r"(s3),"r"(s2),"r"(state_data));

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[2]"::"r"(s1),"r"(s0),"r"(state_data));
    
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[3]"::"r"(s3),"r"(s2),"r"(state_data));

        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[4]"::"r"(s1),"r"(s0),"r"(state_data));
    
        asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[5]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[5]"::"r"(s3),"r"(s2),"r"(state_data));
            
        asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[6]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[6]"::"r"(s1),"r"(s0),"r"(state_data));
    
        asm("ldd %0,%1,%2[7]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[7]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[7]"::"r"(s3),"r"(s2),"r"(state_data));
        {ah,al} = macs( filter_coeffs[16], s0, ah, al );
		break;

		case 16:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[0]"::"r"(s1),"r"(s0),"r"(state_data));
    
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[1]"::"r"(s3),"r"(s2),"r"(state_data));

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[2]"::"r"(s1),"r"(s0),"r"(state_data));
    
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[3]"::"r"(s3),"r"(s2),"r"(state_data));

        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[4]"::"r"(s1),"r"(s0),"r"(state_data));
    
        asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[5]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[5]"::"r"(s3),"r"(s2),"r"(state_data));
            
        asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[6]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[6]"::"r"(s1),"r"(s0),"r"(state_data));
    
        asm("ldd %0,%1,%2[7]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[7]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[7]"::"r"(s3),"r"(s2),"r"(state_data));
		break;
		
        case 15:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[0]"::"r"(s1),"r"(s0),"r"(state_data));
    
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[1]"::"r"(s3),"r"(s2),"r"(state_data));

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[2]"::"r"(s1),"r"(s0),"r"(state_data));
    
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[3]"::"r"(s3),"r"(s2),"r"(state_data));
            
        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[4]"::"r"(s1),"r"(s0),"r"(state_data));
        
		asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[5]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[5]"::"r"(s3),"r"(s2),"r"(state_data));
        
        asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[6]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[6]"::"r"(s1),"r"(s0),"r"(state_data));

        {ah,al} = macs( filter_coeffs[14], s2, ah, al );
		break;

        case 14:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[0]"::"r"(s1),"r"(s0),"r"(state_data));
    
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[1]"::"r"(s3),"r"(s2),"r"(state_data));

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[2]"::"r"(s1),"r"(s0),"r"(state_data));
    
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[3]"::"r"(s3),"r"(s2),"r"(state_data));
            
        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[4]"::"r"(s1),"r"(s0),"r"(state_data));
        
		asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[5]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[5]"::"r"(s3),"r"(s2),"r"(state_data));
        
        asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[6]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[6]"::"r"(s1),"r"(s0),"r"(state_data));
		break;

        case 13:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[0]"::"r"(s1),"r"(s0),"r"(state_data));
    
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[1]"::"r"(s3),"r"(s2),"r"(state_data));

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[2]"::"r"(s1),"r"(s0),"r"(state_data));
    
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[3]"::"r"(s3),"r"(s2),"r"(state_data));
            
        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[4]"::"r"(s1),"r"(s0),"r"(state_data));
        
		asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[5]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[5]"::"r"(s3),"r"(s2),"r"(state_data));
        {ah,al} = macs( filter_coeffs[12], s0, ah, al );
		break;

		case 12:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[0]"::"r"(s1),"r"(s0),"r"(state_data));
    
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[1]"::"r"(s3),"r"(s2),"r"(state_data));

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[2]"::"r"(s1),"r"(s0),"r"(state_data));
    
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[3]"::"r"(s3),"r"(s2),"r"(state_data));
            
        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[4]"::"r"(s1),"r"(s0),"r"(state_data));
    
        asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[5]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[5]"::"r"(s3),"r"(s2),"r"(state_data));
		break;

        case 11:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[0]"::"r"(s1),"r"(s0),"r"(state_data));
    
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[1]"::"r"(s3),"r"(s2),"r"(state_data));
            
        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[2]"::"r"(s1),"r"(s0),"r"(state_data));
        
		asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[3]"::"r"(s3),"r"(s2),"r"(state_data));
        
        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[4]"::"r"(s1),"r"(s0),"r"(state_data));

        {ah,al} = macs( filter_coeffs[10], s2, ah, al );
		break;

        case 10:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[0]"::"r"(s1),"r"(s0),"r"(state_data));
    
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[1]"::"r"(s3),"r"(s2),"r"(state_data));
            
        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[2]"::"r"(s1),"r"(s0),"r"(state_data));
        
		asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[3]"::"r"(s3),"r"(s2),"r"(state_data));
        
        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[4]"::"r"(s1),"r"(s0),"r"(state_data));
		break;

        case 9:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[0]"::"r"(s1),"r"(s0),"r"(state_data));
    
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[1]"::"r"(s3),"r"(s2),"r"(state_data));
            
        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[2]"::"r"(s1),"r"(s0),"r"(state_data));
        
		asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[3]"::"r"(s3),"r"(s2),"r"(state_data));
        {ah,al} = macs( filter_coeffs[8], s0, ah, al );
		break;

		case 8:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[0]"::"r"(s1),"r"(s0),"r"(state_data));
    
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[1]"::"r"(s3),"r"(s2),"r"(state_data));
            
        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[2]"::"r"(s1),"r"(s0),"r"(state_data));
    
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[3]"::"r"(s3),"r"(s2),"r"(state_data));
		break;

        case 7:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[0]"::"r"(s1),"r"(s0),"r"(state_data));
        
		asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[1]"::"r"(s3),"r"(s2),"r"(state_data));
        
        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[2]"::"r"(s1),"r"(s0),"r"(state_data));

        {ah,al} = macs( filter_coeffs[6], s2, ah, al );
		break;

        case 6:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[0]"::"r"(s1),"r"(s0),"r"(state_data));
        
		asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[1]"::"r"(s3),"r"(s2),"r"(state_data));
        
        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[2]"::"r"(s1),"r"(s0),"r"(state_data));
		break;

        case 5:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[0]"::"r"(s1),"r"(s0),"r"(state_data));
        
		asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[1]"::"r"(s3),"r"(s2),"r"(state_data));
        {ah,al} = macs( filter_coeffs[4], s0, ah, al );
		break;

        case 4:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[0]"::"r"(s1),"r"(s0),"r"(state_data));
        
		asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[1]"::"r"(s3),"r"(s2),"r"(state_data));
		break;

		case 3:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[0]"::"r"(s1),"r"(s0),"r"(state_data));
        {ah,al} = macs( filter_coeffs[2], s2, ah, al );
		break;
    
        case 2:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[0]"::"r"(s1),"r"(s0),"r"(state_data));
		break;

        case 1:        
        {ah,al} = macs( filter_coeffs[0], s0, ah, al );
        break;
    }
    asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
    return ah;
}

/*
       case 4:
        // b0=b[0] b1=b[1] b2=b[2] ??=b[3]
        // s0=x    s1=s[0] s2=s[1]  s[2]
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[0]"::"r"(s1),"r"(s0),"r"(state_data));
    
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        {ah,al} = macs( b0, s2, ah, al );
        {ah,al} = macs( b1, s3, ah, al );
        asm("std %0,%1,%2[1]"::"r"(s3),"r"(s2),"r"(state_data));
		break;
            

		case 3:
        // b0=b[0] b1=b[1] ??=b[2]
        // s0=x    s1=s[0] s2=s[1]
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, s1, ah, al );
        asm("std %0,%1,%2[0]"::"r"(s1),"r"(s0),"r"(state_data));
        {ah,al} = macs( filter_coeffs[2], s2, ah, al );
        state_data[2] = s2;
		break;
    
        case 2:
        // b0=b[0] b1=b[1]
        // s0=x    s1=s[0]
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        {ah,al} = macs( b0, s0, ah, al );
        {ah,al} = macs( b1, state_data[0], ah, al );
        asm("std %0,%1,%2[0]"::"r"(s1),"r"(s0),"r"(state_data));
        // s[0]=x  s[1]=s1
		break;

        case 1:        
        {ah,al} = macs( filter_coeffs[0], s0, ah, al );
        state_data[0] = s0;
        break;
*/

// Biquadratic IIR filter (direct form I)
//
// 'input_sample':  The new sample to be processed
// 'filter_coeffs': Pointer to biquad coefficients arranged as [b0,b1,b2,a1,a2]
// 'state_data':    Pointer to filter state data (initialized at startup to zero's)
// 'q_format':      Fixed point format, number of bits making up fractional part
//
// return value:    Resulting filter output sample

double _xmos_dsp_iir_biquad__float
(
    double        input_sample,
    const double* filter_coeffs,
    double*       state_data
)
{
    double accum = 0;
    accum += filter_coeffs[0] * input_sample;
    accum += filter_coeffs[1] * state_data[0];
    accum += filter_coeffs[2] * state_data[1];
    accum += filter_coeffs[3] * state_data[2];
    accum += filter_coeffs[4] * state_data[3];
    state_data[1] = state_data[0];
    state_data[0] = input_sample;
    state_data[3] = state_data[2];
    state_data[2] = accum;
    return accum;
}

int _xmos_dsp_iir_biquad__fixed
(
    int        input_sample,
    const int* filter_coeffs,
    int*       state_data,
    int        q_format
)
{
    register long long accum = 1 << (q_format-1);
    register int result;
    accum += (long long)filter_coeffs[0] * input_sample;
    accum += (long long)filter_coeffs[1] * state_data[0];
    accum += (long long)filter_coeffs[2] * state_data[1];
    accum += (long long)filter_coeffs[3] * state_data[2];
    accum += (long long)filter_coeffs[4] * state_data[3];
    result = (int) (accum >> q_format);
    state_data[1] = state_data[0];
    state_data[0] = input_sample;
    state_data[3] = state_data[2];
    state_data[2] = result;
    return result;
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
    {ah,al} = macs( input_sample, c1, 0, 1 << (q_format-1) );
    asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
    asm("std %0,%1,%2[0]"::"r"(s1),"r"(input_sample),"r"(state_data));
    {ah,al} = macs( s1, c2, ah, al );
    asm("ldd %0,%1,%2[1]":"=r"(c2),"=r"(c1):"r"(filter_coeffs));
    {ah,al} = macs( s2, c1, ah, al );
    asm("ldd %0,%1,%2[1]":"=r"(s2),"=r"(s1):"r"(state_data));
    {ah,al} = macs( s1, c2, ah, al );
    {ah,al} = macs( s2, filter_coeffs[4], ah, al );
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

double _xmos_dsp_cascaded_iir_biquad__float
(
    double        input_sample,
    const double* filter_coeffs,
    double*       state_data,
    int           num_sections
)
{
    register double result;
    while( num_sections-- )
    {
		result = _xmos_dsp_iir_biquad__float( input_sample, filter_coeffs, state_data );
	    input_sample = result;
        filter_coeffs += 5; state_data += 4;
    }
    return result;
}

int _xmos_dsp_cascaded_iir_biquad__fixed
(
    int        input_sample,
    const int* filter_coeffs,
    int*       state_data,
    int        num_sections,
    int        q_format
)
{
    register int result;
    while( num_sections-- )
    {
		result = _xmos_dsp_iir_biquad__fixed( input_sample, filter_coeffs, state_data, q_format );
	    input_sample = result;
        filter_coeffs += 5; state_data += 4;
    }
    return result;
}

int xmos_dsp_cascaded_iir_biquad( int input_sample, const int* filter_coeffs, int* state_data, int nn, int ff )
{
    register unsigned al; register int ah, b0,b1, s1,s2;
    for( ;; )
    {
        switch( nn )
        {
            case 4:
            asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            {ah,al} = macs( input_sample, b0, 0, 1 << (ff - 1) );
            asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[0]"::"r"(s1),"r"(input_sample),"r"(state_data));
            {ah,al} = macs( s1, b1, ah, al );
            asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            {ah,al} = macs( s2, b0, ah, al );
            asm("ldd %0,%1,%2[1]":"=r"(s2),"=r"(s1):"r"(state_data));
            {ah,al} = macs( s1, b1, ah, al );
            asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            {ah,al} = macs( s2, b0, ah, al );
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(ff));
            asm("std %0,%1,%2[1]"::"r"(s1),"r"(ah),"r"(state_data));
            input_sample = ah;
            {ah,al} = macs( ah, b1, 0, 1 << (ff - 1) );
            asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[2]"::"r"(s1),"r"(input_sample),"r"(state_data));
            {ah,al} = macs( s1, b0, ah, al );
            {ah,al} = macs( s2, b1, ah, al );
            asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("ldd %0,%1,%2[3]":"=r"(s2),"=r"(s1):"r"(state_data));
            {ah,al} = macs( s1, b0, ah, al );
            {ah,al} = macs( s2, b1, ah, al );
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(ff));
            asm("std %0,%1,%2[3]"::"r"(s1),"r"(ah),"r"(state_data));
            input_sample = ah;
            asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            {ah,al} = macs( input_sample, b0, 0, 1 << (ff - 1) );
            asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[4]"::"r"(s1),"r"(input_sample),"r"(state_data));
            {ah,al} = macs( s1, b1, ah, al );
            asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            {ah,al} = macs( s2, b0, ah, al );
            asm("ldd %0,%1,%2[5]":"=r"(s2),"=r"(s1):"r"(state_data));
            {ah,al} = macs( s1, b1, ah, al );
            asm("ldd %0,%1,%2[7]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            {ah,al} = macs( s2, b0, ah, al );
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(ff));
            asm("std %0,%1,%2[5]"::"r"(s1),"r"(ah),"r"(state_data));
            input_sample = ah;
            {ah,al} = macs( ah, b1, 0, 1 << (ff - 1) );
            asm("ldd %0,%1,%2[8]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("ldd %0,%1,%2[6]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[6]"::"r"(s1),"r"(input_sample),"r"(state_data));
            {ah,al} = macs( s1, b0, ah, al );
            {ah,al} = macs( s2, b1, ah, al );
            asm("ldd %0,%1,%2[9]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("ldd %0,%1,%2[7]":"=r"(s2),"=r"(s1):"r"(state_data));
            {ah,al} = macs( s1, b0, ah, al );
            {ah,al} = macs( s2, b1, ah, al );
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(ff));
            asm("std %0,%1,%2[7]"::"r"(s1),"r"(ah),"r"(state_data));
            return ah;

            case 3:
            asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            {ah,al} = macs( input_sample, b0, 0, 1 << (ff - 1) );
            asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[0]"::"r"(s1),"r"(input_sample),"r"(state_data));
            {ah,al} = macs( s1, b1, ah, al );
            asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            {ah,al} = macs( s2, b0, ah, al );
            asm("ldd %0,%1,%2[1]":"=r"(s2),"=r"(s1):"r"(state_data));
            {ah,al} = macs( s1, b1, ah, al );
            asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            {ah,al} = macs( s2, b0, ah, al );
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(ff));
            asm("std %0,%1,%2[1]"::"r"(s1),"r"(ah),"r"(state_data));
            input_sample = ah;
            {ah,al} = macs( ah, b1, 0, 1 << (ff - 1) );
            asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[2]"::"r"(s1),"r"(input_sample),"r"(state_data));
            {ah,al} = macs( s1, b0, ah, al );
            {ah,al} = macs( s2, b1, ah, al );
            asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("ldd %0,%1,%2[3]":"=r"(s2),"=r"(s1):"r"(state_data));
            {ah,al} = macs( s1, b0, ah, al );
            {ah,al} = macs( s2, b1, ah, al );
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(ff));
            asm("std %0,%1,%2[3]"::"r"(s1),"r"(ah),"r"(state_data));
            input_sample = ah;
            asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            {ah,al} = macs( input_sample, b0, 0, 1 << (ff - 1) );
            asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[4]"::"r"(s1),"r"(input_sample),"r"(state_data));
            {ah,al} = macs( s1, b1, ah, al );
            asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            {ah,al} = macs( s2, b0, ah, al );
            asm("ldd %0,%1,%2[5]":"=r"(s2),"=r"(s1):"r"(state_data));
            {ah,al} = macs( s1, b1, ah, al );
            {ah,al} = macs( s2, filter_coeffs[14], ah, al );
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(ff));
            asm("std %0,%1,%2[5]"::"r"(s1),"r"(ah),"r"(state_data));
            return ah;

            case 2:
            asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            {ah,al} = macs( input_sample, b0, 0, 1 << (ff - 1) );
            asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[0]"::"r"(s1),"r"(input_sample),"r"(state_data));
            {ah,al} = macs( s1, b1, ah, al );
            asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            {ah,al} = macs( s2, b0, ah, al );
            asm("ldd %0,%1,%2[1]":"=r"(s2),"=r"(s1):"r"(state_data));
            {ah,al} = macs( s1, b1, ah, al );
            asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            {ah,al} = macs( s2, b0, ah, al );
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(ff));
            asm("std %0,%1,%2[1]"::"r"(s1),"r"(ah),"r"(state_data));
            input_sample = ah;
            {ah,al} = macs( ah, b1, 0, 1 << (ff - 1) );
            asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[2]"::"r"(s1),"r"(input_sample),"r"(state_data));
            {ah,al} = macs( s1, b0, ah, al );
            {ah,al} = macs( s2, b1, ah, al );
            asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("ldd %0,%1,%2[3]":"=r"(s2),"=r"(s1):"r"(state_data));
            {ah,al} = macs( s1, b0, ah, al );
            {ah,al} = macs( s2, b1, ah, al );
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(ff));
            asm("std %0,%1,%2[3]"::"r"(s1),"r"(ah),"r"(state_data));
            return ah;
            
            case 1:
            asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            {ah,al} = macs( input_sample, b0, 0, 1 << (ff - 1) );
            asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[0]"::"r"(s1),"r"(input_sample),"r"(state_data));
            {ah,al} = macs( s1, b1, ah, al );
            asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            {ah,al} = macs( s2, b0, ah, al );
            asm("ldd %0,%1,%2[1]":"=r"(s2),"=r"(s1):"r"(state_data));
            {ah,al} = macs( s1, b1, ah, al );
            {ah,al} = macs( s2, filter_coeffs[4], ah, al );
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(ff));    
            asm("std %0,%1,%2[1]"::"r"(s1),"r"(ah),"r"(state_data));
            return ah;

            default:
            asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            {ah,al} = macs( input_sample, b0, 0, 1 << (ff - 1) );
            asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[0]"::"r"(s1),"r"(input_sample),"r"(state_data));
            {ah,al} = macs( s1, b1, ah, al );
            asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            {ah,al} = macs( s2, b0, ah, al );
            asm("ldd %0,%1,%2[1]":"=r"(s2),"=r"(s1):"r"(state_data));
            {ah,al} = macs( s1, b1, ah, al );
            asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            {ah,al} = macs( s2, b0, ah, al );
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(ff));
            asm("std %0,%1,%2[1]"::"r"(s1),"r"(ah),"r"(state_data));
            input_sample = ah;
            {ah,al} = macs( ah, b1, 0, 1 << (ff - 1) );
            asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[2]"::"r"(s1),"r"(input_sample),"r"(state_data));
            {ah,al} = macs( s1, b0, ah, al );
            {ah,al} = macs( s2, b1, ah, al );
            asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("ldd %0,%1,%2[3]":"=r"(s2),"=r"(s1):"r"(state_data));
            {ah,al} = macs( s1, b0, ah, al );
            {ah,al} = macs( s2, b1, ah, al );
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(ff));
            asm("std %0,%1,%2[3]"::"r"(s1),"r"(ah),"r"(state_data));
            input_sample = ah;
            asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            {ah,al} = macs( input_sample, b0, 0, 1 << (ff - 1) );
            asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[4]"::"r"(s1),"r"(input_sample),"r"(state_data));
            {ah,al} = macs( s1, b1, ah, al );
            asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            {ah,al} = macs( s2, b0, ah, al );
            asm("ldd %0,%1,%2[5]":"=r"(s2),"=r"(s1):"r"(state_data));
            {ah,al} = macs( s1, b1, ah, al );
            asm("ldd %0,%1,%2[7]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            {ah,al} = macs( s2, b0, ah, al );
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(ff));
            asm("std %0,%1,%2[5]"::"r"(s1),"r"(ah),"r"(state_data));
            input_sample = ah;
            {ah,al} = macs( ah, b1, 0, 1 << (ff - 1) );
            asm("ldd %0,%1,%2[8]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("ldd %0,%1,%2[6]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[6]"::"r"(s1),"r"(input_sample),"r"(state_data));
            {ah,al} = macs( s1, b0, ah, al );
            {ah,al} = macs( s2, b1, ah, al );
            asm("ldd %0,%1,%2[9]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("ldd %0,%1,%2[7]":"=r"(s2),"=r"(s1):"r"(state_data));
            {ah,al} = macs( s1, b0, ah, al );
            {ah,al} = macs( s2, b1, ah, al );
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(ff));
            asm("std %0,%1,%2[7]"::"r"(s1),"r"(ah),"r"(state_data));
            nn -= 4; filter_coeffs += 20; state_data += 16;
            input_sample = ah;
            break;
        }
    }
    return 0;
}

double _xmos_dsp_lms__float
(
    double  input_sample,
    double  reference_sample,
    //double* error_sample_,
    double* filter_coeffs,
    double* state_data,
    int     tap_count,
    double  step_size
)
{
    double output_sample = 0.0, error_sample, delta;
    state_data[0] = input_sample;
    for( int i = 0; i < tap_count; ++i ) output_sample += state_data[i] * filter_coeffs[i];
    error_sample = reference_sample - output_sample;
    //sigma=alpha*(px[0]*px[0])+(1-alpha)*sigma;
    //mu_e=mu*e/sigma;
    delta = step_size * error_sample;
    for( int i = 0 ; i < tap_count ; ++i ) {
        filter_coeffs[i] += delta * state_data[i];
    }
    for( int i = tap_count-1 ; i >= 1 ; --i ) state_data[i] = state_data[i-1];
    return output_sample;
}

// LMS filter
//
// 'input_sample':     The new sample to be processed.
// 'reference_sample': Reference sample
// 'error_sample':     Pointer to resulting error sample (error = reference - output)
// 'filter_coeffs':    Pointer to FIR coefficients arranged as [b0,b1,b2, ...,bN-1].
// 'state_data':       Pointer to FIR filter state data array of length N.
//                     Must be initialized at startup to all zero's.
// 'tap_count':        Filter tap count where N = tap_count = filter order + 1.
// 'step_size':        Coefficient adjustment step size, controls rate of convergence.
// 'q_format':         Fixed point format, the number of bits making up fractional part.
//
// return value:    Resulting filter output sample.

int xmos_dsp_lms
(
    int  input_sample,
    int  reference_sample,
    int* error_sample,
    int* filter_coeffs,
    int* state_data,
    int  tap_count,
    int  step_size,
    int  q_format
)
{
    int output_sample, mu_err;
    
    // Output signal y[n] is computed via standard FIR filter:
    // y[n] = b[0] * x[n] + b[1] * x[n-1] + b[2] * x[n-2] + ...+ b[N-1] * x[n-N+1]

    output_sample = xmos_dsp_fir( input_sample, filter_coeffs, state_data, tap_count, q_format );
    
    // Error equals difference between reference and filter output:
    // e[n] = d[n] - y[n]

    *error_sample = reference_sample - output_sample;
    
    // FIR filter coefficients b[k] are updated on a sample-by-sample basis:
    // b[k] = b[k] + mu_err * x[n-k] --- where mu_err = e[n] * step_size
    
    mu_err = xmos_dsp_math_multiply( *error_sample, step_size, q_format );
    xmos_dsp_vector_muls_addv( filter_coeffs, state_data, mu_err, filter_coeffs, tap_count, q_format );
        
    //*error_sample_ = error_sample;
    return output_sample;
}

// Normalized LMS filter
//
// 'input_sample':     The new sample to be processed.
// 'reference_sample': Reference sample
// 'error_sample':     Pointer to resulting error sample (error = reference - output)
// 'filter_coeffs':    Pointer to FIR coefficients arranged as [b0,b1,b2, ...,bN-1].
// 'state_data':       Pointer to FIR filter state data array of length N.
//                     Must be initialized at startup to all zero's.
// 'tap_count':        Filter tap count where N = tap_count = filter order + 1.
// 'step_size':        Coefficient adjustment step size, controls rate of convergence.
// 'q_format':         Fixed point format, the number of bits making up fractional part.
//
// return value:    Resulting filter output sample.

int xmos_dsp_nlms
(
    int  input_sample,
    int  reference_sample,
    int* error_sample,
    int* filter_coeffs,
    int* state_data,
    int  tap_count,
    int  step_size,
    int  q_format
)
{
    int output_sample, mu_err_egy, energy;
    
    // Output signal y[n] is computed via standard FIR filter:
    // y[n] = b[0] * x[n] + b[1] * x[n-1] + b[2] * x[n-2] + ...+ b[N-1] * x[n-N+1]

    output_sample = xmos_dsp_fir( input_sample, filter_coeffs, state_data, tap_count, q_format );
    
    // Error equals difference between reference and filter output:
    // e[n] = d[n] - y[n]

    *error_sample = reference_sample - output_sample;
    
    // Compute the instantaneous enegry: E = x[n]^2 + x[n-1]^2 + ... + x[n-N+1]^2
    energy = xmos_dsp_vector_power( state_data, tap_count, q_format );
    
    // mu_err_egy = error * mu / energy
    energy     = xmos_dsp_math_reciprocal( energy, q_format );
    mu_err_egy = xmos_dsp_math_multiply  ( *error_sample, step_size, q_format );
    mu_err_egy = xmos_dsp_math_multiply  ( energy, mu_err_egy, q_format );
    
    // FIR filter coefficients b[k] are updated on a sample-by-sample basis:
    // b[k] = b[k] + mu_err * x[n-k] --- where mu_err = e[n] * step_size
    
    xmos_dsp_vector_muls_addv( filter_coeffs, state_data, mu_err_egy, filter_coeffs, tap_count, q_format );
        
    //*error_sample_ = error_sample;
    return output_sample;
}

