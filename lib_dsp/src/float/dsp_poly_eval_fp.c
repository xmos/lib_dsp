// Copyright (c) 2019, XMOS Ltd, All rights reserved

double dsp_poly_eval_fp(const double x, const double * coefs, const unsigned n_coefs){
    double r = coefs[n_coefs-1];
    for(int i = n_coefs-2; i>=0; i--){
        r = (coefs[i] + (r*x));
    }
    return r;
}
