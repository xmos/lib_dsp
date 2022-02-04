// Copyright (c) 2022, XMOS Ltd, All rights reserved
    
#if defined(__XS3A__)

#define NSTACKWORDS 4
	.text
    .issue_mode  dual
	.globl	dsp_vect_dot_prod_xs3
	.type	dsp_vect_dot_prod_xs3,@function
	.cc_top dsp_vect_dot_prod_xs3.function,dsp_vect_dot_prod_xs3

    //    float dsp_vect_dot_prod_xs3(float *ptr1, float *ptr2, uint32_t N)
    .align 16
dsp_vect_dot_prod_xs3:
    dualentsp NSTACKWORDS
    std r4, r5, sp[0]

    { sub r2, r2, 1             ; ldc r11, 0 }
    nop                         // align the loop.
.loop:
    ldw r4, r0[r2]
    ldw r5, r1[r2]
    fmacc r11, r11, r4, r5
    { bt  r2, .loop             ; sub r2, r2, 1 }
    
    ldd r4, r5, sp[0]
    add r0, r11, 0
    retsp NSTACKWORDS
	
	// RETURN_REG_HOLDER
	.cc_bottom dsp_vect_dot_prod_xs3.function
	.set	dsp_vect_dot_prod_xs3.nstackwords,NSTACKWORDS
	.globl	dsp_vect_dot_prod_xs3.nstackwords
	.set	dsp_vect_dot_prod_xs3.maxcores,1
	.globl	dsp_vect_dot_prod_xs3.maxcores
	.set	dsp_vect_dot_prod_xs3.maxtimers,0
	.globl	dsp_vect_dot_prod_xs3.maxtimers
	.set	dsp_vect_dot_prod_xs3.maxchanends,0
	.globl	dsp_vect_dot_prod_xs3.maxchanends
.Ltmp0:
	.size	dsp_vect_dot_prod_xs3, .Ltmp0-dsp_vect_dot_prod_xs3

#undef NSTACKWORDS

#define NSTACKWORDS 4
	.text
    .issue_mode  dual
	.globl	dsp_vect_dot_prod_fast_xs3
	.type	dsp_vect_dot_prod_fast_xs3,@function
	.cc_top dsp_vect_dot_prod_fast_xs3.function,dsp_vect_dot_prod_fast_xs3

    //    float dsp_vect_dot_prod_fast_xs3(float *ptr1, float *ptr2, uint32_t N)
    .align 16
dsp_vect_dot_prod_fast_xs3:
    dualentsp NSTACKWORDS
    std r4, r5, sp[0]
    std r6, r7, sp[1]

    { add r3, r2, 0             ; zext r2, 1 }
    { bf r2, .even              ; ldc r11, 0 }
    sub r3, r3, 1
    ldw r4, r0[r3]
    ldw r5, r1[r3]
    fmacc r11, r11, r4, r5
.even:
    { shr r6, r0, 2             ; shr r7, r1, 2 }
    { zext r6, 1                ; zext r7, 1 }
    { shl r6, r6, 1             ; mov r2, r3 }
    { or r6, r6, r7             ; nop }
    { bru r6                    ; sub r2, r2, 1 }
    bu .together
    bu .r1_odd
    bu .r0_odd

.r0r1_odd:
    { ldw r6, r0[r2]              ; sub r0, r0, 4 }
    { ldw r7, r1[r2]              ; sub r1, r1, 4 }

    shr r2, r2, 1
    
.r0r1_mis_aligned_loop:
    fmacc r11, r11, r6, r7
    ldd r4, r6, r0[r2]
    ldd r5, r7, r1[r2]
    fmacc r11, r11, r4, r5
    { bt  r2, .r0r1_mis_aligned_loop ; sub r2, r2, 1 }
    bu .done

.r1_odd:
    { ldw r7, r1[r2]               ; shr r2, r2, 1 }
    sub r1, r1, 4
