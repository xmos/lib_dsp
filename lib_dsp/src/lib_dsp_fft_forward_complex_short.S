// Copyright (c) 2015, XMOS Ltd, All rights reserved
    
#if defined(__XS2A__)
#include <xs1.h>


	.text
    .issue_mode  dual
	.globl	lib_dsp_fft_forward_complex_short_asm
	.align	16
    .skip 8
	.type	lib_dsp_fft_forward_complex_short_asm,@function
	.cc_top lib_dsp_fft_forward_complex_short_asm.function,lib_dsp_fft_forward_complex_short_asm
	
lib_dsp_fft_forward_complex_short_asm: //void lib_dsp_fft_forward_complex_short_asm(
        //lib_dsp_fft_complex_short_t pts[],
        //int                         N,
        //const short                 sine[] );

	dualentsp 32
    
	stw r4, sp[27]
	stw r5, sp[26]
	stw r6, sp[25]
	stw r7, sp[24]
	stw r8, sp[23]
	stw r9, sp[22]
	stw r10, sp[19]
    
    ldc r4, 1
    ldc r5, 15
    shl r4, r4, r5
	std r4, r4, sp[10]              //  0x8000 x 2

	stw r2, sp[18]             // sine
	stw r1, sp[17]             // N
	stw r0, sp[16]             // pts

	clz r11, r1
	ldc r10, 30
	sub r11, r10, r11
    stw r11, sp[15]            // Shift

    ldc r11, 2
    stw r11, sp[14]            // step
outerLoop:
    ldw r11, sp[14]
    shl r10, r11, 3
    stw r10, sp[9]             // step * 8
    shr r11, r11, 1
    stw r11, sp[13]            // step2
    shr r10, r11, 1
    stw r10, sp[12]            // step4

// First loop

    ldc r9, 1
    and r11, r11, r9
    add r10, r11, r10          // step4 + (step2&1)
    stw r10, sp[11]
    ldc r9, 0
kLoop1:
    stw r9, sp[10]             // k
    ldw r10, sp[15]            // shift
    { shl r7, r9, r10  ;   ldw r6, sp[18] }// sine
    
    {ldw r8, sp[17]    ; nop}        // N
    { add r11, r9, r8    ; ld16s r0, r6[r7]     }       // k + N        // rIm
    { shr r8, r8, 2 ;   ldw r5, sp[14] }
    { sub r11, r11, r5 ;  sub r8, r8, r7 }        // k + N - step: BLOCK.
                 // N>>2 - k<<shift
    { ld16s r1, r6[r8]   ;     shl r3, r11, 3 }          //  rRe


    { ldw r11, sp[16]	  ; ldc r8, 0}
    { add r4, r11, r3   ; ldw r11, sp[9]  }          // & pts[block]
    ldw r9, sp[13]             // step2

innerLoop1:
    ldc r2, 0
 	{ld16s r3, r4[r2];  add r2, r2, 1}       	      // r3: tIM; increment r2
 	ld16s r6, r4[r2]                                  // r6: tRE,
 	//ldd r3, r6, r4[0]               // r6: tRE,  r3: tIM

	ashr r6, r6, 1                   // tRE
	ashr r3, r3, 1                   // tIM

	{ld16s r2, r4[r9]; add r9, r9, 1}       	   // r2: tIM, increment r9
 	{ld16s r5, r4[r9]; sub r9, r9, 1}              // r5: tRE, reset r9
	//ldd r2, r5, r4[r9]               // r5: tRE2, r2: tIM2

	ldd r10, r7, sp[10]              //  0x80000 x 2
	maccs r8, r7, r5, r1             // rRe x tRe2
	maccs r8, r7, r2, r0             // rIM x tIm2
	                                 // r7: sRE2
	ashr r7, r7, 16                   // sRe2 =  (int) (l>>16);

	{ ldc r8, 0            ; neg r5, r5}
	maccs r8, r10, r5, r0            // rIM x -tRE2
	maccs r8, r10, r2, r1            // rRE x tIM2
                                     // r10: sIM2
    ashr r10, r10, 16                 // sIm2 =  (short) (l>>16);

    // int re = (tRe + sRe2) // int re_step = (tRe - sRe2)
	{add  r6, r6, r7         ; sub r7, r6, r7}
	// im = (tIm + sIm2)     // im_step = (tIm - sIm2)
	{add  r3, r3, r10        ; sub r10, r3, r10}

    // pack the 16 bit data into 32 bit words.
    // Note: st16 can't be used because it's only available in format l3r so dual issue cant' be used
    // im &= 0xffff; // lower 16 bit
    ldc r8, 0xffff
    and r3, r3, r8
    // im |= (re << 16); // or in upper 16 bit (real part)
    shl r6, r6, 16
    or r3, r3, r6

	stw r3, r4[0]     // pts[block].im = (short) im; pts[block].re = (short) re;
	//std  r3, r6, r4[0]

    // im_step &= 0xffff; // lower 16 bit
    and r10, r10, r8
    // im_step |= (re_step << 16); // or in upper 16 bit (real part)
    shl r7, r7, 16
    or r10, r10, r7

	stw r10, r4[r9]    // pts[block+step].im = (short) im; pts[block+step2].re = (short) re_step;
	//std  r7, r8, r4[r9]


	{ldw r6, sp[16]        ; sub r4, r4, r11}
	lsu r8, r4, r6    

	bf r8, innerLoop1

    ldw r9, sp[10]             // k
    add r9, r9, 1
	ldw r10, sp[11]
    lsu r10, r9, r10
	bt r10, kLoop1

