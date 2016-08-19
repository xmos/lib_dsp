// Copyright (c) 2015-2016, XMOS Ltd, All rights reserved

#include <platform.h>
#include "dsp_qformat.h"
#include "dsp_vector.h"


int32_t dsp_vector_minimum
(
    const int32_t* input_vector,
    const int32_t  vector_length
) {
    int32_t x1, x0, result_location;
    int32_t search_location = 0;
    int32_t min_val = 2147483647;                    // Maximum 32 bit signed integer

    int32_t vl = vector_length;  
    while( vl >= 4 )
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
        vl -= 4; input_vector += 4; search_location += 4;
    }
    switch( vl )
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



int32_t dsp_vector_maximum
(
    const int32_t* input_vector,
    const int32_t  vector_length
) {
    int32_t x1, x0, result_location;
    int32_t search_location = 0;
    int32_t max_val = -2147483648;                    // Minimum 32 bit signed integer

    int32_t vl = vector_length;  
    while( vl >= 4 )
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
        vl -= 4; input_vector += 4; search_location += 4;
    }
    switch( vl )
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



void dsp_vector_negate
(
    const int32_t* input_vector_X,
    int32_t*       result_vector_R,
    const int32_t  vector_length
) {
    int32_t x1, x0;
    int32_t vl = vector_length;  
    while( vl >= 4 )
    {
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        x0 = -x0; x1 = -x1;
        asm("std %0,%1,%2[0]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        asm("ldd %0,%1,%2[1]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        x0 = -x0; x1 = -x1;
        asm("std %0,%1,%2[1]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        vl -= 4; input_vector_X += 4; result_vector_R += 4;
    }
    switch( vl )
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



void dsp_vector_abs
(
    const int32_t* input_vector_X,
    int32_t*       result_vector_R,
    const int32_t  vector_length
) {
    int32_t x1, x0;
    int32_t vl = vector_length;  
    while( vl >= 4 )
    {
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        if( x0 < 0 ) x0 = -x0; if( x1 < 0 ) x1 = -x1;
        asm("std %0,%1,%2[0]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        asm("ldd %0,%1,%2[1]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        if( x0 < 0 ) x0 = -x0; if( x1 < 0 ) x1 = -x1;
        asm("std %0,%1,%2[1]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        vl -= 4; input_vector_X += 4; result_vector_R += 4;
    }
    switch( vl )
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



void dsp_vector_adds
(
    const int32_t* input_vector_X,
    int32_t        input_scalar_A,
    int32_t*       result_vector_R,
    const int32_t  vector_length
) {
    int32_t x1, x0;
    int32_t vl = vector_length;  
    while( vl >= 4 )
    {
        asm("ldd %0,%1,%2[0]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        x1 += input_scalar_A; x0 += input_scalar_A;
        asm("std %0,%1,%2[0]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        asm("ldd %0,%1,%2[1]":"=r"(x1),"=r"(x0):"r"(input_vector_X));
        x1 += input_scalar_A; x0 += input_scalar_A;
        asm("std %0,%1,%2[1]"::"r"(x1), "r"(x0),"r"(result_vector_R));
        vl -= 4; input_vector_X += 4; result_vector_R += 4;
    }
    switch( vl )
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



void dsp_vector_muls
(
    const int32_t* input_vector_X,
    int32_t        input_scalar_A,
    int32_t*       result_vector_R,
    const int32_t  vector_length,
    const int32_t  q_format
) {
    int32_t ah, x1, x0; uint32_t al;
    
    int32_t vl = vector_length;  
    while( vl >= 4 )
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
        
        vl -= 4; input_vector_X += 4; result_vector_R += 4;
    }
    switch( vl )
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



void dsp_vector_addv
(
    const int32_t* input_vector_X,
    const int32_t* input_vector_Y,
    int32_t*       result_vector_R,
    const int32_t  vector_length
) {
    int32_t x1, x0, y1, y0;

    int32_t vl = vector_length;  
    while( vl >= 8 )
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
        vl -= 8;
        input_vector_X += 8; input_vector_Y += 8; result_vector_R += 8;
    }
    while( vl-- )
        *result_vector_R++ = *input_vector_X++ + *input_vector_Y++;
}



void dsp_vector_subv
(
    const int32_t* input_vector_X,
    const int32_t* input_vector_Y,
    int32_t*       result_vector_R,
    const int32_t  vector_length
) {
    int32_t x1, x0, y1, y0;
    int32_t vl = vector_length;  
    while( vl >= 8 )
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
        vl -= 8;
        input_vector_X += 8; input_vector_Y += 8; result_vector_R += 8;
    }
    while( vl-- )
        *result_vector_R++ = *input_vector_X++ - *input_vector_Y++;
}



void dsp_vector_mulv
(
    const int32_t* input_vector_X,
    const int32_t* input_vector_Y,
    int32_t*       result_vector_R,
    const int32_t  vector_length,
    const int32_t  q_format
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
    int32_t vl = vector_length;      
    while( vl >= 8 )
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
        
        vl -= 8;
        input_vector_X += 8; input_vector_Y += 8; result_vector_R += 8;
    }
    while( vl-- )
    {
        x0 = *input_vector_X++; y0 = *input_vector_Y++;
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(y0),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32":"=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        *result_vector_R++ = x0;
    }
}



void dsp_vector_mulv_adds
(
    const int32_t* input_vector_X,
    const int32_t* input_vector_Y,
    int32_t        input_scalar_A,
    int32_t*       result_vector_R,
    const int32_t  vector_length,
    const int32_t  q_format
) {
    int32_t ah, x1, x0, y1, y0; uint32_t al;  
    int32_t vl = vector_length;    
    while( vl >= 8 )
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
        
        vl -= 8;
        input_vector_X += 8; input_vector_Y += 8; result_vector_R += 8;
    }
    while( vl-- )
    {
        x0 = *input_vector_X++; y0 = *input_vector_Y++;
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(y0),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32":"=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        *result_vector_R++ = x0 + input_scalar_A;
    }
}



void dsp_vector_muls_addv
(
    const int32_t* input_vector_X,
    int32_t        input_scalar_A,
    const int32_t* input_vector_Y,
    int32_t*       result_vector_R,
    const int32_t  vector_length,
    const int32_t  q_format
) {
    int32_t ah, x1, x0, y1, y0; uint32_t al;  
    int32_t vl = vector_length;        
    while( vl >= 8 )
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
        
        vl -= 8;
        input_vector_X += 8; input_vector_Y += 8; result_vector_R += 8;
    }
    while( vl-- )
    {
        x0 = *input_vector_X++; y0 = *input_vector_Y++;
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(input_scalar_A),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32":"=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        *result_vector_R++ = x0 + y0;
    }
}



void dsp_vector_muls_subv
(
    const int32_t* input_vector_X,
    int32_t        input_scalar_A,
    const int32_t* input_vector_Y,
    int32_t*       result_vector_R,
    const int32_t  vector_length,
    const int32_t  q_format
) {
    int32_t ah, x1, x0, y1, y0; uint32_t al;    
    int32_t vl = vector_length;  
    while( vl >= 8 )
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
        
        vl -= 8;
        input_vector_X += 8; input_vector_Y += 8; result_vector_R += 8;
    }
    while( vl-- )
    {
        x0 = *input_vector_X++; y0 = *input_vector_Y++;
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(y0),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32":"=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        *result_vector_R++ = x0 - y0;
    }
}



void dsp_vector_mulv_addv
(
    const int32_t* input_vector_X,
    const int32_t* input_vector_Y,
    const int32_t* input_vector_Z,
    int32_t*       result_vector_R,
    const int32_t  vector_length,
    const int32_t  q_format
) {
    int32_t ah, x1, x0, y1, y0, z1, z0; uint32_t al;    

    int32_t vl = vector_length;
    while( vl >= 8 )
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
        
        vl -= 8; result_vector_R += 8;
        input_vector_X += 8; input_vector_Y += 8; input_vector_Z += 8;
    }
    while( vl-- )
    {
        x0 = *input_vector_X++; y0 = *input_vector_Y++; z0 = *input_vector_Z++;
        //{ah,al} = macs( x0, y0, 0, (1 << (q_format-1)) );
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32":"=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        *result_vector_R++ = x0 + z0;
    }
}



void dsp_vector_mulv_subv
(
    const int32_t* input_vector_X,
    const int32_t* input_vector_Y,
    const int32_t* input_vector_Z,
    int32_t*       result_vector_R,
    const int32_t  vector_length,
    const int32_t  q_format
) {
    int32_t ah, x1, x0, y1, y0, z1, z0; uint32_t al;  

    int32_t vl = vector_length;  
    while( vl >= 8 )
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
        
        vl -= 8; result_vector_R += 8;
        input_vector_X += 8; input_vector_Y += 8; input_vector_Z += 8;
    }
    while( vl-- )
    {
        x0 = *input_vector_X++; y0 = *input_vector_Y++; z0 = *input_vector_Z++;
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(y0),"0"(0),"1"(1<<(q_format-1)));
        asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
        asm("lextract %0,%1,%2,%3,32":"=r"(x0):"r"(ah),"r"(al),"r"(q_format));
        *result_vector_R++ = x0 - z0;
    }
}