.r1_mis_aligned_loop:
    ldd r4, r6, r0[r2]
    fmacc r11, r11, r4, r7
    ldd r5, r7, r1[r2]
    fmacc r11, r11, r6, r5
    { bt  r2, .r1_mis_aligned_loop ; sub r2, r2, 1 }
    bu .done

.r0_odd:
    { ldw r6, r0[r2]               ; shr r2, r2, 1 }
    sub r0, r0, 4
.r0_mis_aligned_loop:
    ldd r5, r7, r1[r2]
    fmacc r11, r11, r6, r5
    ldd r4, r6, r0[r2]
    fmacc r11, r11, r4, r7
    { bt  r2, .r0_mis_aligned_loop ; sub r2, r2, 1 }
    bu .done

    nop
.together:
    shr r2, r2, 1
.together_loop:
    ldd r4, r6, r0[r2]
    ldd r5, r7, r1[r2]
    fmacc r11, r11, r4, r5
    fmacc r11, r11, r6, r7
    { bt  r2, .together_loop     ; sub r2, r2, 1 }
.done:
    ldd r4, r5, sp[0]
    ldd r6, r7, sp[1]
    add r0, r11, 0
    retsp NSTACKWORDS
    
	
	// RETURN_REG_HOLDER
	.cc_bottom dsp_vect_dot_prod_fast_xs3.function
	.set	dsp_vect_dot_prod_fast_xs3.nstackwords,NSTACKWORDS
	.globl	dsp_vect_dot_prod_fast_xs3.nstackwords
	.set	dsp_vect_dot_prod_fast_xs3.maxcores,1
	.globl	dsp_vect_dot_prod_fast_xs3.maxcores
	.set	dsp_vect_dot_prod_fast_xs3.maxtimers,0
	.globl	dsp_vect_dot_prod_fast_xs3.maxtimers
	.set	dsp_vect_dot_prod_fast_xs3.maxchanends,0
	.globl	dsp_vect_dot_prod_fast_xs3.maxchanends
.Ltmp1:
	.size	dsp_vect_dot_prod_fast_xs3, .Ltmp1-dsp_vect_dot_prod_fast_xs3

#undef NSTACKWORDS


#if 0
void FFVectMacC
(
    OBJStruct   * const op,
    const SIZE_T        N,
    const FLOAT * const APES_RESTRICT in1, /* Conj */
    const FLOAT * const APES_RESTRICT in2,
          FLOAT * const APES_RESTRICT io
)
{
    SIZE_T c1, c2;

    io[ DC(N) ] += in1[ DC(N) ] * in2[ DC(N) ];
    io[ NQ(N) ] += in1[ NQ(N) ] * in2[ NQ(N) ];

    for( c1 = 2, c2 = 3; c1 < N; c1 += 2, c2 += 2 )
    {
        FLOAT t1 = in1[ c1 ];
        FLOAT t2 = in1[ c2 ];
        FLOAT t3 = in2[ c1 ];
        FLOAT t4 = in2[ c2 ];
        io[ c1 ] += t1 * t3 + t2 * t4;
        io[ c2 ] += t1 * t4 - t2 * t3;
    }
}
#endif
    

#define NSTACKWORDS 6
	.text
    .issue_mode  dual
	.globl	FFVectMacC_xs3
	.type	FFVectMacC_xs3,@function
	.cc_top FFVectMacC_xs3.function,FFVectMacC_xs3

    //    float FFVectMacC_xs3(uint32_t N, float *ptr1, float *ptr2, float *ptro)
    .align 16
FFVectMacC_xs3:
    dualentsp NSTACKWORDS
    std r4, r5, sp[0]
    std r6, r7, sp[1]
    std r8, r9, sp[2]

    { shr r0, r0, 1             ; nop }
    { ldc r11, 1                ; sub r0, r0, 2 }
    { bitrev r11, r11           ; add r3, r3, 8 }
    { add r1, r1, 8             ; add r2, r2, 8 }
