#ifndef XMOS_DSP_STATISTICS
#define XMOS_DSP_STATISTICS

// Vector mean: result = (X[0] + ... X[N-1]) / N
//
// 'input_vector_X': Pointer to source data array X
// 'vector_length':  Length of the input vector
// 'q_format':       Fixed point format, the number of bits making up fractional part

int xmos_dsp_vector_mean
(
    const int input_vector_X[],
    int       vector_length,
    int       q_format
);

// Vector power: result = X[0]^2 + ... X[N-1]^2
//
// 'input_vector_X': Pointer to source data array X
// 'vector_length':  Length of the input vector
// 'q_format':       Fixed point format, the number of bits making up fractional part

int xmos_dsp_vector_power
(
    const int input_vector_X[],
    int       vector_length,
    int       q_format
);

// Vector root mean square: result = ((X[0]^2 + ... + X[N-1]^2) / N) ^ 0.5)
//
// 'input_vector_X': Pointer to source data array X
// 'vector_length':  Length (N) of the input vector
// 'q_format':       Fixed point format, the number of bits making up fractional part

int xmos_dsp_vector_rms
(
    const int input_vector_X[],
    int       vector_length,
    int       q_format
);

// Vector dot product: result = X[0] * Y[0] + ... X[N-1] * Y[N-1]
//
// 'input_vector_X': Pointer to source data array X
// 'input_vector_Y': Pointer to source data array Y
// 'vector_length':  Length of the input vectors
// 'q_format':       Fixed point format, the number of bits making up fractional part

int xmos_dsp_vector_dotprod
(
    const int input_vector_X[],
    const int input_vector_Y[],
    int       vector_length,
    int       q_format
);

#endif