void dsp_vector_mulv_complex
(
    const int32_t* input_vector_X_re,
    const int32_t* input_vector_X_im,
    const int32_t* input_vector_Y_re,
    const int32_t* input_vector_Y_im,
    int32_t*       result_vector_R_re,
    int32_t*       result_vector_R_im,
    const int32_t  vector_length,
    const int32_t  q_format
) {
    int32_t   x0_re0, x0_im0, y0_re0, y0_im0, z0_00, z0_10;
    int32_t   x0_re1, x0_im1, y0_re1, y0_im1, z0_01, z0_11;
    int32_t   ah;
    uint32_t  al;
    int32_t   zero = 0;

    int32_t   vl = vector_length;

    while( vl >= 8 )
    {
// First stage

        asm ("ldd %0, %1, %2[0]" : "=r"(x0_re1), "=r"(x0_re0) : "r"(input_vector_X_re));
        asm ("ldd %0, %1, %2[0]" : "=r"(x0_im1), "=r"(x0_im0) : "r"(input_vector_X_im));

        asm ("ldd %0, %1, %2[0]" : "=r"(y0_re1), "=r"(y0_re0) : "r"(input_vector_Y_re));
        asm ("ldd %0, %1, %2[0]" : "=r"(y0_im1), "=r"(y0_im0) : "r"(input_vector_Y_im));

        asm ("lmul %0, %1, %2, %3, %4, %5" : "=r" (ah), "=r" (al) : "r"(x0_re0), "r"(y0_re0), "r"(zero), "r"(zero));
        asm ("lextract %0, %1, %2, %3, 32" : "=r"(z0_00) : "r"(ah), "r"(al), "r"(q_format));

        asm ("lmul %0, %1, %2, %3, %4, %5" : "=r" (ah), "=r" (al) : "r"(x0_im0), "r"(y0_im0), "r"(zero), "r"(zero));
        asm ("lextract %0, %1, %2, %3, 32" : "=r"(z0_10) : "r"(ah), "r"(al), "r"(q_format));
        asm ("sub %0, %1, %2" : "=r" (z0_00) : "r"(z0_00), "r"(z0_10));

        asm ("lmul %0, %1, %2, %3, %4, %5" : "=r" (ah), "=r" (al) : "r"(x0_re0), "r"(y0_im0), "r"(zero), "r"(zero));
        asm ("maccs %0, %1, %2, %3" : "=r"(ah), "=r"(al) : "r"(x0_im0), "r"(y0_re0), "0"(ah), "1"(al));

        asm ("lsats %0, %1, %2" : "=r"(ah), "=r"(al) : "r"(q_format), "0"(ah), "1"(al));
        asm ("lextract %0, %1, %2, %3, 32" : "=r"(z0_10) : "r"(ah), "r"(al), "r"(q_format));


        asm ("lmul %0, %1, %2, %3, %4, %5" : "=r" (ah), "=r" (al) : "r"(x0_re1), "r"(y0_re1), "r"(zero), "r"(zero));
        asm ("lextract %0, %1, %2, %3, 32" : "=r"(z0_01) : "r"(ah), "r"(al), "r"(q_format));

        asm ("lmul %0, %1, %2, %3, %4, %5" : "=r" (ah), "=r" (al) : "r"(x0_im1), "r"(y0_im1), "r"(zero), "r"(zero));
        asm ("lextract %0, %1, %2, %3, 32" : "=r"(z0_11) : "r"(ah), "r"(al), "r"(q_format));
        asm ("sub %0, %1, %2" : "=r" (z0_01) : "r"(z0_01), "r"(z0_11));

        asm ("lmul %0, %1, %2, %3, %4, %5" : "=r" (ah), "=r" (al) : "r"(x0_re1), "r"(y0_im1), "r"(zero), "r"(zero));
        asm ("maccs %0, %1, %2, %3" : "=r"(ah), "=r"(al) : "r"(x0_im1), "r"(y0_re1), "0"(ah), "1"(al));

        asm ("lsats %0, %1, %2" : "=r"(ah), "=r"(al) : "r"(q_format), "0"(ah), "1"(al));
        asm ("lextract %0, %1, %2, %3, 32" : "=r"(z0_11) : "r"(ah), "r"(al), "r"(q_format));

        asm("std %0,%1,%2[0]"::"r"(z0_01), "r"(z0_00), "r"(result_vector_R_re));
        asm("std %0,%1,%2[0]"::"r"(z0_11), "r"(z0_10), "r"(result_vector_R_im));

// Second stage

        asm ("ldd %0, %1, %2[1]" : "=r"(x0_re1), "=r"(x0_re0) : "r"(input_vector_X_re));
        asm ("ldd %0, %1, %2[1]" : "=r"(x0_im1), "=r"(x0_im0) : "r"(input_vector_X_im));

        asm ("ldd %0, %1, %2[1]" : "=r"(y0_re1), "=r"(y0_re0) : "r"(input_vector_Y_re));
        asm ("ldd %0, %1, %2[1]" : "=r"(y0_im1), "=r"(y0_im0) : "r"(input_vector_Y_im));

        asm ("lmul %0, %1, %2, %3, %4, %5" : "=r" (ah), "=r" (al) : "r"(x0_re0), "r"(y0_re0), "r"(zero), "r"(zero));
        asm ("lextract %0, %1, %2, %3, 32" : "=r"(z0_00) : "r"(ah), "r"(al), "r"(q_format));

        asm ("lmul %0, %1, %2, %3, %4, %5" : "=r" (ah), "=r" (al) : "r"(x0_im0), "r"(y0_im0), "r"(zero), "r"(zero));
        asm ("lextract %0, %1, %2, %3, 32" : "=r"(z0_10) : "r"(ah), "r"(al), "r"(q_format));
        asm ("sub %0, %1, %2" : "=r" (z0_00) : "r"(z0_00), "r"(z0_10));

        asm ("lmul %0, %1, %2, %3, %4, %5" : "=r" (ah), "=r" (al) : "r"(x0_re0), "r"(y0_im0), "r"(zero), "r"(zero));
        asm ("maccs %0, %1, %2, %3" : "=r"(ah), "=r"(al) : "r"(x0_im0), "r"(y0_re0), "0"(ah), "1"(al));

        asm ("lsats %0, %1, %2" : "=r"(ah), "=r"(al) : "r"(q_format), "0"(ah), "1"(al));
        asm ("lextract %0, %1, %2, %3, 32" : "=r"(z0_10) : "r"(ah), "r"(al), "r"(q_format));


        asm ("lmul %0, %1, %2, %3, %4, %5" : "=r" (ah), "=r" (al) : "r"(x0_re1), "r"(y0_re1), "r"(zero), "r"(zero));
        asm ("lextract %0, %1, %2, %3, 32" : "=r"(z0_01) : "r"(ah), "r"(al), "r"(q_format));

        asm ("lmul %0, %1, %2, %3, %4, %5" : "=r" (ah), "=r" (al) : "r"(x0_im1), "r"(y0_im1), "r"(zero), "r"(zero));
        asm ("lextract %0, %1, %2, %3, 32" : "=r"(z0_11) : "r"(ah), "r"(al), "r"(q_format));
        asm ("sub %0, %1, %2" : "=r" (z0_01) : "r"(z0_01), "r"(z0_11));

        asm ("lmul %0, %1, %2, %3, %4, %5" : "=r" (ah), "=r" (al) : "r"(x0_re1), "r"(y0_im1), "r"(zero), "r"(zero));
        asm ("maccs %0, %1, %2, %3" : "=r"(ah), "=r"(al) : "r"(x0_im1), "r"(y0_re1), "0"(ah), "1"(al));

        asm ("lsats %0, %1, %2" : "=r"(ah), "=r"(al) : "r"(q_format), "0"(ah), "1"(al));
        asm ("lextract %0, %1, %2, %3, 32" : "=r"(z0_11) : "r"(ah), "r"(al), "r"(q_format));

        asm("std %0,%1,%2[1]"::"r"(z0_01), "r"(z0_00), "r"(result_vector_R_re));
        asm("std %0,%1,%2[1]"::"r"(z0_11), "r"(z0_10), "r"(result_vector_R_im));

// Third stage

        asm ("ldd %0, %1, %2[2]" : "=r"(x0_re1), "=r"(x0_re0) : "r"(input_vector_X_re));
        asm ("ldd %0, %1, %2[2]" : "=r"(x0_im1), "=r"(x0_im0) : "r"(input_vector_X_im));

        asm ("ldd %0, %1, %2[2]" : "=r"(y0_re1), "=r"(y0_re0) : "r"(input_vector_Y_re));
        asm ("ldd %0, %1, %2[2]" : "=r"(y0_im1), "=r"(y0_im0) : "r"(input_vector_Y_im));

        asm ("lmul %0, %1, %2, %3, %4, %5" : "=r" (ah), "=r" (al) : "r"(x0_re0), "r"(y0_re0), "r"(zero), "r"(zero));
        asm ("lextract %0, %1, %2, %3, 32" : "=r"(z0_00) : "r"(ah), "r"(al), "r"(q_format));

        asm ("lmul %0, %1, %2, %3, %4, %5" : "=r" (ah), "=r" (al) : "r"(x0_im0), "r"(y0_im0), "r"(zero), "r"(zero));
        asm ("lextract %0, %1, %2, %3, 32" : "=r"(z0_10) : "r"(ah), "r"(al), "r"(q_format));
        asm ("sub %0, %1, %2" : "=r" (z0_00) : "r"(z0_00), "r"(z0_10));

        asm ("lmul %0, %1, %2, %3, %4, %5" : "=r" (ah), "=r" (al) : "r"(x0_re0), "r"(y0_im0), "r"(zero), "r"(zero));
        asm ("maccs %0, %1, %2, %3" : "=r"(ah), "=r"(al) : "r"(x0_im0), "r"(y0_re0), "0"(ah), "1"(al));

        asm ("lsats %0, %1, %2" : "=r"(ah), "=r"(al) : "r"(q_format), "0"(ah), "1"(al));
        asm ("lextract %0, %1, %2, %3, 32" : "=r"(z0_10) : "r"(ah), "r"(al), "r"(q_format));


        asm ("lmul %0, %1, %2, %3, %4, %5" : "=r" (ah), "=r" (al) : "r"(x0_re1), "r"(y0_re1), "r"(zero), "r"(zero));
        asm ("lextract %0, %1, %2, %3, 32" : "=r"(z0_01) : "r"(ah), "r"(al), "r"(q_format));

        asm ("lmul %0, %1, %2, %3, %4, %5" : "=r" (ah), "=r" (al) : "r"(x0_im1), "r"(y0_im1), "r"(zero), "r"(zero));
        asm ("lextract %0, %1, %2, %3, 32" : "=r"(z0_11) : "r"(ah), "r"(al), "r"(q_format));
        asm ("sub %0, %1, %2" : "=r" (z0_01) : "r"(z0_01), "r"(z0_11));

        asm ("lmul %0, %1, %2, %3, %4, %5" : "=r" (ah), "=r" (al) : "r"(x0_re1), "r"(y0_im1), "r"(zero), "r"(zero));
        asm ("maccs %0, %1, %2, %3" : "=r"(ah), "=r"(al) : "r"(x0_im1), "r"(y0_re1), "0"(ah), "1"(al));

        asm ("lsats %0, %1, %2" : "=r"(ah), "=r"(al) : "r"(q_format), "0"(ah), "1"(al));
        asm ("lextract %0, %1, %2, %3, 32" : "=r"(z0_11) : "r"(ah), "r"(al), "r"(q_format));

        asm("std %0,%1,%2[2]"::"r"(z0_01), "r"(z0_00), "r"(result_vector_R_re));
        asm("std %0,%1,%2[2]"::"r"(z0_11), "r"(z0_10), "r"(result_vector_R_im));

// Fourth stage

        asm ("ldd %0, %1, %2[3]" : "=r"(x0_re1), "=r"(x0_re0) : "r"(input_vector_X_re));
        asm ("ldd %0, %1, %2[3]" : "=r"(x0_im1), "=r"(x0_im0) : "r"(input_vector_X_im));

        asm ("ldd %0, %1, %2[3]" : "=r"(y0_re1), "=r"(y0_re0) : "r"(input_vector_Y_re));
        asm ("ldd %0, %1, %2[3]" : "=r"(y0_im1), "=r"(y0_im0) : "r"(input_vector_Y_im));

        asm ("lmul %0, %1, %2, %3, %4, %5" : "=r" (ah), "=r" (al) : "r"(x0_re0), "r"(y0_re0), "r"(zero), "r"(zero));
        asm ("lextract %0, %1, %2, %3, 32" : "=r"(z0_00) : "r"(ah), "r"(al), "r"(q_format));

        asm ("lmul %0, %1, %2, %3, %4, %5" : "=r" (ah), "=r" (al) : "r"(x0_im0), "r"(y0_im0), "r"(zero), "r"(zero));
        asm ("lextract %0, %1, %2, %3, 32" : "=r"(z0_10) : "r"(ah), "r"(al), "r"(q_format));
        asm ("sub %0, %1, %2" : "=r" (z0_00) : "r"(z0_00), "r"(z0_10));

        asm ("lmul %0, %1, %2, %3, %4, %5" : "=r" (ah), "=r" (al) : "r"(x0_re0), "r"(y0_im0), "r"(zero), "r"(zero));
        asm ("maccs %0, %1, %2, %3" : "=r"(ah), "=r"(al) : "r"(x0_im0), "r"(y0_re0), "0"(ah), "1"(al));

        asm ("lsats %0, %1, %2" : "=r"(ah), "=r"(al) : "r"(q_format), "0"(ah), "1"(al));
        asm ("lextract %0, %1, %2, %3, 32" : "=r"(z0_10) : "r"(ah), "r"(al), "r"(q_format));


        asm ("lmul %0, %1, %2, %3, %4, %5" : "=r" (ah), "=r" (al) : "r"(x0_re1), "r"(y0_re1), "r"(zero), "r"(zero));
        asm ("lextract %0, %1, %2, %3, 32" : "=r"(z0_01) : "r"(ah), "r"(al), "r"(q_format));

        asm ("lmul %0, %1, %2, %3, %4, %5" : "=r" (ah), "=r" (al) : "r"(x0_im1), "r"(y0_im1), "r"(zero), "r"(zero));
        asm ("lextract %0, %1, %2, %3, 32" : "=r"(z0_11) : "r"(ah), "r"(al), "r"(q_format));
        asm ("sub %0, %1, %2" : "=r" (z0_01) : "r"(z0_01), "r"(z0_11));

        asm ("lmul %0, %1, %2, %3, %4, %5" : "=r" (ah), "=r" (al) : "r"(x0_re1), "r"(y0_im1), "r"(zero), "r"(zero));
        asm ("maccs %0, %1, %2, %3" : "=r"(ah), "=r"(al) : "r"(x0_im1), "r"(y0_re1), "0"(ah), "1"(al));

        asm ("lsats %0, %1, %2" : "=r"(ah), "=r"(al) : "r"(q_format), "0"(ah), "1"(al));
        asm ("lextract %0, %1, %2, %3, 32" : "=r"(z0_11) : "r"(ah), "r"(al), "r"(q_format));

        asm("std %0,%1,%2[3]"::"r"(z0_01), "r"(z0_00), "r"(result_vector_R_re));
        asm("std %0,%1,%2[3]"::"r"(z0_11), "r"(z0_10), "r"(result_vector_R_im));


        vl -= 8;
        input_vector_X_re += 8; input_vector_Y_re += 8; result_vector_R_re += 8;
        input_vector_X_im += 8; input_vector_Y_im += 8; result_vector_R_im += 8;
    }

    while( vl-- )
    {
        x0_re0 = *input_vector_X_re++; x0_im0 = *input_vector_X_im++;
        y0_re0 = *input_vector_Y_re++; y0_im0 = *input_vector_Y_im++;

        asm ("lmul %0, %1, %2, %3, %4, %5" : "=r" (ah), "=r" (al) : "r"(x0_re0), "r"(y0_re0), "r"(zero), "r"(zero));
        asm ("lextract %0, %1, %2, %3, 32" : "=r"(z0_00) : "r"(ah), "r"(al), "r"(q_format));

        asm ("lmul %0, %1, %2, %3, %4, %5" : "=r" (ah), "=r" (al) : "r"(x0_im0), "r"(y0_im0), "r"(zero), "r"(zero));
        asm ("lextract %0, %1, %2, %3, 32" : "=r"(z0_10) : "r"(ah), "r"(al), "r"(q_format));
        asm ("sub %0, %1, %2" : "=r" (z0_00) : "r"(z0_00), "r"(z0_10));

        asm ("lmul %0, %1, %2, %3, %4, %5" : "=r" (ah), "=r" (al) : "r"(x0_re0), "r"(y0_im0), "r"(zero), "r"(zero));
        asm ("maccs %0, %1, %2, %3" : "=r"(ah), "=r"(al) : "r"(x0_im0), "r"(y0_re0), "0"(ah), "1"(al));

        asm ("lsats %0, %1, %2" : "=r"(ah), "=r"(al) : "r"(q_format), "0"(ah), "1"(al));
        asm ("lextract %0, %1, %2, %3, 32" : "=r"(z0_10) : "r"(ah), "r"(al), "r"(q_format));

        *result_vector_R_re = z0_00;
        *result_vector_R_im = z0_10;
        result_vector_R_re++;
        result_vector_R_im++;
    }
}

