// Copyright (c) 2015-2017, XMOS Ltd, All rights reserved
    
#if defined(__XS2A__)
	.text
    
#define NSTACKWORDS 0
    .issue_mode dual
	.globl	dsp_bfp_tx
	.type	dsp_bfp_tx,@function
	.cc_top dsp_bfp_tx.function,dsp_bfp_tx

    
dsp_bfp_tx:
    ecallt r0
    { dualentsp NSTACKWORDS  ; chkct res[r0], 1 }
    { out res[r0], r3        ; shr r2, r2, 1    }
    { nop                    ; sub r2, r2, 1    }
.Lloop_tx:
    ldd r3, r11, r1[r2]
    { out res[r0], r3        ; sub r2, r2, 1    }
    { out res[r0], r11       ; bt r2, .Lloop_tx }
    ldd r3, r11, r1[r2]
    { out res[r0], r3        ; nop              }
    { out res[r0], r11       ; nop              }
    { retsp NSTACKWORDS      ; outct res[r0], 1 }
	
	.cc_bottom dsp_bfp_tx.function
	.set	dsp_bfp_tx.nstackwords, NSTACKWORDS
	.globl	dsp_bfp_tx.nstackwords
	.set	dsp_bfp_tx.maxcores,1
	.globl	dsp_bfp_tx.maxcores
	.set	dsp_bfp_tx.maxtimers,0
	.globl	dsp_bfp_tx.maxtimers
	.set	dsp_bfp_tx.maxchanends,0
	.globl	dsp_bfp_tx.maxchanends
.Ltmp_dsp_bfp_tx:
	.size	dsp_bfp_tx, .Ltmp_dsp_bfp_tx-dsp_bfp_tx


#undef NSTACKWORDS    
#define NSTACKWORDS 0
	.globl	dsp_bfp_rx_state_init
	.type	dsp_bfp_rx_state_init,@function
	.cc_top dsp_bfp_rx_state_init.function,dsp_bfp_rx_state_init

dsp_bfp_rx_state_init:
    ecallt r0
    { dualentsp NSTACKWORDS  ; nop              }
    { ldc r11, 0             ; bu .Lloop_in     }
.Lloop_rx_state_init:
    std r11, r11, r0[r1]
.Lloop_in:
    { sub r1, r1, 1          ; bt r1, .Lloop_rx_state_init }
    { retsp NSTACKWORDS      ; nop    }
	
	.cc_bottom dsp_bfp_rx_state_init.function
	.set	dsp_bfp_rx_state_init.nstackwords, NSTACKWORDS
	.globl	dsp_bfp_rx_state_init.nstackwords
	.set	dsp_bfp_rx_state_init.maxcores,1
	.globl	dsp_bfp_rx_state_init.maxcores
	.set	dsp_bfp_rx_state_init.maxtimers,0
	.globl	dsp_bfp_rx_state_init.maxtimers
	.set	dsp_bfp_rx_state_init.maxchanends,0
	.globl	dsp_bfp_rx_state_init.maxchanends
.Ltmp_dsp_bfp_rx_state_init:
	.size	dsp_bfp_rx_state_init, .Ltmp_dsp_bfp_rx_state_init-dsp_bfp_rx_state_init


#undef NSTACKWORDS    
#define NSTACKWORDS 8
	.globl	dsp_bfp_rx
	.type	dsp_bfp_rx,@function
	.cc_top dsp_bfp_rx.function,dsp_bfp_rx

    
dsp_bfp_rx:
    ecallt r0
    { dualentsp NSTACKWORDS  ; nop }
    std r4, r5, sp[1]
    std r6, r7, sp[2]
    std r8, r9, sp[3]

    ldw r4, sp[NSTACKWORDS+1]
    // First copy the old data from the state into the target array
    // for(int i = 0; i < old_data; i++) {
    //    target[i] = (state, int32_t[])[i];
    // }

    { shr r3, r3, 1               ; shr r4, r4, 1}
    sub r11, r3, r4
    sub r8, r11, 1
.Lloop_copy_state_to_target:
    ldd r6, r7, r1[r8]
    std r6, r7, r2[r8]
    { bt r8, .Lloop_copy_state_to_target ; sub r8, r8, 1}

    // Now advance the state, removing the oldest data.
    // for(int i = 0; i < (old_data - advance)/2; i++) {
    //     state[i] = state[i+(advance/2)];
    // }

    { sub r8, r11, r4             ; shl r9, r4, 3 }
    { sub r8, r8, 1               ; add r9, r1, r9 }
.Lloop_copy_state_to_state:
    ldd r6, r7, r9[r8]
    std r6, r7, r1[r8]
    { bt r8, .Lloop_copy_state_to_state ; sub r8, r8, 1}
    
    // Now receive the new data straight into the target array
    // outct(x, 1);
    // int exponent = inuint(x);
    // for(int i = N-1; i >= N-advance; i--) {
    //     target[i] = inuint(x);
    // }
    // chkct(x, 1);
    
    { sub r8, r4, 0               ; shl r9, r11, 3 }
    { outct res[r0], 1            ; add r9, r2, r9 }

    { bu .Lloop_rx_in             ; in r5, res[r0] }
.Lloop_rx:
    std r6, r7, r9[r8]
.Lloop_rx_in:
    { in r6, res[r0]              ; sub r8, r8, 1    }
    { in r7, res[r0]              ; bt r8, .Lloop_rx }
    std r6, r7, r9[r8]

    // Finally, shift the new data down
    // for(int i = N-1; i >= N-advance; i--) {
    //     int y = target[i] >> exponent;
    //     target[i] = y;
    //     (state, int32_t[])[i-advance] = y;
    // }

    { chkct res[r0], 1            ; nop            }
    { sub r0, r11, r4             ; sub r8, r4, 1  }
    { shl r0, r0, 3               ; shl r9, r11, 3 }
    { add r0, r1, r0              ; add r9, r2, r9 }

.Lloop_shift_new_data_down:
    ldd r6, r7, r9[r8]
    ashr r6, r6, r5
    ashr r7, r7, r5
    std r6, r7, r9[r8]
    std r6, r7, r0[r8]
    { bt r8, .Lloop_shift_new_data_down ; sub r8, r8, 1}

    { add r0, r2, 0               ; add r1, r3, 0 }
    { add r4, r0, 0               ; add r5, r1, 0 }
    bl dsp_bfp_cls
    { add r1, r5, 0               ; ldw r11, sp[NSTACKWORDS+2] }
    { sub r2, r0, r11             ; nop }
    { add r0, r4, 0               ; add r5, r2, 0 }
    bl dsp_bfp_shl

    { add r0, r5, 0 }
    ldd r4, r5, sp[1]
    ldd r6, r7, sp[2]
    ldd r8, r9, sp[3]
    { retsp NSTACKWORDS      ; nop }
	
	.cc_bottom dsp_bfp_rx.function

    .linkset dsp_bfp_rx.nstackwords, NSTACKWORDS + (dsp_bfp_cls.nstackwords $M dsp_bfp_shl.nstackwords)
	.globl	dsp_bfp_rx.nstackwords
	.set	dsp_bfp_rx.maxcores,1
	.globl	dsp_bfp_rx.maxcores
	.set	dsp_bfp_rx.maxtimers,0
	.globl	dsp_bfp_rx.maxtimers
	.set	dsp_bfp_rx.maxchanends,0
	.globl	dsp_bfp_rx.maxchanends
.Ltmp_dsp_bfp_rx:
	.size	dsp_bfp_rx, .Ltmp_dsp_bfp_rx-dsp_bfp_rx

#endif

    #include "bfp_tx_rx.h"
