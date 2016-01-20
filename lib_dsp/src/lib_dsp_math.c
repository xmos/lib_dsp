// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <platform.h>
#include "lib_dsp_qformat.h"
#include "lib_dsp_math.h"

#define ASM_OPTIMISED
//#define ASM_ROUNDING

int lib_dsp_math_multiply( int input1_value, int input2_value, int q_format )
{

#ifdef ASM_OPTIMISED

    int ah; unsigned al;
    int result;
    //asm( "maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(input1_value),"r"(input2_value),"0"(0),"1"(1<<((32-q_format)*2-1)));
    asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(input1_value),"r"(input2_value),"0"(0),"1"(1<<(q_format-1)));
    asm("lextract %0,%1,%2,%3,32":"=r"(result):"r"(ah),"r"(al),"r"(q_format));
#ifdef ASM_ROUNDING
    int round;
    asm("lextract %0,%1,%2,%3,32":"=r"(round):"r"(ah),"r"(al),"r"(q_format+1));
    // rounding
    result = result + (round & 1);
#endif
    return result;
#else
    // Brute force testing of lib_dsp_math_sin shows that the additional rounding here is not required to achieve accuracy of 1 LSB (error <= 1)
    long long aa = input1_value;
    long long bb = input2_value;
    long long res = aa * bb;
    long long result = (res >> q_format) + ((res >> (q_format - 1)) & 1);
    return result;
#endif
}


int lib_dsp_math_multiply_sat( int input1_value, int input2_value, int q_format )
{
    int ah; unsigned al;
    asm( "maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(input1_value),"r"(input2_value),"0"(0),"1"(1<<(q_format-1)) );
    asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
    asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
    return ah;
}

/** Scalar reciprocal
 * 
 *  This function computes the reciprocal of the input value using an iterative
 *  approximation method as follows:
 * 
 *  \code
 *  1) result = 1.0
 *  2) result = result + result * (1 − input_value * result)
 *  3) Repeat step #2 until desired precision is achieved
 *  \endcode
 * 
 *  Example:
 * 
 *  \code
 *  int result;
 *  result = lib_dsp_math_reciprocal( sample, 28 );
 *  \endcode
 * 
 *  \param  input_value  Input value for computation.
 *  \param  q_format     Fixed point format (i.e. number of fractional bits).
 *  \returns             The reciprocal of the input value.
 */

int lib_dsp_math_reciprocal( int input_value, int q_format )
{
    int ah, temp; unsigned al;
    int sign = input_value < 0;
    // <FIXME> Limit calculation to q_format range, return early for out-of-range operands
    // Value of one for upper word of 64-bit accumulator for the given 'q_format'
    int one = 1 << (2 * q_format - 32); 
    // Start with the minimum positive 32-bit fixed-point value
    int result = 0x080000000 >> (63 - 2 * q_format);
    if( sign ) input_value = -input_value + 1;    
    if( q_format == 31 ) result = Q31(0.9999999999); // FIXME
    else {
        // Approximation algorithm: x[0] = min, loop: x[k+1] = x[k] + x[k] * (1 − d * x[k])
        for( int i = 0; i < 30; ++i ) {
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(result),"r"(-input_value),"0"(one),"1"(1<<(q_format-1)) );
            asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
            asm("lextract %0,%1,%2,%3,32":"=r"(temp):"r"(ah),"r"(al),"r"(q_format));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(result),"r"(temp),"0"(0),"1"(1<<(q_format-1)) );
            asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
            asm("lextract %0,%1,%2,%3,32":"=r"(temp):"r"(ah),"r"(al),"r"(q_format));
            result += temp;
        }
    }
    if( sign ) result = -result;
    return result;
}

/** Scalar inverse square root
 * 
 *  This function computes the reciprocal of the square root of the input value
 *  using an iterative approximation method as follows:
 * 
 *  \code
 *  1) result = 1.0
 *  2) result = result + result * (1 - input * result^2) / 2
 *  3) Repeat step #2 until desired precision is achieved
 *  \endcode
 * 
 *  Example:
 * 
 *  \code
 *  int result;
 *  result = lib_dsp_math_invsqrroot( sample, 28 );
 *  \endcode
 * 
 *  \param  input_value  Input value for computation.
 *  \param  q_format     Fixed point format (i.e. number of fractional bits).
 *  \returns             The inverse square root of the input value.
 */