.MacCloop:
    ldd r4, r5, r1[r0]
    ldd r6, r7, r2[r0]
    ldd r8, r9, r3[r0]
    fmacc r9, r9, r4, r6
    fmacc r9, r9, r5, r7
    add r4, r4, r11
    fmacc r8, r8, r4, r7
    fmacc r8, r8, r5, r6
    std r8, r9, r3[r0]
    { bt  r0, .MacCloop         ; sub r0, r0, 1 }
    
    ldd r4, r5, r1[r0]
    ldd r6, r7, r2[r0]
    ldd r8, r9, r3[r0]
    fmacc r8, r8, r4, r6
    fmacc r9, r9, r5, r7
    std r8, r9, r3[r0]

    ldd r4, r5, sp[0]
    ldd r6, r7, sp[1]
    ldd r8, r9, sp[2]
    add r0, r11, 0
    retsp NSTACKWORDS
    
	
	// RETURN_REG_HOLDER
	.cc_bottom FFVectMacC_xs3.function
	.set	FFVectMacC_xs3.nstackwords,NSTACKWORDS
	.globl	FFVectMacC_xs3.nstackwords
	.set	FFVectMacC_xs3.maxcores,1
	.globl	FFVectMacC_xs3.maxcores
	.set	FFVectMacC_xs3.maxtimers,0
	.globl	FFVectMacC_xs3.maxtimers
	.set	FFVectMacC_xs3.maxchanends,0
	.globl	FFVectMacC_xs3.maxchanends
.Ltmp2:
	.size	FFVectMacC_xs3, .Ltmp2-FFVectMacC_xs3

#undef NSTACKWORDS
    
    //    io[ c1 ] += t1 * t3 - t2 * t4;
    //    io[ c2 ] += t1 * t4 + t2 * t3;

#define NSTACKWORDS 6
	.text
    .issue_mode  dual
	.globl	FFVectMac_xs3
	.type	FFVectMac_xs3,@function
	.cc_top FFVectMac_xs3.function,FFVectMac_xs3

    //    float FFVectMac_xs3(uint32_t N, float *ptr1, float *ptr2, float *ptro)
    .align 16
FFVectMac_xs3:
    dualentsp NSTACKWORDS
    std r4, r5, sp[0]
    std r6, r7, sp[1]
    std r8, r9, sp[2]

    { shr r0, r0, 1             ; nop }
    { ldc r11, 1                ; sub r0, r0, 2 }
    { bitrev r11, r11           ; add r3, r3, 8 }
    { add r1, r1, 8             ; add r2, r2, 8 }
.Macloop:
    ldd r4, r5, r1[r0]
    ldd r6, r7, r2[r0]
    ldd r8, r9, r3[r0]
    fmacc r8, r8, r4, r7
    fmacc r8, r8, r5, r6
    add r4, r4, r11
    fmacc r9, r9, r4, r6
    fmacc r9, r9, r5, r7
    std r8, r9, r3[r0]
    { bt  r0, .Macloop         ; sub r0, r0, 1 }
    
    ldd r4, r5, r1[r0]
    ldd r6, r7, r2[r0]
    ldd r8, r9, r3[r0]
    fmacc r8, r8, r4, r6
    fmacc r9, r9, r5, r7
    std r8, r9, r3[r0]

    ldd r4, r5, sp[0]
    ldd r6, r7, sp[1]
    ldd r8, r9, sp[2]
    add r0, r11, 0
    retsp NSTACKWORDS
    
	
	// RETURN_REG_HOLDER
	.cc_bottom FFVectMac_xs3.function
	.set	FFVectMac_xs3.nstackwords,NSTACKWORDS
	.globl	FFVectMac_xs3.nstackwords
	.set	FFVectMac_xs3.maxcores,1
	.globl	FFVectMac_xs3.maxcores
	.set	FFVectMac_xs3.maxtimers,0
	.globl	FFVectMac_xs3.maxtimers
	.set	FFVectMac_xs3.maxchanends,0
	.globl	FFVectMac_xs3.maxchanends
.Ltmp3:
	.size	FFVectMac_xs3, .Ltmp3-FFVectMac_xs3

#undef NSTACKWORDS
    
#endif
