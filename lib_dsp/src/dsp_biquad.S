// Copyright 2017-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if (defined(__XS2A__) || defined (__XS3A__))

#define NSTACKWORDS 8

	.text
    .issue_mode  dual
    .align 4
	.globl	dsp_filters_biquads
	.type	dsp_filters_biquads,@function
	.cc_top dsp_filters_biquads.function,dsp_filters_biquads

	/*
int32_t dsp_filters_biquads
(
    int32_t       input_sample,
    const int32_t filter_coeffs[],
    int32_t       state_data[],
    const uint32_t num_sections,
    const uint32_t q_format
);
	*/
	#define s0 r0
	#define filter_coeffs r1
	#define state_data r2
	#define num_sections r3
	#define h r4
	#define l r5
	#define s1 r6
	#define s2 r7
	#define c0 r8
	#define c1 r9

	#define q r11

dsp_filters_biquads:
	{dualentsp NSTACKWORDS}
    std r4, r5, sp[0]
    std r6, r7, sp[1]
    std r8, r9, sp[2]
	{ldw q, sp[NSTACKWORDS+1];sub num_sections, num_sections, 1}
loop:
	{ldc h, 0; mkmsk l, q}			//s0 = x[n]
	shr l, l, 1
	ldd c1, c0, filter_coeffs[0] 	// b0 and b1
	ldd s1, s2, state_data[0]	 	// x[n-1] and x[n-2]
	std s0, s1, state_data[0]

	maccs h, l, s0, c0 				// x[n] * b0
	maccs h, l, s1, c1				// x[n-1] * b1

	ldd c1, c0, filter_coeffs[1] 	// b2 and a1

	maccs h, l, s2, c0 				// x[n-2] * b2
	ldd s1, s2, state_data[1]	 	// y[n-1] and y[n-2]
	maccs h, l, s1, c1				// y[n-1] * a1

	ldd c0, c1, filter_coeffs[2] 	// a2 and b0(next round)
	maccs h, l, s2, c1				// y[n-2] * a2

	lsats h, l, q
	lextract s0, h, l, q, 32
	std s0, s1, state_data[1]

	{bf num_sections, done;sub num_sections, num_sections, 1}

	{ldc h, 0; mkmsk l, q}			//s0 = x[n]
	shr l, l, 1
	ldd s1, s2, state_data[2]	 	// x[n-1] and x[n-2]
	std s0, s1, state_data[2]

	maccs h, l, s0, c0 				// x[n] * b0
	ldd c1, c0, filter_coeffs[3] 	// b1 and b2

	maccs h, l, s1, c0				// x[n-1] * b1
	maccs h, l, s2, c1 				// x[n-2] * b2

	ldd c0, c1, filter_coeffs[4] 	// a1 and a2

	ldd s1, s2, state_data[3]	 	// y[n-1] and y[n-2]

	maccs h, l, s1, c1				// y[n-1] * a1
	maccs h, l, s2, c0				// y[n-2] * a2

	lsats h, l, q
	lextract s0, h, l, q, 32
	std s0, s1, state_data[3]

	ldaw filter_coeffs, filter_coeffs[10]
	ldaw state_data, state_data[8]

	{bt num_sections, loop;sub num_sections, num_sections, 1}

done:
    ldd r4, r5, sp[0]
    ldd r6, r7, sp[1]
    ldd r8, r9, sp[2]
	retsp NSTACKWORDS

	// RETURN_REG_HOLDER
	.cc_bottom dsp_filters_biquads.function
	.set	dsp_filters_biquads.nstackwords,NSTACKWORDS
	.globl	dsp_filters_biquads.nstackwords
	.set	dsp_filters_biquads.maxcores,1
	.globl	dsp_filters_biquads.maxcores
	.set	dsp_filters_biquads.maxtimers,0
	.globl	dsp_filters_biquads.maxtimers
	.set	dsp_filters_biquads.maxchanends,0
	.globl	dsp_filters_biquads.maxchanends
.Ltmp0:
	.size	dsp_filters_biquads, .Ltmp0-dsp_filters_biquads


#endif
