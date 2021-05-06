// Copyright 2015-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
    
#if (defined(__XS2A__) || defined (__XS3A__))

	.text
    .issue_mode  dual
	.globl	dsp_fft_gc_xs2
	.align	16
    .skip 12
	.type	dsp_fft_gc_xs2,@function
	.cc_top dsp_fft_gc_xs2.function,dsp_fft_gc_xs2
	
dsp_fft_gc_xs2:

	dualentsp 32
    
	stw r4, sp[27]
    std r9, r10, sp[10]
    std r7, r8, sp[11]
    std r5, r6, sp[9]
    
    { ldc r6, 1                 ;  ldc r5, 31 }
    { mkmsk r4, r5              ;  shl r5, r6, r5 }
	std r4, r4, sp[2]              //  0x800000000 x 2

	{ stw r0, sp[16]            ;  nop } // pts
	{ stw r1, sp[17]            ;  nop }// N
	{ stw r2, sp[29]            ;  ldc r11, 0 }  // sine, shift = 0
    divu r3, r1, r3
    shr r3, r3, 2
    stw r3, sp[30]
    { stw r11, sp[15]           ;  nop         } // Shift

	{ nop           ;    stw r1, sp[14] }           // step


    
.Ltmp_outerLoop:
    { ldw r11, sp[14]             ;    ldc r9, 0 }
    { shl r10, r11, 3             ;    shr r11, r11, 1 }
    { stw r10, sp[9]              ;    shr r10, r11, 1  }// step * 8
    std r11, r10, sp[6]            // step2

    { stw r10, sp[11]             ; ldc r11, 0 }
    stw r9, sp[10]             // k
.Ltmp_kLoop1:
    ldw r10, sp[15]            // shift
    { shl r7, r9, r10  ;   ldw r6, sp[29] }// sine
    
    {ldw r8, sp[17]    ; nop}        // N
    { add r11, r9, r8    ; ldw r0, r6[r7]     }       // k + N        // rIm
    { shr r8, r8, 2 ;   ldw r5, sp[14] }
    { sub r11, r11, r5 ;  sub r8, r8, r7 }        // k + N - step: BLOCK.
    // N>>2 - k<<shift
    stw r11, sp[28]
    { ldw r1, r6[r8]   ;     shl r3, r11, 3 }          //  rRe


    { ldw r11, sp[16]	  ; ldc r8, 0}
    { add r4, r11, r3   ; ldw r11, sp[9]  }          // & pts[block]
    ldw r9, sp[13]             // step2


.Ltmp_innerLoop1:
 	ldd r6, r3, r4[0]                // r6: tRE,  r3: tIM
	ldd r5, r2, r4[r9]               // r5: tRE2, r2: tIM2
	{add  r6, r6, r5       ; sub r5, r6, r5}
	{add  r3, r3, r2       ; sub r2, r3, r2}
	ldd r10, r7, sp[2]              //  0x800000000 x 2
	maccs r8, r7, r5, r1             // rRe x tRe2
	maccs r8, r7, r2, r0             // rIM x tIm2
	                                 // r8: sRE2
	{ ldc r7, 0            ; neg r5, r5}
	maccs r7, r10, r5, r0            // rIM x -tRE2
	maccs r7, r10, r2, r1            // rRE x tIM2
                                     // r7: sIM2    
    { shl r8, r8, 1                  ; shl r7, r7, 1 }
	std  r6, r3, r4[0]
	std  r8, r7, r4[r9]
	{ldw r6, sp[16]        ; sub r4, r4, r11}
	lsu r8, r4, r6    

	bf r8, .Ltmp_innerLoop1


    { neg r1, r0                ; add r0, r1, 0}

    
    ldd r7, r5, sp[6]     // step4

    { ldw r11, sp[28]  ;    ldc r8, 0  }  // k + N - step
    { bu skip          ;    add r11, r11, r5 } // k + N - step + step4: BLOCK.
    nop // aligning second loop...
    nop
skip:   
    {shl r3, r11, 3    ;    ldw r11, sp[16]}
    {add r4, r11, r3   ;    ldw r11, sp[9] }            // step2        // & pts[block]
    
