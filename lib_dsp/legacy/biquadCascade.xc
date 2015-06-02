// Copyright (c) 2011, XMOS Ltd, All rights reserved
// This software is freely distributable under a derivative of the
// University of Illinois/NCSA Open Source License posted in
// LICENSE.txt and at <http://github.xcore.com/>

#include <stdio.h>
#include <xs1.h>
#include <print.h>
#include "biquadCascade.h"

void initBiquads(biquadState &state, int zeroDb) {
    for(int i = 0; i <= BANKS; i++) {
        state.b[i].xn1 = 0;
        state.b[i].xn2 = 0;
    }
    for(int i = 0; i < BANKS; i++) {
        state.b[i].db = zeroDb;
        state.desiredDb[i] = zeroDb;
    }
    state.adjustCounter = BANKS;
    state.adjustDelay = 0;
}

extern int biquadAsm(int xn, biquadState &state);

#pragma unsafe arrays
int biquadCascade(biquadState &state, int xn) {

#if 1
    xn = biquadAsm(xn, state);
#else
    for(int j=0; j<BANKS; j++) {
        ynl = (1<<(FRACTIONALBITS-1));        // 0.5, for rounding, could be triangular noise
        ynh = 0;
        // biquad formula:  y[n] = (b0/a0)*x[n] + (b1/a0)*x[n-1] + (b2/a0)*x[n-2]
        //                                      - (a1/a0)*y[n-1] - (a2/a0)*y[n-2]  
        // where y[n-1] in stage j is x[n-1] from the previous iteration of stage j+1
        // source: biquad EQ cookbook by Robert Bristow-Johnson
        //
        {ynh, ynl} = macs( biquads[state.b[j].db][j].b0, xn, ynh, ynl);
        {ynh, ynl} = macs( biquads[state.b[j].db][j].b1, state.b[j].xn1, ynh, ynl);
        {ynh, ynl} = macs( biquads[state.b[j].db][j].b2, state.b[j].xn2, ynh, ynl);
        {ynh, ynl} = macs( biquads[state.b[j].db][j].a1, state.b[j+1].xn1, ynh, ynl);
        {ynh, ynl} = macs( biquads[state.b[j].db][j].a2, state.b[j+1].xn2, ynh, ynl);
        if (sext(ynh,FRACTIONALBITS) == ynh) {
            ynh = (ynh << (32-FRACTIONALBITS)) | (ynl >> FRACTIONALBITS);
        } else if (ynh < 0) {
            ynh = 0x80000000;
        } else {
            ynh = 0x7fffffff;
        }
        state.b[j].xn2 = state.b[j].xn1;
        state.b[j].xn1 = xn;

        xn = ynh;
    }
    state.b[BANKS].xn2 = state.b[BANKS].xn1;
    state.b[BANKS].xn1 = ynh;
    if (state.adjustDelay > 0) {
        state.adjustDelay--;
    } else {
        state.adjustCounter--;
        if (state.b[state.adjustCounter].db > state.desiredDb[state.adjustCounter]) {
            state.b[state.adjustCounter].db--;
        }
        if (state.b[state.adjustCounter].db < state.desiredDb[state.adjustCounter]) {
            state.b[state.adjustCounter].db++;
        }
        if (state.adjustCounter == 0) {
            state.adjustCounter = BANKS;
        }
        state.adjustDelay = 40;
    }
#endif
    return xn;
}

