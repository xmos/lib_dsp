#include <stdint.h>
#include "dsp_bfp.h"

extern int32_t dsp_float_to_bfp_xs3(float f[], uint32_t N);
extern void    dsp_bfp_to_float_xs3(float f[], uint32_t N, int32_t exponent);

int32_t dsp_float_to_bfp(float f[], uint32_t N) {
#if !defined(__XS3A__)
    asm volatile("ecallf %0" :: "r" (0));
#endif
    return dsp_float_to_bfp_xs3(f, N);
}

void dsp_bfp_to_float(float f[], uint32_t N, int32_t exponent) {
#if !defined(__XS3A__)
    asm volatile("ecallf %0" :: "r" (0));
#endif
    dsp_bfp_to_float_xs3(f, N, exponent);
}
