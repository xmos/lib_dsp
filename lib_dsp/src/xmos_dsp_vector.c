// ================================================================================================

#include <platform.h>
#include "xmos_dsp_qformat.h"
#include "xmos_dsp_vector.h"

// ================================================================================================

// Locate the vector's first occurring minimum value
//
// 'input_vector':  Pointer to source data array.
// 'vector_length': Length of the output and input arrays.
//
// return value:     Array index where first minimum value occurs.

int xmos_dsp_vector_minimum
(
    const int* input_vector,
    int        vector_length
) {
    int x1, x0, result_location;
    int search_location = 0;
    int min_val = 2147483647;                    // Maximum 32 bit signed integer

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

// ================================================================================================

// Locate the vector's first occurring maximum value
//
// 'input_vector':  Pointer to source data array.
// 'vector_length': Length of the output and input arrays.
//
// return value:     Array index where first maximum value occurs.

int xmos_dsp_vector_maximum
(
    const int* input_vector,
    int        vector_length
) {
    int x1, x0, result_location;
    int search_location = 0;
    int max_val = -2147483648;                    // Minimum 32 bit signed integer

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

// ================================================================================================

// Vector negation: R = -X
//
// 'input_vector_X':  Pointer/reference to source data.
// 'result_vector_R': Pointer to the resulting data array.
// 'vector_length':   Length of the input and output vectors.

void xmos_dsp_vector_negate
(
    const int* input_vector_X,
    int*       result_vector_R,
    int        vector_length
) {
    int x1, x0;
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

// ================================================================================================

// Vector absolute value: R = |X|
//
// 'input_vector_X':  Pointer/reference to source data.
// 'result_vector_R': Pointer to the resulting data array.
// 'vector_length':   Length of the input and output vectors.

void xmos_dsp_vector_abs
(
    const int* input_vector_X,
    int*       result_vector_R,
    int        vector_length
) {
    int x1, x0;
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

// ================================================================================================

// Vector / scalar addition: R = X + a
//
// 'input_vector_X':  Pointer/reference to source data.
// 'input_scalar_A':  Scalar value to add to each 'input' element.
// 'result_vector_R': Pointer to the resulting data array.
// 'vector_length':   Length of the input and output vectors.

void xmos_dsp_vector_adds
(
    const int* input_vector_X,
    int        input_scalar_A,
    int*       result_vector_R,
    int        vector_length
) {
    int x1, x0;
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

// ================================================================================================

// Vector / scalar multiplication: R = X * a
//
// 'input_vector_X':  Pointer/reference to source data X.
// 'input_scalar_A':  Scalar value to multiply each 'input' element by.
// 'result_vector_R': Pointer to the resulting data array.
// 'vector_length':   Length of the input and output vectors.
// 'q_format':        Fixed point format, the number of bits making up fractional part.

void xmos_dsp_vector_muls
(
    const int* input_vector_X,
    int        input_scalar_A,
    int*       result_vector_R,
    int        vector_length,
    int        q_format
) {
    int ah, x1, x0; unsigned al;
    
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

// ================================================================================================

// Vector / vector addition: R = X + Y
//
// 'input_vector_X':  Pointer to source data array X.
// 'input_vector_Y':  Pointer to source data array Y.
// 'result_vector_R': Pointer to the resulting data array.
// 'vector_length':   Length of the input and output vectors.

void xmos_dsp_vector_addv
(
    const int* input_vector_X,
    const int* input_vector_Y,
    int*       result_vector_R,
    int        vector_length
) {
    int x1, x0, y1, y0;
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

// ================================================================================================

// Vector / vector subtraction: R = X - Y
//
// 'input_vector_X':  Pointer to source data array X
// 'input_vector_Y':  Pointer to source data array Y
// 'result_vector_R': Pointer to the resulting data array
// 'vector_length':   Length of the input and output vectors

void xmos_dsp_vector_subv
(
    const int* input_vector_X,
    const int* input_vector_Y,
    int*       result_vector_R,
    int        vector_length
) {
    int x1, x0, y1, y0;
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

// ================================================================================================

// Vector / vector multiplication: R = X * Y
//
// 'input_vector_X':  Pointer to source data array X.
// 'input_vector_Y':  Pointer to source data array Y.
// 'result_vector_R': Pointer to the resulting data array.
// 'vector_length':   Length of the input and output vectors.
// 'q_format':        Fixed point format, the number of bits making up fractional part.

void xmos_dsp_vector_mulv
(
    const int* input_vector_X,
    const int* input_vector_Y,
    int*       result_vector_R,
    int        vector_length,
    int        q_format
) {
    int ah, x1, x0, y1, y0; unsigned al;
    
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

// ================================================================================================

// Vector multiplication and scalar addition: R = X * Y + a
//
// 'input_vector_X':  Pointer to source data array X.
// 'input_vector_Y':  Pointer to source data array Y.
// 'input_scalar_A':  Scalar value to add to each X*Y result.
// 'result_vector_R': Pointer to the resulting data array.
// 'vector_length':   Length of the input and output vectors.
// 'q_format':        Fixed point format, the number of bits making up fractional part.

void xmos_dsp_vector_mulv_adds
(
    const int* input_vector_X,
    const int* input_vector_Y,
    int        input_scalar_A,
    int*       result_vector_R,
    int        vector_length,
    int        q_format
) {
    int ah, x1, x0, y1, y0; unsigned al;    
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

// ================================================================================================

// Scalar multiplication and vector addition: R = a * X + Y
//
// 'input_vector_X':  Pointer to source data array X.
// 'input_scalar_A':  Scalar value to multiply each 'input' element by.
// 'input_vector_Y':  Pointer to source data array Y.
// 'result_vector_R': Pointer to the resulting data array.
// 'vector_length':   Length of the input and output vectors.
// 'q_format':        Fixed point format, the number of bits making up fractional part.

void xmos_dsp_vector_muls_addv
(
    const int* input_vector_X,
    int        input_scalar_A,
    const int* input_vector_Y,
    int*       result_vector_R,
    int        vector_length,
    int        q_format
) {
    int ah, x1, x0, y1, y0; unsigned al;    
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

// ================================================================================================

// Scalar multiplication and vector subtraction: R = a * X - Y
//
// 'input_vector_X':  Pointer to source data array X.
// 'input_scalar_A':  Scalar value to multiply each 'input' element by.
// 'input_vector_Y':  Pointer to source data array Y.
// 'result_vector_R': Pointer to the resulting data array.
// 'vector_length':   Length of the input and output vectors.
// 'q_format':        Fixed point format, the number of bits making up fractional part.

void xmos_dsp_vector_muls_subv
(
    const int* input_vector_X,
    int        input_scalar_A,
    const int* input_vector_Y,
    int*       result_vector_R,
    int        vector_length,
    int        q_format
) {
    int ah, x1, x0, y1, y0; unsigned al;    
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

// ================================================================================================

// Vector multiplication and vector addition: R = X * Y + Z
//
// 'input_vector_X':  Pointer to source data array X.
// 'input_vector_Y':  Pointer to source data array Y.
// 'input_vector_Z':  Pointer to source data array Z.
// 'result_vector_R': Pointer to the resulting data array.
// 'vector_length':   Length of the input and output vectors.
// 'q_format':        Fixed point format, the number of bits making up fractional part.

void xmos_dsp_vector_mulv_addv
(
    const int* input_vector_X,
    const int* input_vector_Y,
    const int* input_vector_Z,
    int*       result_vector_R,
    int        vector_length,
    int        q_format
) {
    int ah, x1, x0, y1, y0, z1, z0; unsigned al;    
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

// ================================================================================================

// Vector multiplication and vector addition: R = X * Y - Z
//
// 'input_vector_X':  Pointer to source data array X.
// 'input_vector_Y':  Pointer to source data array Y.
// 'input_vector_Z':  Pointer to source data array Z.
// 'result_vector_R': Pointer to the resulting data array.
// 'q_format':        Fixed point format, the number of bits making up fractional part.

void xmos_dsp_vector_mulv_subv
(
    const int* input_vector_X,
    const int* input_vector_Y,
    const int* input_vector_Z,
    int*       result_vector_R,
    int        vector_length,
    int        q_format
) {
    int ah, x1, x0, y1, y0, z1, z0; unsigned al;    
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

// ================================================================================================

