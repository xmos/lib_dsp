// Copyright (c) 2016-2017, XMOS Ltd, All rights reserved

#ifndef DSP_BFP_H_
#define DSP_BFP_H_

#include <stdint.h>
#include <dsp_complex.h>

#if defined(__XS2A__)

/** This function computes the number of leading sign bits in an array of
 * complex numbers. This can be used to calculate the current headroom, and
 * using dsp_bfp_shl to increase the headroom or to reduce the headroom (in
 * order to increase dynamic range).
 *
 * Requires at most 7 x N instructions.
 *
 * \param[in]     pts   Array of dsp_complex_t elements.
 * \param[in]     N     Number of points.
 * 
 * \returns       number of bits of headroom
 */
uint32_t dsp_bfp_cls(dsp_complex_t pts[], const uint32_t N);

/** This function shifts an array of complex numbers to adjust the headroom
 * It shifts left if the shift argument is positive, or right if the shift
 * argument is negative. Hence, a sequence::
 *
 *   unsigned ls = dsp_bfp_cls(x,N);
 *   dsp_bfp_shl(x, ls-1, N);
 *
 * will result in one leading sign bit; minimising headroom and maximising
 * dynamic range.
 *
 * Requires at most 6 x N instructions.
 *
 * \param[in,out] pts       Array of dsp_complex_t elements.
 * \param[in]     N         Number of points. Must be a power of two.
 * \param[in]     shift     Number of places to shift left by.
 */
void dsp_bfp_shl( dsp_complex_t pts[], const uint32_t N, const int32_t shift );

/** This function shifts an array of complex numbers to adjust the headroom
 * It shifts left if the shift argument is positive, or right if the shift
 * argument is negative. Hence, a sequence::
 *
 *   unsigned ls = dsp_bfp_cls(x,N)
 *   dsp_bfp_shl(x, ls-1, N)
 *
 * will result in one leading sign bit; minimising headroom and maximising
 * dynamic range.
 *
 * Requires at most 6 x N instructions.
 *
 * \param[in,out] pts       Array of dsp_complex_t elements.
 * \param[in]     N         Number of points. Must be a power of two.
 * \param[in]     shift_re  Number of places to shift the real part left by.
 * \param[in]     shift_im  Number of places to shift the imaginary part left by
 */
void dsp_bfp_shl2( dsp_complex_t * unsafe pts, const uint32_t N,
                   const int32_t shift_re, const int32_t shift_im );

/** This function shifts an array of complex numbers to adjust the headroom,
 * whilst simultaneously shuffling the array around in preparation for an FFT.
 * It shifts left if the shift argument is positive, or right if the shift
 * argument is negative.
 *
 * \param[in,out] pts       Array of dsp_complex_t elements.
 * \param[in]     N         Number of points. Must be a power of two.
 * \param[in]     shift     Number of places to shift left by.
 */
void dsp_bfp_bit_reverse_shl( dsp_complex_t pts[], const uint32_t N, const int32_t shift );

#include "bfp_tx_rx.h"

/** Function that transmits data in BFP format to the next DSP block
 *
 * \param x        channel end where to send the data to
 * \param data     array containing the data.
 * \param CHANS    Number of channels in the data
 * \param advance  Number of new elements in the data
 * \param shr      Number of bits that the data has been shifted left by
 */
extern void dsp_bfp_tx(chanend x, int32_t data[],
                       uint32_t CHANS,
                       uint32_t advance,
                       int32_t shr);
extern void dsp_bfp_tx_xc(chanend x, int32_t data[],
                       uint32_t CHANS,
                       uint32_t advance,
                       int32_t shr);

/** Function that initialises a BFP_RX state. The BFP_RX state comprises an
 * array of BFP_RX_STATE_UINT64_SIZE(CHANS, N, advance) uint64_t values.
 * CHANS is the number of channels on which the DSP block operates. N is
 * the total number of samples needed before the DSP block can operate,
 * advance is the number of samples that the DSP block wants the data
 * advanced every iteration.
 *
 * \param state    array that holds state, must be an array declared as
 *                 uint64_t state[BFP_RX_STATE_UINT64_SIZE(CHANS, N, advance)];
 *
 * \param SIZE     Array size: BFP_RX_STATE_UINT64_SIZE(CHANS, N, advance)
 */
extern void dsp_bfp_rx_state_init(uint64_t state[], int SIZE);
extern void dsp_bfp_rx_state_init_xc(uint64_t state[], int SIZE);

/** Function that receives data in BFP format for this DSP block
 *
 * \param x        channel end where to send the data to
 * \param state    array that holds state, must be initialised with a call to
 *                 dsp_bfp_rx_state_init()
 * \param target   array where data should be stored.
 * \param CHANS    Number of elements required for DSP block
 * \param N        Number of elements required for DSP block
 * \param advance  Number of new elements on each iteration
 * \param headroom Desired headroom in number of bits.
 *
 * \returns        Number of bits that the data has been shifted left by
 */
extern int32_t dsp_bfp_rx(chanend x, uint64_t state[], int32_t target[],
                          uint32_t CHANS,
                          uint32_t N,
                          uint32_t advance,
                          uint32_t headroom);
extern int32_t dsp_bfp_rx_xc(chanend x, uint64_t state[], int32_t target[],
                          uint32_t CHANS,
                          uint32_t N,
                          uint32_t advance,
                          uint32_t headroom);

extern void dsp_bfp_tx_pairs(chanend x, dsp_complex_t data[],
                             uint32_t CHANS, uint32_t advance, int32_t shr);

extern int32_t dsp_bfp_rx_pairs(chanend x, uint64_t state[],
                                dsp_complex_t target[],
                                uint32_t CHANS,
                                uint32_t N,
                                uint32_t advance,
                                uint32_t headroom);

#endif

#endif

