// Copyright 2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#ifndef DSP_DOT_XS3_H_
#define DSP_DOT_XS3_H_

#include <stdint.h>

/** This function computes the dot product of 2 arrays of floating-point values
 *
 * \param   ptr1    First array of float values.
 * \param   ptr2    Second array of float values.
 * \param   N       Number of values in the input arrays.
 *
 * \returns         floating-point value with the resulting dot product
 */
float dsp_vect_dot_prod_xs3(
                float *ptr1,
                float *ptr2,
                uint32_t N);

/** This function computes the dot product of 2 arrays of floating-point values using the VPU
 *
 * \param   ptr1    First array of float values.
 * \param   ptr2    Second array of float values.
 * \param   N       Number of values in the input arrays.
 *
 * \returns         floating-point value with the resulting dot product
 */
float dsp_vect_dot_prod_fast_xs3(
                float *ptr1,
                float *ptr2,
                uint32_t N);
#endif
