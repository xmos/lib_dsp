// Copyright (c) 2016, XMOS Ltd, All rights reserved
	.section	.dp.data,"awd",@progbits
	.text

	.cc_top fir_inner_loop_asm.function
	.globl	fir_inner_loop_asm
	.align	8
	.type	fir_inner_loop_asm,@function

fir_inner_loop_asm:

#define piData_p		 	r0
#define piCoefs_p		 	r1
#define iDataRet_p    		r2
#define step				r2
#define counter		    	r3
#define iCoef0 		   		r4
#define iCoef1		 	 	r5
#define iData0  			r6
#define iData1    			r7
#define Acc0_l	    		r8
#define Acc0_h 		  		r9



//In 32b words
#define stack_size		8

.issue_mode dual
	//Push registers
	DUALENTSP_lu6 stack_size
	std r4, r5, sp[0]
	std r6, r7, sp[1]
	std r8, r9, sp[2]

	//push return value pointers onto stack. Frees up r2
	//Divide counter by 8
	{stw iDataRet_p, sp[6]; shr counter, counter, 3}

	//Constant for adding onto stuff. Gets updated later
	ldc step, 0x40

	//Initilise MACC regsiters to zero
	{ldc Acc0_h, 0; ldc Acc0_l, 0}


	bf counter, done
main_loop:
	sub counter, counter, 1

	ldd iData0, iData1, piData_p[0]
	ldd iCoef0, iCoef1, piCoefs_p[0]
	maccs Acc0_h, Acc0_l, iData0, iCoef0
	maccs Acc0_h, Acc0_l, iData1, iCoef1

	ldd iData0, iData1, piData_p[1]
	ldd iCoef0, iCoef1, piCoefs_p[1]
	maccs Acc0_h, Acc0_l, iData0, iCoef0
	maccs Acc0_h, Acc0_l, iData1, iCoef1

	ldd iData0, iData1, piData_p[2]
	ldd iCoef0, iCoef1, piCoefs_p[2]
	maccs Acc0_h, Acc0_l, iData0, iCoef0
	maccs Acc0_h, Acc0_l, iData1, iCoef1

	ldd iData0, iData1, piData_p[3]
	ldd iCoef0, iCoef1, piCoefs_p[3]
	maccs Acc0_h, Acc0_l, iData0, iCoef0
	maccs Acc0_h, Acc0_l, iData1, iCoef1

	ldd iData0, iData1, piData_p[4]
	ldd iCoef0, iCoef1, piCoefs_p[4]
	maccs Acc0_h, Acc0_l, iData0, iCoef0
	maccs Acc0_h, Acc0_l, iData1, iCoef1

	ldd iData0, iData1, piData_p[5]
	ldd iCoef0, iCoef1, piCoefs_p[5]
	maccs Acc0_h, Acc0_l, iData0, iCoef0
	maccs Acc0_h, Acc0_l, iData1, iCoef1

	ldd iData0, iData1, piData_p[6]
	ldd iCoef0, iCoef1, piCoefs_p[6]
	maccs Acc0_h, Acc0_l, iData0, iCoef0
	maccs Acc0_h, Acc0_l, iData1, iCoef1

	ldd iData0, iData1, piData_p[7]
	ldd iCoef0, iCoef1, piCoefs_p[7]
	maccs Acc0_h, Acc0_l, iData0, iCoef0
	maccs Acc0_h, Acc0_l, iData1, iCoef1

	//Decrement loop counter and increment pointers and branch if still running
	{add piData_p, piData_p, step; add piCoefs_p, piCoefs_p, step}

	bt counter, main_loop

done:
	//pop return value pointer from stack so we can use it to write back
	//Write back the two return values
	{ldw iDataRet_p, sp[6]; 	ldc counter, 31}						//Reuse counter (r3)

	lsats Acc0_h, Acc0_l, r3			//Saturate first
	lextract iData0, Acc0_h, Acc0_l, counter, 32	//Extract
	stw iData0, iDataRet_p[0]

	//Pop registers
	ldd r4, r5, sp[0]
	ldd r6, r7, sp[1]
	ldd r8, r9, sp[2]
  retsp stack_size

.atmp:
	.size	fir_inner_loop_asm, .atmp-fir_inner_loop_asm
	.align	8
	.cc_bottom fir_inner_loop_asm.function

	.set	fir_inner_loop_asm.nstackwords, stack_size
	.globl	fir_inner_loop_asm.nstackwords
	.set	fir_inner_loop_asm.maxcores, 1
	.globl	fir_inner_loop_asm.maxcores
	.set	fir_inner_loop_asm.maxtimers,0
	.globl	fir_inner_loop_asm.maxtimers
	.set	fir_inner_loop_asm.maxchanends,0
	.globl	 .maxchanends


		.section	.dp.data,"awd",@progbits
	.text

	.cc_top fir_inner_loop_asm_odd.function
	.globl	fir_inner_loop_asm_odd
	.align	8
	.type	fir_inner_loop_asm_odd,@function

