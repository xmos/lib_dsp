// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <platform.h>
#include "lib_dsp_qformat.h"
#include "lib_dsp_vector.h"

/** Vector Minimum
 * 
 *  Locate the vector's first occurring minimum value, returning the index of
 *  the first occurring minimum value.
 * 
 *  Example:
 * 
 *  \code 
 *  int32_t samples[256];
 *  int32_t result = lib_dsp_vector_minimum( samples, 256 );
 *  \endcode 
 * 
 *  \param  input_vector   Pointer to source data array.
 *  \param  vector_length  Length of the output and input arrays.
 *  \returns               Array index where first minimum value occurs.
 */

int32_t lib_dsp_vector_minimum
(
    const int32_t* input_vector,
    int32_t        vector_length
) {
    int32_t x1, x0, result_location;
    int32_t search_location = 0;
    int32_t min_val = 2147483647;                    // Maximum 32 bit signed integer

    while( vector_length >= 4 )
    {
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector));
        if( x0 < min_val )
        {
          min_val = x0;
          result_location = search_location;
        }
        if( x1 < min_val )
        {
          min_val = x1;
          result_location = search_location + 1;
        }
        asm("ldd %0,%1,%2[1]":"=r"(x1),"=r"(x0):"r"(input_vector));
        if( x0 < min_val )
        {
          min_val = x0;
          result_location = search_location + 2;
        }
        if( x1 < min_val )
        {
          min_val = x1;
          result_location = search_location + 3;
        }
        vector_length -= 4; input_vector += 4; search_location += 4;
    }
    switch( vector_length )
    {
        case 3:
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector));
        if( x0 < min_val )
        {
          min_val = x0;
          result_location = search_location;
        }
        if( x1 < min_val )
        {
          min_val = x1;
          result_location = search_location + 1;
        }
        x0 = input_vector[2];
        if( x0 < min_val )
        {
          min_val = x0;
          result_location = search_location + 2;
        }
        break;
        case 2:
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector));
        if( x0 < min_val )
        {
          min_val = x0;
          result_location = search_location;
        }
        if( x1 < min_val )
        {
          min_val = x1;
          result_location = search_location + 1;
        }
        break;
        case 1:
        x0 = input_vector[0];
        if( x0 < min_val )
        {
          min_val = x0;
          result_location = search_location;
        }
        break;
    }
    return result_location;
}

/** Vector Minimum
 * 
 *  Locate the vector's first occurring maximum value, returning the index of
 *  the first occurring maximum value.
 * 
 *  Example:
 * 
 *  \code 
 *  int32_t samples[256];
 *  int32_t result = lib_dsp_vector_maximum( samples, 256 );
 *  \endcode 
 * 
 *  \param  input_vector   Pointer to source data array.
 *  \param  vector_length  Length of the output and input arrays.
 *  \returns               Array index where first maximum value occurs.
 */

int32_t lib_dsp_vector_maximum
(
    const int32_t* input_vector,
    int32_t        vector_length
) {
    int32_t x1, x0, result_location;
    int32_t search_location = 0;
    int32_t max_val = -2147483648;                    // Minimum 32 bit signed integer

    while( vector_length >= 4 )
    {
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector));
        if( x0 > max_val )
        {
          max_val = x0;
          result_location = search_location;
        }
        if( x1 > max_val )
        {
          max_val = x1;
          result_location = search_location + 1;
        }
        asm("ldd %0,%1,%2[1]":"=r"(x1),"=r"(x0):"r"(input_vector));
        if( x0 > max_val )
        {
          max_val = x0;
          result_location = search_location + 2;
        }
        if( x1 > max_val )
        {
          max_val = x1;
          result_location = search_location + 3;
        }
        vector_length -= 4; input_vector += 4; search_location += 4;
    }
    switch( vector_length )
    {
        case 3:
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector));
        if( x0 > max_val )
        {
          max_val = x0;
          result_location = search_location;
        }
        if( x1 > max_val )
        {
          max_val = x1;
          result_location = search_location + 1;
        }
        x0 = input_vector[2];
        if( x0 > max_val )
        {
          max_val = x0;
          result_location = search_location + 2;
        }
        break;
        case 2:
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector));
        if( x0 > max_val )
        {
          max_val = x0;
          result_location = search_location;
        }
        if( x1 > max_val )
        {
          max_val = x1;
          result_location = search_location + 1;
        }
        break;
        case 1:
        x0 = input_vector[0];
        if( x0 > max_val )
        {
          max_val = x0;
          result_location = search_location;
        }
        break;
    }
    return result_location;
}

/** Vector negation: ``R[i] = -X[i]``
 *
 *  This function computes the negative value for each input element and sets
 *  the corresponding result element to its negative value.
 *
 *  Each negated element is computed by twos-compliment negation therefore the
 *  minimum negative fixed-point value can not be negated to generate it's
 *  corresponding maximum positive fixed-point value.  For example: -Q28(-8.0)
 *  will not result in a fixed-point value representing +8.0.
 * 
 *  Example:
 *
 *  \code 
 *  int32_t samples[256];
 *  int32_t result[256];
 *  lib_dsp_vector_negate( samples, result, 256 );
 *  \endcode 
 * 
 *  \param  input_vector_X   Pointer/reference to source data.
 *  \param  result_vector_R  Pointer to the resulting data array.
 *  \param  vector_length    Length of the input and output vectors.
 */

