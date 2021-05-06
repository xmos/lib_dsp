// Copyright 2018-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include <xs1.h>
#include <xclib.h>
#include <limits.h>

#include "dsp_fast_float.h"
#include "dsp_bfp.h"
#include "dsp_math.h"

unsigned dsp_normalise_s32(dsp_s32_float_t *val){
    if(val->m == 0){
        val->m = 0;
        val->e = DSP_FLOAT_ZERO_EXP;
        return 0;
    }
    int32_t mantissa = val->m;
    if(mantissa<0) mantissa = -mantissa;
    unsigned count = clz(mantissa) - 1;
    val->m <<= count;
    val->e -= count;
    return count;
}

unsigned dsp_normalise_u32(dsp_u32_float_t *val){
    if(val->m == 0){
        val->m = 0;
        val->e = DSP_FLOAT_ZERO_EXP;
        return 0;
    }
    unsigned count = clz(val->m);
    val->m <<= count;
    val->e -= count;
    return count;
}

unsigned dsp_normalise_s64(dsp_s64_float_t *val){
    if(val->m == 0){
        val->m = 0;
        val->e = DSP_FLOAT_ZERO_EXP;
        return 0;
    }
    int64_t mantissa = val->m;
    if(mantissa < 0) mantissa = -mantissa;
    unsigned count = dsp_bfp_clz_uint64(mantissa) - 1;
    val->m <<= count;
    val->e -= count;
    return count;
}

unsigned dsp_normalise_u64(dsp_u64_float_t *val){
    if(val->m == 0){
        val->m = 0;
        val->e = DSP_FLOAT_ZERO_EXP;
        return 0;
    }
    unsigned count = dsp_bfp_clz_uint64(val->m);
    val->m <<= count;
    val->e -= count;
    return count;
}

dsp_s32_float_t dsp_u32_to_s32(const dsp_u32_float_t a){
    dsp_s32_float_t r;
    r.m = a.m >> 1;
    r.e = a.e + 1;
    return r;
}


dsp_u32_float_t dsp_u64_to_u32(const dsp_u64_float_t a){
    dsp_u32_float_t r;
    r.m = a.m >>32;
    r.e = a.e + 32;
    return r;
}

dsp_s32_float_t dsp_s64_to_s32(const dsp_s64_float_t a){
    dsp_s32_float_t r;
    r.m = a.m >>32;
    r.e = a.e + 32;
    return r;
}

uint32_t dsp_denormalise_and_saturate_u32(const dsp_u32_float_t a, const int output_exp){
    if(a.e > output_exp){
        return UINT_MAX;
    } else {
        return a.m >> (output_exp - a.e);
    }
}

int32_t  dsp_denormalise_and_saturate_s32(const dsp_s32_float_t a, const int output_exp){
    if(a.e > output_exp){
        if(a.m > 0){
            return INT_MAX;
        } else {
            return INT_MIN;
        }
    } else {
        return a.m >> (output_exp - a.e);
    }
}

//things

dsp_s32_float_t dsp_abs_s32_to_s32(const dsp_s32_float_t a){
    if(a.m > 0){
        return a;
    } else {
        return dsp_neg_s32(a);
    }
}

dsp_u32_float_t dsp_abs_s32_to_u32(const dsp_s32_float_t a){
    dsp_u32_float_t r;
    if(a.m > 0){
        r.m = a.m;
        r.e = a.e;
    } else {
        dsp_s32_float_t t = dsp_neg_s32(a);
        r.m = t.m;
        r.e = t.e;
    }
    r.m <<= 1;
    r.e -= 1;
    return r;
}

dsp_s32_float_t dsp_neg_s32(const dsp_s32_float_t a){
    dsp_s32_float_t r;
    if(a.m == INT_MIN){
        r.m = INT_MAX;
        r.e = a.e;
        return r;
    } else {
        r.m = -a.m;
    }
    r.e = a.e;
    return r;
}

//equalities

int dsp_gte_s32_s32(const dsp_s32_float_t a, const dsp_s32_float_t b){
    if(a.e > b.e){
        int v = b.m >> (a.e - b.e);
        return a.m >= v;
    } else {
        int v = a.m >> (b.e - a.e);
        return v >= b.m;
    }
}

int dsp_gte_u32_u32(const dsp_u32_float_t a, const dsp_u32_float_t b){
    if(a.e > b.e){
        int v = b.m >> (a.e - b.e);
        return a.m >= v;
    } else {
        int v = a.m >> (b.e - a.e);
        return v >= b.m;
    }
}

