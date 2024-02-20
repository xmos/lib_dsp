// Copyright 2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

    
#if defined(__XS3A__)

	.text
    .issue_mode  dual
	.globl	dsp_fft_float4_forward_xs3
	.align	16
    .skip 8
	.type	dsp_fft_float4_forward_xs3,@function
	.cc_top dsp_fft_float4_forward_xs3.function,dsp_fft_float4_forward_xs3
	
dsp_fft_float4_forward_xs3:

	dualentsp 32
    
	stw r4, sp[27]
    std r9, r10, sp[10]
    std r7, r8, sp[11]
    std r5, r6, sp[9]
    
    { ldc r6, 1                 ;  ldc r5, 31 }
    { mkmsk r4, r5              ;  shl r5, r6, r5 }
    add r4, r4, 1
	std r4, r4, sp[2]              //  0x800000000 x 2

	{ stw r0, sp[16]            ;  ldc r10, 29 } // pts
	{ stw r1, sp[17]            ;  clz r11, r1 }// N
	{ stw r2, sp[29]            ;  sub r11, r10, r11 }  // sine

    { stw r11, sp[15]           ;  ldc r11, 4         } // Shift

	{ nop           ;    stw r11, sp[14] }           // step


// First iteration
    
    { ldw r11, sp[17]     ;  ldc r8, 0     }        // N
    { sub r11, r11, 2     ;  ldc r7, 1     }        // k + N - step: BLOCK.
                 // N>>2 - k<<shift
    { shl r3, r11, 3     ;  ldw r11, sp[16]  }          //  rRe

    { add r4, r11, r3     ; ldc r11, 16  }          // & pts[block]

.Ltmp_first_level:
 	ldd r3, r6, r4[0]                // r6: tRE,  r3: tIM
	ldd r2, r5, r4[1]               // r5: tRE2, r2: tIM2
	fadd  r10, r6, r5
    fsub r8, r6, r5
	fadd  r5, r3, r2
    fsub r7, r3, r2
	std  r5, r10, r4[0]
	std  r7, r8, r4[1]

	{ldw r6, sp[16]        ; sub r4, r4, r11}
	lsu r8, r4, r6    

	bf r8, .Ltmp_first_level




    
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


	ldw r10, sp[4]                   //  0x800000000
.Ltmp_innerLoop1:
 	ldd r3, r6, r4[0]                // r6: tRE,  r3: tIM
	ldd r2, r5, r4[r9]               // r5: tRE2, r2: tIM2
	fmacc r8, r8, r5, r1             // rRe x tRe2
	fmacc r8, r8, r2, r0             // rIM x tIm2
	                                 // r8: sRE2
	{ ldc r7, 0            ; add r5, r5, r10}    // NEG r5
	fmacc r7, r7, r5, r0            // rIM x -tRE2
	fmacc r7, r7, r2, r1            // rRE x tIM2
                                     // r7: sIM2
    fadd r2, r6, r8
    fadd r5, r3, r7
	std  r5, r2, r4[0]
    fsub r8, r6, r8
    fsub r7, r3, r7
	std  r7, r8, r4[r9]

	{ldw r6, sp[16]        ; sub r4, r4, r11}
	lsu r8, r4, r6    

	bf r8, .Ltmp_innerLoop1


    { add r5, r1, 0             ; ldw r7, sp[4] }             //  0x800000000
    { add r1, r0, r7            ; add r0, r5, 0 }

    
    ldd r7, r5, sp[6]     // step4
    { ldw r11, sp[28]  ;    ldc r8, 0  }  // k + N - step
    { nop              ;    add r11, r11, r5 } // k + N - step + step4: BLOCK.
    
    {shl r3, r11, 3    ;    ldw r11, sp[16]}
    {add r4, r11, r3   ;    ldw r11, sp[9] }            // step2        // & pts[block]
    
	ldw r10, sp[4]              //  0x800000000 x 2
.Ltmp_innerloop2:
 	ldd r3, r6, r4[0]               // r6: tRE,  r3: tIM
	ldd r2, r5, r4[r7]              // r5: tRE2, r2: tIM2

	fmacc r8, r8, r5, r1            // rRe x tRe2
	fmacc r8, r8, r2, r0            // rIM x tIm2
	                                // r8: sRE2
	{ ldc r9, 0                     ; add r5, r5, r10 }

	fmacc r9, r9, r5, r0            // rIM x -tRE2
	fmacc r9, r9, r2, r1            // rRE x tIM2
                                    // r9: sIM2
    fadd r2, r6, r8
    fadd r5, r3, r9
	std  r5, r2, r4[0]
    fsub r8, r6, r8
    fsub r9, r3, r9
	std  r9, r8, r4[r7]

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
	.cc_bottom dsp_fft_float4_forward_xs3.function
	.set	dsp_fft_float4_forward_xs3.nstackwords,32
	.globl	dsp_fft_float4_forward_xs3.nstackwords
	.set	dsp_fft_float4_forward_xs3.maxcores,1
	.globl	dsp_fft_float4_forward_xs3.maxcores
	.set	dsp_fft_float4_forward_xs3.maxtimers,0
	.globl	dsp_fft_float4_forward_xs3.maxtimers
	.set	dsp_fft_float4_forward_xs3.maxchanends,0
	.globl	dsp_fft_float4_forward_xs3.maxchanends
.Ltmp0:
	.size	dsp_fft_float4_forward_xs3, .Ltmp0-dsp_fft_float4_forward_xs3

    .issue_mode  single
    
#endif
