// Copyright (c) 2022, XMOS Ltd, All rights reserved
    
#if defined(__XS3A__)

#define NSTACKWORDS 4
	.text
    .issue_mode  dual
	.globl	dsp_float_to_bfp_xs3
	.type	dsp_float_to_bfp_xs3,@function
	.cc_top dsp_float_to_bfp_xs3.function,dsp_float_to_bfp_xs3

    //    int32_t dsp_float_to_bfp_xs3({float,float}*ptr, uint32_t N)
    .align 16
dsp_float_to_bfp_xs3:
    dualentsp NSTACKWORDS
    std r4, r5, sp[0]
    std r6, r7, sp[1]

    { sub r1, r1, 1             ; mkmsk r2, 32 }
    { shl r2, r2, 16            ; add r3, r1, 0 }
.loop:
    ldd r4, r5, r0[r1]
    fsexp r11, r4, r4
    fsexp r6, r5, r5
    { lss r11, r4, r2           ; nop }
    { bt r11, .not              ; lss r11, r5, r2 }
    { add r2, r4, 0             ; lss r11, r5, r4 }
.not:   
    bt r11, .not2
    add r2, r5, 0
.not2:
    { bt  r1, .loop             ; sub r1, r1, 1 }
    sub r2, r2, 5
    nop                         // This aligns the next loop...
    
.loop2:
    ldd r4, r5, r0[r3]
    fsexp r11, r6, r4
    fmant r4, r4
    { bf r11, .not3             ; sub r6, r6, r2 }
    neg r4, r4
.not3:
    shl r4, r4, r6
    fsexp r11, r6, r5
    fmant r5, r5
    { bf r11, .not4             ; sub r6, r6, r2 }
    neg r5, r5
.not4:
    shl r5, r5, r6
    std r4, r5, r0[r3]
    { bt  r3, .loop2            ; sub r3, r3, 1 }
    
    ldd r4, r5, sp[0]
    ldd r6, r7, sp[1]
    add r0, r2, 0
    retsp NSTACKWORDS
	
	// RETURN_REG_HOLDER
	.cc_bottom dsp_float_to_bfp_xs3.function
	.set	dsp_float_to_bfp_xs3.nstackwords,NSTACKWORDS
	.globl	dsp_float_to_bfp_xs3.nstackwords
	.set	dsp_float_to_bfp_xs3.maxcores,1
	.globl	dsp_float_to_bfp_xs3.maxcores
	.set	dsp_float_to_bfp_xs3.maxtimers,0
	.globl	dsp_float_to_bfp_xs3.maxtimers
	.set	dsp_float_to_bfp_xs3.maxchanends,0
	.globl	dsp_float_to_bfp_xs3.maxchanends
.Ltmp0:
	.size	dsp_float_to_bfp_xs3, .Ltmp0-dsp_float_to_bfp_xs3

#undef NSTACKWORDS
#define NSTACKWORDS 4
    
	.globl	dsp_bfp_to_float_xs3
	.type	dsp_bfp_to_float_xs3,@function
	.cc_top dsp_bfp_to_float_xs3.function,dsp_bfp_to_float_xs3

    //    void dsp_bfp_to_float_xs3({float,float}*ptr, uint32_t N, int32_t exp)

    .align 16
dsp_bfp_to_float_xs3:
    dualentsp NSTACKWORDS
    std r4, r5, sp[0]
    { sub r1, r1, 1             ; ldc r3, 0 }
    std r6, r7, sp[1]

.loop3:
    ldd r4, r5, r0[r1]
    lss r11, r4, r3
    bf r11, .not5
    neg r4, r4
.not5:
    fmake r4, r11, r2, r3, r4
    lss r11, r5, r3
    bf r11, .not6
    neg r5, r5
.not6:
    fmake r5, r11, r2, r3, r5
    std r4, r5, r0[r1]
    { bt  r1, .loop3            ; sub r1, r1, 1 }
    
    ldd r4, r5, sp[0]
    ldd r6, r7, sp[1]
    add r0, r2, 0
    retsp NSTACKWORDS

	
	// RETURN_REG_HOLDER
	.cc_bottom dsp_bfp_to_float_xs3.function
	.set	dsp_bfp_to_float_xs3.nstackwords,NSTACKWORDS
	.globl	dsp_bfp_to_float_xs3.nstackwords
	.set	dsp_bfp_to_float_xs3.maxcores,1
	.globl	dsp_bfp_to_float_xs3.maxcores
	.set	dsp_bfp_to_float_xs3.maxtimers,0
	.globl	dsp_bfp_to_float_xs3.maxtimers
	.set	dsp_bfp_to_float_xs3.maxchanends,0
	.globl	dsp_bfp_to_float_xs3.maxchanends
.Ltmp1:
	.size	dsp_bfp_to_float_xs3, .Ltmp1-dsp_bfp_to_float_xs3

        
#endif
