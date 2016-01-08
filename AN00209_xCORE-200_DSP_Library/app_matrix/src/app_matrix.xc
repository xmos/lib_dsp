// Copyright (c) 2016, XMOS Ltd, All rights reserved
// XMOS DSP Library - Matrix Functions Test Program
// Uses Q24 format

// Include files
#include <stdio.h>
#include <xs1.h>
#include <lib_dsp.h>

// Define constants

#define Q_M               8
#define Q_N               24

#define MATRIX_NUM_ROWS   3
#define MATRIX_NUM_COLS   3

// Declare global variables and arrays
int  Src1[] = { Q24(.11), Q24(.12), Q24(.13),
                Q24(.21), Q24(.22), Q24(.23),
                Q24(.31), Q24(.32), Q24(.33)};
int  Src2[] = { Q24(.41), Q24(.42), Q24(.43),
                Q24(.51), Q24(.52), Q24(.53),
                Q24(.61), Q24(.62), Q24(.63)};
int           Dst[MATRIX_NUM_ROWS*MATRIX_NUM_COLS];

int main(void)
{

                                                  // Matrix negation: R = -X
  lib_dsp_matrix_negate (Src1,                    // 'input_matrix_X':  Pointer/reference to source data
                         Dst,                     // 'result_matrix_R': Pointer to the resulting 2-dimensional data array
                         MATRIX_NUM_ROWS,         // 'row_count':       Number of rows in input and output matrices
                         MATRIX_NUM_COLS);        // 'column_count':    Number of columns in input and output matrices

  printf ("Matrix negation: R = -X\n");
  printf ("%lf, %lf, %lf\n", F24 (Dst[0]), F24 (Dst[1]), F24 (Dst[2]));
  printf ("%lf, %lf, %lf\n", F24 (Dst[3]), F24 (Dst[4]), F24 (Dst[5]));
  printf ("%lf, %lf, %lf\n", F24 (Dst[6]), F24 (Dst[7]), F24 (Dst[8]));

                                                  // Matrix / scalar addition: R = X + a
  lib_dsp_matrix_adds (Src1,                      // 'input_matrix_X':  Pointer/reference to source data
                       Q24(2.),                   // 'scalar_value_A':  Scalar value to add to each 'input' element
                       Dst,                       // 'result_matrix_R': Pointer to the resulting 2-dimensional data array
                       MATRIX_NUM_ROWS,           // 'row_count':       Number of rows in input and output matrices
                       MATRIX_NUM_COLS);          // 'column_count':    Number of columns in input and output matrices

  printf ("Matrix / scalar addition: R = X + a\n");
  printf ("%lf, %lf, %lf\n", F24 (Dst[0]), F24 (Dst[1]), F24 (Dst[2]));
  printf ("%lf, %lf, %lf\n", F24 (Dst[3]), F24 (Dst[4]), F24 (Dst[5]));
  printf ("%lf, %lf, %lf\n", F24 (Dst[6]), F24 (Dst[7]), F24 (Dst[8]));

                                                  // Matrix / scalar multiplication: R = X * a
  lib_dsp_matrix_muls (Src1,                      // 'input_matrix_X':  Pointer/reference to source data
                       Q24(2.),                   // 'scalar_value_A':  Scalar value to multiply each 'input' element by
                       Dst,                       // 'result_matrix_R': Pointer to the resulting 2-dimensional data array
                       MATRIX_NUM_ROWS,           // 'row_count':       Number of rows in input and output matrices
                       MATRIX_NUM_COLS,           // 'column_count':    Number of columns in input and output matrices
                       Q_N);                      // 'q_format':        Fixed point format, the number of bits making up fractional part

  printf ("Matrix / scalar multiplication: R = X + a\n");
  printf ("%lf, %lf, %lf\n", F24 (Dst[0]), F24 (Dst[1]), F24 (Dst[2]));
  printf ("%lf, %lf, %lf\n", F24 (Dst[3]), F24 (Dst[4]), F24 (Dst[5]));
  printf ("%lf, %lf, %lf\n", F24 (Dst[6]), F24 (Dst[7]), F24 (Dst[8]));

                                                  // Matrix / matrix addition: R = X + Y
  lib_dsp_matrix_addm (Src1,                      // 'input_matrix_X':  Pointer to source data array X
                       Src2,                      // 'input_matrix_Y':  Pointer to source data array Y
                       Dst,                       // 'result_matrix_R': Pointer to the resulting 2-dimensional data array
                       MATRIX_NUM_ROWS,           // 'row_count':       Number of rows in input and output matrices
                       MATRIX_NUM_COLS);          // 'column_count':    Number of columns in input and output matrices

  printf ("Matrix / matrix addition: R = X + Y\n");
  printf ("%lf, %lf, %lf\n", F24 (Dst[0]), F24 (Dst[1]), F24 (Dst[2]));
  printf ("%lf, %lf, %lf\n", F24 (Dst[3]), F24 (Dst[4]), F24 (Dst[5]));
  printf ("%lf, %lf, %lf\n", F24 (Dst[6]), F24 (Dst[7]), F24 (Dst[8]));

                                                  // Matrix / matrix subtraction: R = X - Y
  lib_dsp_matrix_subm (Src1,                      // 'input_matrix_X':  Pointer to source data array X
                       Src2,                      // 'input_matrix_Y':  Pointer to source data array Y
                       Dst,                       // 'result_matrix_R': Pointer to the resulting 2-dimensional data array
                       MATRIX_NUM_ROWS,           // 'row_count':       Number of rows in input and output matrices
                       MATRIX_NUM_COLS);          // 'column_count':    Number of columns in input and output matrices

  printf ("Matrix / matrix subtraction: R = X - Y\n");
  printf ("%lf, %lf, %lf\n", F24 (Dst[0]), F24 (Dst[1]), F24 (Dst[2]));
  printf ("%lf, %lf, %lf\n", F24 (Dst[3]), F24 (Dst[4]), F24 (Dst[5]));
  printf ("%lf, %lf, %lf\n", F24 (Dst[6]), F24 (Dst[7]), F24 (Dst[8]));

                                                  // Matrix / matrix multiplication: R = X * Y
  lib_dsp_matrix_mulm (Src1,                      // 'input_matrix_X':  Pointer to source data array X
                       Src2,                      // 'input_matrix_Y':  Pointer to source data array Y
                       Dst,                       // 'result_matrix_R': Pointer to the resulting 2-dimensional data array
                       MATRIX_NUM_ROWS,           // 'row_count':       Number of rows in input and output matrices
                       MATRIX_NUM_COLS,           // 'column_count':    Number of columns in input and output matrices
                       Q_N);                      // 'q_format':        Fixed point format, the number of bits making up fractional part

  printf ("Matrix / matrix multiplication: R = X * Y\n");
  printf ("%lf, %lf, %lf\n", F24 (Dst[0]), F24 (Dst[1]), F24 (Dst[2]));
  printf ("%lf, %lf, %lf\n", F24 (Dst[3]), F24 (Dst[4]), F24 (Dst[5]));
  printf ("%lf, %lf, %lf\n", F24 (Dst[6]), F24 (Dst[7]), F24 (Dst[8]));

                                                  // Matrix transposition
  lib_dsp_matrix_transpose (Src1,                 // 'input_matrix_X':  Pointer to source data array
                            Dst,                  // 'result_matrix_R': Pointer to the resulting 2-dimensional data array
                            MATRIX_NUM_ROWS,      // 'row_count':       Number of rows in input and output matrices
                            MATRIX_NUM_COLS,      // 'column_count':    Number of columns in input and output matrices
                            Q_N);                 // 'q_format':        Fixed point format, the number of bits making up fractional part

  printf ("Matrix transposition\n");
  printf ("%lf, %lf, %lf\n", F24 (Dst[0]), F24 (Dst[1]), F24 (Dst[2]));
  printf ("%lf, %lf, %lf\n", F24 (Dst[3]), F24 (Dst[4]), F24 (Dst[5]));
  printf ("%lf, %lf, %lf\n", F24 (Dst[6]), F24 (Dst[7]), F24 (Dst[8]));

  return (0);
}