.Ltmp_innerloop2:
 	ldd r6, r3, r4[0]               // r6: tRE,  r3: tIM
	ldd r5, r2, r4[r7]              // r5: tRE2, r2: tIM2
	{add  r6, r6, r5       ; sub r5, r6, r5}
	{add  r3, r3, r2       ; sub r2, r3, r2}
    
	ldd r10, r9, sp[2]              //  0x800000000 x 2

	maccs r8, r9, r5, r1            // rRe x tRe2
	maccs r8, r9, r2, r0            // rIM x tIm2
	                                // r8: sRE2
	{ ldc r9, 0                     ; neg r5, r5 }

	maccs r9, r10, r5, r0            // rIM x -tRE2
	maccs r9, r10, r2, r1            // rRE x tIM2
                                    // r9: sIM2
    { shl r8, r8, 1                  ; shl r9, r9, 1 }
	std  r6, r3, r4[0]
	std  r8, r9, r4[r7]
	{ldw r6, sp[16]        ; sub r4, r4, r11}
	{lsu r8, r4, r6         ;    ldw r9, sp[10]}             // k

	bf r8, .Ltmp_innerloop2

    {add r9, r9, 1              ;	ldw r10, sp[12]}             // step4
    {lsu r10, r9, r10            ;    stw r9, sp[10]}             // k
	bt r10, .Ltmp_kLoop1

	ldd r10, r11, sp[7]
	{add r10, r10, 1; shr r11, r11, 1}
	std r10, r11, sp[7]

    eq r10, r11, 4
    bf r10, .Ltmp_outerLoop



// Last iteration
    
    { ldw r11, sp[17]     ;  nop            }        // N
    { sub r11, r11, 4     ;  nop            }        // k + N - step: BLOCK.
    { shl r3, r11, 3     ;  ldw r0, sp[16]  }        // base pointer

    { add r4, r0, r3     ; ldc r11, 32  }          // & pts[block]
    ldc r10, 1
    stw r10, sp[8]
 #if 1
.Ltmp_last_level:
 	ldd r0, r1, r4[0]   
 	ldd r2, r3, r4[1]   
	ldd r5, r6, r4[2]   
	ldd r7, r8, r4[3]
    ldw r10, sp[8]
    sub r10, r10, 1
    stw r10, sp[8]
    bf r10, .LthreeZeroes
.LtwoZeroes:
    {add r10, r3, 0             ;ldc r11, 1}
    ashr r9, r3, 32
    maccs r9, r10, r8, r11
    maccs r9, r10, r7, r11
    neg r11, r11
    maccs r9, r10, r2, r11
    ldc r11, 2
    maccs r9, r10, r1, r11
    lextract r9, r9, r10, r11, 32
    stw r9, r4[0]

    {add r10, r2, 0             ;ldc r11, 1}
    ashr r9, r2, 32
    maccs r9, r10, r7, r11
    maccs r9, r10, r3, r11
    neg r11, r11
    maccs r9, r10, r8, r11
    ldc r11, 2
    maccs r9, r10, r0, r11
    lextract r9, r9, r10, r11, 32
    stw r9, r4[1]
    
    {add r10, r1, 0             ;ldc r11, 1}
    ashr r9, r1, 32
    maccs r9, r10, r6, r11
    maccs r9, r10, r0, r11
    neg r11, r11
    maccs r9, r10, r5, r11
    ldc r11, 2
    maccs r9, r10, r3, r11
    lextract r9, r9, r10, r11, 32
    stw r9, r4[2]
    
    {add r10, r0, 0             ;ldc r11, 1}
    ashr r9, r0, 32
    maccs r9, r10, r5, r11
    maccs r9, r10, r6, r11
    neg r11, r11
    maccs r9, r10, r1, r11
    ldc r11, 2
    maccs r9, r10, r2, r11
    lextract r9, r9, r10, r11, 32
    stw r9, r4[3]
    
    {add r10, r2, 0             ;ldc r11, 1}
    ashr r9, r2, 32
    maccs r9, r10, r3, r11
    maccs r9, r10, r8, r11
    neg r11, r11
    maccs r9, r10, r7, r11
    ldc r11, 2
    maccs r9, r10, r6, r11
    lextract r9, r9, r10, r11, 32
    stw r9, r4[4]
    
    {add r10, r2, 0             ;ldc r11, 1}
    ashr r9, r2, 32
    maccs r9, r10, r7, r11
    maccs r9, r10, r8, r11
    neg r11, r11
    maccs r9, r10, r3, r11
    ldc r11, 2
    maccs r9, r10, r5, r11
    lextract r9, r9, r10, r11, 32
    stw r9, r4[5]
    
    {add r10, r1, 0             ;ldc r11, 1}
    ashr r9, r1, 32
    maccs r9, r10, r6, r11
    maccs r9, r10, r5, r11
    neg r11, r11
    maccs r9, r10, r0, r11
    ldc r11, 2
    maccs r9, r10, r8, r11
    lextract r9, r9, r10, r11, 32
    stw r9, r4[6]
    
    {add r10, r0, 0             ;ldc r11, 1}
    ashr r9, r0, 32
    maccs r9, r10, r5, r11
    maccs r9, r10, r1, r11
    neg r11, r11
    maccs r9, r10, r6, r11
    ldc r11, 2
    maccs r9, r10, r7, r11
    lextract r9, r9, r10, r11, 32
    stw r9, r4[7]

    bu .LdoneZeroes