// Second inner loop

	ldw r10, sp[12]             // step4
    {ldw r11, sp[13]             ;    ldc r9, 1}
    {and r7, r11, r9             ;   bu  kLoop2In}
kLoop2:
    { ldw r10, sp[15]  ; add r11, r7, 0}           // shift
    { shl r9, r11, r10 ;     ldw r7, sp[18]   }         // sine         // k << shift

    ld16s r1, r7[r9]            // rRe
    { neg r1, r1                ;    ldw r8, sp[17]}             // N
    { add r11, r11, r8 ;    ldw r6, sp[14]}      // k + N
    
    { shr r8, r8, 2               ;    sub r11, r11, r6}

    { sub r8, r8, r9  ;     ldw r5, sp[12] }            // step4          // N>>2 - k<<shift
    { ld16s r0, r7[r8]  ;     add r11, r11, r5 }          // k + N - step + step4: BLOCK.            // rIm
    
    {shl r3, r11, 3              ;    ldw r11, sp[16]}
    {add r4, r11, r3   ;     ldw r9, sp[13] }            // step2        // & pts[block]

	{ldc r8, 0                   ;    ldw r11, sp[9]}
innerLoop2:

    ldc r2, 0
 	{ld16s r3, r4[r2];  add r2, r2, 1}       	      // r3: tIM; increment r2
 	ld16s r6, r4[r2]                                  // r6: tRE,
 	//ldd r3, r6, r4[0]               // r6: tRE,  r3: tIM

	ashr r6, r6, 1                   // tRE
	ashr r3, r3, 1                   // tIM

	{ld16s r2, r4[r9]; add r9, r9, 1}       	   // r2: tIM, increment r9
 	{ld16s r5, r4[r9]; sub r9, r9, 1}              // r5: tRE, reset r9
	//ldd r2, r5, r4[r9]               // r5: tRE2, r2: tIM2

	ldd r10, r7, sp[10]              //  0x8000 x 2

	maccs r8, r7, r5, r1            // rRe x tRe2
	maccs r8, r7, r2, r0            // rIM x tIm2
		                            // r7: sRE2
	ashr r7, r7, 16                  // sRe2 =  (int) (l>>16);

	{ ldc r8, 0            ; neg r5, r5}
	maccs r8, r10, r5, r0            // rIM x -tRE2
	maccs r8, r10, r2, r1            // rRE x tIM2
                                     // r10: sIM2
    ashr r10, r10, 16                 // sIm2 =  (short) (l>>16);

    // int re = (tRe + sRe2) // int re_step = (tRe - sRe2)
	{add  r6, r6, r7         ; sub r7, r6, r7}
	// im = (tIm + sIm2)     // im_step = (tIm - sIm2)
	{add  r3, r3, r10        ; sub r10, r3, r10}

    // pack the 16 bit data into 32 bit words.
    // Note: st16 can't be used because it's only available in format l3r so dual issue cant' be used
    // im &= 0xffff; // lower 16 bit
    ldc r8, 0xffff
    and r3, r3, r8
    // im |= (re << 16); // or in upper 16 bit (real part)
    shl r6, r6, 16
    or r3, r3, r6

	stw r3, r4[0]     // pts[block].im = (short) im; pts[block].re = (short) re;
	//std  r3, r6, r4[0]

    // im_step &= 0xffff; // lower 16 bit
    and r10, r10, r8
    // im_step |= (re_step << 16); // or in upper 16 bit (real part)
    shl r7, r7, 16
    or r10, r10, r7

	stw r10, r4[r9]    // pts[block+step].im = (short) im; pts[block+step2].re = (short) re_step;
	//std  r7, r8, r4[r9]

	{ldw r6, sp[16]         ;  sub r4, r4, r11}
	{lsu r8, r4, r6         ;  ldw r7, sp[10]}             // k

	bf r8, innerLoop2

    {add r7, r7, 1              ;	ldw r10, sp[12]}             // step4

kLoop2In:   
    {lsu r10, r7, r10            ;    stw r7, sp[10]}             // k
	bt r10, kLoop2

    ldw r11, sp[15]
    sub r11, r11, 1
    stw r11, sp[15]              // shift -= 1
    
    ldw r11, sp[14]
    shl r11, r11, 1
    stw r11, sp[14]              // step *= 2

    ldw r10, sp[17]
    add r10, r10, 1
    lsu r10, r10, r11
    bf  r10, outerLoop
    
	ldw r10, sp[19]
	ldw r9, sp[22]
	ldw r8, sp[23]
	ldw r7, sp[24]
	ldw r6, sp[25]
	ldw r5, sp[26]
	ldw r4, sp[27]
	retsp 32
	
	// RETURN_REG_HOLDER
	.cc_bottom lib_dsp_fft_forward_complex_short_asm.function
	.set	lib_dsp_fft_forward_complex_short_asm.nstackwords,32
	.globl	lib_dsp_fft_forward_complex_short_asm.nstackwords
	.set	lib_dsp_fft_forward_complex_short_asm.maxcores,1
	.globl	lib_dsp_fft_forward_complex_short_asm.maxcores
	.set	lib_dsp_fft_forward_complex_short_asm.maxtimers,0
	.globl	lib_dsp_fft_forward_complex_short_asm.maxtimers
	.set	lib_dsp_fft_forward_complex_short_asm.maxchanends,0
	.globl	lib_dsp_fft_forward_complex_short_asm.maxchanends
.Ltmp0:
	.size	lib_dsp_fft_forward_complex_short_asm, .Ltmp0-lib_dsp_fft_forward_complex_short_asm

    .issue_mode  single
    
#endif
