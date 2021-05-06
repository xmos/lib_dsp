// Copyright 2019-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

double dsp_poly_eval_fp(const double x, const double * coefs, const unsigned n_coefs){
    double r = coefs[n_coefs-1];
    for(int i = n_coefs-2; i>=0; i--){
        r = (coefs[i] + (r*x));
    }
    return r;
}
