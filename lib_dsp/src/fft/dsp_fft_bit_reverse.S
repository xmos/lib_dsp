// Copyright (c) 2015-2017, XMOS Ltd, All rights reserved
    
#if defined(__XS2A__)

	.text
    .issue_mode  dual
	.globl	dsp_fft_bit_reverse
	.align	4
	.type	dsp_fft_bit_reverse,@function
	.cc_top dsp_fft_bit_reverse.function,dsp_fft_bit_reverse

#define NSTACKWORDS 4
    
dsp_fft_bit_reverse:
	{ dualentsp NSTACKWORDS ; clz r2, r1}
    std r6, r7, sp[1]
    std r4, r5, sp[0]
    { add r2, r2, 1             ; shr r1, r1, 1 }
    { shl r1, r1, 1             ; nop }

loop:   
    { bitrev r3, r1               ; shl r11, r1, r2 }
    { lsu r11, r3, r11            ; shr r3, r3, r2 }
    { bf r11, dontswap            ; sub r11, r1, 1 }
    ldd r4, r5, r0[r1]
    ldd r6, r7, r0[r3]
    std r4, r5, r0[r3]
    std r6, r7, r0[r1]
dontswap:
    { bitrev r3, r11              ; shl r1, r11, r2 }
    { lsu r1, r3, r1              ; shr r3, r3, r2 }
    { bf r1, dontswap2            ; sub r1, r11, 1 }
    ldd r4, r5, r0[r11]
    ldd r6, r7, r0[r3]
    std r4, r5, r0[r3]
    std r6, r7, r0[r11]
dontswap2:
    bt r1, loop
    
    ldd r6, r7, sp[1]
    ldd r4, r5, sp[0]
	retsp NSTACKWORDS
	
	// RETURN_REG_HOLDER
	.cc_bottom dsp_fft_bit_reverse.function
	.set	dsp_fft_bit_reverse.nstackwords,NSTACKWORDS
	.globl	dsp_fft_bit_reverse.nstackwords
	.set	dsp_fft_bit_reverse.maxcores,1
	.globl	dsp_fft_bit_reverse.maxcores
	.set	dsp_fft_bit_reverse.maxtimers,0
	.globl	dsp_fft_bit_reverse.maxtimers
	.set	dsp_fft_bit_reverse.maxchanends,0
	.globl	dsp_fft_bit_reverse.maxchanends
.Ltmp0:
	.size	dsp_fft_bit_reverse, .Ltmp0-dsp_fft_bit_reverse

    .issue_mode  single
    
#endif
