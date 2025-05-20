awk '
BEGIN {
  header = "sine_float.h";
  impl = "sine_float.c";
  max_points = 2^12
  #note: Twos complement means that MIN_INT - 1 is MAX_INT ! 
  min_int = 2^31
  min_short_int = 2^15


  printf("#ifndef SINE_H_\n") > header;
  printf("#define SINE_H_\n")> header;
  for(j = 2; j < max_points; j = j * 2) {
     printf("extern const float dsp_sine_float_%d[%d];\n", 4*j, j+1) > header;
  }
  printf("\n") > header;
  

  printf("// Copyright (c) 2022, XMOS Ltd, All rights reserved\n")  > impl;
  printf("\n")  > impl;


  for(j = 1; j < max_points; j = j * 2) {
    printf("const float dsp_sine_float_%d[%d] = {\n ", 4*j, j+1) > impl;
    for(i = 0; i <= j; i++) {
      printf(" %20.18f,", sin(i*3.1415926535/2/j)) > impl;
      if (i % 4 == 3) printf("\n ") > impl;
    }
    printf("\n};\n\n") > impl;
  }
}
'
