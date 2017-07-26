// Copyright (c) 2015-2017, XMOS Ltd, All rights reserved
    
#if defined(__XS2A__)

	.text
    .issue_mode  dual
    .align 4
	.globl	dsp_complex_mul_vector
	.type	dsp_complex_mul_vector,@function
	.cc_top dsp_complex_mul_vector.function,dsp_complex_mul_vector

#undef NSTACKWORDS
#define NSTACKWORDS 8
	
dsp_complex_mul_vector:
	dualentsp NSTACKWORDS
    std r5, r6, sp[0]
    std r7, r8, sp[1]
    std r9, r10, sp[2]
    { sub r2, r2, 1               ; stw r4, sp[6] }
.Lloop:
    { ldc r10, 0                  ; ldc r11, 0  }
    { ldc r8, 0                   ; ldc r9, 0   }
    ldd r4, r5, r0[r2]
    ldd r6, r7, r1[r2]
    maccs r10, r11, r4, r7
    maccs r10, r11, r5, r6
    { neg r6, r6                  ; nop }
    maccs r8, r9, r5, r7
    maccs r8, r9, r4, r6
    lextract r8, r8, r9, r3, 32
    lextract r10, r10, r11, r3, 32
    std r10, r8, r0[r2]
    { sub r2, r2, 1               ; bt r2, .Lloop }
    
    ldd r5, r6, sp[0]
    ldd r7, r8, sp[1]
    ldd r9, r10, sp[2]
	ldw r4, sp[6]
	retsp NSTACKWORDS
	
	// RETURN_REG_HOLDER
	.cc_bottom dsp_complex_mul_vector.function
	.set	dsp_complex_mul_vector.nstackwords,NSTACKWORDS
	.globl	dsp_complex_mul_vector.nstackwords
	.set	dsp_complex_mul_vector.maxcores,1
	.globl	dsp_complex_mul_vector.maxcores
	.set	dsp_complex_mul_vector.maxtimers,0
	.globl	dsp_complex_mul_vector.maxtimers
	.set	dsp_complex_mul_vector.maxchanends,0
	.globl	dsp_complex_mul_vector.maxchanends
.Ltmpdsp_complex_mul_vector:
	.size	dsp_complex_mul_vector, .Ltmpdsp_complex_mul_vector-dsp_complex_mul_vector


#undef NSTACKWORDS
#define NSTACKWORDS 8

    .align 4
	.globl	dsp_complex_mul_conjugate_vector
	.type	dsp_complex_mul_conjugate_vector,@function
	.cc_top dsp_complex_mul_conjugate_vector.function,dsp_complex_mul_conjugate_vector
	
dsp_complex_mul_conjugate_vector:
	dualentsp NSTACKWORDS
    std r5, r6, sp[0]
    std r7, r8, sp[1]
    std r9, r10, sp[2]
    { sub r2, r2, 1               ; stw r4, sp[6] }
.Lloop2:
    { ldc r10, 0                  ; ldc r11, 0  }
    { ldc r8, 0                   ; ldc r9, 0   }
    ldd r4, r5, r0[r2]
    ldd r6, r7, r1[r2]
    maccs r8, r9, r5, r7
    maccs r8, r9, r4, r6
    { neg r6, r6                  ; nop }
    maccs r10, r11, r4, r7
    maccs r10, r11, r5, r6
    lextract r8, r8, r9, r3, 32
    lextract r10, r10, r11, r3, 32
    std r10, r8, r0[r2]
    { sub r2, r2, 1               ; bt r2, .Lloop2 }
    
    ldd r5, r6, sp[0]
    ldd r7, r8, sp[1]
    ldd r9, r10, sp[2]
	ldw r4, sp[6]
	retsp NSTACKWORDS
	
	// RETURN_REG_HOLDER
	.cc_bottom dsp_complex_mul_conjugate_vector.function
	.set	dsp_complex_mul_conjugate_vector.nstackwords,NSTACKWORDS
	.globl	dsp_complex_mul_conjugate_vector.nstackwords
	.set	dsp_complex_mul_conjugate_vector.maxcores,1
	.globl	dsp_complex_mul_conjugate_vector.maxcores
	.set	dsp_complex_mul_conjugate_vector.maxtimers,0
	.globl	dsp_complex_mul_conjugate_vector.maxtimers
	.set	dsp_complex_mul_conjugate_vector.maxchanends,0
	.globl	dsp_complex_mul_conjugate_vector.maxchanends
.Ltmpdsp_complex_mul_conjugate_vector:
	.size	dsp_complex_mul_conjugate_vector, .Ltmpdsp_complex_mul_conjugate_vector-dsp_complex_mul_conjugate_vector
    

