#ifndef XMOS_DSP_FILTERS2
#define XMOS_DSP_FILTERS2

// LMS filter
//
// 'input_sample':     The new sample to be processed.
// 'reference_sample': Reference sample
// 'error_sample':     Pointer to resulting error sample (error = reference - output)
// 'filter_coeffs':    Pointer to FIR coefficients arranged as [b0,b1,b2, ...,bN-1].
// 'state_data':       Pointer to FIR filter state data array of length N.
//                     Must be initialized at startup to all zero's.
// 'tap_count':        Filter tap count where N = tap_count = filter order + 1.
// 'step_size':        Coefficient adjustment step size, controls rate of convergence.
// 'q_format':         Fixed point format, the number of bits making up fractional part.
//
// return value:    Resulting filter output sample.

int xmos_dsp_lms
(
    int  input_sample,
    int  reference_sample,
    int* error_sample,
    int* filter_coeffs,
    int* state_data,
    int  tap_count,
    int  step_size,
    int  q_format
);

// Normalized LMS filter
//
// 'input_sample':     The new sample to be processed.
// 'reference_sample': Reference sample
// 'error_sample':     Pointer to resulting error sample (error = reference - output)
// 'filter_coeffs':    Pointer to FIR coefficients arranged as [b0,b1,b2, ...,bN-1].
// 'state_data':       Pointer to FIR filter state data array of length N.
//                     Must be initialized at startup to all zero's.
// 'tap_count':        Filter tap count where N = tap_count = filter order + 1.
// 'step_size':        Coefficient adjustment step size, controls rate of convergence.
// 'q_format':         Fixed point format, the number of bits making up fractional part.
//
// return value:    Resulting filter output sample.

int xmos_dsp_nlms
(
    int  input_sample,
    int  reference_sample,
    int* error_sample,
    int* filter_coeffs,
    int* state_data,
    int  tap_count,
    int  step_size,
    int  q_format
);

#endif