int dsp_eq_s32_s32(const dsp_s32_float_t a, const dsp_s32_float_t b){
    if(a.e > b.e){
        int v = b.m >> (a.e - b.e);
        return a.m == v;
    } else {
        int v = a.m >> (b.e - a.e);
        return v == b.m;
    }
}

int dsp_eq_u32_u32(const dsp_u32_float_t a, const dsp_u32_float_t b){
    if(a.e > b.e){
        int v = b.m >> (a.e - b.e);
        return a.m == v;
    } else {
        int v = a.m >> (b.e - a.e);
        return v == b.m;
    }
}


//arithemetic

dsp_s32_float_t dsp_add_s32_s32(const dsp_s32_float_t a, const dsp_s32_float_t b){
    dsp_s32_float_t r;

    if(a.m == 0){
        r.m = b.m;
        r.e = b.e;
        return r;
    }
    if(b.m == 0){
        r.m = a.m;
        r.e = a.e;
        return r;
    }

    if(a.e > b.e){
        r.m = (a.m >> 1) + (b.m>>(a.e - b.e + 1)); //Lose 1b of precision when adding 0
        r.e = a.e + 1;
    } else {
        r.m = (b.m >> 1) + (a.m>>(b.e - a.e + 1));
        r.e = b.e + 1;
    }

    dsp_normalise_s32(&r);
    return r;
}

dsp_u32_float_t dsp_add_u32_u32(const dsp_u32_float_t a, const dsp_u32_float_t b){
    dsp_u32_float_t r;

    if(a.m == 0){
        r.m = b.m;
        r.e = b.e;
        return r;
    }
    if(b.m == 0){
        r.m = a.m;
        r.e = a.e;
        return r;
    }
    if(a.e > b.e){
        r.m = (a.m >> 1) + (b.m>>(a.e - b.e + 1));
        r.e = a.e + 1;
    } else {
        r.m = (b.m >> 1) + (a.m>>(b.e - a.e + 1));
        r.e = b.e + 1;
    }

    dsp_normalise_u32(&r);
    return r;
}

dsp_s32_float_t dsp_sub_s32_s32(const dsp_s32_float_t a, const dsp_s32_float_t b){
    dsp_s32_float_t c = dsp_neg_s32(b);
    return dsp_add_s32_s32(a, c);
}

dsp_s32_float_t dsp_sub_u32_u32(const dsp_u32_float_t a, const dsp_u32_float_t b){
    dsp_s32_float_t d = dsp_u32_to_s32(b);
    dsp_s32_float_t c = dsp_neg_s32(d);
    dsp_s32_float_t e = dsp_u32_to_s32(a);
    return dsp_add_s32_s32(e, c);
}


dsp_s32_float_t dsp_mul_s32_s32(const dsp_s32_float_t a, const dsp_s32_float_t b){
    dsp_s32_float_t r;

    int64_t va = a.m;
    int64_t vb = b.m;

    r.m = (va*vb)>>31;
    r.e = a.e + b.e + 31;

    dsp_normalise_s32(&r);

    return r;
}

dsp_s32_float_t dsp_mul_s32_u32(const dsp_s32_float_t a, const dsp_u32_float_t b){
    dsp_s32_float_t r;

    int64_t va = a.m;
    int64_t vb = b.m>>1;

    r.m = (va*vb)>>31;
    r.e = a.e + b.e + 32;

    dsp_normalise_s32(&r);

    return r;
}


dsp_u32_float_t dsp_mul_u32_u32(const dsp_u32_float_t a, const dsp_u32_float_t b){
    dsp_u32_float_t r;

    uint64_t va = a.m;
    uint64_t vb = b.m;

    r.m = (va*vb)>>32;
    r.e = a.e + b.e + 32;

    dsp_normalise_u32(&r);
    return r;
}

//Warning: This is not to be extenally accessable
static dsp_u32_float_t s32_to_u32(const dsp_s32_float_t a){
    dsp_u32_float_t r;
    r.m = a.m << 1;
    r.e = a.e - 1;
    return r;
}

//Warning: This is not to be extenally accessable
static dsp_u32_float_t neg_s32_to_u32(const dsp_s32_float_t a){
    dsp_u32_float_t r;
    r.m = (-a.m)<<1;
    r.e = a.e - 1;
    return r;
}

