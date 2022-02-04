#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "dsp_vector_xs3.h"
float f[1024];
float g[1024];
float r_p[1024];
float r_x[1024];
float rC_p[1024];
float rC_x[1024];

#define OBJStruct void
#define APES_RESTRICT
#define SIZE_T uint32_t
#define FLOAT float

// Original functions
#define DC(M) 0
#define NQ(M) 1

void FFVectMac
(
    OBJStruct   * const op,
    const SIZE_T        N,
    const FLOAT * const APES_RESTRICT in1,
    const FLOAT * const APES_RESTRICT in2,
          FLOAT * const APES_RESTRICT io
)
{
    SIZE_T c1, c2;

    io[ DC(N) ] += in1[ DC(N) ] * in2[ DC(N) ];
    io[ NQ(N) ] += in1[ NQ(N) ] * in2[ NQ(N) ];

    for( c1 = 2, c2 = 3; c1 < N; c1 += 2, c2 += 2 )
    {
        FLOAT t1 = in1[ c1 ];
        FLOAT t2 = in1[ c2 ];
        FLOAT t3 = in2[ c1 ];
        FLOAT t4 = in2[ c2 ];
        io[ c1 ] += t1 * t3 - t2 * t4;
        io[ c2 ] += t1 * t4 + t2 * t3;
    }
}

void FFVectMacC
(
    OBJStruct   * const op,
    const SIZE_T        N,
    const FLOAT * const APES_RESTRICT in1, /* Conj */
    const FLOAT * const APES_RESTRICT in2,
          FLOAT * const APES_RESTRICT io
)
{
    SIZE_T c1, c2;

    io[ DC(N) ] += in1[ DC(N) ] * in2[ DC(N) ];
    io[ NQ(N) ] += in1[ NQ(N) ] * in2[ NQ(N) ];

    for( c1 = 2, c2 = 3; c1 < N; c1 += 2, c2 += 2 )
    {
        FLOAT t1 = in1[ c1 ];
        FLOAT t2 = in1[ c2 ];
        FLOAT t3 = in2[ c1 ];
        FLOAT t4 = in2[ c2 ];
        io[ c1 ] += t1 * t3 + t2 * t4;
        io[ c2 ] += t1 * t4 - t2 * t3;
    }
}

void FFVectMpy
(
    OBJStruct   * const op,
    const SIZE_T        N,
    const FLOAT * const in1,
    const FLOAT * const in2,
          FLOAT * const APES_RESTRICT out
)
{
    SIZE_T c1, c2;

    out[ DC(N) ] = in1[ DC(N) ] * in2[ DC(N) ];
    out[ NQ(N) ] = in1[ NQ(N) ] * in2[ NQ(N) ];

    for( c1 = 2, c2 = 3; c1 < N; c1 += 2, c2 += 2 )
    {
        FLOAT t1 = in1[ c1 ];
        FLOAT t2 = in1[ c2 ];
        FLOAT t3 = in2[ c1 ];
        FLOAT t4 = in2[ c2 ];
        out[ c1 ] = t1 * t3 - t2 * t4;
        out[ c2 ] = t1 * t4 + t2 * t3;
    }
}

void FFVectMpyC
(
    OBJStruct   * const op,
    const SIZE_T        N,
    const FLOAT * const in1, /* Conj */
    const FLOAT * const in2,
          FLOAT * const APES_RESTRICT out
)
{
    SIZE_T c1, c2;

    out[ DC(N) ] = in1[ DC(N) ] * in2[ DC(N) ];
    out[ NQ(N) ] = in1[ NQ(N) ] * in2[ NQ(N) ];

    for( c1 = 2, c2 = 3; c1 < N; c1 += 2, c2 += 2 )
    {
        FLOAT t1 = in1[ c1 ];
        FLOAT t2 = in1[ c2 ];
        FLOAT t3 = in2[ c1 ];
        FLOAT t4 = in2[ c2 ];
        out[ c1 ] = t1 * t3 + t2 * t4;
        out[ c2 ] = t1 * t4 - t2 * t3;
    }
}

void FFVectAcc
(
    OBJStruct   * const op,
    const SIZE_T        N,
    const FLOAT * const APES_RESTRICT in,
          FLOAT * const APES_RESTRICT io
)
{
    SIZE_T c1;
    for( c1 = 0; c1 < N; c1++ )
    {
        io[ c1 ] = io[ c1 ] + in[ c1 ];
    }
}

