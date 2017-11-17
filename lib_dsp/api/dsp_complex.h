// Copyright (c) 2016-2017, XMOS Ltd, All rights reserved

#ifndef DSP_COMPLEX_H_
#define DSP_COMPLEX_H_

#include <stdint.h>

/** Type that represents a complex number. Both the real and imaginary
 * parts are represented as 32-bit fixed point values, with a Q value that
 * is dependent on the use case
 */
typedef struct {
    int32_t re;
    int32_t im;
} dsp_complex_t;

/** Type that represents a complex number. Both the real and imaginary
 * parts are represented as 16-bit fixed point values, with a Q value that
 * is dependent on the use case
 */
typedef struct {
    int16_t re;
    int16_t im;
} dsp_complex_short_t;

/** Function that adds two complex numbers that use the same fixed point
 * representation
 *
 * \param[in] a   first complex number
 * \param[in] b   second complex number
 *
 * \returns       sum - it may overflow.
 */
dsp_complex_t dsp_complex_add(dsp_complex_t a, dsp_complex_t b);

/** Function that subtracts two complex numbers that use the same fixed point
 * representation
 *
 * \param[in] a   first complex number
 * \param[in] b   second complex number
 *
 * \returns       difference - it may overflow.
 */
dsp_complex_t dsp_complex_sub(dsp_complex_t a, dsp_complex_t b);

/** Function that multiplies two complex numbers. The fixed point
 * representation of one number has to be specified, the result will use
 * the fixed point representation of the other number.
 *
 * \param[in] a   first complex number
 * \param[in] b   second complex number
 * \param[in] Q   Number of bits behind the binary point in one of the numbers
 * 
 * \returns       product - it may overflow.
 */
dsp_complex_t dsp_complex_mul(dsp_complex_t a, dsp_complex_t b, uint32_t Q);

/** Function that multiplies one complex number with the conjugate of
 * another complex number. The fixed point representation of one number has
 * to be specified, the result will use the fixed point representation of
 * the other number.
 *
 * \param[in] a   first complex number
 * \param[in] b   second complex number
 * \param[in] Q   Number of bits behind the binary point in one of the numbers
 * 
 * \returns       product - it may overflow.
 */
dsp_complex_t dsp_complex_mul_conjugate(dsp_complex_t a, dsp_complex_t b, uint32_t Q);

/** Function that computes the inner product of two complex vectors. The
 * representation of one vector has to be specified, the result will use
 * the fixed point representation of the other number.
 *
 * \param[in] a   first complex vector
 * \param[in] b   second complex vector
 * \param[in] N   Length of the vectors
 * \param[in] offset  starting point to use in vector a
 * \param[in] Q   Number of bits behind the binary point in one of the vectors
 * 
 * \returns       inner product - it may overflow.
 */
dsp_complex_t dsp_complex_fir(dsp_complex_t a[], dsp_complex_t b[],
                              uint32_t N, uint32_t offset, uint32_t Q);

/** Function that computes the element-by-element product of two complex
 * vectors. The representation of one vector has to be specified, the
 * result will use the fixed point representation of the other number.
 *
 *   a = a * b
 *
 * \param[in,out] a   first complex vector, also output
 * \param[in]     b   second complex vector
 * \param[in]     N   Length of the vectors
 * \param[in]     Q   Number of bits behind the binary point in one of the
 *                    vectors
 */
void dsp_complex_mul_vector(dsp_complex_t a[], dsp_complex_t b[],
                            uint32_t N, uint32_t Q);

/** Function that computes the element-by-element product of two complex
 * vectors, where the complex conjugate of the second vector is used. The
 * representation of one vector has to be specified, the result will use
 * the fixed point representation of the other number::
 *
 *   a = a * conj(b)
 *
 * \param[in,out] a   first complex vector, also output
 * \param[in]     b   second complex vector
 * \param[in]     N   Length of the vectors
 * \param[in]     Q   Number of bits behind the binary point in one of the
 *                    vectors
 */
void dsp_complex_mul_conjugate_vector(dsp_complex_t a[], dsp_complex_t b[],
                                      uint32_t N, uint32_t Q);

/** Function that computes the element-by-element product of two complex
 * vectors, where the complex conjugate of the second vector is used. The
 * representation of one vector has to be specified, the result will use
 * the fixed point representation of the other number::
 *
 *   o = a * conj(b)
 *
 * \param[out]    o   first complex vector, also output
 * \param[in]     a   first complex vector, also output
 * \param[in]     b   second complex vector
 * \param[in]     N   Length of the vectors
 * \param[in]     Q   Number of bits behind the binary point in one of the
 *                    vectors
 */
void dsp_complex_mul_conjugate_vector3(dsp_complex_t o[],
                                       dsp_complex_t a[],
                                       dsp_complex_t b[],
                                      uint32_t N, uint32_t Q);

/** Function that computes the element-by-element sum of two complex vectors::
 *
 *   a = a + b
 *
 * \param[in,out] a   first complex vector, also output
 * \param[in]     b   second complex vector
 * \param[in]     N   Length of the vectors
 */
void dsp_complex_add_vector(dsp_complex_t a[], dsp_complex_t b[],
                            uint32_t N);

/** Function that computes the element-by-element sum of two complex vectors,
 * scaling one vector by a fixed number of bit positions::
 *
 *   a = a + (b << shift)
 *
 * \param[in,out] a   first complex vector, also output
 * \param[in]     b   second complex vector
 * \param[in]     N   Length of the vectors
 * \param[in]     shift Number of bits to shift b left by prior to adding.
 *                      negative values indicate right shift.
 */