int lib_dsp_math_invsqrroot( int input_value, int q_format )
{
    int ah = 1 << q_format, result; unsigned al;
    // Approximation algorithm: ah = 1.0, loop[ yy = ah + ah*(1-x*ah^2)/2, ah=yy ]
    // <TODO> Determine iteration count per required precision, use 10 for now ...
    int one = 1 << (2 * q_format - 32);
    for( int i = 0; i < 10; ++i )
    {
        asm("maccs %0,%1,%2,%3":"=r"(result),"=r"(al):"r"(ah),"r"(ah),"0"(0),"1"(1<<(q_format-1)) );
        asm("lsats %0,%1,%2":"=r"(result),"=r"(al):"r"(q_format),"0"(result),"1"(al));
        asm("lextract %0,%1,%2,%3,32":"=r"(result):"r"(result),"r"(al),"r"(q_format));
        asm("maccs %0,%1,%2,%3":"=r"(result),"=r"(al):"r"(result),"r"(-input_value),"0"(one),"1"(1<<(q_format-1)) );
        asm("lsats %0,%1,%2":"=r"(result),"=r"(al):"r"(q_format),"0"(result),"1"(al));
        asm("lextract %0,%1,%2,%3,32":"=r"(result):"r"(result),"r"(al),"r"(q_format));
        asm("maccs %0,%1,%2,%3":"=r"(result),"=r"(al):"r"(ah),"r"(result/2),"0"(0),"1"(1<<(q_format-1)) );
        asm("lsats %0,%1,%2":"=r"(result),"=r"(al):"r"(q_format),"0"(result),"1"(al));
        asm("lextract %0,%1,%2,%3,32":"=r"(result):"r"(result),"r"(al),"r"(q_format));
        ah += result;
    }
    return ah;
}

/** Scalar square root
 * 
 *  This function computes the square root of the input value using the
 *  following steps:
 * 
 *  \code
 *  int result;
 *  result = lib_dsp_math_invsqrroot( input )
 *  result = lib_dsp_math_reciprocal( result )
 *  \endcode
 * 
 *  Example:
 * 
 *  \code
 *  int result;
 *  result = lib_dsp_math_squareroot( sample, 28 );
 *  \endcode
 * 
 *  \param  input_value  Input value for computation.
 *  \param  q_format     Fixed point format (i.e. number of fractional bits).
 *  \returns             The square root of the input value.
 */

int lib_dsp_math_squareroot( int input_value, int q_format )
{
    int ah; unsigned al;
    ah = lib_dsp_math_invsqrroot( input_value, q_format );
    // <FIXME> Determine appropriate initial lower-word value
    //asm( "maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(ah),"r"(input_value),"0"(0),"1"(1<<(q_format-1)) );
    asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(ah),"r"(input_value),"0"(0),"1"(0) );
    asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
    asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
    return ah;
}

/******************************************************************
 * Derived from "Software Manual for the Elementary
 * Functions" by Cody and Waite, fixed point sin/cos chapter.
 ******************************************************************/

// coefficients are scaled up for improved rounding
#define R0 (-11184804*8)
#define R1   (2236879*4)
#define R2   (-212681*2)
#define R3     (11175)

q8_24 lib_dsp_math_sin(q8_24 rad) {
    int finalSign;
    int modulo;
    int sqr;

    if (rad < 0) {
        rad = -rad;
        finalSign = -1;
    } else /* rad >= 0 */ {
        finalSign = 1;
    }
    // Now rad >= 0.

    modulo = lib_dsp_math_multiply(rad, ONE_OVER_HALFPI_Q8_24, 24) >> 24;
    rad -= (modulo >> 2) * PI2_Q8_24;
    if (modulo & 2) {
        finalSign = -finalSign;
        rad -= (PI2_Q8_24+1)>>1;
    }
    if (modulo & 1) {
        rad = ((PI2_Q8_24+1)>>1) - rad;
    }
    sqr = (lib_dsp_math_multiply(rad, rad, 24)+1)>>1;
    return (rad +
            ((lib_dsp_math_multiply(
              lib_dsp_math_multiply(
                lib_dsp_math_multiply(
                  lib_dsp_math_multiply(
                    lib_dsp_math_multiply(R3, sqr, 24) + R2,
                    sqr, 24) + R1,
                  sqr, 24) + R0,
                sqr, 24)
            ,rad, 24)+6)>>4)
            )* finalSign;
}



