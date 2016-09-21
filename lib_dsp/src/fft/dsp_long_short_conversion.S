// Copyright (c) 2016, XMOS Ltd, All rights reserved
    
	.section	.dp.data,"awd",@progbits
	.text

#if defined(__XS2A__)
	.cc_top dsp_fft_short_to_long_xs2.function
	.globl	dsp_fft_short_to_long_xs2
	.align	4
	.type	dsp_fft_short_to_long_xs2,@function

#define NSTACKWORDS 4

#define SHIFT 16
//void dsp_fft_short_to_long( const dsp_complex_short_t s[], dsp_complex_t l[], uint32_t N );

dsp_fft_short_to_long_xs2:
.align 4
.issue_mode dual
	DUALENTSP_lu6 2
	{mkmsk r11, 16;sub r2, r2, 1}
	std r5, r4, sp[0]

.align 8
.Ltmp_loop2:
	ldw r4, r0[r2]
	{andnot r4, r11; shl r3, r4, 16}
	std r4, r3, r1[r2]
	{bt r2, .Ltmp_loop2; sub r2, r2, 1}

	ldd r5, r4, sp[0]
	retsp 2

.tmp_dsp_fft_short_to_long_xs2:
	.size	dsp_fft_short_to_long_xs2, .tmp_dsp_fft_short_to_long_xs2-dsp_fft_short_to_long_xs2
	.align	4
	.cc_bottom dsp_fft_short_to_long_xs2.function

	.set	dsp_fft_short_to_long_xs2.nstackwords,2
	.globl	dsp_fft_short_to_long_xs2.nstackwords
	.set	dsp_fft_short_to_long_xs2.maxcores,1
	.globl	dsp_fft_short_to_long_xs2.maxcores
	.set	dsp_fft_short_to_long_xs2.maxtimers,0
	.globl	dsp_fft_short_to_long_xs2.maxtimers
	.set	dsp_fft_short_to_long_xs2.maxchanends,0
	.globl	dsp_fft_short_to_long_xs2.maxchanends


		.cc_top dsp_fft_long_to_short_xs2.function
	.globl	dsp_fft_long_to_short_xs2
	.align	4
	.type	dsp_fft_long_to_short_xs2,@function

//void dsp_fft_long_to_short( const dsp_complex_t l[], dsp_complex_short_t s[], uint32_t N );

dsp_fft_long_to_short_xs2:
.align 8
.issue_mode dual
	DUALENTSP_lu6 2
	{ldc r3, 16;sub r2, r2, 1}
	std r5, r4, sp[0]
	loop:
	ldd r5, r4, r0[r2]
	{linsert r5, r4, r4, r3, 32};
	stw r5, r1[r2]
	{bt r2, loop; sub r2, r2, 1;}
	ldd r5, r4, sp[0]
	retsp 2

.tmp_dsp_fft_long_to_short_xs2:
	.size	dsp_fft_long_to_short_xs2, .tmp_dsp_fft_long_to_short_xs2-dsp_fft_long_to_short_xs2
	.align	4
	.cc_bottom dsp_fft_long_to_short_xs2.function

	.set	dsp_fft_long_to_short_xs2.nstackwords,2
	.globl	dsp_fft_long_to_short_xs2.nstackwords
	.set	dsp_fft_long_to_short_xs2.maxcores,1
	.globl	dsp_fft_long_to_short_xs2.maxcores
	.set	dsp_fft_long_to_short_xs2.maxtimers,0
	.globl	dsp_fft_long_to_short_xs2.maxtimers
	.set	dsp_fft_long_to_short_xs2.maxchanends,0
	.globl	dsp_fft_long_to_short_xs2.maxchanends

#endif
