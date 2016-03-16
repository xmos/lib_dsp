// Copyright (c) 2016, XMOS Ltd, All rights reserved
    
	.section	.dp.data,"awd",@progbits
	.text

#if defined(__XS2A__)
	.cc_top lib_dsp_fft_short_to_long_xs2.function
	.globl	lib_dsp_fft_short_to_long_xs2
	.align	4
	.type	lib_dsp_fft_short_to_long_xs2,@function

#define NSTACKWORDS 4

#define SHIFT 16
//void lib_dsp_fft_short_to_long_xs2( lib_dsp_fft_complex_t l[], lib_dsp_fft_complex_short_t s[], unsigned N );

lib_dsp_fft_short_to_long_xs2:
.align 4
.issue_mode dual
	DUALENTSP_lu6 2
	{mkmsk r11, 16;sub r2, r2, 1}
	std r5, r4, sp[0]

.align 8
loop2:
	ldw r4, r1[r2]
	{andnot r4, r11; shl r3, r4, 16}
	std r4, r3, r0[r2]
	{bt r2, loop2; sub r2, r2, 1}

	ldd r5, r4, sp[0]
	retsp 2

.tmp_lib_dsp_fft_short_to_long_xs2:
	.size	lib_dsp_fft_short_to_long_xs2, .tmp_lib_dsp_fft_short_to_long_xs2-lib_dsp_fft_short_to_long_xs2
	.align	4
	.cc_bottom lib_dsp_fft_short_to_long_xs2.function

	.set	lib_dsp_fft_short_to_long_xs2.nstackwords,2
	.globl	lib_dsp_fft_short_to_long_xs2.nstackwords
	.set	lib_dsp_fft_short_to_long_xs2.maxcores,1
	.globl	lib_dsp_fft_short_to_long_xs2.maxcores
	.set	lib_dsp_fft_short_to_long_xs2.maxtimers,0
	.globl	lib_dsp_fft_short_to_long_xs2.maxtimers
	.set	lib_dsp_fft_short_to_long_xs2.maxchanends,0
	.globl	lib_dsp_fft_short_to_long_xs2.maxchanends


		.cc_top lib_dsp_fft_long_to_short_xs2.function
	.globl	lib_dsp_fft_long_to_short_xs2
	.align	4
	.type	lib_dsp_fft_long_to_short_xs2,@function

//void lib_dsp_fft_long_to_short_xs2( lib_dsp_fft_complex_short_t s[], lib_dsp_fft_complex_t l[], unsigned N );

lib_dsp_fft_long_to_short_xs2:
.align 8
.issue_mode dual
	DUALENTSP_lu6 2
	{ldc r3, 16;sub r2, r2, 1}
	std r5, r4, sp[0]
	loop:
	ldd r5, r4, r1[r2]
	{linsert r5, r4, r4, r3, 32};
	stw r5, r0[r2]
	{bt r2, loop; sub r2, r2, 1;}
	ldd r5, r4, sp[0]
	retsp 2

.tmp_lib_dsp_fft_long_to_short_xs2:
	.size	lib_dsp_fft_long_to_short_xs2, .tmp_lib_dsp_fft_long_to_short_xs2-lib_dsp_fft_long_to_short_xs2
	.align	4
	.cc_bottom lib_dsp_fft_long_to_short_xs2.function

	.set	lib_dsp_fft_long_to_short_xs2.nstackwords,2
	.globl	lib_dsp_fft_long_to_short_xs2.nstackwords
	.set	lib_dsp_fft_long_to_short_xs2.maxcores,1
	.globl	lib_dsp_fft_long_to_short_xs2.maxcores
	.set	lib_dsp_fft_long_to_short_xs2.maxtimers,0
	.globl	lib_dsp_fft_long_to_short_xs2.maxtimers
	.set	lib_dsp_fft_long_to_short_xs2.maxchanends,0
	.globl	lib_dsp_fft_long_to_short_xs2.maxchanends

#endif
