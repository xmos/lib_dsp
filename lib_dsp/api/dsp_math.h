// Copyright (c) 2015-2016, XMOS Ltd, All rights reserved

#ifndef DSP_MATH_H_
#define DSP_MATH_H_

#include "xccompat.h"
#include "stdint.h"


/** Q1.31 fixed point format with 31 fractional bits
 * Explcit type to make it clear which functions are fixed to this Q format.
 */
typedef int32_t  q1_31;
typedef uint32_t uq1_31;

/** Q8.24 fixed point format with 24 fractional bits
 * Explicit type to make it clear which functions are fixed to this Q format.
 */
typedef int32_t  q8_24;
typedef uint32_t uq8_24;

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


#define HALF_Q8_24   (1<<(24-1))

#define ONE_Q8_24    (1<<24)


/**  Scalar multipliplication
 * 
 *  This function multiplies two scalar values and produces a result according
 *  to fixed-point format specified by the ``q_format`` parameter.
 * 
 *  The two operands are multiplied to produce a 64-bit result,
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
 *  int32_t  result;
 *  result = dsp_math_multiply( Q28(-0.33), sample, 28 );
 *  \endcode
 * 
 *  \param  input1_value  Multiply operand #1.
 *  \param  input2_value  Multiply operand #2.
 *  \param  q_format      Fixed point format (i.e. number of fractional bits).
 *  \returns              input1_value * input2_value.
 */

int32_t  dsp_math_multiply
(
    int32_t  input1_value,
    int32_t  input2_value,
    const int32_t q_format
);

/**  Scalar saturated multipliplication
 *
 *  This function multiplies two scalar values and produces a result according
 *  to fixed-point format specified by the ``q_format`` parameter.
 *
 *  The two operands are multiplied to produce a 64-bit result,
 *  saturated at the minimum/maximum value given the fixed-point format if overflow occurs,
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
 *  int32_t  result;
 *  result = dsp_math_multiply_sat( Q28(-0.33), sample, 28 );
 *  \endcode
 *
 *
 *  \param  input1_value  Multiply operand #1.
 *  \param  input2_value  Multiply operand #2.
 *  \param  q_format      Fixed point format (i.e. number of fractional bits).
 *  \returns              input1_value * input2_value.
 */
int32_t  dsp_math_multiply_sat(
    int32_t  input1_value,
    int32_t  input2_value,
    const int32_t q_format 
);


/** Signed Division
 *
 *  This function divides two signed integer values and produces a result according
 *  to fixed-point format specified by the ``q_format`` parameter.
 *  It was optimised for performance using a dedicated instruction for unsinged long division.
 *
 *  Example:
 *
 *  \code
 *  uint32_t  quotient;
 *  quotient = dsp_math_divide(divident, divisor, 24);
 *  \endcode
 *
 *  \param  dividend     Value to be divided
 *  \param  divisor      Dividing value
 *  \param  q_format     Fixed point32_t  format (i.e. number of fractional bits).
 *  \returns             Quotient of dividend/divisor
 */
int32_t  dsp_math_divide( int32_t  dividend, int32_t  divisor, uint32_t  q_format );

/** Unsigned Division
 *
 *  This function divides two unsigned integer values and produces a result according
 *  to fixed-point format specified by the ``q_format`` parameter.
 *  It was optimised for performance using a dedicated instruction for unsinged long division.
 *
 *  Example:
 *
 *  \code
 *  uint32_t  quotient;
 *  quotient = dsp_math_divide_unsigned(divident, divisor, 24);
 *  \endcode
 *
 *  \param  dividend     Value to be divided
 *  \param  divisor      Dividing value
 *  \param  q_format     Fixed point32_t  format (i.e. number of fractional bits).
 *  \returns             Quotient of dividend/divisor
 */
uint32_t dsp_math_divide_unsigned (uint32_t  dividend, uint32_t  divisor, uint32_t  q_format );

/** Scalar square root
 * 
 *  This function computes the square root of an unsigned input value
 *  using the Babylonian method of successive averaging.
 *  Error is <= 1 LSB and worst case performance is 96 cycles.
 *
 *  \param  x            Unsigned 32-bit value in Q8.24 format
 *  \returns             Unsigned 32-bit value in Q8.24 format
 */
uq8_24 dsp_math_sqrt(uq8_24 x);


/** This function returns the sine of a q8_24 fixed point number in radians. The
 * input number has to be in the range -MIN_Q8_24 + PI and MIN_Q8_24 - PI.
 *
 * \param rad input value in radians
 * \returns sine(rad)
 **/
q8_24 dsp_math_sin(q8_24 rad);

/** This function returns the cosine of a q8_24 fixed point number in radians. The
 * input number has to be in the range -MIN_Q8_24 + PI and MIN_Q8_24 - PI.
 *
 * \param rad input value in radians
 * \returns cosine(rad)
 **/
inline q8_24 dsp_math_cos(q8_24 rad) {
    return dsp_math_sin(rad+PIHALF_Q8_24);
}

