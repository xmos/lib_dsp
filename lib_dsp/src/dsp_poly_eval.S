// Copyright 2017-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if (defined(__XS2A__) || defined (__XS3A__))

#define NSTACKWORDS 4

	.text
    .issue_mode  dual
    .align 4
	.globl	dsp_poly_eval_impl_xs2
	.type	dsp_poly_eval_impl_xs2,@function
	.cc_top dsp_poly_eval_impl_xs2.function,dsp_poly_eval_impl_xs2

	/*
int32_t dsp_poly_eval_impl_xs2
(
    int32_t x,
    const int32_t * coefs,
    const unsigned n_coefs
);
	*/
	#define x r0
	#define coefs r1
	#define n_coefs r2
	#define thirty_one r3

	#define h r4
	#define l r5
	#define rounding r6

	#define t r11

dsp_poly_eval_impl_xs2:
	{dualentsp NSTACKWORDS}
    std r4, r5, sp[0]
    std r6, r7, sp[1]

	{sub n_coefs, n_coefs, 1; ldc h, 1}
    {ldw t, coefs[n_coefs]; ldc l, 30}
    {shl rounding, h, l; ldc thirty_one, 31}

	{sub n_coefs, n_coefs, 1; nop}
    {ldc h, 0; mov l, rounding}
loop:
    maccs h, l, x, t
    lextract t, h, l, thirty_one, 32
    {ldw h, coefs[n_coefs]; mov l, rounding}
    {add t, t, h; ldc h, 0}
    {sub n_coefs, n_coefs, 1; bt n_coefs, loop}

done:
    ldd r4, r5, sp[0]
    ldd r6, r7, sp[1]
	{retsp NSTACKWORDS;mov r0, t}

	// RETURN_REG_HOLDER
	.cc_bottom dsp_poly_eval_impl_xs2.function
	.set	dsp_poly_eval_impl_xs2.nstackwords,NSTACKWORDS
	.globl	dsp_poly_eval_impl_xs2.nstackwords
	.set	dsp_poly_eval_impl_xs2.maxcores,1
	.globl	dsp_poly_eval_impl_xs2.maxcores
	.set	dsp_poly_eval_impl_xs2.maxtimers,0
	.globl	dsp_poly_eval_impl_xs2.maxtimers
	.set	dsp_poly_eval_impl_xs2.maxchanends,0
	.globl	dsp_poly_eval_impl_xs2.maxchanends
.Ltmp0:
	.size	dsp_poly_eval_impl_xs2, .Ltmp0-dsp_poly_eval_impl_xs2


#endif
