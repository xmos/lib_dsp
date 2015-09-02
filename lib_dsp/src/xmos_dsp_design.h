// ================================================================================================

#ifndef XMOS_DSP_DESIGN
#define XMOS_DSP_DESIGN

// ================================================================================================

// Biquad filter design - notch filter
//
// 'frequency':    Filter center frequency normalized to the sampling frequency.
//                 0 < frequency < 0.5, where 0.5 represents Fs/2.
// 'Q':            Filter Q factor.
// 'coefficients': Resulting fixed-point filter coefficients ordered as [b0,b1,b2,a1,a2].
// 'q_format':     Fixed point format, the number of bits making up fractional part.

void xmos_dsp_design_biquad_notch( double frequency, double Q, int coefficients[5], int q_format );

// Biquad filter design - low pass filter
//
// 'frequency':    Filter cutoff (-3db) frequency normalized to the sampling frequency.
//                 0 < frequency < 0.5, where 0.5 represents Fs/2.
// 'Q':            Filter Q factor.
// 'coefficients': Resulting fixed-point filter coefficients ordered as [b0,b1,b2,a1,a2].
// 'q_format':     Fixed point format, the number of bits making up fractional part.

void xmos_dsp_design_biquad_lowpass( double frequency, double Q, int coefficients[5], int q_format );

// Biquad filter design - high pass filter
//
// 'frequency':    Filter cutoff (-3db) frequency normalized to the sampling frequency.
//                 0 < frequency < 0.5, where 0.5 represents Fs/2.
// 'Q':            Filter Q factor.
// 'coefficients': Resulting fixed-point filter coefficients ordered as [b0,b1,b2,a1,a2].
// 'q_format':     Fixed point format, the number of bits making up fractional part.

void xmos_dsp_design_biquad_highpass( double frequency, double Q, int coefficients[5], int q_format );

// Biquad filter design - all pass filter
//
// 'frequency':    Filter center frequency normalized to the sampling frequency.
//                 0 < frequency < 0.5, where 0.5 represents Fs/2.
// 'Q':            Filter Q factor.
// 'coefficients': Resulting fixed-point filter coefficients ordered as [b0,b1,b2,a1,a2].
// 'q_format':     Fixed point format, the number of bits making up fractional part.

void xmos_dsp_design_biquad_allpass( double frequency, double Q, int coefficients[5], int q_format );

// Biquad filter design - band pass filter
//
// 'frequency1':   Filter cutoff #1 (-3db) frequency normalized to the sampling frequency.
//                 0 < frequency1 < 0.5, where 0.5 represents Fs/2.
// 'frequency2':   Filter cutoff #2 (-3db) frequency normalized to the sampling frequency.
//                 0 < frequency2 < 0.5, where 0.5 represents Fs/2.
// 'coefficients': Resulting fixed-point filter coefficients ordered as [b0,b1,b2,a1,a2].
// 'q_format':     Fixed point format, the number of bits making up fractional part.

void xmos_dsp_design_biquad_bandpass( double frequency1, double frequency2, int coefficients[5], int q_format );

// Biquad filter design - peaking filter
//
// 'frequency':    Filter center frequency normalized to the sampling frequency.
//                 0 < frequency < 0.5, where 0.5 represents Fs/2.
// 'Q':            Filter Q factor.
// 'qain_db'       The filter gain in dB (postive or negative).
//                 +gain results in peaking gain (max gain at peak center = 'gain_db').
//                 -gain results in peaking attenuation (min gain at peak center = 'gain_db').
// 'coefficients': Resulting fixed-point filter coefficients ordered as [b0,b1,b2,a1,a2].
// 'q_format':     Fixed point format, the number of bits making up fractional part.

void xmos_dsp_design_biquad_peaking( double frequency, double Q, double gain_db, int coefficients[5], int q_format );

// Biquad filter design - low side (bass) shelving filter
//
// 'frequency':    Filter frequency (+3db or -3db) normalized to the sampling frequency.
//                 0 < frequency < 0.5, where 0.5 represents Fs/2.
// 'Q':            Filter Q factor.
// 'qain_db'       The filter gain in dB (postive or negative).
//                 +gain results in bass shelf with max gain of 'gain_db'.
//                 -gain results in bass shelf with max attenuation of '-gain_db'.
// 'coefficients': Resulting fixed-point filter coefficients ordered as [b0,b1,b2,a1,a2].
// 'q_format':     Fixed point format, the number of bits making up fractional part.

void xmos_dsp_design_biquad_lowshelf( double frequency, double Q, double gain_db, int coefficients[5], int q_format );

// Biquad filter design - high side (treble) shelving filter
//
// 'frequency':    Filter frequency (+3db or -3db) normalized to the sampling frequency.
//                 0 < frequency < 0.5, where 0.5 represents Fs/2.
// 'Q':            Filter Q factor.
// 'qain_db'       The filter gain in dB (postive or negative).
//                 +gain results in treble shelf with max gain of 'gain_db'.
//                 -gain results in treble shelf with max attenuation of '-gain_db'.
// 'coefficients': Resulting fixed-point filter coefficients ordered as [b0,b1,b2,a1,a2].
// 'q_format':     Fixed point format, the number of bits making up fractional part.

void xmos_dsp_design_biquad_highshelf( double frequency, double Q, double gain_db, int coefficients[5], int q_format );

#endif
