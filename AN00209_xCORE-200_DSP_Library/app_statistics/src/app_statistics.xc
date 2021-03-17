// Copyright 2015-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// XMOS DSP Library - Statistics Functions Test Program
// Uses Q24 format

// Include files
#include <stdio.h>
#include <xs1.h>
#include <dsp.h>

// Define constants

#define Q_M               8
#define Q_N               24

#define SAMPLE_LENGTH         50
#define SHORT_SAMPLE_LENGTH   5

// Declare global variables and arrays
int32_t  Src[] = { Q24(.11), Q24(.12), Q24(.13), Q24(.14), Q24(.15), Q24(.16), Q24(.17), Q24(.18), Q24(.19), Q24(.20),
               Q24(.21), Q24(.22), Q24(.23), Q24(.24), Q24(.25), Q24(.26), Q24(.27), Q24(.28), Q24(.29), Q24(.30),
               Q24(.31), Q24(.32), Q24(.33), Q24(.34), Q24(.35), Q24(.36), Q24(.37), Q24(.38), Q24(.39), Q24(.40),
               Q24(.41), Q24(.42), Q24(.43), Q24(.44), Q24(.45), Q24(.46), Q24(.47), Q24(.48), Q24(.49), Q24(.50),
               Q24(.51), Q24(.52), Q24(.53), Q24(.54), Q24(.55), Q24(.56), Q24(.57), Q24(.58), Q24(.59), Q24(.60)};
int32_t  Src2[] = { Q24(.51), Q24(.52), Q24(.53), Q24(.54), Q24(.55)};
int32_t           Dst[SAMPLE_LENGTH];

int main(void)
{
  int32_t result;

  result =
    dsp_vector_mean (Src,                       // Input vector
                         SAMPLE_LENGTH,             // Vector length
                         Q_N);                      // Q Format N

  printf ("Vector Mean = %lf\n", F24 (result));

  result =
    dsp_vector_power (Src,                      // Input vector
                          SAMPLE_LENGTH,            // Vector length
                          Q_N);                     // Q Format N

  printf ("Vector Power (sum of squares) = %lf\n", F24 (result));

  result =
    dsp_vector_rms (Src,                        // Input vector
                        SAMPLE_LENGTH,              // Vector length
                        Q_N);                       // Q Format N

  printf ("Vector Root Mean Square = %lf\n", F24 (result));

  result =
    dsp_vector_dotprod (Src,                    // Input vector 1
                            Src2,                   // Input vector 2
                            SHORT_SAMPLE_LENGTH,    // Vector length
                            Q_N);                   // Q Format N

  printf ("Vector Dot Product = %lf\n", F24 (result));

  return (0);
}

