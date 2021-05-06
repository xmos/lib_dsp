// Copyright 2015-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// XMOS DSP Library - Matrix Functions Test Program
// Uses Q24 format

// Include files
#include <stdio.h>
#include <xs1.h>
#include <dsp.h>
#include <math.h>

// Define constants

#define Q_M               8
#define Q_N               24

// Declare global variables and arrays
int32_t  Src1[] = { Q24(.11), Q24(.12), Q24(.13),
                Q24(.21), Q24(.22), Q24(.23),
                Q24(.31), Q24(.32), Q24(.33)};
int32_t  Src2[] = { Q24(.41), Q24(.42), Q24(.43),
                Q24(.51), Q24(.52), Q24(.53),
                Q24(.61), Q24(.62), Q24(.63)};
int32_t         Dst[3*3];

int main(void)
{

    printf ("Matrix multiplication: R = X * Y\n");
    // Input vector
    /*
    int32_t input_vector[2] = {
            Q24(1),
            Q24(1)
    }; // 45 degree vector
    */

    int32_t input_vector[2] = {
            Q24(sqrt(3)/2),
            Q24(0.5)
    }; // 30 degree vector

    printf ("Input column vector X[2] (30 degrees from x axis):\n");
    printf ("%.8f\n", F24 (input_vector[0]));
    printf ("%.8f\n", F24 (input_vector[1]));

    int32_t rotated_vector[2];
    // rotate by 90 degrees (pi/2)
    q8_24 alpha = Q24(1.5707963268);
    cos(alpha);
    int32_t rotation_matrix[4] = {
            dsp_math_cos(alpha), -dsp_math_sin(alpha),
            dsp_math_sin(alpha), dsp_math_cos(alpha),
    };

    dsp_matrix_mulm (rotation_matrix,  // 'input_matrix_X':  Pointer to source data array X
            input_vector,                  // 'input_matrix_Y':  Pointer to source data array Y
            rotated_vector,                // 'result_matrix_R': Pointer to the resulting data array
            2,                             // 'rows_X':    Number of rows in X
            1,                             // 'cols_Y':    Number of columns in Y
            2,                             // 'cols_X_rows_Y'
            24);                           // 'q_format':        Fixed point format, the number of bits making up fractional part

    printf ("Result of multiplying column vector X[2] with rotation matrix Y[2][2] (90 degrees rotation):\n");
    printf ("%.8f\n", F24 (rotated_vector[0]));
    printf ("%.8f\n", F24 (rotated_vector[1]));
                                                  // Matrix negation: R = -X
  dsp_matrix_negate (Src1,                    // 'input_matrix_X':  Pointer/reference to source data
                         Dst,                     // 'result_matrix_R': Pointer to the resulting 2-dimensional data array
                         3,         // 'row_count':       Number of rows in input and output matrices
                         3);        // 'column_count':    Number of columns in input and output matrices
  printf ("\n");

  printf ("Matrix negation: R = -X\n");
  printf ("%lf, %lf, %lf\n", F24 (Dst[0]), F24 (Dst[1]), F24 (Dst[2]));
  printf ("%lf, %lf, %lf\n", F24 (Dst[3]), F24 (Dst[4]), F24 (Dst[5]));
  printf ("%lf, %lf, %lf\n", F24 (Dst[6]), F24 (Dst[7]), F24 (Dst[8]));
  printf ("\n");

                                                  // Matrix / scalar addition: R = X + a
  dsp_matrix_adds (Src1,                      // 'input_matrix_X':  Pointer/reference to source data
                       Q24(2.),                   // 'scalar_value_A':  Scalar value to add to each 'input' element
                       Dst,                       // 'result_matrix_R': Pointer to the resulting 2-dimensional data array
                       3,           // 'row_count':       Number of rows in input and output matrices
                       3);          // 'column_count':    Number of columns in input and output matrices

  printf ("Matrix / scalar addition: R = X + a\n");
  printf ("%lf, %lf, %lf\n", F24 (Dst[0]), F24 (Dst[1]), F24 (Dst[2]));
  printf ("%lf, %lf, %lf\n", F24 (Dst[3]), F24 (Dst[4]), F24 (Dst[5]));
  printf ("%lf, %lf, %lf\n", F24 (Dst[6]), F24 (Dst[7]), F24 (Dst[8]));
  printf ("\n");

                                                  // Matrix / scalar multiplication: R = X * a
  dsp_matrix_muls (Src1,                      // 'input_matrix_X':  Pointer/reference to source data
                       Q24(2.),                   // 'scalar_value_A':  Scalar value to multiply each 'input' element by
                       Dst,                       // 'result_matrix_R': Pointer to the resulting 2-dimensional data array
                       3,           // 'row_count':       Number of rows in input and output matrices
                       3,           // 'column_count':    Number of columns in input and output matrices
                       Q_N);                      // 'q_format':        Fixed point format, the number of bits making up fractional part

  printf ("Matrix / scalar multiplication: R = X + a\n");
  printf ("%lf, %lf, %lf\n", F24 (Dst[0]), F24 (Dst[1]), F24 (Dst[2]));
  printf ("%lf, %lf, %lf\n", F24 (Dst[3]), F24 (Dst[4]), F24 (Dst[5]));
  printf ("%lf, %lf, %lf\n", F24 (Dst[6]), F24 (Dst[7]), F24 (Dst[8]));
  printf ("\n");

                                                  // Matrix / matrix addition: R = X + Y
  dsp_matrix_addm (Src1,                      // 'input_matrix_X':  Pointer to source data array X
                       Src2,                      // 'input_matrix_Y':  Pointer to source data array Y
                       Dst,                       // 'result_matrix_R': Pointer to the resulting 2-dimensional data array
                       3,           // 'row_count':       Number of rows in input and output matrices
                       3);          // 'column_count':    Number of columns in input and output matrices

  printf ("Matrix / matrix addition: R = X + Y\n");
  printf ("%lf, %lf, %lf\n", F24 (Dst[0]), F24 (Dst[1]), F24 (Dst[2]));
  printf ("%lf, %lf, %lf\n", F24 (Dst[3]), F24 (Dst[4]), F24 (Dst[5]));
  printf ("%lf, %lf, %lf\n", F24 (Dst[6]), F24 (Dst[7]), F24 (Dst[8]));
  printf ("\n");

                                                  // Matrix / matrix subtraction: R = X - Y
  dsp_matrix_subm (Src1,                      // 'input_matrix_X':  Pointer to source data array X
                       Src2,                      // 'input_matrix_Y':  Pointer to source data array Y
                       Dst,                       // 'result_matrix_R': Pointer to the resulting 2-dimensional data array
                       3,           // 'row_count':       Number of rows in input and output matrices
                       3);          // 'column_count':    Number of columns in input and output matrices

  printf ("Matrix / matrix subtraction: R = X - Y\n");
  printf ("%lf, %lf, %lf\n", F24 (Dst[0]), F24 (Dst[1]), F24 (Dst[2]));
  printf ("%lf, %lf, %lf\n", F24 (Dst[3]), F24 (Dst[4]), F24 (Dst[5]));
  printf ("%lf, %lf, %lf\n", F24 (Dst[6]), F24 (Dst[7]), F24 (Dst[8]));
  printf ("\n");

                                                  // Matrix / matrix multiplication: R = X * Y

                                                  // Matrix transposition
  dsp_matrix_transpose (Src1,                 // 'input_matrix_X':  Pointer to source data array
                            Dst,                  // 'result_matrix_R': Pointer to the resulting 2-dimensional data array
                            3,      // 'row_count':       Number of rows in input and output matrices
                            3,      // 'column_count':    Number of columns in input and output matrices
                            Q_N);                 // 'q_format':        Fixed point format, the number of bits making up fractional part

  printf ("Matrix transposition\n");
  printf ("%lf, %lf, %lf\n", F24 (Dst[0]), F24 (Dst[1]), F24 (Dst[2]));
  printf ("%lf, %lf, %lf\n", F24 (Dst[3]), F24 (Dst[4]), F24 (Dst[5]));
  printf ("%lf, %lf, %lf\n", F24 (Dst[6]), F24 (Dst[7]), F24 (Dst[8]));
  printf ("\n");

  return (0);
}

