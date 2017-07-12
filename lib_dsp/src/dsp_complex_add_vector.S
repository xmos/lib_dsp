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
#define NSTACKWORDS 8

	.text
    .issue_mode  dual
    .align 4
	.globl	dsp_complex_add_vector_scale
	.type	dsp_complex_add_vector_scale,@function
	.cc_top dsp_complex_add_vector_scale.function,dsp_complex_add_vector_scale
	
dsp_complex_add_vector_scale:
	{ dualentsp NSTACKWORDS       ; sub r2, r2, 1 }
    std r4, r5, sp[0]
    std r6, r7, sp[1]
    std r8, r9, sp[2]
    { stw r10, sp[6]              ; ldc r10, 24 }
.Lloop3:
    ldd r4, r5, r0[r2]
    ldd r8, r11, r1[r2]
    ashr r6, r4, 8
    { shl r7, r4, 24              ; shl r9, r5, 24 }
    maccs r6, r7, r3, r8
    lextract r4, r6, r7, r10, 32
    ashr r6, r5, 8
    maccs r6, r9, r3, r11
    lextract r5, r6, r9, r10, 32
    std r4, r5, r0[r2]
    { sub r2, r2, 1               ; bt r2, .Lloop3 }
    
    ldw r10, sp[6]
    ldd r4, r5, sp[0]
    ldd r6, r7, sp[1]
    ldd r8, r9, sp[2]
	retsp NSTACKWORDS
	
	// RETURN_REG_HOLDER
	.cc_bottom dsp_complex_add_vector_scale.function
	.set	dsp_complex_add_vector_scale.nstackwords,NSTACKWORDS
	.globl	dsp_complex_add_vector_scale.nstackwords
	.set	dsp_complex_add_vector_scale.maxcores,1
	.globl	dsp_complex_add_vector_scale.maxcores
	.set	dsp_complex_add_vector_scale.maxtimers,0
	.globl	dsp_complex_add_vector_scale.maxtimers
	.set	dsp_complex_add_vector_scale.maxchanends,0
	.globl	dsp_complex_add_vector_scale.maxchanends
.Ltmp3:
	.size	dsp_complex_add_vector_scale, .Ltmp3-dsp_complex_add_vector_scale

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
