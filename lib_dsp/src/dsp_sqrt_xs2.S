// Copyright 2016-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
    
	.section	.dp.data,"awd",@progbits
	.text

#if (defined(__XS2A__) || defined (__XS3A__))
	.cc_top dsp_sqrt30_xs2.function
	.globl	dsp_sqrt30_xs2
	.type	dsp_sqrt30_xs2,@function

#define NSTACKWORDS 2

// void dsp_sqrt30_xs2(uint32_t x)

    .align 16
    .globl sqrt_as
    .issue_mode dual
dsp_sqrt30_xs2:
 	dualentsp 0 //TODO remove this - it's here to allow call from SI
    { bf r0, zero_sqrt          ; clz r11, r0     }
    { dualentsp NSTACKWORDS     ; shr r3, r11, 1  }
    { shl r2, r3, 1             ; ldc r11, 25     }
    { shl r0, r0, r2            ; stw r5, sp[0]   }
    { shr r1, r0, 2             ; ldc r5, 2       }
    { shr r2, r1, r11           ; ldap r11, firstbitslookup - 8 }
    { ld8u r2, r11[r2]          ; shl r1, r1, 4   }
    { mul r11, r2, r2                             }
    { shl r11, r11, 24          ; stw r4, sp[1]   }
    { shl r2, r2, 6             ; shl r0, r2, 24  }
    { sub r1, r1, r11           ; ldap r11, jumptable-16*64}
    { add r2, r2, r11           ; ldc r4, 26      }
    { shr r11, r1, r4           ; shl r5, r5, 24  }

next:
    { ld8u r11, r2[r11]         ; shr r5, r5, 2    }
    { bru  r11                  ; shl r1, r1, 2    }
//bp
    { bt r5, next               ; shr r11, r1, r4  }
    { ldw r4, sp[1]             ; shl r0, r0, 1    }
    { ldw r5, sp[0]             ;                  }
    { retsp NSTACKWORDS         ; shr r0, r0, r3   }
//nobp_sh_ne
    { bf r5, lastbit_sh_ne      ; sub r11, r0, r5  }
    { sub r0, r0, r5            ; shl r11, r11, 3  }
    { sub r0, r0, r5            ; add r1, r1, r11  }
    { shr r11, r1, r4           ; bu  next         }
//nobp_sh_none
    { bf r5, lastbit_sh_none    ; add r11, r0, r5  }
    { add r0, r0, r5            ; shl r11, r11, 3  }
    { add r0, r0, r5            ; sub r1, r1, r11  }
    { shr r11, r1, r4           ; bu  next         }
//nobp_nosh_ne
    { shl r11, r5, 1            ; bf r5, lastbit_nosh_ne  }
    { shl r11, r0, 2            ; sub r1, r1, r11  }
    { add r1, r1, r11           ; sub r0, r0, r5   }
    { shr r11, r1, r4           ; bu  next         }
//nobp_nosh_none
    { shl r11, r5, 1            ; bf r5, lastbit_nosh_none   }
    { shl r11, r0, 2            ; sub r1, r1, r11  }
    { sub r1, r1, r11           ; add r0, r0, r5   }
    { shr r11, r1, r4           ; bu next          }
    .align 16
lastbit_sh_ne:
    { ldw r4, sp[1]             ; shl r0, r0, 1    }
    { ldw r5, sp[0]             ; sub r0, r0, 2    }
    { retsp NSTACKWORDS         ; shr r0, r0, r3   }
    .align 16
lastbit_sh_none:
    { ldw r4, sp[1]             ; shl r0, r0, 1    }
    { ldw r5, sp[0]             ; add r0, r0, 2    }
    { retsp NSTACKWORDS         ; shr r0, r0, r3   }
    .align 16
lastbit_nosh_ne:
    { ldw r4, sp[1]             ; shl r0, r0, 1    }
    { ldw r5, sp[0]             ; sub r0, r0, 1    }
    { retsp NSTACKWORDS         ; shr r0, r0, r3   }
    .align 16
lastbit_nosh_none:
    { ldw r4, sp[1]             ; shl r0, r0, 1    }
    { ldw r5, sp[0]             ; add r0, r0, 1    }
    { retsp NSTACKWORDS         ; shr r0, r0, r3   }
zero_sqrt:
    { retsp 0                   ;                  }

firstbitslookup:
    .byte    0x10, 0x11, 0x12, 0x13, 0x14, 0x14, 0x15, 0x16, 0x17, 0x17, 0x18, 0x19, 0x19, 0x1a, 0x1b, 0x1b, 0x1c, 0x1c, 0x1d, 0x1d, 0x1e, 0x1e, 0x1f, 0x1f

jumptable:
    .byte 0,0,16,16,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,12,12,12,12,0,0
    .byte 0,0,16,16,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,12,12,12,12,0,0
    .byte 0,0,16,16,16,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,12,12,12,12,12,0,0
    .byte 0,0,16,16,16,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,12,12,12,12,12,0,0
    .byte 0,0,0,16,16,16,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,12,12,12,12,12,0,0,0
    .byte 0,0,0,16,16,16,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,12,12,12,12,12,0,0,0
    .byte 0,0,0,16,16,16,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,12,12,12,12,12,0,0,0
    .byte 0,0,0,16,16,16,16,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,12,12,12,12,12,12,0,0,0
    .byte 0,0,0,16,16,16,16,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,12,12,12,12,12,12,0,0,0
    .byte 0,0,0,16,16,16,16,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,12,12,12,12,12,12,0,0,0
    .byte 0,0,0,16,16,16,16,16,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,12,12,12,12,12,12,12,0,0,0
    .byte 0,0,0,16,16,16,16,16,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,12,12,12,12,12,12,12,0,0,0
    .byte 0,0,0,16,16,16,16,16,16,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,12,12,12,12,12,12,12,12,0,0,0
    .byte 0,0,0,16,16,16,16,16,16,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,12,12,12,12,12,12,12,12,0,0,0
    .byte 0,0,0,16,16,16,16,16,16,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,12,12,12,12,12,12,12,12,0,0,0
    .byte 0,0,0,16,16,16,16,16,16,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,12,12,12,12,12,12,12,12,0,0,0

.tmp_dsp_sqrt30_xs2:
	.size	dsp_sqrt30_xs2, .tmp_dsp_sqrt30_xs2-dsp_sqrt30_xs2
	.align	4
	.cc_bottom dsp_sqrt30_xs2.function

	.set	dsp_sqrt30_xs2.nstackwords,NSTACKWORDS
	.globl	dsp_sqrt30_xs2.nstackwords
	.set	dsp_sqrt30_xs2.maxcores,1
	.globl	dsp_sqrt30_xs2.maxcores
	.set	dsp_sqrt30_xs2.maxtimers,0
	.globl	dsp_sqrt30_xs2.maxtimers
	.set	dsp_sqrt30_xs2.maxchanends,0
	.globl	dsp_sqrt30_xs2.maxchanends
#endif
