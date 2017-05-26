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

/** Function that computes the inner product of two complex vectors. The
 * representation of one vector has to be specified, the result will use
 * the fixed point representation of the other number.
 *
 * \param[in] a   first complex vector
 * \param[in] b   second complex vector
 * \param[in] L   Length of the vectors
 * \param[in] offset  starting point to use in vector a
 * \param[in] Q   Number of bits behind the binary point in one of the vectors
 * 
 * \returns       inner product - it may overflow.
 */
dsp_complex_t dsp_complex_fir(dsp_complex_t a[], dsp_complex_t b[],
                              uint32_t L, uint32_t offset, uint32_t Q);

#endif