fir_inner_loop_asm_odd:

#define piData_p		 	r0
#define piCoefs_p		 	r1
#define iDataRet_p    		r2
#define step				r2
#define counter		    	r3
#define iCoef0 		   		r4
#define iCoef1		 	 	r5
#define iData0  			r6
#define iData1    			r7
#define Acc0_l	    		r8
#define Acc0_h 		  		r9



//In 32b words
#define stack_size		8

.issue_mode dual
	//Push registers
	DUALENTSP_lu6 stack_size
	std r4, r5, sp[0]
	std r6, r7, sp[1]
	std r8, r9, sp[2]

	//push return value pointers onto stack. Frees up r2
	//Divide counter by 8
	{stw iDataRet_p, sp[6]; shr counter, counter, 3}

	//Initilise MACC regsiters to zero
	{ldc Acc0_h, 0; ldc Acc0_l, 0}

	bf counter, done
main_loop_odd:

	{sub counter, counter, 1; ldc step, 0x38}

	//Constant for adding onto stuff. Gets updated later

	{ldw iData0, piData_p[0]; add piData_p, piData_p, 4}				//Now 64b aligned

	ldd iCoef1, iCoef0, piCoefs_p[0]
	maccs Acc0_h, Acc0_l, iData0, iCoef0
	ldd iData0, iData1, piData_p[0]
	maccs Acc0_h, Acc0_l, iData1, iCoef1

	ldd iCoef1, iCoef0, piCoefs_p[1]
	maccs Acc0_h, Acc0_l, iData0, iCoef0
	ldd iData0, iData1, piData_p[1]
	maccs Acc0_h, Acc0_l, iData1, iCoef1

	ldd iCoef1, iCoef0, piCoefs_p[2]
	maccs Acc0_h, Acc0_l, iData0, iCoef0
	ldd iData0, iData1, piData_p[2]
	maccs Acc0_h, Acc0_l, iData1, iCoef1

	ldd iCoef1, iCoef0, piCoefs_p[3]
	maccs Acc0_h, Acc0_l, iData0, iCoef0
	ldd iData0, iData1, piData_p[3]
	maccs Acc0_h, Acc0_l, iData1, iCoef1

	ldd iCoef1, iCoef0, piCoefs_p[4]
	maccs Acc0_h, Acc0_l, iData0, iCoef0
	ldd iData0, iData1, piData_p[4]
	maccs Acc0_h, Acc0_l, iData1, iCoef1

	ldd iCoef1, iCoef0, piCoefs_p[5]
	maccs Acc0_h, Acc0_l, iData0, iCoef0
	ldd iData0, iData1, piData_p[5]
	maccs Acc0_h, Acc0_l, iData1, iCoef1

	ldd iCoef1, iCoef0, piCoefs_p[6]
	maccs Acc0_h, Acc0_l, iData0, iCoef0
	ldd iData0, iData1, piData_p[6]
	maccs Acc0_h, Acc0_l, iData1, iCoef1

	ldd iCoef1, iCoef0, piCoefs_p[7]
	maccs Acc0_h, Acc0_l, iData0, iCoef0

	add piData_p, piData_p, step

	{ldw iData1, piData_p[0]; add piData_p, piData_p, 4} //0x4 + 0x38 + 0x4 = 0x40)
	maccs Acc0_h, Acc0_l, iData1, iCoef1
	//Increment pointers and branch if still running

	ldc step, 0x40
	{bt counter, main_loop_odd; add piCoefs_p, piCoefs_p, step}

done_odd:
	//pop return value pointer from stack so we can use it to write back
	{ldw iDataRet_p, sp[6]; ldc counter, 31}						//Reuse counter (r3)

	//Write back the return value
	lsats Acc0_h, Acc0_l, r3			//Saturate first
	lextract iData0, Acc0_h, Acc0_l, counter, 32	//Extract
	stw iData0, iDataRet_p[0]

	//Pop registers
	ldd r4, r5, sp[0]
	ldd r6, r7, sp[1]
	ldd r8, r9, sp[2]
  retsp stack_size

.atmp_odd:
	.size	fir_inner_loop_asm_odd, .atmp-fir_inner_loop_asm_odd
	.align	8
	.cc_bottom fir_inner_loop_asm_odd.function

	.set	fir_inner_loop_asm_odd.nstackwords, stack_size
	.globl	fir_inner_loop_asm_odd.nstackwords
	.set	fir_inner_loop_asm_odd.maxcores, 1
	.globl	fir_inner_loop_asm_odd.maxcores
	.set	fir_inner_loop_asm_odd.maxtimers,0
	.globl	fir_inner_loop_asm_odd.maxtimers
	.set	fir_inner_loop_asm_odd.maxchanends,0
	.globl	 .maxchanends
