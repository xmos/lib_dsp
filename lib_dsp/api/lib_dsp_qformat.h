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

// Convert from fixed point to double precision floating point
#define Q31(f) round(f * (double)(1<<31))
#define Q30(f) round(f * (double)(1<<30))
#define Q29(f) round(f * (double)(1<<29))
#define Q28(f) round(f * (double)(1<<28))
#define Q27(f) round(f * (double)(1<<27))
#define Q26(f) round(f * (double)(1<<26))
#define Q25(f) round(f * (double)(1<<25))
#define Q24(f) round(f * (double)(1<<24))
#define Q23(f) round(f * (double)(1<<23))
#define Q22(f) round(f * (double)(1<<22))
#define Q21(f) round(f * (double)(1<<21))
#define Q20(f) round(f * (double)(1<<20))
#define Q19(f) round(f * (double)(1<<19))
#define Q18(f) round(f * (double)(1<<18))
#define Q17(f) round(f * (double)(1<<17))
#define Q16(f) round(f * (double)(1<<16))

// Convert from double precision floating point to fixed point
#define F31(x) ((double)(x)/(double)(1<<31))
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

