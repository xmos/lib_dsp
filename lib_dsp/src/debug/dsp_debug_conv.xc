/*
 * dsp_debug_conv.xc
 *
 *  Created on: Jul 10, 2019
 *      Author: andrew
 */


/*
 * Type conversion
 */
float_t dsp_conv_int16_to_float (const int16_t x,  const int x_exp){
    return ldexp((float_t)x, x_exp);
}

float_t dsp_conv_uint16_to_float(const uint16_t x, const int x_exp){
    return ldexp((float_t)x, x_exp);
}

float_t dsp_conv_int32_to_float (const int32_t x,  const int x_exp){
    return ldexp((float_t)x, x_exp);
}

float_t dsp_conv_uint32_to_float(const uint32_t x, const int x_exp){
    return ldexp((float_t)x, x_exp);
}

float_t dsp_conv_int64_to_float (const int64_t x,  const int x_exp){
    return ldexp((float_t)x, x_exp);
}

float_t dsp_conv_uint64_to_float(const uint64_t x, const int x_exp){
    return ldexp((float_t)x, x_exp);
}

int16_t  dsp_conv_float_to_int16 (float_t d, const int d_exp, int &error){
    int m_exp;
    double m = frexp (d, &m_exp);

    double r = ldexp(m, m_exp - d_exp);
    int output_exponent;
    frexp(r, &output_exponent);
    if(output_exponent>15){
        error = 1;
        return 0;
    }
    error = 0;
    return r;
}

uint16_t dsp_conv_float_to_uint16(float_t d, const int d_exp, int &error){
    int m_exp;
    double m = frexp (d, &m_exp);
    if(m<0.0){
        error = 1;
        return 0;
    }
    error = 0;
    return ldexp(m, m_exp - d_exp);
}

int32_t  dsp_conv_float_to_int32 (float_t d, const int d_exp, int &error){
    int m_exp;
    double m = frexp (d, &m_exp);

    double r = ldexp(m, m_exp - d_exp);
    int output_exponent;
    frexp(r, &output_exponent);
    if(output_exponent>31){
        error = 1;
        return 0;
    }
    error = 0;
    return r;
}

uint32_t dsp_conv_float_to_uint32(float_t d, const int d_exp, int &error){
    int m_exp;
    double m = frexp (d, &m_exp);
    if(m<0.0){
        error = 1;
        return 0;
    }
    error = 0;
    return ldexp(m, m_exp - d_exp);
}

int64_t  dsp_conv_float_to_int64 (float_t d, const int d_exp, int &error){
    int m_exp;
    double m = frexp (d, &m_exp);

    double r = ldexp(m, m_exp - d_exp);
    int output_exponent;
    frexp(r, &output_exponent);
    if(output_exponent>63){
        error = 1;
        return 0;
    }
    error = 0;
    return r;
}

uint64_t dsp_conv_float_to_uint64(float_t d, const int d_exp, int &error){
    int m_exp;
    double m = frexp (d, &m_exp);
    if(m<0.0){
        error = 1;
        return 0;
    }
    error = 0;
    return (uint64_t)ldexp(m, m_exp - d_exp);
}


dsp_complex_float_t dsp_conv_complex_int16_to_complex_float(dsp_complex_int16_t x, const int x_exp){
    dsp_complex_t r;
    r.re = att_double_to_int32(d.re, d_exp);
    r.im = att_double_to_int32(d.im, d_exp);
    return r;
}
dsp_complex_float_t dsp_conv_complex_int32_to_complex_float(dsp_complex_int32_t x, const int x_exp);
dsp_ch_pair_float_t dsp_conv_ch_pair_int16_to_complex_float(dsp_ch_pair_int16_t x, const int x_exp);
dsp_ch_pair_float_t dsp_conv_ch_pair_int32_to_complex_float(dsp_ch_pair_int32_t x, const int x_exp);
