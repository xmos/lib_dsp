// Copyright 2015-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// XMOS DSP Library - Vector Functions Test Program
// Uses Q24 format

#include <stdio.h>
#include <xs1.h>
#include <dsp.h>

#define Q_M                   8
#define Q_N                   24

#define SAMPLE_LENGTH         50
#define SHORT_SAMPLE_LENGTH   5
#define ABS_NEGATE_Q_ELE_CNT  2

#define COMPLEX_VECTOR_LENGTH 12


int32_t  Src[] = { Q24(.11), Q24(.12), Q24(.13), Q24(.14), Q24(.15), Q24(.16), Q24(.17), Q24(.18), Q24(.19), Q24(.20),
                   Q24(.21), Q24(.22), Q24(.23), Q24(.24), Q24(.25), Q24(.26), Q24(.27), Q24(.28), Q24(.29), Q24(.30),
                   Q24(.31), Q24(.32), Q24(.33), Q24(.34), Q24(.35), Q24(.36), Q24(.37), Q24(.38), Q24(.39), Q24(.40),
                   Q24(.41), Q24(.42), Q24(.43), Q24(.44), Q24(.45), Q24(.46), Q24(.47), Q24(.48), Q24(.49), Q24(.50),
                   Q24(.51), Q24(.52), Q24(.53), Q24(.54), Q24(.55), Q24(.56), Q24(.57), Q24(.58), Q24(.59), Q24(.60)};
int32_t  Src2[] = { Q24(.51), Q24(.52), Q24(.53), Q24(.54), Q24(.55), Q24(.56), Q24(.57), Q24(.58), Q24(.59), Q24(.60)};
int32_t  Src3[] = { Q24(.61), Q24(.62), Q24(.63), Q24(.64), Q24(.65), Q24(.66), Q24(.67), Q24(.68), Q24(.69), Q24(.70)};
int32_t  Src4[] = { Q24(.51), Q24(-.31), Q24(0), Q24(.64), Q24(-0.3), Q24(.60), Q24(.67), Q24(.68), Q24(.69), Q24(.71)};
int32_t  Src5[] = { Q24(.11), Q24(-.12), 0, INT32_MAX, INT32_MIN};
int32_t  Dst[SAMPLE_LENGTH];

int32_t A_real[] = {Q24(1.), Q24(2.), Q24(3.), Q24(4.), Q24(3.), Q24(4.), Q24(1.), Q24(2.), Q24(1.), Q24(3.), Q24(5.), Q24(7.)};
int32_t A_imag[] = {Q24(5.), Q24(6.), Q24(7.), Q24(8.), Q24(7.), Q24(8.), Q24(5.), Q24(6.), Q24(2.), Q24(4.), Q24(6.), Q24(8.)};
int32_t B_real[] = {Q24(3.), Q24(4.), Q24(5.), Q24(6.), Q24(7.), Q24(1.), Q24(3.), Q24(7.), Q24(5.), Q24(6.), Q24(5.), Q24(6.)};
int32_t B_imag[] = {Q24(5.), Q24(6.), Q24(7.), Q24(8.), Q24(9.), Q24(3.), Q24(5.), Q24(9.), Q24(7.), Q24(8.), Q24(7.), Q24(8.)};
int32_t C_real[COMPLEX_VECTOR_LENGTH];
int32_t C_imag[COMPLEX_VECTOR_LENGTH];


