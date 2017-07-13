// Copyright (c) 2015-2017, XMOS Ltd, All rights reserved
    
#if defined(__XS2A__)
#define NSTACKWORDS 2
	.text
    .issue_mode  dual
	.globl	dsp_bfp_cls
	.align	16
    .skip 4
	.type	dsp_bfp_cls,@function
	.cc_top dsp_bfp_cls.function,dsp_bfp_cls

dsp_bfp_cls:
	{ dualentsp NSTACKWORDS       ; ldc r11, 0   } // r11 collects mask
    { stw r4, sp[0]               ; sub r0, r0, 8   } // r0[r1] now last element
	
.Lloop:
    ldd r2, r3, r0[r1]                                // pick re/im from vector
    { sub r1, r1, 1               ; clz r4, r2 }      // headroom if positive
    { bt r4, .Lr2pos              ; clz r4, r3 }      // check if re is negative
    { neg r2, r2                  ; nop }             // re headroom if negative
.Lr2pos:
    { bt r4, .Lr3pos              ; or r11, r11, r2 } // check if im is negative
    { neg r3, r3                  ; nop }             // im headroom if negative
.Lr3pos:
    { or r11, r11, r3             ; bt r1, .Lloop }   // combine headrooms
        
	{ ldw r4, sp[0]               ; nop }
	{ retsp NSTACKWORDS           ; clz r0, r11  }
	
	// RETURN_REG_HOLDER
	.cc_bottom dsp_bfp_cls.function
	.set	dsp_bfp_cls.nstackwords, NSTACKWORDS
	.globl	dsp_bfp_cls.nstackwords
	.set	dsp_bfp_cls.maxcores,1
	.globl	dsp_bfp_cls.maxcores
	.set	dsp_bfp_cls.maxtimers,0
	.globl	dsp_bfp_cls.maxtimers
	.set	dsp_bfp_cls.maxchanends,0
	.globl	dsp_bfp_cls.maxchanends
.Ltmp0:
	.size	dsp_bfp_cls, .Ltmp0-dsp_bfp_cls
    
#endif
