// Copyright (c) 2015-2016, XMOS Ltd, All rights reserved
// XMOS DSP Library - DCT Functions test program

#include <stdio.h>
#include <dsp_math.h>

void test(int x, int y) {
    int z[2];
    z[0] = y;
    z[1] = x;
    dsp_math_atan2_hypot(z, 0);
    printf("hypot(%10d,%10d) = %10u; atan2(%10d,%10d) = %08x\n", x, y, z[0], x, y, z[1]);

}

int main(void) {
    test(1000,1000);
    test(0xFFFFFF,0xFFFFFF);
    test(0xFFFFFFF,0xFFFFFFF);
    test(0x3FFFFFFF,0x3FFFFFFF);
    test(0x7FFFFFFF,0x7FFFFFFF);
    test(0,4243);
    test(3000,3000);
    test(4243,0);
    test(3000,-3000);
    test(0,-4243);
    test(-3000,-3000);
    test(-4243,0);
    test(-3000,3000);
    return 0;
}
