// Copyright 2015-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
    
#if (defined(__XS2A__) || defined (__XS3A__))

	.text
    .issue_mode  dual
	.globl	dsp_fft_inverse_DIF_xs2
	.align	16
    .skip 8
	.type	dsp_fft_inverse_DIF_xs2,@function
	.cc_top dsp_fft_inverse_DIF_xs2.function,dsp_fft_inverse_DIF_xs2
	
dsp_fft_inverse_DIF_xs2:

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

    eq r10, r11, 2
    bf r10, .Ltmp_outerLoop



// Last iteration
    
    { ldw r11, sp[17]     ;  nop            }        // N
    { sub r11, r11, 2     ;  nop            }        // k + N - step: BLOCK.
    { shl r3, r11, 3     ;  ldw r0, sp[16]  }        // base pointer

    { add r4, r0, r3     ; ldc r11, 16  }          // & pts[block]

.Ltmp_last_level:
 	ldd r3, r6, r4[0]                // r6: tRE,  r3: tIM
	ldd r7, r8, r4[1]               // r5: tRE2, r2: tIM2
	{add  r6, r6, r8       ; sub r8, r6, r8}
	{add  r3, r3, r7       ; sub r7, r3, r7}
	std  r3, r6, r4[0]
	std  r7, r8, r4[1]

	{eq r8, r4, r0        ; sub r4, r4, r11}
	    

	bf r8, .Ltmp_last_level


    
    
    ldd r9, r10, sp[10]
    ldd r7, r8, sp[11]
    ldd r5, r6, sp[9]
	ldw r4, sp[27]
	retsp 32
	
	// RETURN_REG_HOLDER
	.cc_bottom dsp_fft_inverse_DIF_xs2.function
	.set	dsp_fft_inverse_DIF_xs2.nstackwords,32
	.globl	dsp_fft_inverse_DIF_xs2.nstackwords
	.set	dsp_fft_inverse_DIF_xs2.maxcores,1
	.globl	dsp_fft_inverse_DIF_xs2.maxcores
	.set	dsp_fft_inverse_DIF_xs2.maxtimers,0
	.globl	dsp_fft_inverse_DIF_xs2.maxtimers
	.set	dsp_fft_inverse_DIF_xs2.maxchanends,0
	.globl	dsp_fft_inverse_DIF_xs2.maxchanends
.Ltmp0:
	.size	dsp_fft_inverse_DIF_xs2, .Ltmp0-dsp_fft_inverse_DIF_xs2

    .issue_mode  single
    
#endif
