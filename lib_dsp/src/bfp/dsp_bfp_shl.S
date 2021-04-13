// Copyright 2015-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
    
#if (defined(__XS2A__) || defined (__XS3A__))

#undef NSTACKWORDS
#define NSTACKWORDS 0

	.text
    .issue_mode  dual
	.globl	dsp_bfp_shl
	.align	16
    .skip   0
	.type	dsp_bfp_shl,@function
	.cc_top dsp_bfp_shl.function,dsp_bfp_shl

dsp_bfp_shl:
    { dualentsp NSTACKWORDS       ; clz r11, r2     } // r2 < 0 indicates SHR
    { bf r11, .Lshr               ; sub r1, r1, 1   } // Start on N-1
.Lloopl:
    ldd r3, r11, r0[r1]                               // pick re/im from vector
    { shl r3, r3, r2              ; shl r11, r11, r2} // Shift up re and im
    std r3, r11, r0[r1]                               // store re/im back
    { bt r1, .Lloopl              ; sub r1, r1, 1 } 
        
    retsp NSTACKWORDS
.Lshr:
    neg r2, r2
.Lloopr:
    ldd r3, r11, r0[r1]                               // pick re/im from vector
    ashr r3, r3, r2
    ashr r11, r11, r2                                 // Shift down re and im
    std r3, r11, r0[r1]                               // store re/im back
    { bt r1, .Lloopr              ; sub r1, r1, 1 } 
        
    retsp NSTACKWORDS
    
	// RETURN_REG_HOLDER
	.cc_bottom dsp_bfp_shl.function
	.set	dsp_bfp_shl.nstackwords, NSTACKWORDS
	.globl	dsp_bfp_shl.nstackwords
	.set	dsp_bfp_shl.maxcores,1
	.globl	dsp_bfp_shl.maxcores
	.set	dsp_bfp_shl.maxtimers,0
	.globl	dsp_bfp_shl.maxtimers
	.set	dsp_bfp_shl.maxchanends,0
	.globl	dsp_bfp_shl.maxchanends
.Ltmpdsp_bfp_shl:
	.size	dsp_bfp_shl, .Ltmpdsp_bfp_shl-dsp_bfp_shl


#undef NSTACKWORDS
#define NSTACKWORDS 2

	.text
    .issue_mode  dual
	.globl	dsp_bfp_shl_2
	.align	16
    .skip   0
	.type	dsp_bfp_shl_2,@function
	.cc_top dsp_bfp_shl_2.function,dsp_bfp_shl_2

dsp_bfp_shl_2:
    { dualentsp NSTACKWORDS       ; clz r11, r3     } // r3 < 0 indicates SHR
    std r4, r5, sp[0]
    { bf r11, .LshrX              ; sub r2, r2, 1   } // Start on N-1
.LlooplX:
    ldd r4, r11, r1[r2]                               // pick re/im from vector
    { shl r4, r4, r3              ; shl r11, r11, r3} // Shift up re and im
    std r4, r11, r0[r2]                               // store re/im back
    { bt r2, .LlooplX             ; sub r2, r2, 1 } 
    bu .LdoneX

.LshrX:
    neg r3, r3
.LlooprX:
    ldd r4, r11, r1[r2]                               // pick re/im from vector
    ashr r4, r4, r3
    ashr r11, r11, r3                                 // Shift down re and im
    std r4, r11, r0[r2]                               // store re/im back
    { bt r2, .LlooprX             ; sub r2, r2, 1 } 
.LdoneX:        
    ldd r4, r5, sp[0]
    retsp NSTACKWORDS
    
	// RETURN_REG_HOLDER
	.cc_bottom dsp_bfp_shl_2.function
	.set	dsp_bfp_shl_2.nstackwords, NSTACKWORDS
	.globl	dsp_bfp_shl_2.nstackwords
	.set	dsp_bfp_shl_2.maxcores,1
	.globl	dsp_bfp_shl_2.maxcores
	.set	dsp_bfp_shl_2.maxtimers,0
	.globl	dsp_bfp_shl_2.maxtimers
	.set	dsp_bfp_shl_2.maxchanends,0
	.globl	dsp_bfp_shl_2.maxchanends
.Ltmpdsp_bfp_shl_2:
	.size	dsp_bfp_shl_2, .Ltmpdsp_bfp_shl_2-dsp_bfp_shl_2


#undef NSTACKWORDS
#define NSTACKWORDS 2

	.text
    .issue_mode  dual
	.globl	dsp_bfp_shl2
	.align	16
    .skip   0
	.type	dsp_bfp_shl2,@function
	.cc_top dsp_bfp_shl2.function,dsp_bfp_shl2

dsp_bfp_shl2:
    { dualentsp NSTACKWORDS       ; clz r11, r2     } // r2 < 0 indicates SHR
    std r4, r5, sp[0]
    { bf r11, .Lshr2              ; sub r1, r1, 1   } // Start on N-1
    clz r11, r3
    bf r11, .Lshlr2
.Lloopll2:
    ldd r4, r11, r0[r1]                               // pick re/im from vector
    { shl r4, r4, r3              ; shl r11, r11, r2} // Shift up re and im
    std r4, r11, r0[r1]                               // store re/im back
    { bt r1, .Lloopll2             ; sub r1, r1, 1 } 
    bu .Ldone

.Lshlr2:
    neg r3, r3
.Llooplr2:
    ldd r4, r11, r0[r1]                               // pick re/im from vector
    ashr r4, r4, r3
    shl r11, r11, r2                                 // Shift down re and im
    std r4, r11, r0[r1]                               // store re/im back
    { bt r1, .Llooplr2             ; sub r1, r1, 1 } 
    bu .Ldone
    
.Lshr2:
    {neg r2, r2                  ; clz r11, r3}
    bf r11, .Lshrr2
 
.Llooprl2:
    ldd r4, r11, r0[r1]                               // pick re/im from vector
    shl r4, r4, r3
    ashr r11, r11, r2                                 // Shift down re and im
    std r4, r11, r0[r1]                               // store re/im back
    { bt r1, .Llooprl2             ; sub r1, r1, 1 }
    bu .Ldone
    

.Lshrr2:
    neg r3, r3
.Llooprr2:
    ldd r4, r11, r0[r1]                               // pick re/im from vector
    ashr r4, r4, r3
    ashr r11, r11, r2                                 // Shift down re and im
    std r4, r11, r0[r1]                               // store re/im back
    { bt r1, .Llooprr2             ; sub r1, r1, 1 } 
    
.Ldone:        
    ldd r4, r5, sp[0]
    retsp NSTACKWORDS
    
	// RETURN_REG_HOLDER
	.cc_bottom dsp_bfp_shl2.function
	.set	dsp_bfp_shl2.nstackwords, NSTACKWORDS
	.globl	dsp_bfp_shl2.nstackwords
	.set	dsp_bfp_shl2.maxcores,1
	.globl	dsp_bfp_shl2.maxcores
	.set	dsp_bfp_shl2.maxtimers,0
	.globl	dsp_bfp_shl2.maxtimers
	.set	dsp_bfp_shl2.maxchanends,0
	.globl	dsp_bfp_shl2.maxchanends
.Ltmpdsp_bfp_shl2:
	.size	dsp_bfp_shl2, .Ltmpdsp_bfp_shl2-dsp_bfp_shl2
    
#endif
