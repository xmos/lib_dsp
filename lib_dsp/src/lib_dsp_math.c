// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <platform.h>
#include "lib_dsp_qformat.h"
#include "lib_dsp_math.h"


int lib_dsp_math_multiply( int input1_value, int input2_value, int q_format )
{
    int ah; unsigned al;
    int result;
    // For rounding, accumulator is pre-loaded (1<<(q_format-1))
    asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(input1_value),"r"(input2_value),"0"(0),"1"(1<<(q_format-1)));
    asm("lextract %0,%1,%2,%3,32":"=r"(result):"r"(ah),"r"(al),"r"(q_format));
    return result;
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


// The below algorithm is from ftp://ftp.update.uu.se/pub/pdp11/rt/cmath/atan.c
// It is based on Cody and Waite

/*
 *              a t a n . c
 */

/*)LIBRARY
*/

#ifdef  DOCUMENTATION
title   atan    arctan function
index   arctan function atan
index   arctan function atan2
usage
    .s
    double f, x, atan();
    .br
    f = atan(x);
    .br
    double g, u, v, atan2();
    .br
    g = atan2(v, u);
    .s
description
    .s
    atan(x) returns the arctangent of x.
    .s
    atan2(v, u) returns the arctangent of v / u.
    .s
diagnostics
    .s
    If both arguments of atan2 are zero the result is indeterminate.
    The message 'atan2 args both zero' followed by the value of the
    denominator, is written to stderr and a value of zero is returned.
    .s
internal
    .s
    Algorithm based on Cody and Waite pp. 194-216.  The algorithm here
    has been modified slightly to avoid the test for overflow in v/u.
    By checking which of u or v is greater in magnitude all overflows
    become underflows which are set to zero without a message.  This
    has meant that the division of the work between atan(), atan2()
    and the internal function _atan() is slightly different from that
    suggested in Cody and Waite.
    .s
author
    .s
    Hamish Ross.
    .s
date
    .s
    1-Jan-85
#endif

#include <math.h>

#define PI 3.1415926536
#define HALF_PI PI/2
#define PI_BY_2 HALF_PI //why use a different constant for the same thing?

q8_24 _lib_dsp_math_atan(q8_24 f, int n);

//TODO: Fix issues:
// For x > 1, lib_dsp_math_atan(x) jumps from 0.5235988 (33.33% too small) to the maximum value pi/2 ( 1.5707963)
// That means this mode is not working: if (f > Q24(1.0))
// This also means that the else clause ( f = _lib_dsp_math_atan(f, 0);) produces values that are too small
q8_24 lib_dsp_math_atan(q8_24 x)
//q8_24 x;
{
    //q8_24 _lib_dsp_math_atan(), f;
    q8_24 f;

    f = x < 0 ? -x : x;
    if (f > Q24(1.0))
        f = _lib_dsp_math_atan(Q24(1.0) / f, 2);
    else
        f = _lib_dsp_math_atan(f, 0);
    return(x < 0 ? -f : f);
}

//returns the arctangent of v / u.
q8_24 lib_dsp_math_atan2(q8_24 v, q8_24 u)
//q8_24 v, u;
{
    //q8_24 _lib_dsp_math_atan(), au, av, f;
    q8_24  au, av, f;

    av = v < 0 ? -v : v;
    au = u < 0 ? -u : u;
    if (u != 0) {
    if (av > au) {
        if ((f = au / av) == 0) {
            f = PIHALF_Q8_24;
        }
        else {
            f = _lib_dsp_math_atan(f, 2);
        }
    }
    else {
        if ((f = av / au) == 0)
            f = 0;
        else
            f = _lib_dsp_math_atan(f, 0);
    }
    }
    else {
        if (v != 0) {
            f = PIHALF_Q8_24;
        }
        else {
            printf("ERROR: atan2 args both zero");
            f = 0;
        }
    }
    if (u < 0)
    f = PI - f;
    return(v < 0 ? -f : f);
}

static q8_24  p0 = Q24(-0.136887688941919269e2);
static q8_24  p1 = Q24(-0.205058551958616520e2);
static q8_24  p2 = Q24(-0.849462403513206835e1);
static q8_24  p3 = Q24(-0.837582993681500593e0);
static q8_24  q0 = Q24( 0.410663066825757813e2);
static q8_24  q1 = Q24( 0.861573495971302425e2);
static q8_24  q2 = Q24( 0.595784361425973445e2);
static q8_24  q3 = Q24( 0.150240011600285761e2);
static q8_24 a[] = {
    0,
    Q24(0.523598775598298873),       /* pi / 6           */
    PIHALF_Q8_24,                /* pi / 2           */
    Q24(1.047197551196597746)        /* pi / 3           */
};

#define CON1     Q24(0.267949192431122706)   /* 2 - sqrt(3)          */
#define ROOT_3M1 Q24(0.732050807568877294)   /* sqrt(3) - 1          */
#define ROOT_3   Q24(1.732050807568877)
// From ftp://ftp.update.uu.se/pub/pdp11/rt/cmath/math.h:
#define ROOT_EPS Q24(0.372529029846191406e-8)    /* 2**-(t/2), t = 56    */

q8_24 _lib_dsp_math_atan(q8_24 f, int n)
//q8_24 f;
//int n;
{
    q8_24 g, q, r;
    q8_24 half = Q24(0.5);
    if (f > CON1) {
        //f = (((ROOT_3M1 * f - 0.5) - 0.5) + f) / (ROOT_3 + f);
        f = (((lib_dsp_math_multiply(ROOT_3M1, f, 24) - half) - half) + f) / (ROOT_3 + f);
        n++;
    }
    if (f > ROOT_EPS || f < -ROOT_EPS) {
        //g = f * f;
        g = lib_dsp_math_multiply(f, f, 24);

        //q = (((g + q3)*g + q2)*g + q1)*g + q0;
        q = lib_dsp_math_multiply(
                lib_dsp_math_multiply(
                    lib_dsp_math_multiply((g + q3), g, 24) + q2,
                g, 24) + q1,
            g, 24)
            + q0;
        //r = (((p3*g + p2)*g + p1)*g + p0)*g / q;
        r = lib_dsp_math_multiply(
                lib_dsp_math_multiply(
                    lib_dsp_math_multiply(
                        lib_dsp_math_multiply(p3, g, 24) + p2,
                    g, 24) + p1,
                g, 24) + p0,
            g, 24);

        r =  r / q; // rational function (division 5th order polynomial by 4th order polynomial)
        f = f + lib_dsp_math_multiply(f, r, 24);
    }
    if (n > 1)
        f = -f;
    return(f + a[n]);
}


