
#include <xclib.h>
#include <dsp_complex.h>

unsigned dsp_bfp_clz_uint16(const uint16_t d){
    return clz((unsigned)d);
}

unsigned dsp_bfp_clz_uint32(const uint32_t d){
    return clz(d);
}

unsigned dsp_bfp_clz_uint64(const uint64_t d){
    uint32_t hi = d>>32;
    uint32_t lo = d;
    if(hi)
        return clz(hi);
    else
        return 32 + clz(lo);
}

unsigned dsp_bfp_cls_int16(const int16_t d){
    if(d == INT16_MIN)
        return 0;
    int16_t v=d;
    if(v<0)
        v=-v;
    return dsp_bfp_clz_uint16((uint16_t)v);
}

unsigned dsp_bfp_cls_int32(const int32_t d){
    if(d == INT32_MIN)
        return 0;
    int32_t v=d;
    if(v<0)
        v=-v;
    return dsp_bfp_clz_uint32((uint32_t)v);
}

unsigned dsp_bfp_cls_int64(const int64_t d){
    if(d == INT64_MIN)
        return 0;
    int32_t v=d;
    if(v<0)
        v=-v;
    return dsp_bfp_clz_uint64((uint64_t)v);
}

unsigned dsp_bfp_cls_complex_int16(const dsp_complex_int16_t d){
    unsigned cls_re = dsp_bfp_cls_int16(d.re);
    unsigned cls_im = dsp_bfp_cls_int16(d.im);
    if(cls_re > cls_im){
        return cls_im;
    } else {
        return cls_re;
    }
}

unsigned dsp_bfp_cls_complex_int32(const dsp_complex_int32_t d){
    unsigned cls_re = dsp_bfp_cls_int32(d.re);
    unsigned cls_im = dsp_bfp_cls_int32(d.im);
    if(cls_re > cls_im){
        return cls_im;
    } else {
        return cls_re;
    }
}

unsigned dsp_bfp_clz_vect_uint16(const uint16_t *d, const unsigned length){
    uint16_t mask = 0;
    for(unsigned i=0;i<length;i++){
        mask |= d[i];
    }
    return dsp_bfp_clz_uint16(mask);
}

unsigned dsp_bfp_clz_vect_uint32(const uint32_t *d, const unsigned length){
    uint32_t mask = 0;
    for(unsigned i=0;i<length;i++){
        mask |= d[i];
    }
    return dsp_bfp_clz_uint32(mask);
}

unsigned dsp_bfp_clz_vect_uint64(const uint64_t *d, const unsigned length){
    uint64_t mask = 0;
    for(unsigned i=0;i<length;i++){
        mask |= d[i];
    }
    return dsp_bfp_clz_uint64(mask);
}

/*
 * Vector cls
 */
unsigned dsp_bfp_cls_vect_int16(const int16_t *d, const unsigned length);
unsigned dsp_bfp_cls_vect_int32(const int32_t *d, const unsigned length);
unsigned dsp_bfp_cls_vect_complex_int16(const dsp_complex_int16_t *d, const unsigned length);
unsigned dsp_bfp_cls_vect_complex_int32(const dsp_complex_int32_t *d, const unsigned length);
unsigned dsp_bfp_cls_vect_ch_pair_int16(const dsp_ch_pair_int16_t *d, const unsigned length,
        const unsigned channel_index);
unsigned dsp_bfp_cls_vect_ch_pair_int32(const dsp_ch_pair_int32_t *d, const unsigned length,
        const unsigned channel_index);

/*
 * Vector shl
 */
void dsp_bfp_shl_vect_uint16(uint16_t *d, const unsigned length, const int shl){
    if(shl == 0)
        return;
    for(unsigned i=0;i<length;i++){
        if(shl>0){
            d[i] <<= shl;
        } else {
            d[i] >>= (-shl);
        }
    }
}

void dsp_bfp_shl_vect_uint32(uint32_t *d, const unsigned length, const int shl){
    if(shl == 0)
        return;
    for(unsigned i=0;i<length;i++){
        if(shl>0){
            d[i] <<= shl;
        } else {
            d[i] >>= (-shl);
        }
    }
}

void dsp_bfp_shl_vect_int16(int16_t *d, const unsigned length, const int shl){
    if(shl == 0)
        return;
    for(unsigned i=0;i<length;i++){
        if(shl>0){
            d[i] <<= shl;
        } else {
            d[i] >>= (-shl);
        }
    }
}

void dsp_bfp_shl_vect_int32(int32_t *d, const unsigned length, const int shl){
    if(shl == 0)
        return;
    for(unsigned i=0;i<length;i++){
        if(shl>0){
            d[i] <<= shl;
        } else {
            d[i] >>= (-shl);
        }
    }
}

void dsp_bfp_shl_vect_complex_int16(dsp_complex_int16_t *d, const unsigned length, const int shl){
    dsp_bfp_shl_vect_int16((int16_t*)d, length*2, shl);
}

void dsp_bfp_shl_vect_complex_int32(dsp_complex_int32_t *d, const unsigned length, const int shl){
    dsp_bfp_shl_vect_int32((int32_t*)d, length*2, shl);
}

void dsp_bfp_shl_vect_ch_pair_int16(const dsp_ch_pair_int16_t *d, const unsigned length,
        const unsigned channel_index, const int shl){
    if(shl == 0)
        return;
    for(unsigned i=0;i<length;i++){
        if(shl>0){
            ((int16_t*)&(d[i]))[channel_index] <<= shl;
        } else {
            ((int16_t*)&(d[i]))[channel_index] >>= (-shl);
        }
    }
}

void dsp_bfp_slh_vect_ch_pair_int32(const dsp_ch_pair_int32_t *d, const unsigned length,
        const unsigned channel_index, const int shl){
    if(shl == 0)
        return;
    for(unsigned i=0;i<length;i++){
        if(shl>0){
            ((int32_t*)&(d[i]))[channel_index] <<= shl;
        } else {
            ((int32_t*)&(d[i]))[channel_index] >>= (-shl);
        }
    }
}


