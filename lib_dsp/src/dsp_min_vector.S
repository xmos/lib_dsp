// Copyright (c) 2015-2017, XMOS Ltd, All rights reserved
    
#if defined(__XS2A__)
    
#define NSTACKWORDS 2

	.text
    .issue_mode  dual
    .align 4
	.globl	dsp_vector_minv
	.type	dsp_vector_minv,@function
	.cc_top dsp_vector_minv.function,dsp_vector_minv
	
dsp_vector_minv:
	{ dualentsp NSTACKWORDS       ; sub r2, r2, 1 }
    std r4, r5, sp[0]
.Lloop:
    ldw r4, r0[r2]
    ldw r3, r1[r2]
    lsu r11, r4, r3
    bt r11, .Lnot
    stw r3, r0[r2]
.Lnot:
    { sub r2, r2, 1               ; bt r2, .Lloop }
    
    ldd r4, r5, sp[0]
	retsp NSTACKWORDS
	
	// RETURN_REG_HOLDER
	.cc_bottom dsp_vector_minv.function
	.set	dsp_vector_minv.nstackwords,NSTACKWORDS
	.globl	dsp_vector_minv.nstackwords
	.set	dsp_vector_minv.maxcores,1
	.globl	dsp_vector_minv.maxcores
	.set	dsp_vector_minv.maxtimers,0
	.globl	dsp_vector_minv.maxtimers
	.set	dsp_vector_minv.maxchanends,0
	.globl	dsp_vector_minv.maxchanends
.Ltmp0:
	.size	dsp_vector_minv, .Ltmp0-dsp_vector_minv

    
#endif
