// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <platform.h>
#include "dsp_qformat.h"
#include "dsp_math.h"
#include "dsp_statistics.h"

int32_t dsp_vector_abs_sum
(
    const int32_t* input_vector_X,
    int32_t        vector_length,
    int32_t        q_format
) {
    int32_t ah=0, x1, x0; uint32_t al=0;
    
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
    asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
    asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
    return ah;
}

/** Vector mean: ``R = (X[0] + X[N-1]) / N``
 * 
 *  This function computes the mean of the values contained within
 *  the input vector.
 * 
 *  Due to successive 32-bit additions being accumulated using 64-bit
 *  arithmetic overflow during the summation process is unlikely. The final
 *  value, being effectively the result of a left-shift by ``q_format`` bits
 *  will potentially overflow the final fixed-point value depending on the
 *  resulting summed value and the chosen Q-format.
 * 
 *  Example:
 * 
 *  \code
 *  int32_t result;
 *  result = dsp_vector_mean( input_vector, 256, 28 );
 *  \endcode
 * 
 *  \param  input_vector_X  Pointer to source data array X.
 *  \param  vector_length   Length of the input vector.
 *  \param  q_format        Fixed point format (i.e. number of fractional bits).
 */

int32_t dsp_vector_mean
(
    const int32_t* input_vector_X,
    int32_t        vector_length,
    int32_t        q_format
) {
    int32_t divide_by_N = dsp_math_divide(1, vector_length, q_format );
    int32_t vectort_sum = dsp_vector_abs_sum( input_vector_X, vector_length, q_format );
    return dsp_math_multiply( divide_by_N, vectort_sum, q_format );
}

/** Vector power (sum of squares): ``R = X[0]^2 + X[N-1]^2``
 * 
 *  This function computes the power (also know as the sum-of-squares) of the
 *  values contained within the input vector.
 * 
 *  Since each element in the vector is squared the behavior for fixed-point
 *  multiplication should be considered (see behavior for the function
 *  ``dsp_math_multiply``). Due to successive 32-bit additions being
 *  accumulated using 64-bit arithmetic overflow during the summation process
 *  is unlikely. The final value, being effectively the result of a left-shift
 *  by ``q_format`` bits will potentially overflow the final fixed-point value
 *  depending on the resulting summed value and the chosen Q-format.
 * 
 *  Example:
 * 
 *  \code
 *  int32_t result;
 *  result = dsp_vector_power( input_vector, 256, 28 );
 *  \endcode
 * 
 *  \param  input_vector_X  Pointer to source data array X.
 *  \param  vector_length   Length of the input vector.
 *  \param  q_format        Fixed point format (i.e. number of fractional bits).
 */

int32_t dsp_vector_power
(
    const int32_t* input_vector_X,
    int32_t        vector_length,
    int32_t        q_format
) {
    int32_t ah=0, x1, x0; uint32_t al=0;
    uint32_t ii = -4, len = vector_length;
    
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
    asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
    asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
    return ah;
}

/** Vector root mean square: ``R = ((X[0]^2 + X[N-1]^2) / N) ^ 0.5)``
 * 
 *  This function computes the root-mean-square (RMS) of the values contained
 *  within the input vector.
 * 
 *  \code
 *  result = 0
 *  for i = 0 to N-1: result += input_vector_X[i]
 *  return dsp_math_sqrt( result / vector_length )
 *  \endcode
 * 
 *  Since each element in the vector is squared the behavior for fixed-point
 *  multiplication should be considered (see behavior for the function
 *  ``dsp_math_multiply``). Due to successive 32-bit additions being
 *  accumulated using 64-bit arithmetic overflow during the summation
 *  process is unlikely. The squareroot of the 'sum-of-squares divided by N
 *  uses the function ``dsp_math_sqrt``; see behavior for that
 *  function. The final value, being effectively the result of a left-shift by
 *  ``q_format`` bits will potentially overflow the final fixed-point value
 *  depending on the resulting summed value and the chosen Q-format.
 * 
 *  Example:
 * 
 *  \code
 *  int32_t result;
 *  result = dsp_vector_rms( input_vector, 256, 28 );
 *  \endcode
 * 
 *  \param  input_vector_X  Pointer to source data array X.
 *  \param  vector_length   Length (N) of the input vector.
 *  \param  q_format        Fixed point format (i.e. number of fractional bits).
 */

int32_t dsp_vector_rms
(
    const int32_t* input_vector_X,
    int32_t        vector_length,
    int32_t        q_format
) {
    int32_t divide_by_N = dsp_math_divide( 1, vector_length,    q_format ); // reciprocal
    int32_t vectort_pwr = dsp_vector_power   ( input_vector_X, vector_length, q_format );
    int32_t mean_square = dsp_math_multiply  ( divide_by_N, vectort_pwr,      q_format );
    int32_t rt_mean_sqr = dsp_math_sqrt( mean_square); //ou ,                   q_format );
    return rt_mean_sqr;
}

/** Vector dot product: ``R = X[0] * Y[0] + X[N-1] * Y[N-1]``
 * 
 *  This function computes the dot-product of two equal length vectors.
 * 
 *  The elements in the input vectors are multiplied before being summed
 *  therefore fixed-point multiplication behavior must be considered
 *  (see behavior for the function ``dsp_math_multiply``). Due to
 *  successive 32-bit additions being accumulated using 64-bit arithmetic
 *  overflow during the summation process is unlikely. The final value, being
 *  effectively the result of a left-shift by ``q_format`` bits will
 *  potentially overflow the final fixed-point value depending on the resulting
 *  summed value and the chosen Q-format.
 * 
 *  Example:
 * 
 *  \code
 *  int32_t result;
 *  result = dsp_vector_dotprod( input_vector, 256, 28 );
 *  \endcode
 * 
 *  \param  input_vector_X  Pointer to source data array X.
 *  \param  input_vector_Y  Pointer to source data array Y.
 *  \param  vector_length   Length of the input vectors.
 *  \param  q_format        Fixed point format (i.e. number of fractional bits).
 */

int32_t dsp_vector_dotprod
(
    const int32_t* input_vector_X,
    const int32_t* input_vector_Y,
    int32_t        vector_length,
    int32_t        q_format
) {    
    int32_t ah = 0, x1, x0, y1, y0;
    uint32_t al = 1 << (q_format-1);
    
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
    asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
    asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
    return ah;
}
