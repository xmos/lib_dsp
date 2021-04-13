// Copyright 2015-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
    
#if (defined(__XS2A__) || defined (__XS3A__))
    
#define NSTACKWORDS 6

	.text
    .issue_mode  dual
    .align 4
	.globl	dsp_complex_window_hanning_post_fft_half
	.type	dsp_complex_window_hanning_post_fft_half,@function
	.cc_top dsp_complex_window_hanning_post_fft_half.function,dsp_complex_window_hanning_post_fft_half

// dsp_complex_window_hanning_post_fft_half(data, N)
dsp_complex_window_hanning_post_fft_half:
	{ dualentsp NSTACKWORDS       ; sub r1, r1, 2 }
    std r4, r5, sp[0]
    std r6, r7, sp[1]
    std r8, r9, sp[2]

    // Invariant: r6/r7     term i+1
    //            r8/r9     term i+2
    // Start: i is N-2
    //            r6/r7     term N-1
    //            r8/r9     0/NyquistReal

    
    { add r8, r1, 1               ; nop }
    
    ldd r6, r7, r0[r8]             // R6/R7 IM/RE bin[n-1]
    ldw r9, r0[1]                  // R9: Nyquist real term, R8: 0 (NyIm)
    ashr r6, r6, 1
    ashr r7, r7, 1
    ashr r9, r9, 1
    { ldc r8, 0                    ; add r11, r7, 0 }
.Lloop:
    ldd r4, r5, r0[r1]
    ashr r4, r4, 1
    ashr r5, r5, 1
    { add r3, r5, r9              ; add r2, r4, r8 }
    ashr r2, r2, 1
    ashr r3, r3, 1
    { sub r3, r7, r3              ; sub r2, r6, r2 }
    { add r9, r1, 1                ; sub r1, r1, 1 }
    std r2, r3, r0[r9]
    { add r9, r7, 0               ; add r8, r6, 0 }
    { add r7, r5, 0               ; add r6, r4, 0 }
    bt r1, .Lloop
    
    ldd r4, r5, r0[0]             // R5: DC term; R4: NYQ term
    ashr r4, r4, 1
    ashr r5, r5, 1
    { add r3, r5, r9              ; add r2, r8, 0 }
    ashr r2, r2, 1
    ashr r3, r3, 1
    { sub r3, r7, r3              ; sub r2, r6, r2 }
    std r2, r3, r0[1]

    { sub r4, r4, r11              ; sub r5, r5, r7 }
    std r4, r5, r0[0]
    
    ldd r4, r5, sp[0]
    ldd r6, r7, sp[1]
    ldd r8, r9, sp[2]
	retsp NSTACKWORDS
	
	// RETURN_REG_HOLDER
	.cc_bottom dsp_complex_window_hanning_post_fft_half.function
	.set	dsp_complex_window_hanning_post_fft_half.nstackwords,NSTACKWORDS
	.globl	dsp_complex_window_hanning_post_fft_half.nstackwords
	.set	dsp_complex_window_hanning_post_fft_half.maxcores,1
	.globl	dsp_complex_window_hanning_post_fft_half.maxcores
	.set	dsp_complex_window_hanning_post_fft_half.maxtimers,0
	.globl	dsp_complex_window_hanning_post_fft_half.maxtimers
	.set	dsp_complex_window_hanning_post_fft_half.maxchanends,0
	.globl	dsp_complex_window_hanning_post_fft_half.maxchanends
.Ltmp3:
	.size	dsp_complex_window_hanning_post_fft_half, .Ltmp3-dsp_complex_window_hanning_post_fft_half

#endif