.LthreeZeroes:
    {ldc r10, 2                 ; ldc r11, 1}
    ashr r9, r0, 32
    maccs r9, r0, r2, r11
    maccs r9, r0, r5, r11
    maccs r9, r0, r7, r11
    lextract r0, r9, r0, r10, 32
    ashr r9, r1, 32
    maccs r9, r1, r3, r11
    maccs r9, r1, r6, r11
    maccs r9, r1, r8, r11
    lextract r1, r9, r1, r10, 32

    std r0, r1, r4[0]
    std r0, r1, r4[1]
    std r0, r1, r4[2]
    std r0, r1, r4[3]
    ldw r10, sp[30]
    stw r10, sp[8]
.LdoneZeroes:
    ldw r0, sp[16]
    ldc r11, 32
	{eq r8, r4, r0        ; sub r4, r4, r11}
	bf r8, .Ltmp_last_level
 #else   
.Ltmp_last_level:
 	ldd r0, r1, r4[0]   
 	ldd r2, r3, r4[1]   
	ldd r5, r6, r4[2]   
	ldd r7, r8, r4[3]   
	ashr r0, r0, 2
	ashr r1, r1, 2
	ashr r2, r2, 2
	ashr r3, r3, 2
	ashr r5, r5, 2
	ashr r6, r6, 2
	ashr r7, r7, 2
	ashr r8, r8, 2
    sub r10, r10, 1
    bf r10, .LthreeZeroes
.LtwoZeroes:

    add r9, r1, r3
    add r9, r9, r6
    add r9, r9, r8     // r9: akr
    add r1, r1, r7
    sub r1, r1, r2
    sub r1, r1, r6     // r1: alr
    add r2, r0, r2
    add r2, r2, r5
    add r2, r2, r7     // r2: aki
    add r0, r0, r3
    sub r0, r0, r5
    sub r0, r0, r8     // r0: ali

    add r6, r9, r1
    add r5, r2, r0
    std r5, r6, r4[0]

    sub r6, r9, r0
    add r5, r2, r1
    std r5, r6, r4[3]

    sub r6, r9, r1
    sub r5, r2, r0
    std r5, r6, r4[2]

    add r6, r9, r0
    sub r5, r2, r1
    std r5, r6, r4[1]

    bu .LdoneZeroes
.LthreeZeroes:
    add r0, r0, r2
    add r0, r0, r5
    add r0, r0, r7
    add r1, r1, r3
    add r1, r1, r6
    add r1, r1, r8
    std r0, r1, r4[0]
    std r0, r1, r4[1]
    std r0, r1, r4[2]
    std r0, r1, r4[3]
    ldc r10, 4
.LdoneZeroes:
    ldw r0, sp[16]
    ldc r11, 32
	{eq r8, r4, r0        ; sub r4, r4, r11}
	bf r8, .Ltmp_last_level
