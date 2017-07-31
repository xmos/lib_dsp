// Copyright (c) 2015-2017, XMOS Ltd, All rights reserved
    
#if defined(__XS2A__)

#undef NSTACKWORDS
#define NSTACKWORDS 16

	.text
    .issue_mode  dual
	.globl	dsp_fft_real_fix_forward_xs2
	.align	16
    .skip 0
	.type	dsp_fft_real_fix_forward_xs2,@function
	.cc_top dsp_fft_real_fix_forward_xs2.function,dsp_fft_real_fix_forward_xs2
	
dsp_fft_real_fix_forward_xs2:

	dualentsp NSTACKWORDS
    
	std r4, r5, sp[1]
	std r6, r7, sp[2]
	std r8, r9, sp[3]
	{ stw r10, sp[8]              ; ldc r9, 31 }
    { stw r1, sp[11]              ; ldc r6, 0 }
    ldd r4, r5, r0[r6]
    { add r5, r4, r5              ;    sub r4, r5, r4 }
    { stw r2, sp[9]               ; ldc r2, 1 }
    { shl r9, r2, r9                ; stw r2, sp[10] }
    ashr r4, r4, 1
    stw r9, sp[14]
    ashr r5, r5, 1
    std r4, r5, r0[r6]
.dsp_fft_real_fix_loop:
    { ldw r7, sp[9]               ; sub r11, r1, r2 }
    ldd r6, r5, r0[r2]
    ldd r4, r3, r0[r11]
    { ldw r10, r7[r2]             ; shr r9, r1, 1 }
    { shr r10, r10, 1             ; ldw r1, sp[14] }
    { shr r8, r1, 1               ; sub r2, r9, r2 }
    { sub r11, r8, r10            ; add r10, r8, r10 }
    { neg r7, r10                 ; ldw r9, r7[r2] }
    { shr r9, r9, 1               ; ldc r2, 0 }
    maccs r2, r1, r11, r5
    maccs r2, r1, r9, r6
    maccs r2, r1, r10, r3
    maccs r2, r1, r9, r4
    { neg r8, r9                  ; stw r2, sp[12]}
    { ldc r2, 0                   ; ldw r1, sp[14]}
    maccs r2, r1, r11, r6
    maccs r2, r1, r8, r5
    maccs r2, r1, r9, r3
    maccs r2, r1, r7, r4
    stw r2, sp[13]
    { ldc r2, 0                   ; ldw r1, sp[14]}
    maccs r2, r1, r11, r3
    maccs r2, r1, r8, r4
    maccs r2, r1, r10, r5
    maccs r2, r1, r8, r6
    { ldc r10, 0                  ; ldw r1, sp[14]}
    maccs r10, r1, r11, r4
    maccs r10, r1, r9, r3
    maccs r10, r1, r8, r5
    maccs r10, r1, r7, r6
    ldw r3, sp[10]
    { ldw r1, sp[11]              ; add r7, r3, 1 }
    { sub r11, r1, r3             ; shr r8, r1, 1 }
    std r10, r2, r0[r11]
    ldd r10, r2, sp[6]
    std r10, r2, r0[r3]
    { lsu r8, r7, r8              ; stw r7, sp[10] }
    { bt r8, .dsp_fft_real_fix_loop            ; add r2, r7, 0 }
    
    ldd r4, r5, r0[r2]
    ashr r4, r4, 1
	ldd r6, r7, sp[2]
	ldd r8, r9, sp[3]
	ldw r10, sp[8]
    ashr r5, r5, 1
    neg r4, r4
    std r4, r5, r0[r2]
	ldd r4, r5, sp[1]
    retsp NSTACKWORDS
	
	// RETURN_REG_HOLDER
	.cc_bottom dsp_fft_real_fix_forward_xs2.function
	.set	dsp_fft_real_fix_forward_xs2.nstackwords,NSTACKWORDS
	.globl	dsp_fft_real_fix_forward_xs2.nstackwords
	.set	dsp_fft_real_fix_forward_xs2.maxcores,1
	.globl	dsp_fft_real_fix_forward_xs2.maxcores
	.set	dsp_fft_real_fix_forward_xs2.maxtimers,0
	.globl	dsp_fft_real_fix_forward_xs2.maxtimers
	.set	dsp_fft_real_fix_forward_xs2.maxchanends,0
	.globl	dsp_fft_real_fix_forward_xs2.maxchanends
.Ltmpdsp_fft_real_fix_forward_xs2:
	.size	dsp_fft_real_fix_forward_xs2, .Ltmpdsp_fft_real_fix_forward_xs2-dsp_fft_real_fix_forward_xs2
    
