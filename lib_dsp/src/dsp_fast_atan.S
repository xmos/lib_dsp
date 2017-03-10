// Copyright (c) 2015-2016, XMOS Ltd, All rights reserved
#if defined(__XS2A__)
    
#define ITER(X) \
	{bf t, y_negative_ ## X         ; add j, j, 1}; \
	{shr t, y, j;       shr s, x, j}; \
	{add x, x, t;       ldw t, g[j]}; \
	{sub y, y, s;       add b, b, t}; \
    {bu out_ ## X;      clz t, y}; \
y_negative_ ## X: \
	ashr t, y, j; \
	{sub x, x, t;       shr t, x, j}; \
	{add y, y, t;       ldw t, g[j]}; \
	{sub b, b, t;       clz t, y}; \
out_ ## X:


	.section    .cp.rodata, "ac", @progbits
cordic_const: // This must be just before cordic_angles
	.word 268435456*-4
cordic_angles:
    .word 268435456, 158466703, 83729454, 42502378
    .word 21333666, 10677233, 5339919, 2670123
    .word 1335082, 667543, 333772, 166886
    .word 83443, 41722, 20861, 10430
    .word 5215, 2608, 1304, 652
    .word 326, 163, 81, 41
//    .word 20, 10, 5, 3
//    .word 1, 0
cordic_factor:
    .word 2608131503
    
	.section	.dp.data,"awd",@progbits
	.text
	.cc_top dsp_math_atan2_hypot.function
	.globl	dsp_math_atan2_hypot
	.align	4
	.type	dsp_math_atan2_hypot,@function

dsp_math_atan2_hypot:
.align 8
.issue_mode dual
// void cordic_in_place(fft_complex &c)

#define p 	  r0
#define x 	  r7
#define y 	  r2
#define b     r3

#define j     r4
#define shift_distance  r5

#define t     r6
#define s     r1

#define g     r11


#define NSTACKWORDS 4

    //  1.5    1 | 1       0.5
    //    y+ x-  |   y+ x+ 
    //  2        |          0
    // ----------+--------------
    //  -2       |         -0
    //    y- x-  |   y- x+ 
    //  -1.5  -1 | -1     -0.5
	DUALENTSP_lu6 NSTACKWORDS
	std r5, r4, sp[0]
	std r7, r6, sp[1]
	ldaw g, cp[cordic_angles]
	ldd y, x, p[0]

	{shl shift_distance, s, 3  ; mkmsk j, 32}
	{ldw b, g[j]               ; clz t, x }
	{bf t, x_neg               ; add s, s, shift_distance }
    {bu x_complete             ; ldc b, 0}
x_neg:   
	{neg x, x                  ; neg y, y}
    
x_complete:
    {clz  t, x                 ; clz shift_distance, y}
    {bt shift_distance, y_pos  ; add shift_distance, y, 0 }
    {neg shift_distance, y     ; neg b, b }
    
y_pos:
    {clz shift_distance, shift_distance; eq j, t, 1}
    {bt j, shift_back          ; lss j, shift_distance, t}
    {bf j, shift_by_t          ; eq j, shift_distance, 1}
    {bt j, shift_back          ; sub shift_distance, shift_distance, 2}
    {shl x, x, shift_distance  ; shl y, y, shift_distance}
    {bu shift_done             ; mkmsk   j, 32}
    
shift_by_t:
    {sub shift_distance, t, 2  ;                         }
    {shl x, x, shift_distance  ; shl y, y, shift_distance}
    {bu shift_done             ; mkmsk   j, 32}
shift_back:
    {ashr y, y, 1}
    {shr x, x, 1                ; mkmsk shift_distance, 32}
    {mkmsk j, 32                ;}
    
shift_done:
    {bru s; clz t, y}
    
    ITER(0)
    ITER(1)
    ITER(2)
    ITER(3)
    ITER(4)
    ITER(5)
    ITER(6)
    ITER(7)
    ITER(8)
    ITER(9)
    ITER(10)
    ITER(11)
    ITER(12)
    ITER(13)
    ITER(14)
    ITER(15)
    ITER(16)
    ITER(17)
    ITER(18)
    ITER(19)
    ITER(20)
    ITER(21)
    ITER(22)
    ITER(23)
    
loop_done:
    ldw t, cp[cordic_factor]
    {clz s, shift_distance       ; ldc g, 0}
    {shr s, t, shift_distance              ; bf s, shiftup}
    lmul x, t, x, s, g, g
thelmul:    
	std b, x, p[0]
	ldd r7, r6, sp[1]
	ldd r5, r4, sp[0]
	retsp NSTACKWORDS
shiftup:
    lmul x, t, x, t, g, g
    {shl x, x, 1                 ; bu thelmul}

.tmp_dsp_math_atan2_hypot:
	.size	dsp_math_atan2_hypot, .tmp_dsp_math_atan2_hypot-dsp_math_atan2_hypot
	.align	4
	.cc_bottom dsp_math_atan2_hypot.function

	.set	dsp_math_atan2_hypot.nstackwords,NSTACKWORDS
	.globl	dsp_math_atan2_hypot.nstackwords
	.set	dsp_math_atan2_hypot.maxcores,1
	.globl	dsp_math_atan2_hypot.maxcores
	.set	dsp_math_atan2_hypot.maxtimers,0
	.globl	dsp_math_atan2_hypot.maxtimers
	.set	dsp_math_atan2_hypot.maxchanends,0
	.globl	dsp_math_atan2_hypot.maxchanends

#endif
