#ifndef XMOS_DSP_FILTERS
#define XMOS_DSP_FILTERS

// Nth order FIR filter
//
// 'input_sample':  The new sample to be processed.
// 'filter_coeffs': Pointer to FIR coefficients arranged as [b0,b1,b2, ...,bN-1].
// 'state_data':    Pointer to filter state data array of length N.
//                  Must be initialized at startup to all zero's.
// 'tap_count':     Filter tap count (N = tap_count = filter order + 1).
// 'q_format':      Fixed point format, the number of bits making up fractional part.
//
// return value:    Resulting filter output sample.

int xmos_dsp_fir
(
    int        input_sample,
    const int* filter_coeffs,
    int*       state_data,
    int        tap_count,
    int        q_format
);

// Biquadratic IIR filter (direct form I)
//
// 'input_sample':  The new sample to be processed.
// 'filter_coeffs': Pointer to biquad coefficients arranged as [b0,b1,b2,a1,a2].
// 'state_data':    Pointer to filter state data (initialized at startup to zero's).
//                  The length of the state data array is 4.
// 'q_format':      Fixed point format, number of bits making up fractional part.
//
// return value:    Resulting filter output sample.

int xmos_dsp_iir_biquad
(
    int        input_sample,
    const int* filter_coeffs,
    int*       state_data,
    int        q_format
);

// Cascaded biquadratic IIR filter (direct form I)
//
// 'input_sample':  The new sample to be processed.
// 'filter_coeffs': Pointer to biquad coefficients for all BiQuad sections.
//                  arranged as [section 1: b0,b1,b2,a1,a2, ... section N: b0,b1,b2,a1,a2].
// 'state_data':    Pointer to filter state data (initialized at startup to zero's).
//                  The length of the state data array is 'num_sections' * 4.
// 'num_sections':  Number of BiQuad sections.
// 'q_format':      Fixed point format, the number of bits making up fractional part.
//
// return value:    Resulting filter output sample.

int xmos_dsp_cascaded_iir_biquad
(
    int        input_sample,
    const int* filter_coeffs,
    int*       state_data,
    int        num_sections,
    int        q_format
);

#endif
