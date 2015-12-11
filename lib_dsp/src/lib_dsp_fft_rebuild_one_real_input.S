// Copyright (c) 2015, XMOS Ltd, All rights reserved
    
	.section	.dp.data,"awd",@progbits
	.text
	.cc_top lib_dsp_fft_rebuild_one_real_input.function
	.globl	lib_dsp_fft_rebuild_one_real_input
	.align	4
	.type	lib_dsp_fft_rebuild_one_real_input,@function

#define NSTACKWORDS 4
#define p  			 	 r0
#define j   			 r1
#define re   			 r2
#define im   			 r3


#define i   			 r11

// void lib_dsp_fft_rebuild_one_real_input(fft_complex f[], unsigned N)

lib_dsp_fft_rebuild_one_real_input:
.align 8
.issue_mode dual
	DUALENTSP_lu6 NSTACKWORDS
	{ldc i, 1; sub j, j, 1}

	reorder_loop:
		ldd im, re, p[i]
		{neg im, im; add i, i, 1}
		std im, re, p[j]
		//TODO optimise this
		sub j, j, 1
		eq im,  i, j;
		bf im, reorder_loop;

	{ldw im, p[1]; ldc re, 0}
	std re, im, p[j]
	stw re, p[1];

	retsp NSTACKWORDS

.tmp_lib_dsp_fft_rebuild_one_real_input:
	.size	lib_dsp_fft_rebuild_one_real_input, .tmp_lib_dsp_fft_rebuild_one_real_input-lib_dsp_fft_rebuild_one_real_input
	.align	4
	.cc_bottom lib_dsp_fft_rebuild_one_real_input.function

	.set	lib_dsp_fft_rebuild_one_real_input.nstackwords,NSTACKWORDS
	.globl	lib_dsp_fft_rebuild_one_real_input.nstackwords
	.set	lib_dsp_fft_rebuild_one_real_input.maxcores,1
	.globl	lib_dsp_fft_rebuild_one_real_input.maxcores
	.set	lib_dsp_fft_rebuild_one_real_input.maxtimers,0
	.globl	lib_dsp_fft_rebuild_one_real_input.maxtimers
	.set	lib_dsp_fft_rebuild_one_real_input.maxchanends,0
	.globl	lib_dsp_fft_rebuild_one_real_input.maxchanends
