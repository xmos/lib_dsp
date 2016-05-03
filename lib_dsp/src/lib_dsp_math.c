// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <platform.h>
#include "lib_dsp_qformat.h"
#include "lib_dsp_math.h"
#include "stdio.h"

int32_t lib_dsp_math_multiply( int32_t input1_value, int32_t input2_value, int32_t q_format )
{
    int32_t ah; uint32_t al;
    int32_t result;
    // For rounding, accumulator is pre-loaded (1<<(q_format-1))
    asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(input1_value),"r"(input2_value),"0"(0),"1"(1<<(q_format-1)));
    asm("lextract %0,%1,%2,%3,32":"=r"(result):"r"(ah),"r"(al),"r"(q_format));
    return result;
}

int32_t lib_dsp_math_multiply_sat( int32_t input1_value, int32_t input2_value, int32_t q_format )
{
    int32_t ah; uint32_t al;
    asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(input1_value),"r"(input2_value),"0"(0),"1"(1<<(q_format-1)) );
    asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
    asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
    return ah;
}

#define  ldivu(a,b,c,d,e) asm("ldivu %0,%1,%2,%3,%4" : "=r" (a), "=r" (b): "r" (c), "r" (d), "r" (e))

int32_t lib_dsp_math_divide( int32_t dividend, int32_t divisor, uint32_t q_format )
{
    int32_t sgn = 1;
    uint32_t d, d2, r;
    if (dividend < 0) {
        sgn = -1;
        dividend = -dividend;
    }
    if (divisor < 0) {
        sgn = -sgn;
        divisor = -divisor;
    }
    ldivu(d, r, 0, dividend, divisor);
    ldivu(d2, r, r, 0, divisor);

    r = d << q_format |
        (d2 + (1<<(31-q_format))) >> (32-q_format);
    return r * sgn;
}


uint32_t lib_dsp_math_divide_unsigned(uint32_t dividend, uint32_t divisor, uint32_t q_format )
{
    //h and l hold a 64-bit value
    uint32_t h; uint32_t l;
    uint32_t quotient=0, remainder=0;

    // Create long dividend by shift dividend up q_format positions
    h = dividend >> (32-q_format);
    l = dividend << (q_format);

    // Unsigned Long division
    asm("ldivu %0,%1,%2,%3,%4":"=r"(quotient):"r"(remainder),"r"(h),"r"(l),"r"(divisor));

    return quotient;
}

#define SQRT_COEFF_A ((12466528)/2) // 7143403
#define SQRT_COEFF_B (10920575) // 9633812

uq8_24 lib_dsp_math_squareroot(uq8_24 x)
{
    int32_t zeroes;
    unsigned long long approx;

    if(x==0) return 0;

    asm("clz %0,%1" : "=r" (zeroes) : "r" (x));

    zeroes = zeroes & ~1; // make even
    zeroes = (zeroes - 8) >> 1;

    // initial linear approximation of the result.
    if (zeroes >= 0) {
        approx = (SQRT_COEFF_A >> zeroes) + lib_dsp_math_multiply(x << zeroes, SQRT_COEFF_B, 24);
    } else {
        // For Q8.24 values > 1 (0x01.000000)
        zeroes = -zeroes;
        approx = (SQRT_COEFF_A << zeroes) + lib_dsp_math_multiply(x >> zeroes, SQRT_COEFF_B, 24);
    }

    // successive approximation
    for(int32_t i = 0; i < 3; i++) {
        // Linear approximation. Babylonian Method: Successive averaging
        // xn+1 = (xn + y/xn) / 2
        approx = (approx + lib_dsp_math_divide_unsigned(x, approx, 24)) >> 1;
    }

    // Return format is Q8.24
    return approx;
}


/******************************************************************
 * Derived from "Software Manual for the Elementary
 * Functions" by Cody and Waite, fixed point sin/cos chapter.
 ******************************************************************/

// coefficients are scaled up for improved rounding
#define R0 (-11184804*8)
#define R1   (2236879*4)
#define R2   (-212681*2)
#define R3     (11175)