dsp_s32_float_t dsp_div_s32_s32(const dsp_s32_float_t a, const dsp_s32_float_t b){
    if(b.m == 0){
      dsp_s32_float_t r = {0x40000000,-30};
      return r;
    }
    if(a.m > 0){
        dsp_u32_float_t ua = s32_to_u32(a);

        if(b.m > 0){
            dsp_u32_float_t ub = s32_to_u32(b);
            dsp_u32_float_t c = dsp_div_u32_u32(ua, ub);
            return dsp_u32_to_s32(c);
        } else {
            dsp_u32_float_t ub = neg_s32_to_u32(b);
            dsp_u32_float_t c = dsp_div_u32_u32(ua, ub);
            dsp_s32_float_t d = dsp_u32_to_s32(c);
            return dsp_neg_s32(d);
        }
    } else {
        dsp_u32_float_t ua = neg_s32_to_u32(a);

        if(b.m > 0){
            dsp_u32_float_t ub = s32_to_u32(b);
            dsp_u32_float_t c = dsp_div_u32_u32(ua, ub);
            dsp_s32_float_t d = dsp_u32_to_s32(c);
            return dsp_neg_s32(d);
        } else {
            dsp_u32_float_t ub = neg_s32_to_u32(b);
            dsp_u32_float_t c = dsp_div_u32_u32(ua, ub);
            return dsp_u32_to_s32(c);
        }
    }

}

dsp_s32_float_t dsp_div_s32_u32(const dsp_s32_float_t a, const dsp_u32_float_t b){
    if(b.m == 0){
      dsp_s32_float_t r = {0x40000000,-30};
      return r;
    }
    if(a.m < 0){
        dsp_s32_float_t neg_a = dsp_neg_s32(a);

        dsp_u32_float_t c = s32_to_u32(neg_a);

        dsp_u32_float_t d = dsp_div_u32_u32(c, b);

        dsp_s32_float_t e = dsp_u32_to_s32(d);

        return dsp_neg_s32(e);
    } else {
        dsp_u32_float_t c = s32_to_u32(a);

        dsp_u32_float_t d = dsp_div_u32_u32(c, b);

        return dsp_u32_to_s32(d);
    }
}

dsp_u32_float_t dsp_div_u32_u32(const dsp_u32_float_t a, const dsp_u32_float_t b){
    if(b.m == 0){
      dsp_u32_float_t r = {0x80000000,-31};
      return r;
    }
    uint32_t numerator = a.m;
    int numerator_exp = a.e;

    uint32_t denominator = b.m;
    int denominator_exp = b.e;

    if(numerator == denominator){
        dsp_u32_float_t r = {0x80000000, numerator_exp +1 - denominator_exp -32};
        return r;
    }


    if (numerator >= denominator){
        numerator >>=1;
        numerator_exp += 1;
    }

    uint64_t t = (((uint64_t) numerator)<<32);

    dsp_u32_float_t r;

    int temp;
    uint32_t hi = t>>32;
    uint32_t lo = t;
    asm volatile("ldivu %0, %1, %2, %3, %4":"=r"(r.m), "=r"(temp): "r"(hi), "r"(lo), "r"(denominator));

    r.e = numerator_exp - denominator_exp -32;

    return r;
}

uint32_t dsp_sqrt30_xs2(uint32_t);
void dsp_sqrt_calc_exp(const int int_exp, const  unsigned hr, int * shl, int * out_exp);

dsp_u32_float_t dsp_sqrt_u32(const dsp_u32_float_t a){
    dsp_u32_float_t b;
    int shl;
    if(a.m != 0){

        dsp_sqrt_calc_exp(a.e, clz(a.m), &shl, &b.e);

#if (defined(__XS2A__) || defined (__XS3A__))
        if(shl > 0)
            b.m = dsp_sqrt30_xs2(a.m<<shl);
        else
            b.m = dsp_sqrt30_xs2(a.m>>(-shl));
#endif
        dsp_normalise_u32(&b);
        return b;
    } else {
        dsp_u32_float_t b = {0, DSP_FLOAT_ZERO_EXP};
        return b;
    }
}

void dsp_exponential_average_u32(dsp_u32_float_t *x, dsp_u32_float_t new_sample, uint32_t alpha){

    dsp_u32_float_t s;
    s.m = new_sample.m;
    s.e = new_sample.e;

    unsigned c = clz(alpha);
    unsigned d = clz(UINT_MAX - alpha);
    x->m = (uint32_t)(((uint64_t)x->m* (uint64_t)alpha)>>(32 -c));
    s.m = (uint32_t)(((uint64_t)s.m* (uint64_t)(UINT_MAX - alpha))>>(32- d));

    x->e -= c;
    s.e -= d;

    dsp_normalise_u32(x);
    dsp_normalise_u32(&s);

    *x = dsp_add_u32_u32(*x, s);

}
