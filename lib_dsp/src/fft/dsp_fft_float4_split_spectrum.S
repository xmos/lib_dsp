// Copyright (c) 2015-2016, XMOS Ltd, All rights reserved
    
	.text

#if defined(__XS3A__)
	.cc_top dsp_fft_float4_split_spectrum_xs3.function
	.globl	dsp_fft_float4_split_spectrum_xs3
	.align	4
	.type	dsp_fft_float4_split_spectrum_xs3,@function

#define NSTACKWORDS 8
#define p  			 	 r0
#define j   			 r1
#define re0   			 r2
#define re1   			 r3

#define im0   			 r4
#define im1   			 r5

#define i   			 r11

// void dsp_fft_float4_split_spectrum_xs3(fft_complex f[], unsigned N)

dsp_fft_float4_split_spectrum_xs3:
.align 16
.issue_mode dual
	DUALENTSP_lu6 NSTACKWORDS

	std r5, r4, sp[0]
	std r6, r7, sp[1]
	std r8, r9, sp[2]
	stw r10,    sp[6]
	{mov r6, j                  ; mov r10, r2}
	{ldc i, 1; sub j, j, 1}
    
.Ltmp_reorder_loop:
	ldd im0, re0, p[i]
	ldd im1, re1, p[j]
	fadd r8, re0, re1
    fsub re1, re1, re0
	fadd r9, im0, im1
    fsub im1, im0, im1
    fmul r8, r8, r10
    fmul re1, re1, r10
    fmul r9, r9, r10
    fmul im1, im1, r10

	std im1, r8, p[i]
	std re1, r9, p[j]
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
	ldd r8, r9, sp[2]
	ldw r10,    sp[6]
	retsp NSTACKWORDS

.tmp_dsp_fft_float4_split_spectrum_xs3:
	.size	dsp_fft_float4_split_spectrum_xs3, .tmp_dsp_fft_float4_split_spectrum_xs3-dsp_fft_float4_split_spectrum_xs3
	.align	4
	.cc_bottom dsp_fft_float4_split_spectrum_xs3.function

	.set	dsp_fft_float4_split_spectrum_xs3.nstackwords,NSTACKWORDS
	.globl	dsp_fft_float4_split_spectrum_xs3.nstackwords
	.set	dsp_fft_float4_split_spectrum_xs3.maxcores,1
	.globl	dsp_fft_float4_split_spectrum_xs3.maxcores
	.set	dsp_fft_float4_split_spectrum_xs3.maxtimers,0
	.globl	dsp_fft_float4_split_spectrum_xs3.maxtimers
	.set	dsp_fft_float4_split_spectrum_xs3.maxchanends,0
	.globl	dsp_fft_float4_split_spectrum_xs3.maxchanends
#endif
