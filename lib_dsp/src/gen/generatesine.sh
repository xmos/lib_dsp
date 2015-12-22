awk '
BEGIN {
  header = "sine.h";
  impl = "sine.c";
  max_points = 2^12
  #note: Twos complement means that MIN_INT - 1 is MAX_INT ! 
  min_int = 2^31
  min_short_int = 2^15


  printf("#ifndef SINE_H_\n") > header;
  printf("#define SINE_H_\n")> header;
  for(j = 2; j < max_points; j = j * 2) {
     printf("extern const int lib_dsp_sine_%d[%d];\n", 4*j, j+1) > header;
  }
  printf("\n") > header;

  for(j = 2; j < max_points; j = j * 2) {
     printf("extern const short lib_dsp_sine_short_%d[%d];\n", 4*j, j+1) > header;
  }
  printf("#endif\n") > header;
  

  printf("// Copyright (c) 2015, XMOS Ltd, All rights reserved\n")  > impl;
  printf("\n")  > impl;


  for(j = 2; j < max_points; j = j * 2) {
    printf("const int lib_dsp_sine_%d[%d] = {\n ", 4*j, j+1) > impl;
    for(i = 0; i < j; i++) {
      printf(" %10d,", sin(i*3.1415926535/2/j) * min_int) > impl;
      if (i % 4 == 3) printf("\n ") > impl;
    }
    printf(" %10d,", 1 * min_int-1) > impl;
    printf("\n};\n\n") > impl;
  }
  for(j = 2; j < max_points; j = j * 2) {
    printf("const short lib_dsp_sine_short_%d[%d] = {\n ", 4*j, j+1) > impl;
    for(i = 0; i < j; i++) {
      printf(" %5d,", sin(i*3.1415926535/2/j) * min_short_int) > impl;
      if (i % 4 == 3) printf("\n ") > impl;
    }
    printf(" %5d,", 1 * min_short_int-1) > impl;
    printf("\n};\n\n") > impl;

  }
}
'
