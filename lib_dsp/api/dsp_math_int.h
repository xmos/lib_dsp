// Copyright (c) 2015-2017, XMOS Ltd, All rights reserved

#ifndef DSP_MATH_INT_H_
#define DSP_MATH_INT_H_

#include "stdint.h"

/** Integer square root 32 -> 16 bits
 *
 *  This function computes the square root of an unsigned input value
 *  use dsp_math_sqrt() for fixed point square roots.
 *
 *  \param  x            Unsigned 32-bit integer value
 *  \returns             Unsigned 32-bit integer value
 */
uint32_t dsp_math_int_sqrt(uint32_t x);

/** Integer square root, 64 -> 32 bits
 *
 *  This function computes the square root of an unsigned input value
 *  use dsp_math_sqrt() for fixed point square roots.
 *
 *  \param  x            Unsigned 64-bit integer value
 *  \returns             Unsigned 32-bit integer value
 */
uint32_t dsp_math_int_sqrt64(uint64_t x);

#endif
