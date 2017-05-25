// Copyright (c) 2015-2017, XMOS Ltd, All rights reserved
    
#if defined(__XS2A__)

	.text
    .issue_mode  dual
	.globl	dsp_fft_zero_reverse_forward
	.align	16
    .skip 12
	.type	dsp_fft_zero_reverse_forward,@function
	.cc_top dsp_fft_zero_reverse_forward.function,dsp_fft_zero_reverse_forward
	
dsp_fft_zero_reverse_forward:

	dualentsp 32
    
	stw r4, sp[27]
    std r9, r10, sp[10]
    std r7, r8, sp[11]
    std r5, r6, sp[9]
    
    { ldc r6, 1                 ;  ldc r5, 31 }
    { mkmsk r4, r5              ;  shl r5, r6, r5 }
	std r4, r4, sp[2]              //  0x800000000 x 2

	{ stw r0, sp[16]            ;  ldc r10, 29 } // pts
	{ stw r1, sp[17]            ;  clz r11, r1 }// N
	{ stw r2, sp[29]            ;  sub r11, r10, r11 }  // sine

    { stw r11, sp[15]           ;  ldc r11, 4         } // Shift

	{ nop           ;    stw r11, sp[14] }           // step


// First odd iteration

    { ldw r9, sp[17]            ; ldc r7, 1 }
    { clz r9, r9                ; shr r0, r9, 1  }
    { add r9, r9, 1             ; ldw r4, sp[16]  }

/*
    1 -> 8  (ZERO->1)  OP  (NOT-ODD)
    3 ->12  (ZERO->3) OP  (NOT-ODD)
    5 ->10  (ZERO->5) OP  (NOT-ODD)
    7 ->14  (ZERO->7) OP  (NOT-ODD)
    */
    
.Ltmp_first_level_odd:
 	ldd r3, r6, r4[r7]                // r6: tRE,  r3: tIM
    { shl r8, r7, r9                  ;     add  r7, r7, 2 }
    { bitrev r8, r8                   ; 	lsu r10, r7, r0 }   
	ashr r6, r6, 1                   // tRE
	ashr r3, r3, 1                   // tIM
	std  r3, r6, r4[r8]
    add  r8, r8, 1
	std  r3, r6, r4[r8]

	bt r10, .Ltmp_first_level_odd


// First even iteration
    
    { ldc r7, 0           ;  nop     }        // N

/*
    0 -> 0  NOT     OP  (NOT-EQUAL)
    2 -> 4  (4->2)  OP  OP
    4 -> 2  DONE        (NOT-TWICE - BIGGER)
    6 -> 6  NOT     OP  (NOT-EQUAL)
    */
    
.Ltmp_first_level_even:
 	ldd r3, r6, r4[r7]                // r6: tRE,  r3: tIM
    shl r8, r7, r9
    bitrev r8, r8
    { lsu r11, r8, r7            ;  eq  r10, r7, r8}
    { bf  r11, .even_BOTH        ; add  r7, r7, 2 }
    { bf  r10, .even_NOP          ; lsu r1, r7, r0 }
    
.even_OP:
	ashr r6, r6, 1                   // tRE
	ashr r3, r3, 1                   // tIM
	std  r3, r6, r4[r8]
    add  r8, r8, 1
	std  r3, r6, r4[r8]
    
	bu  .Ltmp_first_level_even
    
.even_BOTH:
 	ldd r10, r11, r4[r8]             // r6: tRE,  r3: tIM
	ashr r6, r6, 1                   // tRE
	ashr r3, r3, 1                   // tIM
	std  r3, r6, r4[r8]
    { add  r8, r8, 1              ;    sub r7, r7, 2 }
	std  r3, r6, r4[r8]
	ashr r11, r11, 1                   // tRE
	ashr r10, r10, 1                   // tIM
	std  r10, r11, r4[r7]
    add  r7, r7, 1
	std  r10, r11, r4[r7]
    
	{ bu .Ltmp_first_level_even ;     add r7, r7, 1} 
    
.even_NOP:

	bt r1, .Ltmp_first_level_even
    
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

	bf r8, .Ltmp_innerLoop1


    { neg r1, r0                ; add r0, r1, 0}

    
    ldd r7, r5, sp[6]     // step4
    { ldw r11, sp[28]  ;    ldc r8, 0  }  // k + N - step
    { nop              ;    add r11, r11, r5 } // k + N - step + step4: BLOCK.
    
    {shl r3, r11, 3    ;    ldw r11, sp[16]}
    {add r4, r11, r3   ;    ldw r11, sp[9] }            // step2        // & pts[block]
    
.Ltmp_innerloop2:
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

	bf r8, .Ltmp_innerloop2

    {add r9, r9, 1              ;	ldw r10, sp[12]}             // step4
    {lsu r10, r9, r10            ;    stw r9, sp[10]}             // k
	bt r10, .Ltmp_kLoop1

	ldd r10, r11, sp[7]
	{sub r10, r10, 1; shl r11, r11, 1}
	std r10, r11, sp[7]

    ldw r10, sp[17]
    add r10, r10, 1
    lsu r10, r10, r11
    bf  r10, .Ltmp_outerLoop

    ldd r9, r10, sp[10]
    ldd r7, r8, sp[11]
    ldd r5, r6, sp[9]
	ldw r4, sp[27]
	retsp 32
	
	// RETURN_REG_HOLDER
	.cc_bottom dsp_fft_zero_reverse_forward.function
	.set	dsp_fft_zero_reverse_forward.nstackwords,32
	.globl	dsp_fft_zero_reverse_forward.nstackwords
	.set	dsp_fft_zero_reverse_forward.maxcores,1
	.globl	dsp_fft_zero_reverse_forward.maxcores
	.set	dsp_fft_zero_reverse_forward.maxtimers,0
	.globl	dsp_fft_zero_reverse_forward.maxtimers
	.set	dsp_fft_zero_reverse_forward.maxchanends,0
	.globl	dsp_fft_zero_reverse_forward.maxchanends
.Ltmp0:
	.size	dsp_fft_zero_reverse_forward, .Ltmp0-dsp_fft_zero_reverse_forward

    .issue_mode  single
    
#endif