q8_24 lib_dsp_math_sin(q8_24 rad) {
    int32_t finalSign;
    int32_t modulo;
    int32_t sqr;

    if (rad < 0) {
        rad = -rad;
        finalSign = -1;
    } else /* rad >= 0 */ {
        finalSign = 1;
    }
    // Now rad >= 0.

    modulo = lib_dsp_math_multiply(rad, ONE_OVER_HALFPI_Q8_24, 24) >> 24;
    rad -= (modulo >> 2) * PI2_Q8_24;
    if (modulo & 2) {
        finalSign = -finalSign;
        rad -= (PI2_Q8_24+1)>>1;
    }
    if (modulo & 1) {
        rad = ((PI2_Q8_24+1)>>1) - rad;
    }
    sqr = (lib_dsp_math_multiply(rad, rad, 24)+1)>>1;
    return (rad +
            ((lib_dsp_math_multiply(
              lib_dsp_math_multiply(
                lib_dsp_math_multiply(
                  lib_dsp_math_multiply(
                    lib_dsp_math_multiply(R3, sqr, 24) + R2,
                    sqr, 24) + R1,
                  sqr, 24) + R0,
                sqr, 24)
            ,rad, 24)+6)>>4)
            )* finalSign;
}

// Polynomial coefficients
// coefficients are scaled up for improved rounding
// they are also changed to positive values to enable using the dedicated instruction fur uint32_t long division:
#define p0 (126388141)//(-7899259*16)
#define p1 (13665937) // (-854121*16)
#define q0 (189582640) // (11848915*16)
#define q1 (8388608*16)

#define ONEOVERTS3 62613429
#define TS3  (4495441*16)  // 2247721 //
#define A   232471924//(14529495*16)  // was ROOT_3M1
#define B   232471924//(14529495*16)  // 7264748 // was ROOT_3

q8_24 lib_dsp_math_atan(q8_24 f) {
    int32_t negative = f < 0;
    if (negative) {
        f = -f;
    }
    uint32_t XN;
    int32_t d, r;
    if (f > ONEOVERTS3) {      // F large
        XN = 421657428; // pi/2 in Q4.28 format
        // 1 / f.
        asm("ldivu %0,%1,%2,%3,%4\n" : "=r"(d), "=r" (r) : "r" (1<<20), "r" (0), "r" (f));
        f = d;
    } else if (f > (1<<24)) {  // F less than 3.6 greater than 1
        XN = 281104952; // pi/3 in Q4.28 format
        f = f << 4;
        uint32_t thed = lib_dsp_math_multiply((1<<27), f, 28);
        uint32_t then = (1<<27) + lib_dsp_math_multiply(B, f, 28);
        if (A >= thed) {
            thed = A-thed;
            asm("ldivu %0,%1,%2,%3,%4\n" : "=r"(d), "=r" (r) : "r" (thed), "r" (0), "r" (then));
            f = d >> 4;
        } else {
            thed = thed-A;
            asm("ldivu %0,%1,%2,%3,%4\n" : "=r"(d), "=r" (r) : "r" (thed), "r" (0), "r" (then));
            f = -d >> 4;
        }
    } else if (f > (TS3 >> 4)) { // F less than 1 > 0.28
        XN = 140552476; // pi/6 in Q4.28 format
        f = f << 4;
        uint32_t thed = lib_dsp_math_multiply(A, f, 28);
        uint32_t then = (f>>1) + B;
        if (thed >> 27) {
            thed = thed -(1<<27);
            asm("ldivu %0,%1,%2,%3,%4\n" : "=r"(d), "=r" (r) : "r" (thed), "r" (0), "r" (then));
            f = d >> 4;
        } else {
            thed = (1<<27) - thed;
            asm("ldivu %0,%1,%2,%3,%4\n" : "=r"(d), "=r" (r) : "r" (thed), "r" (0), "r" (then));
            f = -d >> 4;
        }
    } else {    // F tiny
        XN = 0;
        f = f << 4;
    }

    int32_t g = lib_dsp_math_multiply(f, f, 28);


    uint32_t gPg = lib_dsp_math_multiply(lib_dsp_math_multiply(p1, g, 28) + p0, g, 28);   // Positive - p0/p1 positive
    uint32_t Qg = lib_dsp_math_multiply(q1, g, 28) + q0;              // Positive - q0/q1 positive
    asm("ldivu %0,%1,%2,%3,%4\n" : "=r"(d), "=r" (r) : "r" (gPg >> 4), "r" (gPg << 28), "r" (2*Qg));
    int32_t Rg = d;
    int32_t ffR = f + lib_dsp_math_multiply(f, -Rg, 28);
    if (XN >> 28) {
        ffR = -ffR;
    }
    ffR = XN + ffR;
    if (negative) {
        ffR = - ffR;
    }
    return (ffR + 8)>>4;

}


int32_t lib_dsp_math_round(int32_t x, int q_format) {
    // x += 0.5, truncate franctional bits
    return (x + (1<<(q_format-1))) & ~((1<<q_format) - 1);
}

/******************************************************************
 * Derived from "Software Manual for the Elementary
 * Functions" by Cody and Waite. Can be improved for N>0, in that
 * the last few bits are always zero and oughtn't be. Seems to
 * in general have an error in the last three bits that needs addressing
 ******************************************************************/

