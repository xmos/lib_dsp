// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef LIB_DSP_MATH
#define LIB_DSP_MATH

#include "xccompat.h"

#define MIN_INT (0x80000000)
#define MAX_INT (0x7FFFFFFF)

/** Q1.31 fixed point format with 31 fractional bits
 * Explcit type to make it clear which functions use this Q format.
 */
typedef int q1_31;


/** Q8.24 fixed point format with 24 fractional bits
 * Explcit type to make it clear which functions use this Q format.
 */
typedef int q8_24;

// Constants for the Q8.24 format

/** This constant defines the smallest number that is defined in the fixed
 * point range of Q8.24, which is -128.
 */
#define MIN_Q8_24 (0x80000000)

/** This constant defines the biggest number that is defined in the fixed
 * point range of Q8.24, which is 127.999999940395355224609375
 */
#define MAX_Q8_24 (0x7FFFFFFF)

/** This constant is the closest q8_24 fixed point representation of 2 PI.
 */
#define PI2_Q8_24     (105414357)

/** This constant is the closest q8_24 fixed point representation of PI.
 */
#define PI_Q8_24     (105414357>>1)

/** This constant is the q8_24 fixed point representation of PI/2
 */
#define PIHALF_Q8_24   (26353589)

/** This constant is the q8_24 fixed point representation of 2/PI
 */
#define ONE_OVER_HALFPI_Q8_24 (10680707)


/**  Scalar multipliplication
 * 
 *  This function multiplies two scalar values and produces a result according
 *  to fixed-point format specified by the ``q_format`` parameter.
 * 
 *  The two operands are multiplied to produce a 64-bit result which is tested for overflow,
 *  and shifted right by ``q_format`` bits.
 *
 *  Algorithm:
 * 
 *  \code
 *  1) Y = X1 * X2
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
 *  \param  input1_value  Multiply operand #1.
 *  \param  input2_value  Multiply operand #2.
 *  \param  q_format      Fixed point format (i.e. number of fractional bits).
 *  \returns              input1_value * input2_value.
 */

int lib_dsp_math_multiply
(
    int input1_value,
    int input2_value,
    int q_format
);

/**  Scalar saturated multipliplication
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
int lib_dsp_math_multiply_sat(
        int input1_value,
        int input2_value,
        int q_format );


/** Scalar reciprocal
 * 
 *  This function computes the reciprocal of the input value using an iterative
 *  approximation method as follows:
 * 
 *  \code
 *  1) result = 1.0
 *  2) result = result + result * (1 - input * result)
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

int lib_dsp_math_reciprocal( int input_value, int q_format );

/** Signed Division
 *
 *  This function divides two signed integer values and produces a result according
 *  to fixed-point format specified by the ``q_format`` parameter.
 *  It was optimised for performance using a dedicated instruction for unsinged long division.
 *
 *  Example:
 *
 *  \code
 *  q8_24 quotient;
 *  quotient = lib_dsp_math_divide(divident, divisor, 24);
 *  \endcode
 *
 *  \param  dividend     Value to be divided
 *  \param  divisor      Dividing value
 *  \param  q_format     Fixed point format (i.e. number of fractional bits).
 *  \returns             Quotient of dividend/divisor
 */
int lib_dsp_math_divide( int dividend, int divisor, unsigned q_format );

/** Unsigned Division
 *
 *  This function divides two unsigned integer values and produces a result according
 *  to fixed-point format specified by the ``q_format`` parameter.
 *  It was optimised for performance using a dedicated instruction for unsinged long division.
 *
 *  Example:
 *
 *  \code
 *  unsigned quotient;
 *  quotient = lib_dsp_math_divide(divident, divisor, 24);
 *  \endcode
 *
 *  \param  dividend     Value to be divided
 *  \param  divisor      Dividing value
 *  \param  q_format     Fixed point format (i.e. number of fractional bits).
 *  \returns             Quotient of dividend/divisor
 */
int lib_dsp_math_divide_unsigned(unsigned dividend, unsigned divisor, unsigned q_format );


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
int lib_dsp_math_invsqrroot( int input_value, int q_format );

/** Scalar square root
 * 
 *  This function computes the square root of an unsigned input value
 *  using the Newton-Raphson approximation method.
 *  Result is irrational so Q5.27 format is used to maximise the precision.
 *
 *  \param  x            Unsigned 32-bit value in Q8.24 format
 *  \returns             Unsigned 32-bit value in Q5.27 format
 */
unsigned lib_dsp_math_squareroot(unsigned x);


/** This function returns the sine of a q8_24 fixed point number in radians. The
 * input number has to be in the range -MIN_Q8_24 + PI and MIN_Q8_24 - PI.
 *
 * \param rad input value in radians
 * \returns sine(rad)
 **/
q8_24 lib_dsp_math_sin(q8_24 rad);

/** This function returns the cosine of a q8_24 fixed point number in radians. The
 * input number has to be in the range -MIN_Q8_24 + PI and MIN_Q8_24 - PI.
 *
 * \param rad input value in radians
 * \returns cosine(rad)
 **/
inline q8_24 lib_dsp_math_cos(q8_24 rad) {
    return lib_dsp_math_sin(rad+PIHALF_Q8_24);
}

/** This function returns the arctangent of x.
 *  It uses an algorithm based on Cody and Waite pp. 194-216.
 *  The algorihm was optimised for accuracy (using improved precision and rounding)
 *  and performance (using a dedicated instruction for unsinged long division)
 *  Error compared to atan from math.h is <= 1 LSB. Performance is 84 cycles worst case.
 *
 *  Example:
 *
 *  \code
 *  q8_24 x = Q24(0.005);
 *  q8_24 rad = lib_dsp_math_atan(x);
 *  \endcode
 *
 *  \param x in Q8.24 format.
 *  \returns arctangent of x in radians between -pi/2 .. +pi/2
 */
q8_24 lib_dsp_math_atan(q8_24 x);


#endif
