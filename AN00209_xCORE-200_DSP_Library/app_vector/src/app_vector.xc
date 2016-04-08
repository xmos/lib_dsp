// Copyright (c) 2016, XMOS Ltd, All rights reserved
// XMOS DSP Library - Vector Functions Test Program
// Uses Q24 format

#include <stdio.h>
#include <xs1.h>
#include <lib_dsp.h>

#define Q_M               8
#define Q_N               24

#define SAMPLE_LENGTH         50
#define SHORT_SAMPLE_LENGTH   5

int32_t  Src[] = { Q24(.11), Q24(.12), Q24(.13), Q24(.14), Q24(.15), Q24(.16), Q24(.17), Q24(.18), Q24(.19), Q24(.20),
               Q24(.21), Q24(.22), Q24(.23), Q24(.24), Q24(.25), Q24(.26), Q24(.27), Q24(.28), Q24(.29), Q24(.30),
               Q24(.31), Q24(.32), Q24(.33), Q24(.34), Q24(.35), Q24(.36), Q24(.37), Q24(.38), Q24(.39), Q24(.40),
               Q24(.41), Q24(.42), Q24(.43), Q24(.44), Q24(.45), Q24(.46), Q24(.47), Q24(.48), Q24(.49), Q24(.50),
               Q24(.51), Q24(.52), Q24(.53), Q24(.54), Q24(.55), Q24(.56), Q24(.57), Q24(.58), Q24(.59), Q24(.60)};
int32_t  Src2[] = { Q24(.51), Q24(.52), Q24(.53), Q24(.54), Q24(.55), Q24(.56), Q24(.57), Q24(.58), Q24(.59), Q24(.60)};
int32_t  Src3[] = { Q24(.61), Q24(.62), Q24(.63), Q24(.64), Q24(.65), Q24(.66), Q24(.67), Q24(.68), Q24(.69), Q24(.70)};
int32_t           Dst[SAMPLE_LENGTH];

int main(void)
{
  int32_t result;
  int32_t i;

  result =
    lib_dsp_vector_minimum (Src,                    // Input vector
                            SAMPLE_LENGTH);         // Vector length

  printf ("Minimum location = %d\n", result);
  printf ("Minimum = %lf\n", F24 (Src[result]));

  result =
    lib_dsp_vector_maximum (Src,                    // Input vector
                            SAMPLE_LENGTH);         // Vector length

  printf ("Maximum location = %d\n", result);
  printf ("Maximum = %lf\n", F24 (Src[result]));

  lib_dsp_vector_negate (Src,                       // Input vector
                         Dst,                       // Output vector
                         SHORT_SAMPLE_LENGTH);      // Vector length

  printf ("Vector Negate Result\n");
  for (i = 0; i < SHORT_SAMPLE_LENGTH; i++)
  {
    printf ("Dst[%d] = %lf\n", i, F24 (Dst[i]));
  }

  lib_dsp_vector_abs (Src,                          // Input vector
                      Dst,                          // Output vector
                      SHORT_SAMPLE_LENGTH);         // Vector length

  printf ("Vector Absolute Result\n");
  for (i = 0; i < SHORT_SAMPLE_LENGTH; i++)
  {
    printf ("Dst[%d] = %lf\n", i, F24 (Dst[i]));
  }

  lib_dsp_vector_adds (Src,                         // Input vector
                       Q24(2.),                     // Input scalar
                       Dst,                         // Output vector
                       SHORT_SAMPLE_LENGTH);        // Vector length

  printf ("Vector / scalar addition Result\n");
  for (i = 0; i < SHORT_SAMPLE_LENGTH; i++)
  {
    printf ("Dst[%d] = %lf\n", i, F24 (Dst[i]));
  }

  lib_dsp_vector_muls (Src,                         // Input vector
                       Q24(2.),                     // Input scalar
                       Dst,                         // Output vector
                       SHORT_SAMPLE_LENGTH,         // Vector length
                       Q_N);                        // Q Format N

  printf ("Vector / scalar multiplication Result\n");
  for (i = 0; i < SHORT_SAMPLE_LENGTH; i++)
  {
    printf ("Dst[%d] = %lf\n", i, F24 (Dst[i]));
  }

  lib_dsp_vector_addv (Src,                         // Input vector
                       Src2,                        // Input vector 2
                       Dst,                         // Output vector
                       SHORT_SAMPLE_LENGTH);        // Vector length

  printf ("Vector / vector addition Result\n");
  for (i = 0; i < SHORT_SAMPLE_LENGTH; i++)
  {
    printf ("Dst[%d] = %lf\n", i, F24 (Dst[i]));
  }

  lib_dsp_vector_subv (Src,                         // Input vector
                       Src2,                        // Input vector 2
                       Dst,                         // Output vector
                       SHORT_SAMPLE_LENGTH);        // Vector length

  printf ("Vector / vector subtraction Result\n");
  for (i = 0; i < SHORT_SAMPLE_LENGTH; i++)
  {
    printf ("Dst[%d] = %lf\n", i, F24 (Dst[i]));
  }

  lib_dsp_vector_mulv (Src,                         // Input vector
                       Src2,                        // Input vector 2
                       Dst,                         // Output vector
                       SHORT_SAMPLE_LENGTH,         // Vector length
                       Q_N);                        // Q Format N

  printf ("Vector / vector multiplication Result\n");
  for (i = 0; i < SHORT_SAMPLE_LENGTH; i++)
  {
    printf ("Dst[%d] = %lf\n", i, F24 (Dst[i]));
  }

  lib_dsp_vector_mulv_adds (Src,                    // Input vector
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

  lib_dsp_vector_muls_addv (Src,                    // Input vector
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

  lib_dsp_vector_muls_subv (Src,                    // Input vector
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

  lib_dsp_vector_mulv_addv (Src,                    // Input vector
                            Src2,                   // Input vector 2
                            Src3,                   // Input vector 2
                            Dst,                    // Output vector
                            SHORT_SAMPLE_LENGTH,    // Vector length
                            Q_N);                   // Q Format N

  printf ("Vector / Vector multiplication and vector addition Result\n");
  for (i = 0; i < SHORT_SAMPLE_LENGTH; i++)
  {
    printf ("Dst[%d] = %lf\n", i, F24 (Dst[i]));
  }

  lib_dsp_vector_mulv_subv (Src,                    // Input vector
                            Src2,                   // Input vector 2
                            Src3,                   // Input vector 2
                            Dst,                    // Output vector
                            SHORT_SAMPLE_LENGTH,    // Vector length
                            Q_N);                   // Q Format N

  printf ("Vector / Vector multiplication and vector subtraction Result\n");
  for (i = 0; i < SHORT_SAMPLE_LENGTH; i++)
  {
    printf ("Dst[%d] = %lf\n", i, F24 (Dst[i]));
  }

  return (0);
}

