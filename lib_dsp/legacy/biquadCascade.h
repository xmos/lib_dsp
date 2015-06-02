// Copyright (c) 2011, XMOS Ltd, All rights reserved
// This software is freely distributable under a derivative of the
// University of Illinois/NCSA Open Source License posted in
// LICENSE.txt and at <http://github.xcore.com/>
/*! \file */

#ifndef _BIQUAD_CASCADE_H_
#define _BIQUAD_CASCADE_H_

#include "coeffs.h"

/* Structure containing sample values for one bank, used internally only */
struct oneBank {
    int xn1;
    int xn2;
    int db;
};

/** This structure defines the state that is required to compute a series
 * of biquads on a channel. It holds both data that is used internally, and
 * data that is accessible to the end user. One of these structures mus be
 * declared for each channel of samples that is to be filtered
 * independently.
 */
typedef struct biquadState {
    struct oneBank b[BANKS+1]; /**< data that holds intermediate values for
                                *   biquad computation. Internal use only,
                                *   do not modify
                                */
    int adjustDelay;           /**< variable that holds the amount of time
                                *   until the next time that the db values
                                *   are to be adjusted. Internal use only.
                                */
    int adjustCounter;         /**< variable that holds the index of the
                                *   filter bank to be adjusted next.
                                *   Internal use only.
                                */
    int desiredDb[BANKS];      /**< variable that holds the *desired* dB
                                *   value for each bank in the filter. This
                                *   array can be modified at any time, but
                                *   changes will only take effect slowly in
                                *   order to prevent the filter from
                                *   becoming instable.
                                */
} biquadState;

/** This structure holds the coefficient values for one particular filter
 * configuration ,for example the filter settings for a 400 Hz low-shelf
 * filter at -10 dB. The values stored are fixed point values (as defined
 * by *FRACTIONALBITS*) and are all normalised against the value of ``a0``.
 * The five values represent the normal values for a biquad filter (see for
 * example the DSP cookbook), ie, b0 is multiplied by the current sample
 * value and should have been pre-divided by a0.
 */
struct coeff {
    int b0,      /**< b0/a0 value, in fixed point representation */
        b1,      /**< b1/a0 value, in fixed point representation */
        b2,      /**< b2/a0 value, in fixed point representation */
        a1,      /**< a1/a0 value, in fixed point representation */
        a2;      /**< a2/a0 value, in fixed point representation */
};

/** This array should hold the values of all coefficients that are to be
 * used. It comprises *DBS* rows of *BANKS* columns: each element holds a
 * set of biquad coefficient for one bank for one particular dB setting.
 * The coefficients should not be changed on-the-fly, because that may
 * cause the filters to become unstable. They can be changed safely prior
 * to calling initBiquads().
 *
 * This module assumes that all channels use the same coefficients, but
 * each channel may have different db Settings per filter. That means, that
 * all channels will use the same number of filters, the same corner
 * frequencies, and the same number of dB levels. Channels can have their
 * own dB setting per filter. Subsequent dB steps shoudl be small (1-2 dB)
 * if audible clicks are to be prevented when changing the filter settings.
 */
extern struct coeff biquads[DBS][BANKS];

/** This function must be called prior to using the biquadCascade, for
 * every single channel on which the biquads are to operate. The state
 * variable is initialised with the value held in zeroDb, which should be
 * an index into the array of coefficients (typically you want it to be the
 * index at the array which holds the coefficient values for 0db).
 *
 * For example, if you have an array of 11 coefficients which are values
 * from -24db to 6db in steps of 3db, and you have two channels that you
 * both want to initialise at 0db, then you need to declare two variables
 * of type biquadState, and call initBiquads for each of those variables
 * with the value 8 for zeroDb
 *
 * \param state  variable that holds the state for a channel. This includes
 *               previous samples, intermediate results, current db level,
 *               and desired db levels.
 *
 * \param zeroDb initial index into the coefficient array for all banks for
 *               this channel
 * 
 */
extern void initBiquads(biquadState &state, int zeroDb);

/** This function performs a series of biquad operations in sequence on a
 * singe sample value, and returns the filtered sample value. The state
 * variable stores the previous values of the stream and intermediate
 * values in order to implement an IIR filter. The number of banks and the
 * number of possible steps for the filter banks are hard coded in a file
 * called 'coefficients.h' and are to be supplied by the user.
 *
 * \param state  variable that holds the state for a channel. This includes
 *               previous samples, intermediate results, current db level,
 *               and desired db levels. By changing the value
 *               state.desiredDb to a new level, the filter settings can be
 *               changed.
 *
 * \param xn value to be filtered in fixed point format. Results that do
 *               not fit are clipped to the maximum positive and negative
 *               values. Input values should nominally be in the range
 *               [-1..+1] leaving headroom for intermediate results.
 *
 * \return       Filtered value in fixed point format. 
 */
extern int biquadCascade(biquadState &state, int xn);

#endif /* _BIQUAD_CASCADE_H_ */