void lib_dsp_vector_negate
(
    const int32_t* input_vector_X,
    int32_t*       result_vector_R,
    int32_t        vector_length
) {
    int32_t x1, x0;
    while( vector_length >= 4 )
    {
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        x0 = -x0; x1 = -x1;
        asm("std %0,%1,%2[0]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        asm("ldd %0,%1,%2[1]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        x0 = -x0; x1 = -x1;
        asm("std %0,%1,%2[1]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        vector_length -= 4; input_vector_X += 4; result_vector_R += 4;
    }
    switch( vector_length )
    {
        case 3:
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        x0 = -x0; x1 = -x1;
        asm("std %0,%1,%2[0]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        result_vector_R[2] = -input_vector_X[2];
        break;
        case 2:
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        x0 = -x0; x1 = -x1;
        asm("std %0,%1,%2[0]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        break;
        case 1:
        result_vector_R[0] = -input_vector_X[0];
        break;
    }
}

/** Vector absolute value: ``R[i] = |X[i]|``
 * 
 *  Set each element of the result vector to the absolute value of the
 *  corresponding input vector element.
 *
 *  Example:
 * 
 *  \code 
 *  int32_t samples[256];
 *  int32_t result[256];
 *  lib_dsp_vector_abs( samples, result, 256 );
 *  \endcode 
 * 
 *  If an element is less than zero it is negated to compute it's absolute
 *  value. Negation is computed via twos-compliment negation therefore the
 *  minimum negative fixed-point value can not be negated to generate its
 *  corresponding maximum positive fixed-point value. For example: -Q28(-8.0)
 *  will not result in a fixed-point value representing +8.0.
 * 
 *  \param  input_vector_X    Pointer/reference to source data.
 *  \param  result_vector_R   Pointer to the resulting data array.
 *  \param  vector_length     Length of the input and output vectors.
 */

void lib_dsp_vector_abs
(
    const int32_t* input_vector_X,
    int32_t*       result_vector_R,
    int32_t        vector_length
) {
    int32_t x1, x0;
    while( vector_length >= 4 )
    {
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        if( x0 < 0 ) x0 = -x0; if( x1 < 0 ) x1 = -x1;
        asm("std %0,%1,%2[0]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        asm("ldd %0,%1,%2[1]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        if( x0 < 0 ) x0 = -x0; if( x1 < 0 ) x1 = -x1;
        asm("std %0,%1,%2[1]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        vector_length -= 4; input_vector_X += 4; result_vector_R += 4;
    }
    switch( vector_length )
    {
        case 3:
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        if( x0 < 0 ) x0 = -x0;
        asm("std %0,%1,%2[0]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        result_vector_R[2] = -input_vector_X[2];
        break;
        case 2:
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        if( x0 < 0 ) x0 = -x0;
        asm("std %0,%1,%2[0]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        break;
        case 1:
        result_vector_R[0] = -input_vector_X[0];
        break;
    }
}

/** Vector / scalar addition: ``R[i] = X[i] + A``
 * 
 *  This function adds a scalar value to each vector element.
 *
 *  32-bit addition is used to compute the scaler plus vector element result.
 *  Therefore fixed-point value overflow conditions should be observed.  The
 *  resulting values are not saturated.
 * 
 *  Example:
 * 
 *  \code 
 *  int32_t input_vector_X[256];
 *  int32_t input_scalar_A = Q28( 0.333 );  
 *  int32_t result_vector_R[256];
 *  lib_dsp_vector_adds( input_vector_X, scalar_value_A, result_vector_R, 256 );
 *  \endcode 
 * 
 *  \param  input_vector_X   Pointer/reference to source data array X
 *  \param  input_scalar_A   Scalar value to add to each input element
 *  \param  result_vector_R  Pointer to the resulting data array
 *  \param  vector_length    Length of the input and output vectors
 */

void lib_dsp_vector_adds
(
    const int32_t* input_vector_X,
    int32_t        input_scalar_A,
    int32_t*       result_vector_R,
    int32_t        vector_length
) {
    int32_t x1, x0;
    while( vector_length >= 4 )
    {
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        x1 += input_scalar_A; x0 += input_scalar_A;
        asm("std %0,%1,%2[0]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        asm("ldd %0,%1,%2[1]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        x1 += input_scalar_A; x0 += input_scalar_A;
        asm("std %0,%1,%2[1]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        vector_length -= 4; input_vector_X += 4; result_vector_R += 4;
    }
    switch( vector_length )
    {
        case 3:
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        x1 += input_scalar_A; x0 += input_scalar_A;
        asm("std %0,%1,%2[0]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        result_vector_R[2] = input_vector_X[2] + input_scalar_A;
        break;
        case 2:
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        x1 += input_scalar_A; x0 += input_scalar_A;
        asm("std %0,%1,%2[0]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        break;
        case 1:
        result_vector_R[0] = input_vector_X[0] + input_scalar_A;
        break;
    }
}

/** Vector / scalar multiplication: ``R[i] = X[i] * A``
 *
 *  32-bit addition is used to compute the scaler plus vector element result.
 *  Therefore fixed-point value overflow conditions should be observed. The
 *  resulting values are not saturated.
 * 
 *  Example:
 *
 *  \code 
 *  int32_t input_vector_X[256];
 *  int32_t input_scalar_A = Q28( 0.333 );  
 *  int32_t result_vector_R[256];
 *  lib_dsp_vector_adds( input_vector_X, scalar_value_A, result_vector_R, 256 );
 *  \endcode 
 * 
 *  \param  input_vector_X    Pointer/reference to source data array X.
 *  \param  input_scalar_A    Scalar value to multiply each element by.
 *  \param  result_vector_R   Pointer to the resulting data array.
 *  \param  vector_length     Length of the input and output vectors.
 *  \param  q_format          Fixed point format, the number of bits making up fractional part.
 */

void lib_dsp_vector_muls
(
    const int32_t* input_vector_X,
    int32_t        input_scalar_A,
    int32_t*       result_vector_R,
    int32_t        vector_length,
    int32_t        q_format
) {
    int32_t ah, x1, x0; uint32_t al;
    
    while( vector_length >= 4 )
    {
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(input_scalar_A),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(input_scalar_A),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x1):"r"(ah),"r"(al),"r"(q_format));
        asm("std %0,%1,%2[0]"::"r"(x1), "r"(x0),"r"(result_vector_R));

        asm("ldd %0,%1,%2[1]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(input_scalar_A),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(input_scalar_A),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x1):"r"(ah),"r"(al),"r"(q_format));
        asm("std %0,%1,%2[1]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        
        vector_length -= 4; input_vector_X += 4; result_vector_R += 4;
    }
    switch( vector_length )
    {
        case 3:
        
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(input_scalar_A),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(input_scalar_A),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x1):"r"(ah),"r"(al),"r"(q_format));
        asm("std %0,%1,%2[0]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(input_vector_X[2]),"r"(input_scalar_A),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        result_vector_R[2] = x0;
        break;
        
        case 2:
        
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(input_scalar_A),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(input_scalar_A),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x1):"r"(ah),"r"(al),"r"(q_format));
        asm("std %0,%1,%2[0]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        break;
        
        case 1:
        
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(input_vector_X[0]),"r"(input_scalar_A),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32":"=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        result_vector_R[0] = x0;
        break;
    }
}

/** Vector / vector addition: ``R[i] = X[i] + Y[i]``
 * 
 *  32-bit addition is used to compute the scaler plus vector element result.
 *  Therefore fixed-point value overflow conditions should be observed. The
 *  resulting values are not saturated.
 *
 *  Example:
 *
 *  \code 
 *  int32_t input_vector_X[256];
 *  int32_t input_vector_Y[256];
 *  int32_t result_vector_R[256];  
 *  lib_dsp_vector_addv( input_vector_X, input_vector_Y, result_vector_R, 256 );
 *  \code 
 * 
 *  \param  input_vector_X    Pointer to source data array X.
 *  \param  input_vector_Y    Pointer to source data array Y.
 *  \param  result_vector_R   Pointer to the resulting data array.
 *  \param  vector_length     Length of the input and output vectors.
 */

void lib_dsp_vector_addv
(
    const int32_t* input_vector_X,
    const int32_t* input_vector_Y,
    int32_t*       result_vector_R,
    int32_t        vector_length
) {
    int32_t x1, x0, y1, y0;
    while( vector_length >= 8 )
    {
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("ldd %0,%1,%2[0]":"=r"(y1),"=r"(y0):"r"(input_vector_Y));
        x1 += y1; x0 += y0;
        asm("std %0,%1,%2[0]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        asm("ldd %0,%1,%2[1]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("ldd %0,%1,%2[1]":"=r"(y1),"=r"(y0):"r"(input_vector_Y));
        x1 += y1; x0 += y0;
        asm("std %0,%1,%2[1]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        asm("ldd %0,%1,%2[2]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("ldd %0,%1,%2[2]":"=r"(y1),"=r"(y0):"r"(input_vector_Y));
        x1 += y1; x0 += y0;
        asm("std %0,%1,%2[2]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        asm("ldd %0,%1,%2[3]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("ldd %0,%1,%2[3]":"=r"(y1),"=r"(y0):"r"(input_vector_Y));
        x1 += y1; x0 += y0;
        asm("std %0,%1,%2[3]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        vector_length -= 8;
        input_vector_X += 8; input_vector_Y += 8; result_vector_R += 8;
    }
    while( vector_length-- )
        *result_vector_R++ = *input_vector_X++ + *input_vector_Y++;
}

/** Vector / vector subtraction: ``R[i] = X[i] - Y[i]``
 * 
 *  32-bit subtraction is used to compute the scaler plus vector element result.
 *  Therefore fixed-point value overflow conditions should be observed. The
 *  resulting values are not saturated.
 * 
 *  Example:
 *
 *  \code 
 *  int32_t input_vector_X[256];
 *  int32_t input_vector_Y[256];
 *  int32_t result_vector_R[256];  
 *  lib_dsp_vector_subv( input_vector_X, input_vector_Y, result_vector_R, 256 );
 *  \endcode 
 * 
 *  \param  input_vector_X    Pointer to source data array X.
 *  \param  input_vector_Y    Pointer to source data array Y.
 *  \param  result_vector_R   Pointer to the resulting data array.
 *  \param  vector_length     Length of the input and output vectors.
 */

void lib_dsp_vector_subv
(
    const int32_t* input_vector_X,
    const int32_t* input_vector_Y,
    int32_t*       result_vector_R,
    int32_t        vector_length
) {
    int32_t x1, x0, y1, y0;
    while( vector_length >= 8 )
    {
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("ldd %0,%1,%2[0]":"=r"(y1),"=r"(y0):"r"(input_vector_Y));
        x1 -= y1; x0 -= y0;
        asm("std %0,%1,%2[0]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        asm("ldd %0,%1,%2[1]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("ldd %0,%1,%2[1]":"=r"(y1),"=r"(y0):"r"(input_vector_Y));
        x1 -= y1; x0 -= y0;
        asm("std %0,%1,%2[1]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        asm("ldd %0,%1,%2[2]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("ldd %0,%1,%2[2]":"=r"(y1),"=r"(y0):"r"(input_vector_Y));
        x1 -= y1; x0 -= y0;
        asm("std %0,%1,%2[2]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        asm("ldd %0,%1,%2[3]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("ldd %0,%1,%2[3]":"=r"(y1),"=r"(y0):"r"(input_vector_Y));
        x1 -= y1; x0 -= y0;
        asm("std %0,%1,%2[3]"::"r"(x1),"r"(x0),"r"(result_vector_R));
        vector_length -= 8;
        input_vector_X += 8; input_vector_Y += 8; result_vector_R += 8;
    }
    while( vector_length-- )
        *result_vector_R++ = *input_vector_X++ - *input_vector_Y++;
}

/** Vector / vector multiplication: ``R[i] = X[i] * Y[i]``
 * 
 *  Elements in each of the input vectors are multiplied together using a
 *  32-bit multiply 64-bit accumulate function therefore fixed-point
 *  multiplication and q-format adjustment overflow behavior must be
 *  considered (see behavior for the function ``lib_dsp_math_multiply``).
 * 
 *  Example:
 *
 *  \code 
 *  int32_t input_vector_X[256];
 *  int32_t input_vector_Y[256];
 *  int32_t result_vector_R[256];  
 *  lib_dsp_vector_mulv( input_vector_X, input_vector_Y, result_vector_R, 256, 28 );
 *  \endcode 
 * 
 *  \param  input_vector_X    Pointer to source data array X.
 *  \param  input_vector_Y    Pointer to source data array Y.
 *  \param  result_vector_R   Pointer to the resulting data array.
 *  \param  vector_length     Length of the input and output vectors.
 *  \param  q_format          Fixed point format (i.e. number of fractional bits).
 */

void lib_dsp_vector_mulv
(
    const int32_t* input_vector_X,
    const int32_t* input_vector_Y,
    int32_t*       result_vector_R,
    int32_t        vector_length,
    int32_t        q_format
) {
    int32_t ah, x1, x0, y1, y0; uint32_t al;
    
    // Q24 * Q24 = Q48 -> Q24  (24 = 24+24-24)
    // iiiiiiii,iiiiiiii,ffffffff,ffffffff,ffffffff,ffffffff,ffffffff,ffffffff
    //          iiiiiiii,ffffffff,ffffffff,ffffffff 24
    //
    // Q16 * Q20 = Q36 -> Q24  (12 = 16+20-24)
    // iiiiiiii,iiiiiiii,iiiiiiii,iiiiffff,ffffffff,ffffffff,ffffffff,ffffffff
    //                       iiiiiiii,ffffffff,ffffffff,ffffffff 12
    //
    // Q16 * Q24 = Q40 -> Q20  (20 = 16+24-20)
    // iiiiiiii,iiiiiiii,iiiiiiii,ffffffff,ffffffff,ffffffff,ffffffff,ffffffff
    //               iiiiiiii,iiiiffff,ffffffff,ffffffff
    //
    // Q20 * Q24 = Q44 -> Q16  (28 = 
    // iiiiiiii,iiiiiiii,iiiiffff,ffffffff,ffffffff,ffffffff,ffffffff,ffffffff
    //     iiiiiiii,iiiiiiii,ffffffff,ffffffff
    
    while( vector_length >= 8 )
    {
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("ldd %0,%1,%2[0]":"=r"(y1),"=r"(y0):"r"(input_vector_Y));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(y0),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(y1),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x1):"r"(ah),"r"(al),"r"(q_format));
        asm("std %0,%1,%2[0]"::"r"(x1), "r"(x0),"r"(result_vector_R));

        asm("ldd %0,%1,%2[1]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("ldd %0,%1,%2[1]":"=r"(y1),"=r"(y0):"r"(input_vector_Y));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(y0),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(y1),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32":"=r"(x1):"r"(ah),"r"(al),"r"(q_format));
        asm("std %0,%1,%2[1]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        
        asm("ldd %0,%1,%2[2]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("ldd %0,%1,%2[2]":"=r"(y1),"=r"(y0):"r"(input_vector_Y));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(y0),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(y1),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x1):"r"(ah),"r"(al),"r"(q_format));
        asm("std %0,%1,%2[2]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        
        asm("ldd %0,%1,%2[3]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("ldd %0,%1,%2[3]":"=r"(y1),"=r"(y0):"r"(input_vector_Y));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(y0),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(y1),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x1):"r"(ah),"r"(al),"r"(q_format));
        asm("std %0,%1,%2[3]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        
        vector_length -= 8;
        input_vector_X += 8; input_vector_Y += 8; result_vector_R += 8;
    }
    while( vector_length-- )
    {
        x0 = *input_vector_X++; y0 = *input_vector_Y++;
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(y0),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32":"=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        *result_vector_R++ = x0;
    }
}

/** Vector multiplication and scalar addition: ``R[i] = X[i] * Y[i] + A``
 * 
 *  Elements in each of the input vectors are multiplied together using a
 *  32-bit multiply 64-bit accumulate function therefore fixed-point
 *  multiplication and q-format adjustment overflow behavior must be
 *  considered (see behavior for the function ``lib_dsp_math_multiply``).
 *  32-bit addition is used to compute the vector element plus scalar value
 *  result. Therefore fixed-point value overflow conditions should be observed. 
 *  The resulting values are not saturated.
 * 
 *  Example:
 *
 *  \code 
 *  int32_t input_vector_X[256];
 *  int32_t input_vector_Y[256];
 *  int32_t input_scalar_A = Q28( 0.333 );
 *  int32_t result_vector_R[256];  
 *  lib_dsp_vector_mulv_adds( input_vector_X, input_vector_Y, scalar_value_A, result_vector_R, 256, 28 );
 *  \endcode 
 * 
 *  \param  input_vector_X    Pointer to source data array X.
 *  \param  input_vector_Y    Pointer to source data array Y.
 *  \param  input_scalar_A    Scalar value to add to each X*Y result.
 *  \param  result_vector_R   Pointer to the resulting data array.
 *  \param  vector_length     Length of the input and output vectors.
 *  \param  q_format          Fixed point format (i.e. number of fractional bits).
 */

void lib_dsp_vector_mulv_adds
(
    const int32_t* input_vector_X,
    const int32_t* input_vector_Y,
    int32_t        input_scalar_A,
    int32_t*       result_vector_R,
    int32_t        vector_length,
    int32_t        q_format
) {
    int32_t ah, x1, x0, y1, y0; uint32_t al;    
    while( vector_length >= 8 )
    {
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("ldd %0,%1,%2[0]":"=r"(y1),"=r"(y0):"r"(input_vector_Y));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(y0),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(y1),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x1):"r"(ah),"r"(al),"r"(q_format));
        x0 += input_scalar_A; x1 += input_scalar_A;
        asm("std %0,%1,%2[0]"::"r"(x1), "r"(x0),"r"(result_vector_R));

        asm("ldd %0,%1,%2[1]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("ldd %0,%1,%2[1]":"=r"(y1),"=r"(y0):"r"(input_vector_Y));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(y0),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(y1),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32":"=r"(x1):"r"(ah),"r"(al),"r"(q_format));
        x0 += input_scalar_A; x1 += input_scalar_A;
        asm("std %0,%1,%2[1]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        
        asm("ldd %0,%1,%2[2]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("ldd %0,%1,%2[2]":"=r"(y1),"=r"(y0):"r"(input_vector_Y));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(y0),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(y1),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x1):"r"(ah),"r"(al),"r"(q_format));
        x0 += input_scalar_A; x1 += input_scalar_A;
        asm("std %0,%1,%2[2]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        
        asm("ldd %0,%1,%2[3]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("ldd %0,%1,%2[3]":"=r"(y1),"=r"(y0):"r"(input_vector_Y));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(y0),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(y1),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x1):"r"(ah),"r"(al),"r"(q_format));
        x0 += input_scalar_A; x1 += input_scalar_A;
        asm("std %0,%1,%2[3]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        
        vector_length -= 8;
        input_vector_X += 8; input_vector_Y += 8; result_vector_R += 8;
    }
    while( vector_length-- )
    {
        x0 = *input_vector_X++; y0 = *input_vector_Y++;
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(y0),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32":"=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        *result_vector_R++ = x0 + input_scalar_A;
    }
}

/** Scalar multiplication and vector addition: ``R[i] = X[i] * A + Y[i]``
 * 
 *  Each element in the input vectors is multiplied by a scalar using a
 *  32bit multiply 64-bit accumulate function therefore fixed-point
 *  multiplication and q-format adjustment overflow behavior must be considered
 *  (see behavior for the function ``lib_dsp_math_multiply``). 32-bit addition is
 *  used to compute the vector element minus vector element result. Therefore
 *  fixed-point value overflow conditions should be observed.  The resulting
 *  values are not saturated.
 * 
 *  Example:
 *
 *  \code 
 *  int32_t input_vector_X[256];
 *  int32_t input_scalar_A = Q28( 0.333 );
 *  int32_t input_vector_Y[256];
 *  int32_t result_vector_R[256];
 *  lib_dsp_vector_muls_addv( input_vector_X, input_scalar_A, input_vector_Y, result_vector_R, 256, 28 );
 *  \endcode 
 * 
 *  \param  input_vector_X    Pointer to source data array X.
 *  \param  input_scalar_A    Scalar value to multiply each element by.
 *  \param  input_vector_Y    Pointer to source data array Y
 *  \param  result_vector_R   Pointer to the resulting data array.
 *  \param  vector_length     Length of the input and output vectors
 *  \param  q_format          Fixed point format (i.e. number of fractional bits).
 */

void lib_dsp_vector_muls_addv
(
    const int32_t* input_vector_X,
    int32_t        input_scalar_A,
    const int32_t* input_vector_Y,
    int32_t*       result_vector_R,
    int32_t        vector_length,
    int32_t        q_format
) {
    int32_t ah, x1, x0, y1, y0; uint32_t al;    
    while( vector_length >= 8 )
    {
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("ldd %0,%1,%2[0]":"=r"(y1),"=r"(y0):"r"(input_vector_Y));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(input_scalar_A),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(input_scalar_A),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x1):"r"(ah),"r"(al),"r"(q_format));
        x0 += y0; x1 += y1;
        asm("std %0,%1,%2[0]"::"r"(x1), "r"(x0),"r"(result_vector_R));

        asm("ldd %0,%1,%2[1]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("ldd %0,%1,%2[1]":"=r"(y1),"=r"(y0):"r"(input_vector_Y));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(input_scalar_A),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(input_scalar_A),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32":"=r"(x1):"r"(ah),"r"(al),"r"(q_format));
        x0 += y0; x1 += y1;
        asm("std %0,%1,%2[1]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        
        asm("ldd %0,%1,%2[2]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("ldd %0,%1,%2[2]":"=r"(y1),"=r"(y0):"r"(input_vector_Y));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(input_scalar_A),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(input_scalar_A),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x1):"r"(ah),"r"(al),"r"(q_format));
        x0 += y0; x1 += y1;
        asm("std %0,%1,%2[2]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        
        asm("ldd %0,%1,%2[3]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("ldd %0,%1,%2[3]":"=r"(y1),"=r"(y0):"r"(input_vector_Y));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(input_scalar_A),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x1):"r"(ah),"r"(al),"r"(q_format));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(input_scalar_A),"0"(0),"1"(1<<(q_format-1)));
        x0 += y0; x1 += y1;
        asm("std %0,%1,%2[3]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        
        vector_length -= 8;
        input_vector_X += 8; input_vector_Y += 8; result_vector_R += 8;
    }
    while( vector_length-- )
    {
        x0 = *input_vector_X++; y0 = *input_vector_Y++;
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(input_scalar_A),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32":"=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        *result_vector_R++ = x0 + y0;
    }
}

/** Scalar multiplication and vector subtraction: ``R[i] = X[i] * A - Y[i]``
 * 
 *  Each element in the input vectors is multiplied by a scalar using a
 *  32bit multiply 64-bit accumulate function therefore fixed-point
 *  multiplication and q-format adjustment overflow behavior must be considered
 *  (see behavior for the function ``lib_dsp_math_multiply``). 32-bit subtraction
 *  is used to compute the vector element minus vector element result. Therefore
 *  fixed-point value overflow conditions should be observed.  The resulting
 *  values are not saturated.
 * 
 *  Example:
 *
 *  \code 
 *  int32_t input_vector_X[256];
 *  int32_t input_scalar_A = Q28( 0.333 );
 *  int32_t input_vector_Y[256];
 *  int32_t result_vector_R[256];
 *  lib_dsp_vector_muls_subv( input_vector_X, input_scalar_A, input_vector_Y, result_vector_R, 256, 28 );
 *  \endcode 
 * 
 *  \param  input_scalar_A    Scalar value to multiply each element by.
 *  \param  input_vector_X    Pointer to source data array X.
 *  \param  input_vector_Y    Pointer to source data array Y.
 *  \param  result_vector_R   Pointer to the resulting data array.
 *  \param  vector_length     Length of the input and output vectors.
 *  \param  q_format          Fixed point format (i.e. number of fractional bits).
 */

void lib_dsp_vector_muls_subv
(
    const int32_t* input_vector_X,
    int32_t        input_scalar_A,
    const int32_t* input_vector_Y,
    int32_t*       result_vector_R,
    int32_t        vector_length,
    int32_t        q_format
) {
    int32_t ah, x1, x0, y1, y0; uint32_t al;    
    while( vector_length >= 8 )
    {
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("ldd %0,%1,%2[0]":"=r"(y1),"=r"(y0):"r"(input_vector_Y));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(input_scalar_A),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(input_scalar_A),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x1):"r"(ah),"r"(al),"r"(q_format));
        x0 -= y0; x1 -= y1;
        asm("std %0,%1,%2[0]"::"r"(x1), "r"(x0),"r"(result_vector_R));

        asm("ldd %0,%1,%2[1]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("ldd %0,%1,%2[1]":"=r"(y1),"=r"(y0):"r"(input_vector_Y));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(input_scalar_A),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(input_scalar_A),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32":"=r"(x1):"r"(ah),"r"(al),"r"(q_format));
        x0 -= y0; x1 -= y1;
        asm("std %0,%1,%2[1]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        
        asm("ldd %0,%1,%2[2]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("ldd %0,%1,%2[2]":"=r"(y1),"=r"(y0):"r"(input_vector_Y));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(input_scalar_A),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(input_scalar_A),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x1):"r"(ah),"r"(al),"r"(q_format));
        x0 -= y0; x1 -= y1;
        asm("std %0,%1,%2[2]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        
        asm("ldd %0,%1,%2[3]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("ldd %0,%1,%2[3]":"=r"(y1),"=r"(y0):"r"(input_vector_Y));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(input_scalar_A),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(input_scalar_A),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x1):"r"(ah),"r"(al),"r"(q_format));
        x0 -= y0; x1 -= y1;
        asm("std %0,%1,%2[3]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        
        vector_length -= 8;
        input_vector_X += 8; input_vector_Y += 8; result_vector_R += 8;
    }
    while( vector_length-- )
    {
        x0 = *input_vector_X++; y0 = *input_vector_Y++;
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(y0),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32":"=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        *result_vector_R++ = x0 - y0;
    }
}

/** Vector multiplication and vector addition: ``R[i] = X[i] * Y[i] + Z[i]``
 * 
 *  The elements in the input vectors are multiplied before being summed
 *  therefore fixed-point multiplication behavior must be considered (see
 *  behavior for the function ``lib_dsp_math_multiply``). Due to successive
 *  32-bit additions being accumulated using 64-bit arithmetic overflow during
 *  the summation process is unlikely. The final value, being effectively the
 *  result of a left-shift by ``q_format`` bits will potentially overflow the
 *  final fixed-point value depending on the resulting summed value and the
 *  chosen Q-format.
 * 
 *  Example:
 *
 *  \code 
 *  int32_t input_vector_X[256];
 *  int32_t input_vector_Y[256];
 *  int32_t input_vector_Z[256];
 *  int32_t result_vector_R[256];
 *  lib_dsp_vector_mulv_subv( input_vector_X, input_vector_Y, input_vector_Z, result_vector_R, 256 );
 *  \endcode 
 * 
 *  \param  input_vector_X    Pointer to source data array X.
 *  \param  input_vector_Y    Pointer to source data array Y.
 *  \param  input_vector_Z    Pointer to source data array Z.
 *  \param  result_vector_R   Pointer to the resulting data array.
 *  \param  vector_length     Length of the input and output vectors.
 *  \param  q_format          Fixed point format (i.e. number of fractional bits).
 */

void lib_dsp_vector_mulv_addv
(
    const int32_t* input_vector_X,
    const int32_t* input_vector_Y,
    const int32_t* input_vector_Z,
    int32_t*       result_vector_R,
    int32_t        vector_length,
    int32_t        q_format
) {
    int32_t ah, x1, x0, y1, y0, z1, z0; uint32_t al;    
    while( vector_length >= 8 )
    {
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("ldd %0,%1,%2[0]":"=r"(y1),"=r"(y0):"r"(input_vector_Y));
        asm("ldd %0,%1,%2[0]":"=r"(z1),"=r"(z0):"r"(input_vector_Z));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(y0),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(y1),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x1):"r"(ah),"r"(al),"r"(q_format));
        x0 += z0; x1 += z1;
        asm("std %0,%1,%2[0]"::"r"(x1), "r"(x0),"r"(result_vector_R));

        asm("ldd %0,%1,%2[1]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("ldd %0,%1,%2[1]":"=r"(y1),"=r"(y0):"r"(input_vector_Y));
        asm("ldd %0,%1,%2[0]":"=r"(z1),"=r"(z0):"r"(input_vector_Z));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(y0),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(y1),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32":"=r"(x1):"r"(ah),"r"(al),"r"(q_format));
        x0 += z0; x1 += z1;
        asm("std %0,%1,%2[1]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        
        asm("ldd %0,%1,%2[2]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("ldd %0,%1,%2[2]":"=r"(y1),"=r"(y0):"r"(input_vector_Y));
        asm("ldd %0,%1,%2[0]":"=r"(z1),"=r"(z0):"r"(input_vector_Z));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(y0),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(y1),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x1):"r"(ah),"r"(al),"r"(q_format));
        x0 += z0; x1 += z1;
        asm("std %0,%1,%2[2]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        
        asm("ldd %0,%1,%2[3]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("ldd %0,%1,%2[3]":"=r"(y1),"=r"(y0):"r"(input_vector_Y));
        asm("ldd %0,%1,%2[0]":"=r"(z1),"=r"(z0):"r"(input_vector_Z));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(y0),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(y1),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x1):"r"(ah),"r"(al),"r"(q_format));
        x0 += z0; x1 += z1;
        asm("std %0,%1,%2[3]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        
        vector_length -= 8; result_vector_R += 8;
        input_vector_X += 8; input_vector_Y += 8; input_vector_Z += 8;
    }
    while( vector_length-- )
    {
        x0 = *input_vector_X++; y0 = *input_vector_Y++; z0 = *input_vector_Z++;
        //{ah,al} = macs( x0, y0, 0, (1 << (q_format-1)) );
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32":"=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        *result_vector_R++ = x0 + z0;
    }
}

/** Vector multiplication and vector addition: ``R[i] = X[i] * Y[i] - Z[i]``
 * 
 *  The elements in the input vectors are multiplied before being subtracted
 *  therefore fixed-point multiplication behavior must be considered (see
 *  behavior for the function ``lib_dsp_math_multiply``). Due to successive
 *  32-bit subtractions being accumulated using 64-bit arithmetic overflow during
 *  the summation process is unlikely. The final value, being effectively the
 *  result of a left-shift by ``q_format`` bits will potentially overflow the
 *  final fixed-point value depending on the resulting summed value and the
 *  chosen Q-format.
 * 
 *  Example:
 *
 *  \code 
 *  int32_t input_vector_X[256];
 *  int32_t input_vector_Y[256];
 *  int32_t input_vector_Z[256];
 *  int32_t result_vector_R[256];
 *  lib_dsp_vector_mulv_subv( input_vector_X, input_vector_Y, input_vector_Z, result_vector_R, 256 );
 *  \endcode 
 * 
 *  \param  input_vector_X    Pointer to source data array X.
 *  \param  input_vector_Y    Pointer to source data array Y.
 *  \param  input_vector_Z    Pointer to source data array Z.
 *  \param  result_vector_R   Pointer to the resulting data array.
 *  \param  vector_length     Length of the input and output vectors.
 *  \param  q_format          Fixed point format (i.e. number of fractional bits).
 */

void lib_dsp_vector_mulv_subv
(
    const int32_t* input_vector_X,
    const int32_t* input_vector_Y,
    const int32_t* input_vector_Z,
    int32_t*       result_vector_R,
    int32_t        vector_length,
    int32_t        q_format
) {
    int32_t ah, x1, x0, y1, y0, z1, z0; uint32_t al;    
    while( vector_length >= 8 )
    {
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("ldd %0,%1,%2[0]":"=r"(y1),"=r"(y0):"r"(input_vector_Y));
        asm("ldd %0,%1,%2[0]":"=r"(z1),"=r"(z0):"r"(input_vector_Z));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(y0),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(y1),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x1):"r"(ah),"r"(al),"r"(q_format));
        x0 -= z0; x1 -= z1;
        asm("std %0,%1,%2[0]"::"r"(x1), "r"(x0),"r"(result_vector_R));

        asm("ldd %0,%1,%2[1]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("ldd %0,%1,%2[1]":"=r"(y1),"=r"(y0):"r"(input_vector_Y));
        asm("ldd %0,%1,%2[0]":"=r"(z1),"=r"(z0):"r"(input_vector_Z));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(y0),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(y1),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32":"=r"(x1):"r"(ah),"r"(al),"r"(q_format));
        x0 -= z0; x1 -= z1;
        asm("std %0,%1,%2[1]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        
        asm("ldd %0,%1,%2[2]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("ldd %0,%1,%2[2]":"=r"(y1),"=r"(y0):"r"(input_vector_Y));
        asm("ldd %0,%1,%2[0]":"=r"(z1),"=r"(z0):"r"(input_vector_Z));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(y0),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(y1),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x1):"r"(ah),"r"(al),"r"(q_format));
        x0 -= z0; x1 -= z1;
        asm("std %0,%1,%2[2]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        
        asm("ldd %0,%1,%2[3]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        asm("ldd %0,%1,%2[3]":"=r"(y1),"=r"(y0):"r"(input_vector_Y));
        asm("ldd %0,%1,%2[0]":"=r"(z1),"=r"(z0):"r"(input_vector_Z));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(y0),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(y1),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32": "=r"(x1):"r"(ah),"r"(al),"r"(q_format));
        x0 -= z0; x1 -= z1;
        asm("std %0,%1,%2[3]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        
        vector_length -= 8; result_vector_R += 8;
        input_vector_X += 8; input_vector_Y += 8; input_vector_Z += 8;
    }
    while( vector_length-- )
    {
        x0 = *input_vector_X++; y0 = *input_vector_Y++; z0 = *input_vector_Z++;
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(y0),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32":"=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        *result_vector_R++ = x0 - z0;
    }
}
