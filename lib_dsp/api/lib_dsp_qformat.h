// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef LIB_DSP_QFORMAT
#define LIB_DSP_QFORMAT

#include <math.h>

/** These Macros can be used to parameterize the conversion macros.
 * E.g.
 * #define BP 20  // location of the binary point
 * Then use the macro like this: F(BP)(fixed_point_val)
 */
#define F0(N) F ## N
#define F(N) F0(N)

#define Q0(N) Q ## N
#define Q(N) Q0(N)

// Convert from floating point to fixed point
#define Q31(f) (int)((signed long long)((f) * ((unsigned long long)1 << (31+20)) + (1<<19)) >> 20)
#define Q30(f) (int)((signed long long)((f) * ((unsigned long long)1 << (30+20)) + (1<<19)) >> 20)
#define Q29(f) (int)((signed long long)((f) * ((unsigned long long)1 << (29+20)) + (1<<19)) >> 20)
#define Q28(f) (int)((signed long long)((f) * ((unsigned long long)1 << (28+20)) + (1<<19)) >> 20)
#define Q27(f) (int)((signed long long)((f) * ((unsigned long long)1 << (27+20)) + (1<<19)) >> 20)
#define Q26(f) (int)((signed long long)((f) * ((unsigned long long)1 << (26+20)) + (1<<19)) >> 20)
#define Q25(f) (int)((signed long long)((f) * ((unsigned long long)1 << (25+20)) + (1<<19)) >> 20)
#define Q24(f) (int)((signed long long)((f) * ((unsigned long long)1 << (24+20)) + (1<<19)) >> 20)
#define Q23(f) (int)((signed long long)((f) * ((unsigned long long)1 << (23+20)) + (1<<19)) >> 20)
#define Q22(f) (int)((signed long long)((f) * ((unsigned long long)1 << (22+20)) + (1<<19)) >> 20)
#define Q21(f) (int)((signed long long)((f) * ((unsigned long long)1 << (21+20)) + (1<<19)) >> 20)
#define Q20(f) (int)((signed long long)((f) * ((unsigned long long)1 << (20+20)) + (1<<19)) >> 20)
#define Q19(f) (int)((signed long long)((f) * ((unsigned long long)1 << (19+20)) + (1<<19)) >> 20)
#define Q18(f) (int)((signed long long)((f) * ((unsigned long long)1 << (18+20)) + (1<<19)) >> 20)
#define Q17(f) (int)((signed long long)((f) * ((unsigned long long)1 << (17+20)) + (1<<19)) >> 20)
#define Q16(f) (int)((signed long long)((f) * ((unsigned long long)1 << (16+20)) + (1<<19)) >> 20)

// Convert from fixed point to double precision floating point
#define F31(x) ((double)(x)/(double)(unsigned)(1<<31)) // needs unsigned cast because bit 31 is 1
#define F30(x) ((double)(x)/(double)(1<<30))
#define F29(x) ((double)(x)/(double)(1<<29))
#define F28(x) ((double)(x)/(double)(1<<28))
#define F27(x) ((double)(x)/(double)(1<<27))
#define F26(x) ((double)(x)/(double)(1<<26))
#define F25(x) ((double)(x)/(double)(1<<25))
#define F24(x) ((double)(x)/(double)(1<<24))
#define F23(x) ((double)(x)/(double)(1<<23))
#define F22(x) ((double)(x)/(double)(1<<22))
#define F21(x) ((double)(x)/(double)(1<<21))
#define F20(x) ((double)(x)/(double)(1<<20))
#define F19(x) ((double)(x)/(double)(1<<19))
#define F18(x) ((double)(x)/(double)(1<<18))
#define F17(x) ((double)(x)/(double)(1<<17))
#define F16(x) ((double)(x)/(double)(1<<16))
#define F15(x) ((double)(x)/(double)(1<<15))

#endif

