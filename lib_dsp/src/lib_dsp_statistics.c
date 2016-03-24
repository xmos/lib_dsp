// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <platform.h>
#include "lib_dsp_qformat.h"
#include "lib_dsp_math.h"
#include "lib_dsp_statistics.h"

int lib_dsp_vector_abs_sum
(
    const int* input_vector_X,
    int        vector_length,
    int        q_format
) {
    int ah=0, x1, x0; unsigned al=0;
    
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
 *  int result;
 *  result = lib_dsp_vector_mean( input_vector, 256, 28 );
 *  \endcode
 * 
 *  \param  input_vector_X  Pointer to source data array X.
 *  \param  vector_length   Length of the input vector.
 *  \param  q_format        Fixed point format (i.e. number of fractional bits).
 */

int lib_dsp_vector_mean
(
    const int* input_vector_X,
    int        vector_length,
    int        q_format
) {
    int divide_by_N = lib_dsp_math_divide(1, vector_length, q_format );
    int vectort_sum = lib_dsp_vector_abs_sum( input_vector_X, vector_length, q_format );
    return lib_dsp_math_multiply( divide_by_N, vectort_sum, q_format );
}

/** Vector power (sum of squares): ``R = X[0]^2 + X[N-1]^2``
 * 
 *  This function computes the power (also know as the sum-of-squares) of the
 *  values contained within the input vector.
 * 
 *  Since each element in the vector is squared the behavior for fixed-point
 *  multiplication should be considered (see behavior for the function
 *  ``lib_dsp_math_multiply``). Due to successive 32-bit additions being
 *  accumulated using 64-bit arithmetic overflow during the summation process
 *  is unlikely. The final value, being effectively the result of a left-shift
 *  by ``q_format`` bits will potentially overflow the final fixed-point value
 *  depending on the resulting summed value and the chosen Q-format.
 * 
 *  Example:
 * 
 *  \code
 *  int result;
 *  result = lib_dsp_vector_power( input_vector, 256, 28 );
 *  \endcode
 * 
 *  \param  input_vector_X  Pointer to source data array X.
 *  \param  vector_length   Length of the input vector.
 *  \param  q_format        Fixed point format (i.e. number of fractional bits).
 */

int lib_dsp_vector_power
(
    const int* input_vector_X,
    int        vector_length,
    int        q_format
) {
    int ah=0, x1, x0; unsigned al=0;
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
 *  return lib_dsp_math_squareroot( result / vector_length )
 *  \endcode
 * 
 *  Since each element in the vector is squared the behavior for fixed-point
 *  multiplication should be considered (see behavior for the function
 *  ``lib_dsp_math_multiply``). Due to successive 32-bit additions being
 *  accumulated using 64-bit arithmetic overflow during the summation
 *  process is unlikely. The squareroot of the 'sum-of-squares divided by N
 *  uses the function ``lib_dsp_math_squareroot``; see behavior for that
 *  function. The final value, being effectively the result of a left-shift by
 *  ``q_format`` bits will potentially overflow the final fixed-point value
 *  depending on the resulting summed value and the chosen Q-format.
 * 
 *  Example:
 * 
 *  \code
 *  int result;
 *  result = lib_dsp_vector_rms( input_vector, 256, 28 );
 *  \endcode
 * 
 *  \param  input_vector_X  Pointer to source data array X.
 *  \param  vector_length   Length (N) of the input vector.
 *  \param  q_format        Fixed point format (i.e. number of fractional bits).
 */

int lib_dsp_vector_rms
(
    const int* input_vector_X,
    int        vector_length,
    int        q_format
) {
    int divide_by_N = lib_dsp_math_divide( 1, vector_length,    q_format ); // reciprocal
    int vectort_pwr = lib_dsp_vector_power   ( input_vector_X, vector_length, q_format );
    int mean_square = lib_dsp_math_multiply  ( divide_by_N, vectort_pwr,      q_format );
    int rt_mean_sqr = lib_dsp_math_squareroot( mean_square); //ou ,                   q_format );
    return rt_mean_sqr;
}

/** Vector dot product: ``R = X[0] * Y[0] + X[N-1] * Y[N-1]``
 * 
 *  This function computes the dot-product of two equal length vectors.
 * 
 *  The elements in the input vectors are multiplied before being summed
 *  therefore fixed-point multiplication behavior must be considered
 *  (see behavior for the function ``lib_dsp_math_multiply``). Due to
 *  successive 32-bit additions being accumulated using 64-bit arithmetic
 *  overflow during the summation process is unlikely. The final value, being
 *  effectively the result of a left-shift by ``q_format`` bits will
 *  potentially overflow the final fixed-point value depending on the resulting
 *  summed value and the chosen Q-format.
 * 
 *  Example:
 * 
 *  \code
 *  int result;
 *  result = lib_dsp_vector_dotprod( input_vector, 256, 28 );
 *  \endcode
 * 
 *  \param  input_vector_X  Pointer to source data array X.
 *  \param  input_vector_Y  Pointer to source data array Y.
 *  \param  vector_length   Length of the input vectors.
 *  \param  q_format        Fixed point format (i.e. number of fractional bits).
 */

int lib_dsp_vector_dotprod
(
    const int* input_vector_X,
    const int* input_vector_Y,
    int        vector_length,
    int        q_format
) {    
    int ah = 0, x1, x0, y1, y0;
    unsigned al = 1 << (q_format-1);
    
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