/** This function returns the arctangent of x.
 *  It uses an algorithm based on Cody and Waite pp. 194-216.
 *  The algorihm was optimised for accuracy (using improved precision and rounding)
 *  and performance (using a dedicated instruction for unsinged long division)
 *  Error compared to atan from math.h is <= 1 LSB. Performance is 84 cycles worst case.
 *  MIN_INT is an invalid input because the algorithm negates all negative inputs 
 *  and there is no positive representation of MIN_INT
 *
 *  Example:
 *
 *  \code
 *  q8_24 x = Q24(0.005);
 *  q8_24 rad = dsp_math_atan(x);
 *  \endcode
 *
 *  \param x input value Q8.24 format.
 *  \returns arctangent of x in radians between -pi/2 .. +pi/2
 */
q8_24 dsp_math_atan(q8_24 x);

#if defined(__XS2A__)
/** Function that computes a fast fixed point atan2 and hypothenuse. The input
 * comprises an array of two integers (notionally a complex number with the
 * real value stored in the first index, and the imaginary value stored in
 * the second index). The output overwrites the input, and comprises a
 * hypothenuse (stored in the first index) and an angle (stored in the
 * second index).
 *
 * The input values are signed fixed point integers [-0x7FFF FFFF..0x7FFF
 * FFFF] with the fixed point in an arbitrary location, the output
 * hypothenuse is an unsigned fixed point integer with the fixed point in
 * the same location. The output angle is a value in the range [-0x4000
 * 0000.. 0x4000 0000] representing the angle; 0 indicates a positive real
 * part and a zero imaginary part, with angles increasing anti clockwise.
 * Hence 0x4000 0000 represents PI radians (180 degrees).
 *
 * The function allows for a trade-off to be made between accuracy and speed.
 *
 * The precision parameter should be set to (24-N) to request N bits
 * accuracy in the hypothenuse and angle. Hence, the value 0 requests a
 * 24-bit accurate hypothenuse/angle, whereas 23 requests a 1-bit accurate
 * hypothenuse/angle. precision must be between 0 and 23 inclusive.
 *
 * Execution time is 31 + 5 N thread cycles; at most 151 thread cycles for
 * 24 bit, at its fastest it will take 36 thread cycles for 1-bit
 * precision. Note that for high precision, atan() with a division may be
 * faster, but that will not compute the hypotenusa.
 *
 * atan2(0,0) returns a 0 hypothenuse and an arbitrary angle
 *
 * Example:
 *
 *  \code
 *  int z[2] = {1000,1000}
 *  dsp_math_atan2_hypot(z, 0);
 *  \endcode
 *
 * results in 1414 (unsigned int stored in z[0]), and 0x0fff ffe1 (signed
 * int, stored in z[1]). The former is 1000*sqrt(2), the latter is a 45
 * degree angle represented by 0x1000 0000 calculated to a precision of 26
 * bits
 *
 * \param  z        array of two signed integers providing real (x) and
 *                  imaginary (y) inputs, and unsigned hypothenuse and
 *                  signed angle outputs
 * \param precision integer that sets the precision; set to 0 for maximum
 *                  precision; and to 23 for no precision; 0 <= precision <= 23.
 */
extern void dsp_math_atan2_hypot(int z[2], unsigned int precision);
#endif


/** This function returns the arcsine of a q8_24 fixed point number in radians. The
 * input number has to be in the range -1..1
 *
 * \param sin input value
 * \returns arcsine(rad) in radians
 **/
q8_24 dsp_math_asin(q8_24 sin);

/** This function returns the arccosine of a q8_24 fixed point number in radians. The
 * input number has to be in the range -1..1
 *
 * \param cos input value
 * \returns arccosine(rad) in radians
 **/
q8_24 dsp_math_acos(q8_24 cos);


/** This function returns the natural exponent of a fixed point number. The
 * input number has to be less than 4.8, otherwise the answer cannot be
 * represented as a fixed point number. For input values larger than 3
 * there is a relatively large error in the last three bits of the answer.
 *
 * \param x input value
 * \returns e^x
 **/
q8_24 dsp_math_exp(q8_24 x);

/** This function returns the natural logarithm (ln) of a fixed point number. The
 * input number has to be positive.
 *
 * \param x input value Q8.24 format.
 * \returns ln(x).
 **/
q8_24 dsp_math_log(uq8_24 x);

extern q8_24 dsp_math_sinh_(q8_24 x, int cosine);

/** This function returns the hyperbolic sine (sinh) of a fixed point
 * number. The input number has to be in the range [-5.5..5.5] in order to
 * avoid overflow, and for values outside the [-4..4] range there are
 * errors up to 4 bits in the result.
 *
 * \param x input value Q8.24 format.
 * \returns sinh(x)
 **/
inline q8_24 dsp_math_sinh(q8_24 x) {
    return dsp_math_sinh_(x, 0);
}

/** This function returns the hyperbolic cosine (cosh) of a fixed point
 * number. The input number has to be in the range [-5.5..5.5] in order to
 * avoid overflow, and for values outside the [-4..4] range there are
 * errors up to 4 bits in the result.
 *
 * \param x input value Q8.24 format.
 * \returns sinh(x)
 **/
inline q8_24 dsp_math_cosh(q8_24 x) {
    return dsp_math_sinh_(x, 1);
}


#endif