void dsp_complex_add_vector_shl(dsp_complex_t a[], dsp_complex_t b[],
                                uint32_t N, int32_t shift);

/** Function that computes the element-by-element sum of two complex vectors,
 * scaling one vector by a fixed number of bit positions::
 *
 *   a = a + ((b * scalar) >> 24)
 *
 * \param[in,out] a   first complex vector, also output
 * \param[in]     b   second complex vector
 * \param[in]     N   Length of the vectors
 * \param[in]     scalar Number to multiply b with, in q8_24 notation.
 *                       Use 0x01000000 to mimic dsp_complex_add_vector().
 */
void dsp_complex_add_vector_scale(dsp_complex_t a[], dsp_complex_t b[],
                                  uint32_t N, int32_t scalar);

/** Function that computes the element-by-element sum of two complex vectors::
 *
 *   a = a - b
 *
 * \param[in,out] a   first complex vector, also output
 * \param[in]     b   second complex vector
 * \param[in]     N   Length of the vectors
 */
void dsp_complex_sub_vector(dsp_complex_t a[], dsp_complex_t b[],
                            uint32_t N);

/** Function that computes the element-by-element difference of two
 * complex vectors, into a third vector::
 *
 *   o = a + b
 *
 * \param[out]    o   output complex vector
 * \param[in]     a   first complex vector
 * \param[in]     b   second complex vector
 * \param[in]     N   Length of the vectors
 */
void dsp_complex_add_vector3(dsp_complex_t o[], dsp_complex_t a[],
                             dsp_complex_t b[], uint32_t N);

/** Function that computes the element-by-element difference of two
 * complex vectors, into a third vector::
 *
 *   o = a - b
 *
 * \param[out]    o   output complex vector
 * \param[in]     a   first complex vector
 * \param[in]     b   second complex vector
 * \param[in]     N   Length of the vectors
 */
void dsp_complex_sub_vector3(dsp_complex_t o[], dsp_complex_t a[],
                             dsp_complex_t b[], uint32_t N);

/** Function that computes the element-by-element product of two complex
 * vectors, and adds the result to a third vector::
 *
 *   a = a + b * c
 *
 * \param[in,out] a   first complex vector, also output
 * \param[in]     b   second complex vector
 * \param[in]     c   third complex vector
 * \param[in]     N   Length of the vectors
 * \param[in]     Q   Number of bits behind the binary point in one of the numbers
 */
void dsp_complex_macc_vector(dsp_complex_t a[], dsp_complex_t b[],
                             dsp_complex_t c[], uint32_t N, int Q);

/** Function that computes the element-by-element product of two complex
 * vectors, and subtracts the result from a third vector::
 *
 *   a = a - b * c
 *
 * \param[in,out] a   first complex vector, also output
 * \param[in]     b   second complex vector
 * \param[in]     c   third complex vector
 * \param[in]     N   Length of the vectors
 * \param[in]     Q   Number of bits behind the binary point in one of the numbers
 */
void dsp_complex_nmacc_vector(dsp_complex_t a[], dsp_complex_t b[],
                              dsp_complex_t c[], uint32_t N, int Q);

/** Function that multiplies a complex vector with a scalar, and shifts
 * the data down. The scalar is a 8.24 number
 *
 *   a = scalar * b >> Q
 *
 * \param[in,out] a   first complex vector, also output
 * \param[in]     b   second complex vector
 * \param[in]     N   Length of the vectors
 * \param[in]     Q   Number of bits behind the binary point in one of the
 *                    vectors
 */
void dsp_complex_scalar_vector3(dsp_complex_t a[], dsp_complex_t b[],
                                uint32_t N, int32_t scalar, uint32_t Q);

/** Function that computes the magnitude of an array of complex numbers
 *
 * The function allows for a trade-off to be made between accuracy and
 * speed. The precision parameter should be set to (24-P) to request P bits
 * accuracy in the magnitude. Hence, the value 0 requests a 24-bit accurate
 * magnitude, whereas 23 requests a 1-bit accurate magnitude. precision
 * must be between 0 and 23 inclusive.
 *
 * Execution time is ((31 + 5 N) x P) thread cycles.
 *
 * \param [out] magnitude array in which magnitudes are stored
 * \param [in]  input     Array of complex numbers of which to compute magnitude
 * \param [in]  N         Number of elements in the input and output arrays
 * \param [in]  precision integer that sets the precision; set to 0 for maximum
 *                        precision; and to 23 for no precision;
 *                         0 <= precision <= 23.
 */
extern void dsp_complex_magnitude_vector(uint32_t magnitude[],
                                         dsp_complex_t input[],
                                         uint32_t N, uint32_t P);

/** Function that scales an array of complex numbers by a fraction. It
 * requires an array of complex number, and an array of numerators and an
 * array of denomiators. It computes:
 *
 *    array = array * numerator/denominator
 *
 * Note that noth numerator and denominator are unsigned values.
 * 
 * The typical calling sequence below may be used to reset the magnitude of
 * each element of a complex vector to a new value::
 *
 *   dsp_complex_magnitude_vector(cur_magnitude, input, N, 0);
 *   dsp_complex_remagnitude_vector(input, cur_magnitude, new_mag, N);
 *
 * \param [in,out] array   Array of complex numbers on which to reset magnitude
 * \param [in] numerator Array of ints that store the current magnitude 
 * \param [in] out_magnitude Array of ints that store the desired magnitude
 * \param [in]  N         Number of elements in the input and output arrays
 */
extern void dsp_complex_scale_vector(dsp_complex_t array[],
                                     uint32_t numerator[],
                                     uint32_t denominator[],
                                     uint32_t N);

#endif
