// Copyright (c) 2015-2016, XMOS Ltd, All rights reserved
    
#if defined(__XS2A__)

	.text
    .issue_mode  dual
	.globl	dsp_bfp_cls
	.align	16
    .skip 4
	.type	dsp_bfp_cls,@function
	.cc_top dsp_bfp_cls.function,dsp_bfp_cls

dsp_bfp_cls:
	{ dualentsp 6                 ; mkmsk r11, 32   } // r11 collects mask
    { stw r10, sp[1]              ; sub r0, r0, 8   } // r0[r1] now last element
	std r4, r5, sp[1]
    
.Lloop:
    ldd r2, r3, r0[r1]                                // pick re/im from vector
    { clz r4, r2                  ; clz r5, r3 }      // headroom if positive
    { bt r4, .Lr2pos              ; neg r10, r2 }     // check if re is negative
    { clz r4, r10                 ; nop }             // re headroom if negative
.Lr2pos:
    { bt r5, .Lr3pos              ; neg r10, r3 }     // check if im is negative
    { clz r5, r10                 ; nop }             // im headroom if negative
.Lr3pos:
    { mkmsk r5, r5                ; mkmsk r4, r4 }    // make headroom into mask
    { and r5, r5, r4              ; sub r1, r1, 1 }   // combine headrooms
    { and r11, r11, r5            ; bt r1, .Lloop }   // max headroom
        
	ldd r4, r5, sp[1]
    { ldw r10, sp[1]              ;     clz r0, r11 } // make headroom to count
    ldc r1, 32
	{ retsp 6                     ; sub r0, r1, r0 }
	
	// RETURN_REG_HOLDER
	.cc_bottom dsp_bfp_cls.function
	.set	dsp_bfp_cls.nstackwords, 6
	.globl	dsp_bfp_cls.nstackwords
	.set	dsp_bfp_cls.maxcores,1
	.globl	dsp_bfp_cls.maxcores
	.set	dsp_bfp_cls.maxtimers,0
	.globl	dsp_bfp_cls.maxtimers
	.set	dsp_bfp_cls.maxchanends,0
	.globl	dsp_bfp_cls.maxchanends
.Ltmp0:
	.size	dsp_bfp_cls, .Ltmp0-dsp_bfp_cls

    
    .issue_mode  single
    
#endif
