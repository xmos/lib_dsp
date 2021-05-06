// Copyright 2019-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include <stdint.h>

int32_t dsp_poly_eval_impl_xs2
(
    const int32_t x,
    const int32_t * coefs,
    const unsigned n_coefs
);

int32_t dsp_poly_eval
(
    const int32_t x,
    const int32_t * coefs,
    const unsigned n_coefs
){

#if (defined(__XS2A__) || defined (__XS3A__))
    return dsp_poly_eval_impl_xs2(x, coefs, n_coefs);
#else
    int32_t r = coefs[n_coefs-1];
    for(int i = n_coefs-2; i>=0; i--){
        r = (coefs[i] + (((int64_t)r*(int64_t)x)>>31));
    }
    return r;
#endif
}