#endif
    
	{ ldw r1, sp[17]            ;  ldc r10, 28 } // pts
	{ nop                       ;  clz r11, r1 }// N
	{ nop                       ;  sub r11, r10, r11 }  // sine

    { stw r11, sp[15]           ;  ldc r11, 8         } // Shift

	{ nop           ;    stw r11, sp[14] }           // step

    
.L_tmp_outerLoop:
    { ldw r11, sp[14]             ;    ldc r9, 0 }
    { shl r10, r11, 3             ;    shr r11, r11, 1 }
    { stw r10, sp[9]              ;    shr r10, r11, 1  }// step * 8
    std r11, r10, sp[6]            // step2

    { stw r10, sp[11]             ; ldc r11, 0 }
    stw r9, sp[10]             // k
.L_tmp_kLoop1:
    ldw r10, sp[15]            // shift
    { shl r7, r9, r10  ;   ldw r6, sp[29] }// sine
    
    {ldw r8, sp[17]    ; nop}        // N
    { add r11, r9, r8    ; ldw r0, r6[r7]     }       // k + N        // rIm
    { shr r8, r8, 2 ;   ldw r5, sp[14] }
    { sub r11, r11, r5 ;  sub r8, r8, r7 }        // k + N - step: BLOCK.
    // N>>2 - k<<shift
    stw r11, sp[28]
    { ldw r1, r6[r8]   ;     shl r3, r11, 3 }          //  rRe


    { ldw r11, sp[16]	  ; ldc r8, 0}
    { add r4, r11, r3   ; ldw r11, sp[9]  }          // & pts[block]
    ldw r9, sp[13]             // step2


.L_tmp_innerLoop1:
#if HIRES
	ldd r2, r5, r4[r9]               // r5: tRE2, r2: tIM2
	ldd r10, r7, sp[2]              //  0x800000000 x 2
	maccs r8, r7, r5, r1             // rRe x tRe2
	maccs r8, r7, r2, r0             // rIM x tIm2
	                                 // r8: sRE2

 	ldd r3, r6, r4[0]                // r6: tRE,  r3: tIM

	maccs r8, r7, r6, r10
	{stw r8, r4[0]; neg r10, r10}
	maccs r8, r7, r6, r10
	maccs r8, r7, r6, r10

	ldd r6, r10, sp[2]              //  0x800000000 x 2
	{ ldc r7, 0            ; neg r5, r5}
	maccs r7, r10, r5, r0            // rIM x -tRE2
	maccs r7, r10, r2, r1            // rRE x tIM2
                                     // r7: sIM2
	maccs r7, r10, r3, r6
	{stw r7, r4[1]; neg r6, r6}
	maccs r7, r10, r3, r6
	maccs r7, r10, r3, r6
	{neg r8, r8; neg r7, r7}
	std  r7, r8, r4[r9]
#else
 	ldd r3, r6, r4[0]                // r6: tRE,  r3: tIM    
	ashr r6, r6, 1                   // tRE
	ashr r3, r3, 1                   // tIM
	ldd r2, r5, r4[r9]               // r5: tRE2, r2: tIM2
	ldd r10, r7, sp[2]              //  0x800000000 x 2
	maccs r8, r7, r5, r1             // rRe x tRe2
	maccs r8, r7, r2, r0             // rIM x tIm2
	                                 // r8: sRE2
	{ ldc r7, 0            ; neg r5, r5}
	maccs r7, r10, r5, r0            // rIM x -tRE2
	maccs r7, r10, r2, r1            // rRE x tIM2
                                     // r7: sIM2    
	{add  r6, r6, r8       ; sub r8, r6, r8}
	{add  r3, r3, r7       ; sub r7, r3, r7}
	std  r3, r6, r4[0]
	std  r7, r8, r4[r9]
