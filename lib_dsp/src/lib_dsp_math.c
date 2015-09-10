// Copyright (c) 2015, XMOS Ltd, All rights reserved

#include <platform.h>
#include "lib_dsp_qformat.h"
#include "lib_dsp_math.h"

/**  Scalar multipliplication
 * 
 *  This function multiplies two scalar values and produces a result according
 *  to fixed-point format specified by the ``q_format`` parameter.
 * 
 *  The two operands are multiplied to produce a 64-bit result which is tested for overflow,
 *  clamped at the minimum/maximum value given the fixed-point format if overflow occurs,
 *  and finally shifted right by ``q_format`` bits. 
 *
 *  Algorithm:
 * 
 *  \code
 *  1) Y = X1 * X2
 *  2) Y = min( max( Q_FORMAT_MIN, Y ), Q_FORMAT_MAX, Y )
 *  3) Y = Y >> q_format
 *  \endcode
 *
 *  Example:
 * 
 *  \code
 *  int result;
 *  result = lib_dsp_math_multiply( Q28(-0.33), sample, 28 );
 *  \endcode
 * 
 *  While saturation is employed after multiplication an overflow condition when preparing the final
 *  result must still be considered when specifying a Q-format whose fixed-point numerical range do
 *  not accomodate the final result of multiplication and saturation (if applied).
 * 
 *  \param  input1_value  Multiply operand #1.
 *  \param  input2_value  Multiply operand #2.
 *  \param  q_format      Fixed point format (i.e. number of fractional bits).
 *  \returns              input1_value * input2_value.
 */

int lib_dsp_math_multiply( int input1_value, int input2_value, int q_format )
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
