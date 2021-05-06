// Copyright 2019-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include "dsp_complex.h"

void dsp_sub_vect_float(
	double * a,
	double * b, 
	double * c,
	unsigned length){
	for (unsigned i=0;i<length;i++){
		a[i] = b[i] - c[i];
	}
}

void dsp_add_vect_float(
	double * a,
	double * b, 
	double * c,
	unsigned length){
	for (unsigned i=0;i<length;i++){
		a[i] = b[i] + c[i];
	}
}

void dsp_mul_vect_float(
	double * a,
	double * b, 
	double * c,
	unsigned length){
	for (unsigned i=0;i<length;i++){
		a[i] = b[i] * c[i];
	}
}

void dsp_mul_vect_complex_float(
	dsp_complex_float_t * a,
	dsp_complex_float_t * b, 
	dsp_complex_float_t * c,
	unsigned length){
	for (unsigned i=0;i<length;i++){
		a[i].re = b[i].re * c[i].re - b[i].im * c[i].im ;
		a[i].im = b[i].re * c[i].im + b[i].im * c[i].re ;
	}
}
