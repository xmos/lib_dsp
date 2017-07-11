// Copyright (c) 2015-2017, XMOS Ltd, All rights reserved
    
#if defined(__XS2A__)
    
#define NSTACKWORDS 2

	.text
    .issue_mode  dual
    .align 4
	.globl	dsp_complex_add_vector
	.type	dsp_complex_add_vector,@function
	.cc_top dsp_complex_add_vector.function,dsp_complex_add_vector
	
dsp_complex_add_vector:
	{ dualentsp NSTACKWORDS       ; sub r2, r2, 1 }
    std r4, r5, sp[0]
.Lloop:
    ldd r4, r5, r0[r2]
    ldd r3, r11, r1[r2]
    { add r4, r4, r3              ; add r5, r5, r11 }
    std r4, r5, r0[r2]
    { sub r2, r2, 1               ; bt r2, .Lloop }
    
    ldd r4, r5, sp[0]
	retsp NSTACKWORDS
	
	// RETURN_REG_HOLDER
	.cc_bottom dsp_complex_add_vector.function
	.set	dsp_complex_add_vector.nstackwords,NSTACKWORDS
	.globl	dsp_complex_add_vector.nstackwords
	.set	dsp_complex_add_vector.maxcores,1
	.globl	dsp_complex_add_vector.maxcores
	.set	dsp_complex_add_vector.maxtimers,0
	.globl	dsp_complex_add_vector.maxtimers
	.set	dsp_complex_add_vector.maxchanends,0
	.globl	dsp_complex_add_vector.maxchanends
.Ltmp0:
	.size	dsp_complex_add_vector, .Ltmp0-dsp_complex_add_vector

#undef NSTACKWORDS
#define NSTACKWORDS 4

    .align 4
	.globl	dsp_complex_add_vector_shl
	.type	dsp_complex_add_vector_shl,@function
	.cc_top dsp_complex_add_vector_shl.function,dsp_complex_add_vector_shl
	
dsp_complex_add_vector_shl:
	{ dualentsp NSTACKWORDS       ; clz r11, r3 }
    std r5, r6, sp[0]
    { sub r2, r2, 1               ; stw r4, sp[2] }
    { bf r11, .Lloop2r            ; neg r11, r3 }
.Lloop2l:
    ldd r4, r5, r0[r2]
    ldd r6, r11, r1[r2]
    { shl r6, r6, r3              ; shl r11, r11, r3 }
    { add r4, r4, r6              ; add r5, r5, r11 }
    std r4, r5, r0[r2]
    { sub r2, r2, 1               ; bt r2, .Lloop2l }
    bu .Ldone
    
.Lloop2r:
    ldd r4, r5, r0[r2]
    ldd r6, r3, r1[r2]
    ashr r6, r6, r11
    ashr r3, r3, r11
    { add r4, r4, r6              ; add r5, r5, r3 }
    std r4, r5, r0[r2]
    { sub r2, r2, 1               ; bt r2, .Lloop2r }
    
.Ldone:
    ldd r5, r6, sp[0]
	ldw r4, sp[2]
	retsp NSTACKWORDS
    
	// RETURN_REG_HOLDER
	.cc_bottom dsp_complex_add_vector_shl.function
	.set	dsp_complex_add_vector_shl.nstackwords,NSTACKWORDS
	.globl	dsp_complex_add_vector_shl.nstackwords
	.set	dsp_complex_add_vector_shl.maxcores,1
	.globl	dsp_complex_add_vector_shl.maxcores
	.set	dsp_complex_add_vector_shl.maxtimers,0
	.globl	dsp_complex_add_vector_shl.maxtimers
	.set	dsp_complex_add_vector_shl.maxchanends,0
	.globl	dsp_complex_add_vector_shl.maxchanends
.Ltmp1:
	.size	dsp_complex_add_vector_shl, .Ltmp1-dsp_complex_add_vector_shl
    
    .issue_mode  single
    
#endif
