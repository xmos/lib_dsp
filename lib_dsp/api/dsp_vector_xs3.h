// Copyright 2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#ifndef DSP_DOT_XS3_H_
#define DSP_DOT_XS3_H_

#include <stdint.h>

/** This function TBD
 *
 * \param       N       Number of values in the input arrays.
 * \param       ptr1    First array of float values.
 * \param       ptr2    Second array of float values.
 * \param [out] ptro    Array containing the computed values
 *
 */
void FFVectMac_xs3(
                uint32_t N,
                float *ptr1,
                float *ptr2,
                float *ptro);

/** This function TBD
 *
 * \param       N       Number of values in the input arrays.
 * \param       ptr1    First array of float values.
 * \param       ptr2    Second array of float values.
 * \param [out] ptro    Array containing the computed values
 *
 */
void FFVectMacC_xs3(
                uint32_t N,
                float *ptr1,
                float *ptr2,
                float *ptro);

/** This function TBD
 *
 * \param       N       Number of values in the input arrays.
 * \param       ptr1    First array of float values.
 * \param       ptr2    Second array of float values.
 * \param [out] ptro    Array containing the computed values
 *
 */
void FFVectMpy_xs3(
                uint32_t N,
                float *ptr1,
                float *ptr2,
                float *ptro);

/** This function TBD
 *
 * \param       N       Number of values in the input arrays.
 * \param       ptr1    First array of float values.
 * \param       ptr2    Second array of float values.
 * \param [out] ptro    Array containing the computed values
 *
 */
void FFVectMpyC_xs3(
                uint32_t N,
                float *ptr1,
                float *ptr2,
                float *ptro);

/** This function TBD
 *
 * \param       N       Number of values in the input arrays.
 * \param       ptr1    First array of float values.
 * \param [out] ptro    Array containing the computed values
 *
 */
void FFVectAcc_xs3(
                uint32_t N,
                float *ptr1,
                float *ptro);

#endif
