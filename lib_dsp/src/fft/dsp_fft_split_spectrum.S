// Copyright (c) 2015-2016, XMOS Ltd, All rights reserved
    
	.section	.dp.data,"awd",@progbits
	.text

#if defined(__XS2A__)
	.cc_top dsp_fft_split_spectrum_xs2.function
	.globl	dsp_fft_split_spectrum_xs2
	.align	4
	.type	dsp_fft_split_spectrum_xs2,@function

#define NSTACKWORDS 4
#define p  			 	 r0
#define j   			 r1
#define re0   			 r2
#define re1   			 r3

#define im0   			 r4
#define im1   			 r5

#define i   			 r11

// void dsp_fft_split_spectrum_xs2(fft_complex f[], unsigned N)

dsp_fft_split_spectrum_xs2:
.align 8
.issue_mode dual
	DUALENTSP_lu6 NSTACKWORDS

	std r5, r4, sp[0]
	std r6, r7, sp[1]
	mov r6, j
	{ldc i, 1; sub j, j, 1}

.Ltmp_reorder_loop:
	ldd im0, re0, p[i]
	ldd im1, re1, p[j]
	ashr im0, im0, 1
	ashr im1, im1, 1
	ashr re0, re0, 1
	ashr re1, re1, 1
	{add re0, re0, re1; sub re1, re1, re0}
	{add im0, im0, im1; sub im1, im0, im1}

	std im1, re0, p[i]
	std re1, im0, p[j]
	{add i, i, 1; sub j, j, 1}
	{eq im0,  i, j; nop}
	{bf im0, .Ltmp_reorder_loop; nop}

	{shr j, r6, 1} // j = N/2
	ldd re1, re0, p[0]
	ldd im1, im0, p[j]
	std im0, re0, p[0]
	std im1, re1, p[j]
	{add i, j, 1; sub j, r6, 1}	//i = N/2+1, j= N-1

.Ltmp_second_reorder_loop:
	ldd im0, re0, p[i]
	ldd im1, re1, p[j]
	std im1, re1, p[i]
	std im0, re0, p[j]
	{add i, i, 1; sub j, j, 1}
	{eq im0,  i, j; nop}
	{bf im0, .Ltmp_second_reorder_loop; nop}

	ldd r5, r4, sp[0]
	ldd r6, r7, sp[1]
	retsp NSTACKWORDS

.tmp_dsp_fft_split_spectrum_xs2:
	.size	dsp_fft_split_spectrum_xs2, .tmp_dsp_fft_split_spectrum_xs2-dsp_fft_split_spectrum_xs2
	.align	4
	.cc_bottom dsp_fft_split_spectrum_xs2.function

	.set	dsp_fft_split_spectrum_xs2.nstackwords,NSTACKWORDS
	.globl	dsp_fft_split_spectrum_xs2.nstackwords
	.set	dsp_fft_split_spectrum_xs2.maxcores,1
	.globl	dsp_fft_split_spectrum_xs2.maxcores
	.set	dsp_fft_split_spectrum_xs2.maxtimers,0
	.globl	dsp_fft_split_spectrum_xs2.maxtimers
	.set	dsp_fft_split_spectrum_xs2.maxchanends,0
	.globl	dsp_fft_split_spectrum_xs2.maxchanends
#endif