#define ONE_OVER_LN2 24204406
#define EXP_C1  11632640
//#define EXP_C2     -3560<<5 
#define EXP_C2     -113920  // Q3.39 format!

#define p0a   67108864
#define p1a   1116769
#define q0a   (ONE_Q8_24*8)
#define q1a   13418331

#define MULT_FUNC 1

/*
 * This function can be further improved by making EXP_C2 a constant in 3.29 format.
 * The current function will have an error in the 21st or 22nd bit of the fraction after
 * a modulo of a large number. However, this would require mulf3_29 functions adding code
 * size. I think it is better to stick to a slightly less accurate version for now.
 */
q8_24 lib_dsp_math_exp(q8_24 x) {
    //valid range for x is [MIN_INT32..ln(MAX_INT32)] 
    //log base conversion rule: log2(x) = ln(x) * 1/ln(2)
    //Max XN = ln(MAX_INT32)/ln(2) = log2(MAX_INT32) 
    //Todo: This overflows for small x. Why is this OK?
    q8_24 XN = lib_dsp_math_round(lib_dsp_math_multiply(x,ONE_OVER_LN2,24), 24);

    q8_24 N = XN >> 24; // truncate fractional bits

    // q_format = input1 fraction bit count + input2 fraction bit count - result fraction bit count
    // q_format = 24 + 29 - 24 = 29
#if MULT_FUNC
    q8_24 N_q8_24 = N<<24;
    q8_24 g = x - lib_dsp_math_multiply(N_q8_24, EXP_C1, 24) - lib_dsp_math_multiply(N_q8_24, EXP_C2, 29);
#else
    q8_24 g = x - N*(EXP_C1 + EXP_C2);
#endif
    q8_24 z = lib_dsp_math_multiply(g,g,24);
    q8_24 gP = lib_dsp_math_multiply(lib_dsp_math_multiply(p1a, z, 24) + p0a, g, 24);
    q8_24 Q = lib_dsp_math_multiply(q1a, z, 24) + q0a;
    q8_24 r = (ONE_Q8_24<<1) + (lib_dsp_math_divide(gP<<1, (Q - gP)>>1, 24));
//    N++;
    N--;
    return N > 0 ? (r<<N)+(1<<(N-1)) : (r+(1<<(-N-1))) >> -N;
}



// helper functions
void log2_with_remainder(q8_24 x, int *log2_p2, q8_24 *rem, int q_format) {
    q8_24 absVal;
    int zeroes; // approximated log2. log2 of the power of two value closest to x.
                 // I.e. x with 31-clz(x) lower bits truncated

    if (x < 0) {
        absVal = -x;
    } else {
        absVal = x;
    }
    
    asm("clz %0,%1" : "=r" (zeroes) : "r" (absVal));
    *log2_p2 = (32 - q_format) - zeroes;

    *rem = *log2_p2 < 0 ? x << (-*log2_p2) : x >> *log2_p2;

}

/******************************************************************
 * Derived from "Software Manual for the Elementary
 * Functions" by Cody and Waite. Needs systematic testing.
 ******************************************************************/

#define ONE_OVER_LN2 24204406

#define LOG_C0   (HALF_Q8_24 + 3474675)
#define C     228186

#define a0    815851
#define b0  11699746
#define b1  -2097152

#define LOG_C1  11632640
#define LOG_C2     -3560

q8_24 lib_dsp_math_log(uq8_24 x) {
    q8_24 f, zden, y, Bw, Aw, rz2, v, qz, rz, z, w;
    int N;
    log2_with_remainder(x, &N, &f, 24);

    if (f < LOG_C0) {
        y = f - HALF_Q8_24;
        zden = (y >> 1) + HALF_Q8_24;
        N--;
    } else {
        y = f - ONE_Q8_24;
        zden = (y >> 1) + ONE_Q8_24;
    }
    z = lib_dsp_math_divide(y, zden, 24);
    w = lib_dsp_math_multiply(z, z, 24);
    Bw = lib_dsp_math_multiply(b1, w, 24) + b0;
    Aw = a0;
    rz2 = lib_dsp_math_multiply(w, C + lib_dsp_math_divide(Aw, Bw, 24), 24);
    v = lib_dsp_math_divide(HALF_Q8_24>>1, zden, 24);
    qz = v + lib_dsp_math_multiply(rz2, v, 24);
    rz = lib_dsp_math_multiply(4*y, qz, 24);
    return (N*LOG_C2+rz)+N*LOG_C1;
}




