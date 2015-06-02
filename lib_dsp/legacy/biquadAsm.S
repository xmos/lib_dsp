// Copyright (c) 2011, XMOS Ltd, All rights reserved
// This software is freely distributable under a derivative of the
// University of Illinois/NCSA Open Source License posted in
// LICENSE.txt and at <http://github.xcore.com/>

#include "coeffs.h"

#define NWORDS 10
 //   .cc_top biquadAsm, biquadAsm.func
    
    .globl biquadAsm
    .globl biquadAsm.nstackwords
    .linkset biquadAsm.nstackwords,NWORDS
    .align 4
biquadAsm:
    entsp NWORDS
    stw   r4, sp[0]
    stw   r5, sp[1]
    stw   r6, sp[2]
    stw   r7, sp[3] // change to make sure that commit goes through to github
    stw   r8, sp[4]
    stw   r9, sp[5]
    stw   r10, sp[6]

    // r0: xn
    // r1: &state.xn12dbs[j]
    // r2: state.xn1[j]
    // r3: state.xn2[j]
    // r4: tmp
    // r5: FRACTIONALBITS
    // r6: 1<<(FRACTIONALBITS-1)
    // r7: &coeffs[0][j]
    // r8: j
    // r9: &coeffs[state.dbs[j]][j]
    // r10: ynl
    // r11: ynh

    ldc   r5, FRACTIONALBITS
    ldaw  r7, dp[biquads]
    ldc   r8, 0

    ldw   r2, r1[0]
    ldw   r3, r1[1]

    ldc   r11, 0
    ldc   r6, 1
    shl   r6, r6, r5
    shr   r6, r6, 1

.xtaloop BANKS    
loop:
    ldc   r4, 5*BANKS       // words per filter * BANKS
    ldw   r9, r1[2]
    or    r10, r6, r6
    mul   r4, r4, r9
    ldaw  r9, r7[r4]

    ldw   r4, r9[0]               // Coefficient b0
    maccs r11, r10, r4, r0        // Multiply by xn
    ldw   r4, r9[1]               // Coefficient b1
    maccs r11, r10, r4, r2        // Multiply by xn1
    ldw   r4, r9[2]               // Coefficient b2
    maccs r11, r10, r4, r3        // Multiply by xn2

    stw   r2, r1[1]               // Save xn1 into xn2
    stw   r0, r1[0]               // Save xn into xn1
    ldaw  r1, r1[3]               // Advance XN pointer to next XN row.

    ldw   r4, r9[3]               // Coefficient a1
    ldw   r2, r1[0]               // Load yn1 (next filter xn1)
    maccs r11, r10, r4, r2
    ldw   r4, r9[4]               // Coefficient a2
    ldw   r3, r1[1]               // Load yn2 (next filter xn2)
    maccs r11, r10, r4, r3

    ldaw  r7, r7[5]               // Advance to next filter by adjusting with 5 words per filter

    or    r4, r11, r11            // copy r11 into r4, to check for overflow
    sext  r4, r5                  // because sext sign extends in place
    eq    r4, r11, r4             // If sext makes no change, result is ok
    bt    r4, resultFits
    ldc   r0, 0x80                // not ok, create MININT or MAXINT
    shl   r0, r0, 24
    lsu   r4, r11, r4
    sub   r0, r0, r4
    bu    resultDone

resultFits:
    ldc  r4, 32-FRACTIONALBITS    // Merge two halves
    shl  r11, r11, r4             // By shifting both halves and oring them together.
    shr  r10, r10, r5
    or   r0, r11, r10

resultDone: 
    add  r8, r8, 1                // Rest bank counter
	  ldc  r11, BANKS
    eq   r11, r8, r11             // Last bank?
    bf   r11, loop                // If not, jump, this clears r11

    stw   r2, r1[1]               // store yn1
    stw   r0, r1[0]               // store yn2

    ldc   r11, 3*BANKS
    ldaw  r9, r1[-r11]            // Adjust r9 to point to start of array that contains db Values (and xns)
    ldc   r11, 3
    ldaw  r1, r1[r11]             // Adjust r1 to point to past array that contains db VAlues (and xns)
    ldw   r11, r1[0]              // Load counter that dictates whether we change db value
    bt    r11, decrementDelay     // If not, then decrement counter.
    ldc   r11, 40                 // Otherwise reset to 40 (magic number - decrease this for faster response)
    stw   r11, r1[0]              // But faster response will give more clicky noises
    ldw   r11, r1[1]              // Now grab which filter to adjust
    sub   r11, r11, 1             // and adjust that to point to the next one
    bt    r11, storeCounter       // Ok if there is one left, if not
    ldc   r4, BANKS               // reset counter to the highest filter.
    stw   r4, r1[1]
    bu    checkDb
storeCounter:   
    stw   r11, r1[1]              // Store pointer to the filter.
checkDb:    
    shl   r10, r11, 1             // Multiply by 3 (sizeof xn1 + xn2 + db element)
    add   r10, r11, r10           // into r10.
    add   r11, r11, 2             // adjust r11 pointer by 2, to adjust for the two counters in the struct
    ldaw  r9, r9[r10]             // Make r9 point to the right db element in the array
    ldw   r10, r9[2]              // and load the current db value
    ldw   r8, r1[r11]             // and load the desired db value.
    lsu   r7, r8, r10             // Test if decrement needed.
    bt    r7, decrement
    lsu   r7, r10, r8             // Test if increment needed
    bf    r7, allDone             // Neither needed, value must be equal
    add   r10, r10, 1             // Perform increment
    stw   r10, r9[2]
    bu    allDone
decrement:
    sub   r10, r10, 1             // or perform decrement
    stw   r10, r9[2]
    bu    allDone

decrementDelay:
    sub   r11, r11, 1             // too soon after previous adjustment - adjust counter.
    stw   r11, r1[0]
allDone:                          // Now just restore all registers.
        
    ldw   r4, sp[0]
    ldw   r5, sp[1]
    ldw   r6, sp[2]
    ldw   r7, sp[3]
    ldw   r8, sp[4]
    ldw   r9, sp[5]
    ldw   r10, sp[6]
    retsp NWORDS

//    .cc_bottom biquadAsm.func
