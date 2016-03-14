// Copyright (c) 2016, XMOS Ltd, All rights reserved
    
#if defined(__XS2A__)
    .section .dp.data,"awd",@progbits
    .text
    .cc_top  lib_dsp_fft_short_to_long.function
    .globl   lib_dsp_fft_short_to_long
    .align   4
    .type    lib_dsp_fft_short_to_long,@function

#define NSTACKWORDS 4


//void lib_dsp_fft_short_to_long(lib_dsp_fft_complex_short_t pts_in[], lib_dsp_fft_complex_t pts_out[], unsigned N);
lib_dsp_fft_short_to_long:
.align 8
.issue_mode dual
    DUALENTSP_lu6 NSTACKWORDS

    // Save context
    std r5, r4, sp[0]
    std r6, r7, sp[1]

    //load from lib_dsp_fft_complex_t
    {mov r3, r2;     shl r2, r2, 1}   // r3 = N;  r2 = N*2
    {sub r3, r3, 1;  sub r2, r2, 1}   // r3 = N-1;  r2 = N*2-1
copy_loop:
    //load 16-bit im and re from lib_dsp_fft_complex_short_t
    {ld16s r4, r0[r2]; sub r2, r2, 1} // r4: re; decrement offset
    {ld16s r5, r0[r2]; sub r2, r2, 1} // r5: im; decrement offset
    std r5, r4, r1[r3]                // store 32-bit im and re to lib_dsp_fft_complex_t
    {bt r3, copy_loop; sub r3, r3, 1} // if index > 0; index--

    // restore context
    ldd r5, r4, sp[0]
    ldd r6, r7, sp[1]
    retsp NSTACKWORDS

.tmp_lib_dsp_fft_short_to_long:
    .size    lib_dsp_fft_short_to_long, .tmp_lib_dsp_fft_short_to_long-lib_dsp_fft_short_to_long
    .align   4
    .cc_bottom lib_dsp_fft_short_to_long.function

    .set    lib_dsp_fft_short_to_long.nstackwords,NSTACKWORDS
    .globl  lib_dsp_fft_short_to_long.nstackwords
    .set    lib_dsp_fft_short_to_long.maxcores,1
    .globl  lib_dsp_fft_short_to_long.maxcores
    .set    lib_dsp_fft_short_to_long.maxtimers,0
    .globl  lib_dsp_fft_short_to_long.maxtimers
    .set    lib_dsp_fft_short_to_long.maxchanends,0
    .globl  lib_dsp_fft_short_to_long.maxchanends

#endif

