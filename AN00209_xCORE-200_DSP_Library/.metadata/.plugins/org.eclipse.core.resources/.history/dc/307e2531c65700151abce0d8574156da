// Copyright (c) 2015, XMOS Ltd, All rights reserved
// XMOS DSP Library - Math Functions Test Program
// Uses Q24 format

// Include files
#include <stdio.h>
#include <xs1.h>
#include <lib_dsp.h>

// Define constants

#define Q_M               8
#define Q_N               24

int main(void)
{
//  printf ("Add With Saturate (2 + 4) : %d\n", F24(xmos_dsp_math_adds (2, 4)));;

  printf ("Multiplication (2 x 4) : %f\n", F24(xmos_dsp_math_multiply (Q24(2.), Q24(4.), Q_N)));;

  printf ("Reciprocal (2) : %f\n", F24(xmos_dsp_math_reciprocal (Q24(2.), Q_N)));;

  printf ("Inverse square root (2) : %f\n", F24(xmos_dsp_math_invsqrroot (Q24(2.), Q_N)));;

  printf ("Square Root (2) : %f\n", F24(xmos_dsp_math_squareroot (Q24(2.), Q_N)));;

  return (0);
}

