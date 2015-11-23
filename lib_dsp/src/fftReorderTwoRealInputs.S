	.section	.dp.data,"awd",@progbits
	.text
	.cc_top lib_dsp_fft_reorder_two_real_inputs.function
	.globl	lib_dsp_fft_reorder_two_real_inputs
	.align	4
	.type	lib_dsp_fft_reorder_two_real_inputs,@function

#define NSTACKWORDS 2
#define p  			 	 r0
#define j   			 r1
#define re0   			 r2
#define re1   			 r3

#define im0   			 r4
#define im1   			 r5

#define i   			 r11

// void lib_dsp_fft_reorder_two_real_inputs(fft_complex f[], unsigned N)

lib_dsp_fft_reorder_two_real_inputs:
.align 8
.issue_mode dual
	DUALENTSP_lu6 NSTACKWORDS

	ldc i, 1;
	shl j, i, r1	//mkmsk?
	sub j, j, 1
	std r5, r4, sp[0]

	reorder_loop:
	ldd im0, re0, p[i]
	ldd im1, re1, p[j]

	{sub im0, im0, im1;nop}
	ashr im0, im0, 1
	{add re1, im0, im1;sub im1, re1, re0}
	ashr im1, im1, 1
	{add re0, im1, re0;nop}

	std im0, re0, p[i]
	std im1, re1, p[j]
	{add i, i, 1; sub j, j, 1}
	{eq im0,  i, j;nop}
	{bf im0, reorder_loop;nop}


	ldd r5, r4, sp[0]
	retsp NSTACKWORDS

.tmp_lib_dsp_fft_reorder_two_real_inputs:
	.size	lib_dsp_fft_reorder_two_real_inputs, .tmp_lib_dsp_fft_reorder_two_real_inputs-lib_dsp_fft_reorder_two_real_inputs
	.align	4
	.cc_bottom lib_dsp_fft_reorder_two_real_inputs.function

	.set	lib_dsp_fft_reorder_two_real_inputs.nstackwords,NSTACKWORDS
	.globl	lib_dsp_fft_reorder_two_real_inputs.nstackwords
	.set	lib_dsp_fft_reorder_two_real_inputs.maxcores,1
	.globl	lib_dsp_fft_reorder_two_real_inputs.maxcores
	.set	lib_dsp_fft_reorder_two_real_inputs.maxtimers,0
	.globl	lib_dsp_fft_reorder_two_real_inputs.maxtimers
	.set	lib_dsp_fft_reorder_two_real_inputs.maxchanends,0
	.globl	lib_dsp_fft_reorder_two_real_inputs.maxchanends
