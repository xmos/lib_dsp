// Copyright (c) 2016, XMOS Ltd, All rights reserved
    
#if defined(__XS2A__)
    .section .dp.data,"awd",@progbits
    .text
    .cc_top  lib_dsp_fft_long_to_short.function
    .globl   lib_dsp_fft_long_to_short
    .align   4
    .type    lib_dsp_fft_long_to_short,@function

#define NSTACKWORDS 4

// array index for pts_in and pts_out
#define i r2

//void lib_dsp_fft_long_to_short(lib_dsp_fft_complex_t pts_in[], lib_dsp_fft_complex_short_t pts_out[], unsigned N);
lib_dsp_fft_long_to_short:
.align 8
.issue_mode dual
    DUALENTSP_lu6 NSTACKWORDS

    // Save context
    std r5, r4, sp[0]
    std r6, r7, sp[1]

    sub i, r2, 1    // i = r2 = N-1
    mkmsk r6, 16    // r6 = 0xffff
copy_loop:
    ldd r5, r4, r0[i] // load 32-bit im and re from lib_dsp_fft_complex_t pts_in[]
    // store 16-bit im and re to lib_dsp_fft_complex_short_t pts_out[]
    // Note: st16 cannot be used because it is only available in format l3r so dual issue cannot be used
    // pack the 16 bit data into 32 bit words.
    { and r4, r4, r6 ; shl r5, r5, 16 }   // re &= 0xffff; (im << 16);
    or r4, r4, r5                         // re |= (im << 16);
    stw r4, r1[i]                         // pts_out[i] = r4;
    {bt i, copy_loop ; sub i, i, 1}       // if index > 0; i--

    // restore context
    ldd r5, r4, sp[0]
    ldd r6, r7, sp[1]
    retsp NSTACKWORDS

.tmp_lib_dsp_fft_long_to_short:
    .size    lib_dsp_fft_long_to_short, .tmp_lib_dsp_fft_long_to_short-lib_dsp_fft_long_to_short
    .align   4
    .cc_bottom lib_dsp_fft_long_to_short.function

    .set    lib_dsp_fft_long_to_short.nstackwords,NSTACKWORDS
    .globl  lib_dsp_fft_long_to_short.nstackwords
    .set    lib_dsp_fft_long_to_short.maxcores,1
    .globl  lib_dsp_fft_long_to_short.maxcores
    .set    lib_dsp_fft_long_to_short.maxtimers,0
    .globl  lib_dsp_fft_long_to_short.maxtimers
    .set    lib_dsp_fft_long_to_short.maxchanends,0
    .globl  lib_dsp_fft_long_to_short.maxchanends

#endif

