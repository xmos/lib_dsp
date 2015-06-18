#include <platform.h>
#include "xmos_dsp_qformat.h"
#include "xmos_dsp_math.h"

int xmos_dsp_math_multiply( int input1_value, int input2_value, int q_format )
{
    register int ah; register unsigned al;
    asm( "maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(input1_value),"r"(input2_value),"0"(0),"1"(1<<(q_format-1)) );
    asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
    return ah;
}

int xmos_dsp_math_reciprocal( int input_value, int q_format )
{
    register int ah, temp; register unsigned al;
    int sign = input_value < 0;
    // <FIXME>: limit calculation to q_format range, return early for out-of-range operands
    // Value of one for upper word of 64-bit accumulator for the given 'q_format'
    int one = 1 << (2 * q_format - 32); 
    // Start with the minimum positive 32-bit fixed-point value
    int result = 0x080000000 >> (63 - 2 * q_format);
    if( sign ) input_value = -input_value + 1;	
    if( q_format == 31 ) result = Q31(0.9999999999); // FIXME
    else {
		// Approximation algorithm: x[0] = min, loop: x[k+1] = x[k] + x[k] * (1 − d * x[k])
		for( int i = 0; i < 30; ++i ) {
			asm( "maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(result),"r"(-input_value),"0"(one),"1"(1<<(q_format-1)) );
			asm("lextract %0,%1,%2,%3,32":"=r"(temp):"r"(ah),"r"(al),"r"(q_format));
			asm( "maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(result),"r"(temp),"0"(0),"1"(1<<(q_format-1)) );
			asm("lextract %0,%1,%2,%3,32":"=r"(temp):"r"(ah),"r"(al),"r"(q_format));
			result += temp;
		}
    }
    if( sign ) result = -result;
    return result;
}

int xmos_dsp_math_invsqrroot( int input_value, int q_format )
{
    register int ah = 1 << q_format, result; register unsigned al;
    // Approximation algorithm: ah = 1.0, loop[ yy = ah + ah*(1-x*ah^2)/2, ah=yy ]
    // <TODO>: Determine iteration count per required precision
    int one = 1 << (2 * q_format - 32);
    for( int i = 0; i < 10; ++i )
    {
        asm( "maccs %0,%1,%2,%3":"=r"(result),"=r"(al):"r"(ah),"r"(ah),"0"(0),"1"(1<<(q_format-1)) );
        asm("lextract %0,%1,%2,%3,32":"=r"(result):"r"(result),"r"(al),"r"(q_format));
        asm( "maccs %0,%1,%2,%3":"=r"(result),"=r"(al):"r"(result),"r"(-input_value),"0"(one),"1"(1<<(q_format-1)) );
        asm("lextract %0,%1,%2,%3,32":"=r"(result):"r"(result),"r"(al),"r"(q_format));
        asm( "maccs %0,%1,%2,%3":"=r"(result),"=r"(al):"r"(ah),"r"(result/2),"0"(0),"1"(1<<(q_format-1)) );
        asm("lextract %0,%1,%2,%3,32":"=r"(result):"r"(result),"r"(al),"r"(q_format));
        ah += result;
    }
    return ah;
}

int xmos_dsp_math_squareroot( int input_value, int q_format )
{
    register int ah; register unsigned al;
    ah = xmos_dsp_math_invsqrroot( input_value, q_format );
    //asm( "maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(ah),"r"(input_value),"0"(0),"1"(1<<(q_format-1)) );
    asm( "maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(ah),"r"(input_value),"0"(0),"1"(0) );
    asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
    return ah;
}