void compare_arrays(float* v1, float* v2, uint32_t N) {
    for (int i=0; i<N; i++){
        if (r_p[i]!=r_x[i]){
            printf("Error: value %d is different: %f vs %f\n", i, v1[i], v2[i]);
            exit(1);
        }
    }
}

void test_FFVectMac(float *ptr1, float *ptr2, int N) {
    for(int i = 0; i < N; i++) {
        ptr1[i] = i + 10.0;
    }
    for(int i = 0; i < N; i++) {
        ptr2[i] = i + 13.0;
    }
    for(int i = 0; i < N; i++) {
        r_p[i] = 0;
        r_x[i] = 0;
    }

    int t0, t1, t2, t3, t4;
    asm volatile("gettime %0" : "=r" (t0));
    FFVectMac(NULL, N, ptr1, ptr2, r_p);
    asm volatile("gettime %0" : "=r" (t1));
    FFVectMac_xs3(N, ptr1, ptr2, r_x);
    asm volatile("gettime %0" : "=r" (t2));
    FFVectMacC(NULL, N, ptr1, ptr2, rC_p);
    asm volatile("gettime %0" : "=r" (t3));
    FFVectMacC_xs3(N, ptr1, ptr2, rC_x);
    asm volatile("gettime %0" : "=r" (t4));

    // Compare the results
    compare_arrays(r_p, r_x, N);
    compare_arrays(rC_p, rC_x, N);

    printf("The results of FFVectMac and FFVectMac_xs3 match\n");
    printf("The results of FFVectMacC and FFVectMacC_xs3 match\n");
    printf("Execution times for %d values: FFVectMac %d, FFVectMac_xs3 %d, FFVectMacC %d, FFVectMacC_xs3 %d\n", N, t1-t0, t2-t1, t3-t2, t4-t3);
}

void test_FFVectMpy(float *ptr1, float *ptr2, int N) {
    for(int i = 0; i < N; i++) {
        ptr1[i] = i + 10.0;
    }
    for(int i = 0; i < N; i++) {
        ptr2[i] = i + 13.0;
    }
    for(int i = 0; i < N; i++) {
        r_p[i] = 0;
        r_x[i] = 0;
    }

    int t0, t1, t2, t3, t4;
    asm volatile("gettime %0" : "=r" (t0));
    FFVectMpy(NULL, N, ptr1, ptr2, r_p);
    asm volatile("gettime %0" : "=r" (t1));
    FFVectMpy_xs3(N, ptr1, ptr2, r_x);
    asm volatile("gettime %0" : "=r" (t2));
    FFVectMpyC(NULL, N, ptr1, ptr2, rC_p);
    asm volatile("gettime %0" : "=r" (t3));
    FFVectMpyC_xs3(N, ptr1, ptr2, rC_x);
    asm volatile("gettime %0" : "=r" (t4));

    // Compare the results
    compare_arrays(r_p, r_x, N);
    compare_arrays(rC_p, rC_x, N);

    printf("The results of FFVectMpy and FFVectMpy_xs3 match\n");
    printf("The results of FFVectMpyC and FFVectMpy_xs3 match\n");
    printf("Execution times for %d values: FFVectMpy %d, FFVectMpy_xs3 %d, FFVectMpyC %d, FFVectMpyC_xs3 %d\n", N, t1-t0, t2-t1, t3-t2, t4-t3);
}

void test_FFVectAcc(float *ptr1, int N) {
    for(int i = 0; i < N; i++) {
        ptr1[i] = i + 10.0;
    }
    for(int i = 0; i < N; i++) {
        r_p[i] = 0;
        r_x[i] = 0;
    }

    int t0, t1, t2;
    asm volatile("gettime %0" : "=r" (t0));
    FFVectAcc(NULL, N, ptr1, r_p);
    asm volatile("gettime %0" : "=r" (t1));
    FFVectAcc_xs3(N, ptr1, r_x);
    asm volatile("gettime %0" : "=r" (t2));

    // Compare the results
    compare_arrays(r_p, r_x, N);

    printf("The results of FFVectAcc and FFVectAcc_xs3 match\n");
    printf("Execution times for %d values: FFVectAcc %d, FFVectAcc_xs3 %d\n", N, t1-t0, t2-t1);
}

int main(void) {
    test_FFVectMac(f, g, 512);
    test_FFVectMac(f, g, 1024);
    test_FFVectMpy(f, g, 512);
    test_FFVectMpy(f, g, 1024);
    test_FFVectAcc(f, 512);
    test_FFVectAcc(f, 1024);

    return 0;
}
