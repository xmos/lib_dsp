// Copyright (c) 2015-2017, XMOS Ltd, All rights reserved
    
#if defined(__XS2A__)

	.text
    .issue_mode  dual
	.globl	dsp_complex_fir
	.type	dsp_complex_fir,@function
	.cc_top dsp_complex_fir.function,dsp_complex_fir
	
dsp_complex_fir:
	dualentsp 8
    std r9, r10, sp[2]
    { nop                        ; stw r4, sp[6] }
    { ldw r11, sp[9]             ; ldc r9, 0}
    { ldc r10, 0                 ; shl r11, r11, 3}
    { add r1, r1, r11            ; ldc r11, 0}
    std r7, r8, sp[1]
    std r5, r6, sp[0]
    { ldc r4, 0                   ; ldc r8, 0 }
.Lloop:
    maccs r8, r9, r4, r6
    sub r3, r3, 1
    ldd r4, r5, r1[r3]
    ldd r6, r7, r2[r3]
    maccs r10, r11, r4, r7
    maccs r10, r11, r5, r6
    maccs r8, r9, r5, r7
    { neg r6, r6                 ; bt r3, .Lloop }
    
    maccs r8, r9, r4, r6
    ldw r3, sp[10]
    lextract r8, r8, r9, r3, 32
    ldd r5, r6, sp[0]
	ldw r4, sp[6]
    lextract r9, r10, r11, r3, 32

    std r9, r8, r0[0]
    ldd r9, r10, sp[2]
    ldd r7, r8, sp[1]
	retsp 8
	
	// RETURN_REG_HOLDER
	.cc_bottom dsp_complex_fir.function
	.set	dsp_complex_fir.nstackwords,8
	.globl	dsp_complex_fir.nstackwords
	.set	dsp_complex_fir.maxcores,1
	.globl	dsp_complex_fir.maxcores
	.set	dsp_complex_fir.maxtimers,0
	.globl	dsp_complex_fir.maxtimers
	.set	dsp_complex_fir.maxchanends,0
	.globl	dsp_complex_fir.maxchanends
.Ltmp0:
	.size	dsp_complex_fir, .Ltmp0-dsp_complex_fir

    .issue_mode  single
    
#endif
