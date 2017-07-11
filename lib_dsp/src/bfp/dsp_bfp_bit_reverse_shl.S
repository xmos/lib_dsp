// Copyright (c) 2015-2017, XMOS Ltd, All rights reserved
    
#if defined(__XS2A__)

	.text
    .issue_mode  dual
	.globl	dsp_bfp_bit_reverse_shl
	.align	4
	.type	dsp_bfp_bit_reverse_shl,@function
	.cc_top dsp_bfp_bit_reverse_shl.function,dsp_bfp_bit_reverse_shl

#define NSTACKWORDS 6
    
dsp_bfp_bit_reverse_shl:
	{ dualentsp NSTACKWORDS     ; clz r11, r2 }
    { stw r8, sp[4]             ; clz r8, r1}
    { add r8, r8, 1             ; shr r1, r1, 1 }
    { shl r1, r1, 1             ; nop }
    std r6, r7, sp[1]
    std r4, r5, sp[0]

    
    { bf r11, .right              ;    sub r1, r1, 1 }

        
    // LEFT
    
.loopl:   
    { bitrev r3, r1               ; shl r11, r1, r8 }
    { lsu r11, r3, r11            ; eq r4, r3, r11 }
    { bt r4, .justshiftl          ; shr r3, r3, r8 }
    { bf r11, .dontswapl          ; sub r11, r1, 1 }
    ldd r4, r5, r0[r1]
    ldd r6, r7, r0[r3]
    { shl r4, r4, r2              ; shl r5, r5, r2 }
    { shl r6, r6, r2              ; shl r7, r7, r2 }
    std r4, r5, r0[r3]
    std r6, r7, r0[r1]
.dontswapl:
    { bitrev r3, r11              ; shl r1, r11, r8 }
    { lsu r1, r3, r1              ; eq r4, r3, r1 }
    { bt r4, .justshiftl2         ; shr r3, r3, r8 }
    { bf r1, .dontswapl2          ; sub r1, r11, 1 }
    ldd r4, r5, r0[r11]
    ldd r6, r7, r0[r3]
    { shl r4, r4, r2              ; shl r5, r5, r2 }
    { shl r6, r6, r2              ; shl r7, r7, r2 }
    std r4, r5, r0[r3]
    std r6, r7, r0[r11]
.dontswapl2:
    bt r11, .loopl
    
    ldd r6, r7, sp[1]
    ldd r4, r5, sp[0]
    ldw r8, sp[4]
	retsp NSTACKWORDS
    
.justshiftl:
    ldd r4, r5, r0[r3]
    { shl r4, r4, r2              ; shl r5, r5, r2 }
    std r4, r5, r0[r3]
    { bu .dontswapl             ; sub r11, r1, 1 }
    
.justshiftl2:
    ldd r4, r5, r0[r3]
    { shl r4, r4, r2              ; shl r5, r5, r2 }
    std r4, r5, r0[r3]
    { bu .dontswapl2            ; sub r1, r11, 1 }
    

    
    // RIGHT    
.right:
    neg r2, r2                  
    
.loopr:   
    { bitrev r3, r1               ; shl r11, r1, r8 }
    { lsu r11, r3, r11            ; eq r4, r3, r11 }
    { bt r4, .justshiftr          ; shr r3, r3, r8 }
    { bf r11, .dontswapr          ; sub r11, r1, 1 }
    ldd r4, r5, r0[r1]
    ldd r6, r7, r0[r3]
    ashr r4, r4, r2
    ashr r5, r5, r2
    ashr r6, r6, r2
    ashr r7, r7, r2
    std r4, r5, r0[r3]
    std r6, r7, r0[r1]
.dontswapr:
    { bitrev r3, r11              ; shl r1, r11, r8 }
    { lsu r1, r3, r1              ; eq r4, r3, r1 }
    { bt r4, .justshiftr2         ; shr r3, r3, r8 }
    { bf r1, .dontswapr2          ; sub r1, r11, 1 }
    ldd r4, r5, r0[r11]
    ldd r6, r7, r0[r3]
    ashr r4, r4, r2
    ashr r5, r5, r2
    ashr r6, r6, r2
    ashr r7, r7, r2
    std r4, r5, r0[r3]
    std r6, r7, r0[r11]
.dontswapr2:
    bt r11, .loopr
    
    ldd r6, r7, sp[1]
    ldd r4, r5, sp[0]
    ldw r8, sp[4]
	retsp NSTACKWORDS
    
.justshiftr:
    ldd r4, r5, r0[r3]
    ashr r4, r4, r2
    ashr r5, r5, r2
    std r4, r5, r0[r3]
    { bu .dontswapr             ; sub r11, r1, 1 }
    
.justshiftr2:
    ldd r4, r5, r0[r3]
    ashr r4, r4, r2
    ashr r5, r5, r2
    std r4, r5, r0[r3]
    { bu .dontswapr2            ; sub r1, r11, 1 }
    
	// RETURN_REG_HOLDER
	.cc_bottom dsp_bfp_bit_reverse_shl.function
	.set	dsp_bfp_bit_reverse_shl.nstackwords,NSTACKWORDS
	.globl	dsp_bfp_bit_reverse_shl.nstackwords
	.set	dsp_bfp_bit_reverse_shl.maxcores,1
	.globl	dsp_bfp_bit_reverse_shl.maxcores
	.set	dsp_bfp_bit_reverse_shl.maxtimers,0
	.globl	dsp_bfp_bit_reverse_shl.maxtimers
	.set	dsp_bfp_bit_reverse_shl.maxchanends,0
	.globl	dsp_bfp_bit_reverse_shl.maxchanends
.Ltmp0:
	.size	dsp_bfp_bit_reverse_shl, .Ltmp0-dsp_bfp_bit_reverse_shl

    .issue_mode  single
    
#endif