#undef NSTACKWORDS
#define NSTACKWORDS 16

	.text
    .issue_mode  dual
	.globl	dsp_fft_real_fix_inverse_xs2
	.align	16
    .skip 0
	.type	dsp_fft_real_fix_inverse_xs2,@function
	.cc_top dsp_fft_real_fix_inverse_xs2.function,dsp_fft_real_fix_inverse_xs2
	
dsp_fft_real_fix_inverse_xs2:

	dualentsp NSTACKWORDS
    
	std r4, r5, sp[1]
	std r6, r7, sp[2]
	std r8, r9, sp[3]
	{ stw r10, sp[8]              ; ldc r9, 31 }
    { stw r1, sp[11]              ; ldc r6, 0 }
    ldd r4, r5, r0[r6]
    { add r5, r4, r5              ; sub r4, r5, r4 }
    { stw r2, sp[9]               ; ldc r2, 1 }
    { shl r9, r2, r9              ; stw r2, sp[10] }
    stw r9, sp[14]
    std r4, r5, r0[r6]
.dsp_fft_real_fix_inverse_loop:
    { ldw r7, sp[9]               ; sub r11, r1, r2 }
    ldd r6, r5, r0[r2]
    ldd r4, r3, r0[r11]
    { ldw r10, r7[r2]             ; shr r9, r1, 1 }
    { shr r10, r10, 1             ; ldw r1, sp[14] }
    { shr r8, r1, 1               ; sub r2, r9, r2 }
    { sub r11, r8, r10            ; add r10, r8, r10 }
    { neg r7, r10                 ; ldw r9, r7[r2] }
    { shr r8, r9, 1               ; ldc r2, 0 }
    neg r9, r8
    maccs r2, r1, r11, r6
    maccs r2, r1, r8, r5
    maccs r2, r1, r9, r3
    maccs r2, r1, r7, r4
    { nop                         ; stw r2, sp[13]}
    
    { ldc r2, 0                   ; ldw r1, sp[14]}
    maccs r2, r1, r11, r5
    maccs r2, r1, r9, r6
    maccs r2, r1, r10, r3
    maccs r2, r1, r9, r4
    stw r2, sp[12]
    
    { ldc r2, 0                   ; ldw r1, sp[14]}
    maccs r2, r1, r11, r3
    maccs r2, r1, r8, r4
    maccs r2, r1, r10, r5
    maccs r2, r1, r8, r6
    { ldc r10, 0                  ; ldw r1, sp[14]}
    maccs r10, r1, r11, r4
    maccs r10, r1, r9, r3
    maccs r10, r1, r8, r5
    maccs r10, r1, r7, r6
    { shl r10, r10, 2             ; shl r2, r2, 2 }
    ldw r3, sp[10]
    { ldw r1, sp[11]              ; add r7, r3, 1 }
    { sub r11, r1, r3             ; shr r8, r1, 1 }
    std r10, r2, r0[r11]
    ldd r10, r2, sp[6]
    { shl r10, r10, 2             ; shl r2, r2, 2 }
    std r10, r2, r0[r3]
    { lsu r8, r7, r8              ; stw r7, sp[10] }
    { bt r8, .dsp_fft_real_fix_inverse_loop            ; add r2, r7, 0 }
    
    ldd r4, r5, r0[r2]
	ldd r6, r7, sp[2]
	ldd r8, r9, sp[3]
	ldw r10, sp[8]
    { shl r4, r4, 1               ; shl r5, r5, 1 }
    neg r4, r4
    std r4, r5, r0[r2]
	ldd r4, r5, sp[1]
    retsp NSTACKWORDS
	
	// RETURN_REG_HOLDER
	.cc_bottom dsp_fft_real_fix_inverse_xs2.function
	.set	dsp_fft_real_fix_inverse_xs2.nstackwords,NSTACKWORDS
	.globl	dsp_fft_real_fix_inverse_xs2.nstackwords
	.set	dsp_fft_real_fix_inverse_xs2.maxcores,1
	.globl	dsp_fft_real_fix_inverse_xs2.maxcores
	.set	dsp_fft_real_fix_inverse_xs2.maxtimers,0
	.globl	dsp_fft_real_fix_inverse_xs2.maxtimers
	.set	dsp_fft_real_fix_inverse_xs2.maxchanends,0
	.globl	dsp_fft_real_fix_inverse_xs2.maxchanends
.Ltmpdsp_fft_real_fix_inverse_xs2:
	.size	dsp_fft_real_fix_inverse_xs2, .Ltmpdsp_fft_real_fix_inverse_xs2-dsp_fft_real_fix_inverse_xs2
#endif
