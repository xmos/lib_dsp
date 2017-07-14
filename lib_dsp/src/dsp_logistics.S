// Copyright (c) 2015-2017, XMOS Ltd, All rights reserved
    
#if defined(__XS2A__)

#define NSTACKWORDS  0
    
	.text
    .issue_mode  dual
	.globl	dsp_math_logistics_fast
	.align	4
	.type	dsp_math_logistics_fast,@function
	.cc_top dsp_math_logistics_fast.function,dsp_math_logistics_fast
	
    
dsp_math_logistics_fast:
	dualentsp NSTACKWORDS
    
    { clz r11, r0                 ; neg r1, r0 }     // test on >= 0
    { bt r11, pos                 ; shr r2, r1, 24 }  // get integer part
    { nop                         ; shr r11, r2, 3 }  // test on >= 8
    { bt r11, zero                ; ldap r11, .log_slope  }
    { ldw r3, r11[r2]             ; ldap r11, .log_offset }
    { ldw r2, r11[r2]             ; mkmsk r0, 24  }
    maccu r2, r11, r3, r1
    { retsp NSTACKWORDS           ; sub r0, r0, r2 }
    
pos:
    { shr r1, r0, 24              ; add r2, r0, 0 }     // get integer part
    { shr r11, r1, 3              ; nop }               // test on >= 8: 1.0
    { bt r11, one                 ; ldap r11, .log_slope } // get first lookup
    { ldw r3, r11[r1]             ; ldap r11, .log_offset } // get second lookup
    { ldw r0, r11[r1]             ; nop }
    maccu r0, r11, r3, r2
    { retsp NSTACKWORDS           ; nop }
zero:
    { retsp NSTACKWORDS           ; ldc r0, 0 }
one:    
    { retsp NSTACKWORDS           ; mkmsk r0, 24 }
    
    .align 4
.log_slope:    
    .word 1015490930,640498971,297985800,120120271,46079377,17219453,6371555,3717288
.log_offset:    
    .word 8388608,9853420,12529304,14613666,15770555,16334225,16588473,16661050

	// RETURN_REG_HOLDER
	.cc_bottom dsp_math_logistics_fast.function
	.set	dsp_math_logistics_fast.nstackwords,NSTACKWORDS
	.globl	dsp_math_logistics_fast.nstackwords
	.set	dsp_math_logistics_fast.maxcores,1
	.globl	dsp_math_logistics_fast.maxcores
	.set	dsp_math_logistics_fast.maxtimers,0
	.globl	dsp_math_logistics_fast.maxtimers
	.set	dsp_math_logistics_fast.maxchanends,0
	.globl	dsp_math_logistics_fast.maxchanends
.Ltmp0:
	.size	dsp_math_logistics_fast, .Ltmp0-dsp_math_logistics_fast

    .issue_mode  single
    
#endif