int main(void)
{
  int32_t result;
  int32_t i;

  for(int i = 0; i < 8; i++) {
      Dst[i] = (unsigned) Src3[i] < (unsigned) Src4[i] ? Src3[i] : Src4[i];
  }
  dsp_vector_minv((Src4, uint32_t[]), (Src3, uint32_t[]),                   // Input vector
                        8);         // Vector length
  for(int i = 0; i < 8; i++) {
      if (Dst[i] != Src4[i]) {
          printf("Error in minv, %d %d\n", Dst[i], Src4[i]);
      }
  }

  result =
    dsp_vector_minimum (Src,                    // Input vector
                        SAMPLE_LENGTH);         // Vector length

  printf ("Minimum location = %d\n", result);
  printf ("Minimum = %lf\n", F24 (Src[result]));

  result =
    dsp_vector_maximum (Src,                    // Input vector
                        SAMPLE_LENGTH);         // Vector length

  printf ("Maximum location = %d\n", result);
  printf ("Maximum = %lf\n", F24 (Src[result]));

  dsp_vector_negate (Src5,                      // Input vector
                     Dst,                       // Output vector
                     SHORT_SAMPLE_LENGTH);      // Vector length

  printf ("Vector Negate Result\n");
  for (i = 0; i < SHORT_SAMPLE_LENGTH; i++)
  {
    if (i < ABS_NEGATE_Q_ELE_CNT )
    {
      printf ("Dst[%d] = %lf\n", i, F24 (Dst[i]));
    }
    else
    {
      printf ("Dst[%d] = %d\n", i, Dst[i]);
    }
  }

  dsp_vector_abs (Src5,                         // Input vector
                  Dst,                          // Output vector
                  SHORT_SAMPLE_LENGTH);         // Vector length

  printf ("Vector Absolute Result\n");
  for (i = 0; i < SHORT_SAMPLE_LENGTH; i++)
  {
    if (i < ABS_NEGATE_Q_ELE_CNT )
    {
      printf ("Dst[%d] = %lf\n", i, F24 (Dst[i]));
    }
    else
    {
      printf ("Dst[%d] = %d\n", i, Dst[i]);
    }
  }

  dsp_vector_adds (Src,                         // Input vector
                   Q24(2.),                     // Input scalar
                   Dst,                         // Output vector
                   SHORT_SAMPLE_LENGTH);        // Vector length

  printf ("Vector / scalar addition Result\n");
  for (i = 0; i < SHORT_SAMPLE_LENGTH; i++)
  {
    printf ("Dst[%d] = %lf\n", i, F24 (Dst[i]));
  }

  dsp_vector_muls (Src,                         // Input vector
                   Q24(2.),                     // Input scalar
                   Dst,                         // Output vector
                   SHORT_SAMPLE_LENGTH,         // Vector length
                   Q_N);                        // Q Format N

  printf ("Vector / scalar multiplication Result\n");
  for (i = 0; i < SHORT_SAMPLE_LENGTH; i++)
  {
    printf ("Dst[%d] = %lf\n", i, F24 (Dst[i]));
  }

  dsp_vector_addv (Src,                         // Input vector
                   Src2,                        // Input vector 2
                   Dst,                         // Output vector
                   SHORT_SAMPLE_LENGTH);        // Vector length

  printf ("Vector / vector addition Result\n");
  for (i = 0; i < SHORT_SAMPLE_LENGTH; i++)
  {
    printf ("Dst[%d] = %lf\n", i, F24 (Dst[i]));
  }

  dsp_vector_subv (Src,                         // Input vector
                   Src2,                        // Input vector 2
                   Dst,                         // Output vector
                   SHORT_SAMPLE_LENGTH);        // Vector length

  printf ("Vector / vector subtraction Result\n");
  for (i = 0; i < SHORT_SAMPLE_LENGTH; i++)
  {
    printf ("Dst[%d] = %lf\n", i, F24 (Dst[i]));
  }

  dsp_vector_mulv (Src,                         // Input vector
                   Src2,                        // Input vector 2
                   Dst,                         // Output vector
                   SHORT_SAMPLE_LENGTH,         // Vector length
                   Q_N);                        // Q Format N

  printf ("Vector / vector multiplication Result\n");
  for (i = 0; i < SHORT_SAMPLE_LENGTH; i++)
  {
    printf ("Dst[%d] = %lf\n", i, F24 (Dst[i]));
  }

  dsp_vector_mulv_adds (Src,                    // Input vector
                        Src2,                   // Input vector 2
                        Q24(2.),                // Input scalar
                        Dst,                    // Output vector
                        SHORT_SAMPLE_LENGTH,    // Vector length
                        Q_N);                   // Q Format N

  printf ("Vector multiplication and scalar addition Result\n");
  for (i = 0; i < SHORT_SAMPLE_LENGTH; i++)
  {
    printf ("Dst[%d] = %lf\n", i, F24 (Dst[i]));
  }

  dsp_vector_muls_addv (Src,                    // Input vector
                        Q24(2.),                // Input scalar
                        Src2,                   // Input vector 2
                        Dst,                    // Output vector
                        SHORT_SAMPLE_LENGTH,    // Vector length
                        Q_N);                   // Q Format N

  printf ("Vector / Scalar multiplication and vector addition Result\n");
  for (i = 0; i < SHORT_SAMPLE_LENGTH; i++)
  {
    printf ("Dst[%d] = %lf\n", i, F24 (Dst[i]));
  }

  dsp_vector_muls_subv (Src,                    // Input vector
                        Q24(2.),                // Input scalar
                        Src2,                   // Input vector 2
                        Dst,                    // Output vector
                        SHORT_SAMPLE_LENGTH,    // Vector length
                        Q_N);                   // Q Format N

  printf ("Vector / Scalar multiplication and vector subtraction Result\n");
  for (i = 0; i < SHORT_SAMPLE_LENGTH; i++)
  {
    printf ("Dst[%d] = %lf\n", i, F24 (Dst[i]));
  }

  dsp_vector_mulv_addv (Src,                    // Input vector
                        Src2,                   // Input vector 2
                        Src3,                   // Input vector 3
                        Dst,                    // Output vector
                        SHORT_SAMPLE_LENGTH,    // Vector length
                        Q_N);                   // Q Format N

  printf ("Vector / Vector multiplication and vector addition Result\n");
  for (i = 0; i < SHORT_SAMPLE_LENGTH; i++)
  {
    printf ("Dst[%d] = %lf\n", i, F24 (Dst[i]));
  }

  dsp_vector_mulv_subv (Src,                    // Input vector
                        Src2,                   // Input vector 2
                        Src3,                   // Input vector 3
                        Dst,                    // Output vector
                        SHORT_SAMPLE_LENGTH,    // Vector length
                        Q_N);                   // Q Format N

  printf ("Vector / Vector multiplication and vector subtraction Result\n");
  for (i = 0; i < SHORT_SAMPLE_LENGTH; i++)
  {
    printf ("Dst[%d] = %lf\n", i, F24 (Dst[i]));
  }


  dsp_vector_mulv_complex (A_real, A_imag, B_real, B_imag, C_real, C_imag, COMPLEX_VECTOR_LENGTH, Q_N);

  printf ("Complex vector / Vector multiplication Result\n");
  printf ("C_real = ");
  for (int i = 0; i < COMPLEX_VECTOR_LENGTH; i++)
    printf ("%lf, ", F24 (C_real[i]));
  printf ("\n");
  printf ("C_imag = ");
  for (int i = 0; i < COMPLEX_VECTOR_LENGTH; i++)
    printf ("%lf, ", F24 (C_imag[i]));
  printf ("\n");


  return (0);
}

