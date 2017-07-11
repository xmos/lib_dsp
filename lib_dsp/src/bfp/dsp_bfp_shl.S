// Copyright (c) 2015-2016, XMOS Ltd, All rights reserved
    
#if defined(__XS2A__)

	.text
    .issue_mode  dual
	.globl	dsp_bfp_shl
	.align	16
    .skip   0
	.type	dsp_bfp_shl,@function
	.cc_top dsp_bfp_shl.function,dsp_bfp_shl

dsp_bfp_shl:
    { dualentsp 0                 ; clz r11, r1     } // r1 < 0 indicates SHR
    { bf r11, .Lshr               ; sub r2, r2, 1   } // Start on N-1
.Lloopl:
    ldd r3, r11, r0[r2]                               // pick re/im from vector
    { shl r3, r3, r1              ; shl r11, r11, r1} // Shift up re and im
    std r3, r11, r0[r2]                               // store re/im back
    { bt r2, .Lloopl              ; sub r2, r2, 1 } 
        
    retsp 0
.Lshr:
    neg r1, r1
.Lloopr:
    ldd r3, r11, r0[r2]                               // pick re/im from vector
    ashr r3, r3, r1
    ashr r11, r11, r1                                 // Shift down re and im
    std r3, r11, r0[r2]                               // store re/im back
    { bt r2, .Lloopr              ; sub r2, r2, 1 } 
        
    retsp 0
    
	// RETURN_REG_HOLDER
	.cc_bottom dsp_bfp_shl.function
	.set	dsp_bfp_shl.nstackwords, 0
	.globl	dsp_bfp_shl.nstackwords
	.set	dsp_bfp_shl.maxcores,1
	.globl	dsp_bfp_shl.maxcores
	.set	dsp_bfp_shl.maxtimers,0
	.globl	dsp_bfp_shl.maxtimers
	.set	dsp_bfp_shl.maxchanends,0
	.globl	dsp_bfp_shl.maxchanends
.Ltmp0:
	.size	dsp_bfp_shl, .Ltmp0-dsp_bfp_shl

    
    .issue_mode  single
    
#endif
