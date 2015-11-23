awk '
BEGIN {
  header = "src/sine.h";
  impl = "src/sine.xc";
  max_bits = 12
  
  printf("#ifndef SINE_H_\n") > header;
  printf("#define SINE_H_\n")> header;
  for(j = 2; j < lshift(1, max_bits); j = j * 2) {
     printf("extern const int sine_%d[%d];\n", 4*j, j+1) > header;
  }
  printf("#define FFT_SINE0(N) sine_ ## N\n") > header;
  printf("#define FFT_SINE(N) FFT_SINE0(N)\n") > header;
  printf("#endif\n") > header;
  
  for(j = 2; j < lshift(1, max_bits); j = j * 2) {
    printf("const int sine_%d[%d] = {\n ", 4*j, j+1) > impl;
    for(i = 0; i < j; i++) {
      printf(" %10d,", sin(i*3.1415926535/2/j) * 1024*1024*1024*2) > impl;
      if (i % 4 == 3) printf("\n ") > impl;
    }
    printf(" %10d,", 1 * 1024*1024*1024*2-1) > impl;
    printf("\n};\n\n") > impl;
  }
}
'
