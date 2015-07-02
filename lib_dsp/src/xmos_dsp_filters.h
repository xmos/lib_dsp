#ifndef XMOS_DSP_FILTERS
#define XMOS_DSP_FILTERS

// FIR filter
//
// 'input_sample':  The new sample to be processed.
// 'filter_coeffs': Pointer to FIR coefficients array arranged as [b0,b1,b2, ...,bN-1].
// 'state_data':    Pointer to filter state data array of length N.
//                  Must be initialized at startup to all zero's.
// 'tap_count':     Filter tap count (N = tap_count = filter order + 1).
// 'q_format':      Fixed point format, the number of bits making up fractional part.
//
// return value:    Resulting filter output sample.

int xmos_dsp_filters_fir
(
    int       input_sample,
    const int filter_coeffs[],
    int       state_data[],
    int       tap_count,
    int       q_format
);

// Interpolating FIR filter
//
// 'output_samples': The resulting interpolated samples.
// 'input_sample':   The new sample to be processed.
// 'filter_coeffs':  Pointer to FIR coefficients array arranged as:
//                      h0,h(1L+0),h(2L+0),...h((N-1)L+0),
//                      h1,h(1L+1),h(2L+1),...h((N-1)L+1),
//                      ...
//                      hM,h(1L+M),h(2L+M),...h((N-1)L+M),
//                   where M = N-1
// 'state_data':     Pointer to filter state data array of length N.
//                   Must be initialized at startup to all zero's.
// 'phase_length':   Phase length (N), FIR filter tap count = N * interp_factor.
// 'interp_factor':  The interpolation factor/index (i.e. the up-sampling ratio).
//                   The interpolation factor/index can range from 2 to 16.
// 'q_format':       Fixed point format, the number of bits making up fractional part.

void xmos_dsp_filters_interpolate
(
    int       output_samples[],
    int       input_sample,
    const int filter_coeffs[],
    int       state_data[],
    int       tap_count,
    int       interp_factor,
    int       q_format
);

// Decimating FIR filter
//
// 'input_samples': The new samples to be decimated.
// 'filter_coeffs': Pointer to FIR coefficients array arranged as [b0,b1,b2, ...,bN-1].
// 'state_data':    Pointer to filter state data array of length N.
//                  Must be initialized at startup to all zero's.
// 'tap_count':     Filter tap count (N = tap_count = filter order + 1).
// 'decim_factor':  The decimation factor/index (i.e. the down-sampling ratio).
// 'q_format':      Fixed point format, the number of bits making up fractional part.
//
// return value:    The resulting decimated sample.

int xmos_dsp_filters_decimate
(
    int       input_samples[],
    const int filter_coeffs[],
    int       state_data[],
    int       tap_count,
    int       decim_factor,
    int       q_format
);

// Biquadratic IIR filter (direct form I)
//
// 'input_sample':  The new sample to be processed.
// 'filter_coeffs': Pointer to biquad coefficients array arranged as [b0,b1,b2,a1,a2].
// 'state_data':    Pointer to filter state data array (initialized at startup to zero's).
//                  The length of the state data array is 4.
// 'q_format':      Fixed point format, number of bits making up fractional part.
//
// return value:    Resulting filter output sample.

int xmos_dsp_filters_biquad
(
    int       input_sample,
    const int filter_coeffs[],
    int       state_data[],
    int       q_format
);

// Cascaded biquadratic IIR filter (direct form I)
//
// 'input_sample':  The new sample to be processed.
// 'filter_coeffs': Pointer to biquad coefficients array for all BiQuad sections.
//                  Arranged as [section 1: b0,b1,b2,a1,a2, ... section N: b0,b1,b2,a1,a2].
// 'state_data':    Pointer to filter state data array (initialized at startup to zero's).
//                  The length of the state data array is 'num_sections' * 4.
// 'num_sections':  Number of BiQuad sections.
// 'q_format':      Fixed point format, the number of bits making up fractional part.
//
// return value:    Resulting filter output sample.

int xmos_dsp_filters_biquads
(
    int       input_sample,
    const int filter_coeffs[],
    int       state_data[],
    int       num_sections,
    int       q_format
);

#endif
