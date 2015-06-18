#include <platform.h>
#include "xmos_dsp_qformat.h"
#include "xmos_dsp_math.h"
#include "xmos_dsp_statistics.h"

// Vector sum: result = X[0] + ... X[N-1]
//
// 'input_vector_X': Pointer to source data array X
// 'vector_length':  Length of the input vector
// 'q_format':       Fixed point format, the number of bits making up fractional part

int xmos_dsp_vector_abs_sum
(
    const int* input_vector_X,
    int        vector_length,
    int        q_format
) {
    register int ah=0, x1, x0; register unsigned al=0;
    
    while( vector_length >= 4 )
    {
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        if( x0 < 0 ) x0 = -x0; if( x1 < 0 ) x1 = -x1;
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(1<<q_format),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(1<<q_format),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[1]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        if( x0 < 0 ) x0 = -x0; if( x1 < 0 ) x1 = -x1;
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(1<<q_format),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(1<<q_format),"0"(ah),"1"(al));
        vector_length -= 4; input_vector_X += 4;
    }
    switch( vector_length )
    {
        case 3:
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        if( x0 < 0 ) x0 = -x0; if( x1 < 0 ) x1 = -x1;
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(1<<q_format),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(1<<q_format),"0"(ah),"1"(al));
        x0 = input_vector_X[2]; if( x0 < 0 ) x0 = -x0;
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(1<<q_format),"0"(ah),"1"(al));
        break;
    
        case 2:
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        if( x0 < 0 ) x0 = -x0; if( x1 < 0 ) x1 = -x1;
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(1<<q_format),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(1<<q_format),"0"(ah),"1"(al));
        break;
    
        case 1:
        x0 = input_vector_X[0]; if( x0 < 0 ) x0 = -x0;
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(1<<q_format),"0"(ah),"1"(al));
        break;
    }
    asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
    return ah;
}

// Vector mean: result = (X[0] + ... X[N-1]) / N
//
// 'input_vector_X': Pointer to source data array X
// 'vector_length':  Length of the input vector
// 'q_format':       Fixed point format, the number of bits making up fractional part

int xmos_dsp_vector_mean
(
    const int* input_vector_X,
    int        vector_length,
    int        q_format
)
{
    int divide_by_N = xmos_dsp_math_reciprocal( vector_length << q_format, q_format );
    int vectort_sum = xmos_dsp_vector_abs_sum( input_vector_X, vector_length, q_format );
    return xmos_dsp_math_multiply( divide_by_N, vectort_sum, q_format );
}

// Vector power: result = X[0]^2 + ... X[N-1]^2
//
// 'input_vector_X': Pointer to source data array X
// 'vector_length':  Length of the input vector
// 'q_format':       Fixed point format, the number of bits making up fractional part

int xmos_dsp_vector_power
(
    const int* input_vector_X,
    int        vector_length,
    int        q_format
) {
    register int ah=0, x1, x0; register unsigned al=0;
    unsigned int ii = -4, len = vector_length;
    
    while( len > 0 ) {++ii; len /= 2;}
    while( vector_length >= 4 )
    {
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(x0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(x1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[1]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(x0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(x1),"0"(ah),"1"(al));
        vector_length -= 4; input_vector_X += 4;
    }
    switch( vector_length )
    {
        case 3:
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(x0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(x1),"0"(ah),"1"(al));
        x0 = input_vector_X[2];
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(x0),"0"(ah),"1"(al));
        break;
    
        case 2:
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(x0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(x1),"0"(ah),"1"(al));
        break;
    
        case 1:
        x0 = input_vector_X[0];
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(x0),"0"(ah),"1"(al));
        break;
    }
    asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
    return ah;
}

// Vector root mean square: result = ((X[0]^2 + ... X[N-1]^2) / N) ^ 0.5)
//
// 'input_vector_X': Pointer to source data array X
// 'vector_length':  Length (N) of the input vector
// 'q_format':       Fixed point format, the number of bits making up fractional part

int xmos_dsp_vector_rms
(
    const int* input_vector_X,
    int        vector_length,
    int        q_format
)
{
    int divide_by_N = xmos_dsp_math_reciprocal( vector_length << q_format,     q_format );
    int vectort_pwr = xmos_dsp_vector_power   ( input_vector_X, vector_length, q_format );
    int mean_square = xmos_dsp_math_multiply  ( divide_by_N, vectort_pwr,      q_format );
    int rt_mean_sqr = xmos_dsp_math_squareroot( mean_square,                   q_format );
    return rt_mean_sqr;
}

// Vector dot product: return = X[0] * Y[0] + ... X[N-1] * Y[N-1]
//
// 'input_vector_X': Pointer to source data array X
// 'input_vector_Y': Pointer to source data array Y
// 'vector_length':  Length of the input vectors
// 'q_format':       Fixed point format, the number of bits making up fractional part

int xmos_dsp_vector_dotprod
(
    const int* input_vector_X,
    const int* input_vector_Y,
    int        vector_length,
    int        q_format
) {    
    register int ah = 0, x1, x0, y1, y0;
    register unsigned al = 1 << (q_format-1);
    
    while( vector_length >= 4 )
    {
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("ldd %0,%1,%2[0]":"=r"(y1),"=r"(y0):"r"(input_vector_Y));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(y0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(y1),"0"(ah),"1"(al));        
        asm("ldd %0,%1,%2[1]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("ldd %0,%1,%2[1]":"=r"(y1),"=r"(y0):"r"(input_vector_Y));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(y0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(y1),"0"(ah),"1"(al));
        vector_length -= 4; input_vector_X += 4; input_vector_Y += 4;
    }
    switch( vector_length )
    {
        case 3:
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("ldd %0,%1,%2[0]":"=r"(y1),"=r"(y0):"r"(input_vector_Y));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(y0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(y1),"0"(ah),"1"(al));
        x0 = input_vector_X[2]; y0 = input_vector_Y[2];
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(y0),"0"(ah),"1"(al));
        break;
        case 2:
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("ldd %0,%1,%2[0]":"=r"(y1),"=r"(y0):"r"(input_vector_Y));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(y0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(y1),"0"(ah),"1"(al));
        break;
        case 1:
        x0 = input_vector_X[0]; y0 = input_vector_Y[0];
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(y0),"0"(ah),"1"(al));
        break;

    }
    asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
	return ah;
}

//MEAN    0.400000
//POWER   0.640000
//RMS     0.400000
//DOTPROD 0.320000

//int __TODO__xmos_dsp_vector_correlation;
//int __TODO__xmos_dsp_vector_coherence;
//int __TODO__xmos_dsp_vector_autocorrelate;
//int __TODO__xmos_dsp_vector_autocoherence;