#undef NSTACKWORDS
#define NSTACKWORDS 8

    .align 4
	.globl	dsp_complex_mul_conjugate_vector3
	.type	dsp_complex_mul_conjugate_vector3,@function
	.cc_top dsp_complex_mul_conjugate_vector3.function,dsp_complex_mul_conjugate_vector3
	
dsp_complex_mul_conjugate_vector3:
	dualentsp NSTACKWORDS
    std r5, r6, sp[0]
    std r7, r8, sp[1]
    std r9, r10, sp[2]
    { sub r3, r3, 1               ; stw r4, sp[6] }
.Lloopdsp_complex_mul_conjugate_vector3:
    { ldc r10, 0                  ; ldc r11, 0  }
    { ldc r8, 0                   ; ldc r9, 0   }
    ldd r4, r5, r1[r3]
    ldd r6, r7, r2[r3]
    maccs r8, r9, r5, r7
    maccs r8, r9, r4, r6
    maccs r10, r11, r4, r7
    { neg r6, r6                  ; ldw r7, sp[NSTACKWORDS+1] }
    maccs r10, r11, r5, r6
    lextract r8, r8, r9, r7, 32
    lextract r10, r10, r11, r7, 32
    std r10, r8, r0[r3]
    { sub r3, r3, 1               ; bt r3, .Lloopdsp_complex_mul_conjugate_vector3 }
    
    ldd r5, r6, sp[0]
    ldd r7, r8, sp[1]
    ldd r9, r10, sp[2]
	ldw r4, sp[6]
	retsp NSTACKWORDS
	
	// RETURN_REG_HOLDER
	.cc_bottom dsp_complex_mul_conjugate_vector3.function
	.set	dsp_complex_mul_conjugate_vector3.nstackwords,NSTACKWORDS
	.globl	dsp_complex_mul_conjugate_vector3.nstackwords
	.set	dsp_complex_mul_conjugate_vector3.maxcores,1
	.globl	dsp_complex_mul_conjugate_vector3.maxcores
	.set	dsp_complex_mul_conjugate_vector3.maxtimers,0
	.globl	dsp_complex_mul_conjugate_vector3.maxtimers
	.set	dsp_complex_mul_conjugate_vector3.maxchanends,0
	.globl	dsp_complex_mul_conjugate_vector3.maxchanends
.Ltmpdsp_complex_mul_conjugate_vector3:
	.size	dsp_complex_mul_conjugate_vector3, .Ltmpdsp_complex_mul_conjugate_vector3-dsp_complex_mul_conjugate_vector3



#undef NSTACKWORDS
#define NSTACKWORDS 6

    .align 4
	.globl	dsp_complex_scalar_vector3
	.type	dsp_complex_scalar_vector3,@function
	.cc_top dsp_complex_scalar_vector3.function,dsp_complex_scalar_vector3
	
dsp_complex_scalar_vector3:
	dualentsp NSTACKWORDS
    std r4, r5, sp[0]
    std r7, r8, sp[1]
    std r9, r10, sp[2]
    { sub r2, r2, 1               ; ldw r9, sp[NSTACKWORDS+1] }
    { ldc r7, 24                  ; nop }
.Lloopdsp_complex_scalar_vector3:
    { ldc r10, 0                  ; ldc r11, 0  }
    ldd r4, r5, r1[r2]
    maccs r10, r11, r4, r3
    lextract r4, r10, r11, r7, 32
    ashr r4, r4, r9
    { ldc r10, 0                  ; ldc r11, 0  }
    maccs r10, r11, r5, r3
    lextract r5, r10, r11, r7, 32
    ashr r5, r5, r9
    std r4, r5, r0[r2]
    { sub r2, r2, 1               ; bt r2, .Lloopdsp_complex_scalar_vector3 }
    
    ldd r4, r5, sp[0]
    ldd r7, r8, sp[1]
    ldd r9, r10, sp[2]
	retsp NSTACKWORDS
	
	// RETURN_REG_HOLDER
	.cc_bottom dsp_complex_scalar_vector3.function
	.set	dsp_complex_scalar_vector3.nstackwords,NSTACKWORDS
	.globl	dsp_complex_scalar_vector3.nstackwords
	.set	dsp_complex_scalar_vector3.maxcores,1
	.globl	dsp_complex_scalar_vector3.maxcores
	.set	dsp_complex_scalar_vector3.maxtimers,0
	.globl	dsp_complex_scalar_vector3.maxtimers
	.set	dsp_complex_scalar_vector3.maxchanends,0
	.globl	dsp_complex_scalar_vector3.maxchanends
.Ltmpdsp_complex_scalar_vector3:
	.size	dsp_complex_scalar_vector3, .Ltmpdsp_complex_scalar_vector3-dsp_complex_scalar_vector3
    
#endif
