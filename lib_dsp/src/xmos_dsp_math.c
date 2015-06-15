#include "xmos_dsp_math.h"

int xmos_dsp_math_multiply( int input1_value, int input2_value, int q_format )
{
    register int ah; register unsigned al;
    {ah,al} = macs( input1_value, input2_value, 0, 1 << (q_format-1) );
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
			{ah,al} = macs( result, -input_value, one, 1<<(q_format-1) );
			asm("lextract %0,%1,%2,%3,32":"=r"(temp):"r"(ah),"r"(al),"r"(q_format));
			{ah,al} = macs( result, temp, 0, 1<<(q_format-1) );
			asm("lextract %0,%1,%2,%3,32":"=r"(temp):"r"(ah),"r"(al),"r"(q_format));
			result += temp;
		}
    }
    if( sign ) result = -result;
    return result;
}

int _xmos_dsp_math_invsqrroot__float( float input_value )
{
    float result = 1.0;
    // Approximation algorithm:  y = y + y * ((1 - x * y^2) / 2)
    for( int i = 0; i < 10; ++i ) {
        result = result + result * (1.0 - input_value * result * result) / 2.0;
    }
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
        {result,al} = macs( ah, ah, 0, 1<<(q_format-1) );
        asm("lextract %0,%1,%2,%3,32":"=r"(result):"r"(result),"r"(al),"r"(q_format));
        {result,al} = macs( result, -input_value, one, 1<<(q_format-1) );
        asm("lextract %0,%1,%2,%3,32":"=r"(result):"r"(result),"r"(al),"r"(q_format));
        {result,al} = macs( ah, result / 2, 0, 1<<(q_format-1) );
        asm("lextract %0,%1,%2,%3,32":"=r"(result):"r"(result),"r"(al),"r"(q_format));
        ah += result;
    }
    return ah;
}

int _xmos_dsp_math_squareroot__float( float input_value, int q_format )
{
    return input_value * _xmos_dsp_math_invsqrroot__float( input_value );
}

int xmos_dsp_math_squareroot( int input_value, int q_format )
{
    register int ah; register unsigned al;
    ah = xmos_dsp_math_invsqrroot( input_value, q_format );
    {ah,al} = macs( ah, input_value, 0, 0 /*(1<<(f-1)*/ );
    asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
    return ah;
}