#endif
	{ldw r6, sp[16]        ; sub r4, r4, r11}
	lsu r8, r4, r6    

	bf r8, .L_tmp_innerLoop1


    { neg r1, r0                ; add r0, r1, 0}

    
    ldd r7, r5, sp[6]     // step4
    { ldw r11, sp[28]  ;    ldc r8, 0  }  // k + N - step
    { nop              ;    add r11, r11, r5 } // k + N - step + step4: BLOCK.
    
    {shl r3, r11, 3    ;    ldw r11, sp[16]}
    {add r4, r11, r3   ;    ldw r11, sp[9] }            // step2        // & pts[block]
    
.L_tmp_innerloop2:
#if HIRES
	ldd r2, r5, r4[r7]               // r5: tRE2, r2: tIM2
	ldd r10, r9, sp[2]              //  0x800000000 x 2
	maccs r8, r9, r5, r1             // rRe x tRe2
	maccs r8, r9, r2, r0             // rIM x tIm2
	                                 // r8: sRE2

 	ldd r3, r6, r4[0]                // r6: tRE,  r3: tIM

	maccs r8, r9, r6, r10
	{stw r8, r4[0]; neg r10, r10}
	maccs r8, r9, r6, r10
	maccs r8, r9, r6, r10

	ldd r6, r10, sp[2]              //  0x800000000 x 2
	{ ldc r9, 0            ; neg r5, r5}
	maccs r9, r10, r5, r0            // rIM x -tRE2
	maccs r9, r10, r2, r1            // rRE x tIM2
                                     // r9: sIM2
	maccs r9, r10, r3, r6
	{stw r9, r4[1]; neg r6, r6}
	maccs r9, r10, r3, r6
	maccs r9, r10, r3, r6
	{neg r8, r8; neg r9, r9}
	std  r9, r8, r4[r7]
#else
 	ldd r3, r6, r4[0]               // r6: tRE,  r3: tIM
	ashr r6, r6, 1                  // tRE
	ashr r3, r3, 1                  // tIM
	ldd r2, r5, r4[r7]              // r5: tRE2, r2: tIM2

	ldd r10, r9, sp[2]              //  0x800000000 x 2

	maccs r8, r9, r5, r1            // rRe x tRe2
	maccs r8, r9, r2, r0            // rIM x tIm2
	                                // r8: sRE2
	{ ldc r9, 0                     ; neg r5, r5 }

	maccs r9, r10, r5, r0            // rIM x -tRE2
	maccs r9, r10, r2, r1            // rRE x tIM2
                                    // r9: sIM2
	{add  r6, r6, r8           ; sub r8, r6, r8}
	{add  r3, r3, r9           ; sub r9, r3, r9}
	std  r3, r6, r4[0]
	std  r9, r8, r4[r7]
#endif
	{ldw r6, sp[16]        ; sub r4, r4, r11}
	{lsu r8, r4, r6         ;    ldw r9, sp[10]}             // k

	bf r8, .L_tmp_innerloop2

    {add r9, r9, 1              ;	ldw r10, sp[12]}             // step4
    {lsu r10, r9, r10            ;    stw r9, sp[10]}             // k
	bt r10, .L_tmp_kLoop1

	ldd r10, r11, sp[7]
	{sub r10, r10, 1; shl r11, r11, 1}
	std r10, r11, sp[7]

    ldw r10, sp[17]
    add r10, r10, 1
    lsu r10, r10, r11
    bf  r10, .L_tmp_outerLoop

    ldd r9, r10, sp[10]
    ldd r7, r8, sp[11]
    ldd r5, r6, sp[9]
	ldw r4, sp[27]
	retsp 32
	
	// RETURN_REG_HOLDER
	.cc_bottom dsp_fft_gc_xs2.function
	.set	dsp_fft_gc_xs2.nstackwords,32
	.globl	dsp_fft_gc_xs2.nstackwords
	.set	dsp_fft_gc_xs2.maxcores,1
	.globl	dsp_fft_gc_xs2.maxcores
	.set	dsp_fft_gc_xs2.maxtimers,0
	.globl	dsp_fft_gc_xs2.maxtimers
	.set	dsp_fft_gc_xs2.maxchanends,0
	.globl	dsp_fft_gc_xs2.maxchanends
.L_tmp0:
	.size	dsp_fft_gc_xs2, .L_tmp0-dsp_fft_gc_xs2

    .issue_mode  single
    
#endif
