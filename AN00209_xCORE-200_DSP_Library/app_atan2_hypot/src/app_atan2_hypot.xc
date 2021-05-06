// Copyright 2015-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// XMOS DSP Library - DCT Functions test program

#include <stdio.h>
#include <stdlib.h>
#include <dsp_math.h>
#include <math.h>
#include <xs1.h>

static int atan2_fails = 0, atan2_tests = 0;

static void single_atan2_test(int y, int x, unsigned hypot, int angle, int precision, int angle_precision) {
    int hypot_precision = 24 - precision;
    int z[2];
    z[0] = x;
    z[1] = y;
    dsp_math_atan2_hypot(z, precision);
    unsigned hypotdiff = abs(((unsigned)z[0]) - hypot);
    unsigned anglediff = abs(((unsigned)z[1]) - angle);
    unsigned hypotlimit = (hypot >> (hypot_precision)) + ((hypot >> (hypot_precision-1))&1) - 1;
    unsigned anglelimit = (0x60000000 >> angle_precision);
    
    if (hypotdiff > hypotlimit) {
        printf("%08x %08x; Hypot %08x expected %08x\n", y, x, z[0], hypot);
        printf("atan2_hypot: Hypot diff %u limit is %d\n", hypotdiff,hypotlimit);
        atan2_fails++;
    }
    if (anglediff > anglelimit) {
        printf("%08x %08x Angle %08x expected %08x\n", y, x, z[1], angle);
        
        printf("atan2_angle: Angle diff %u limit is %d\n", anglediff, anglelimit);
        atan2_fails++;
    }
    atan2_tests++;
    printf("atan2 test %d\n", atan2_tests);

}


static const unsigned random_poly = 0xEDB88320; 
static unsigned random_val = 0x12345678; //seed
int get_random_number(void)
{
  crc32(random_val, -1, random_poly);
  return (int)random_val;
}

#define SQRT2(X) ((unsigned)(1.414213562*(X)))
void atan2_test(void) {
    single_atan2_test(1000,1000, SQRT2(1000), 0x10000000, 0, 25);
    single_atan2_test(0xFFFFFF,0xFFFFFF, SQRT2(0xFFFFFF), 0x10000000, 0, 25);
    single_atan2_test(0xFFFFFFF,0xFFFFFFF, SQRT2(0xFFFFFFF), 0x10000000, 0, 25);
    single_atan2_test(0x3FFFFFFF,0x3FFFFFFF, SQRT2(0x3FFFFFFF), 0x10000000, 0, 25);
    single_atan2_test(0x7FFFFFFF,0x7FFFFFFF, SQRT2(0x7FFFFFFF), 0x10000000, 0, 25);
    single_atan2_test(0x7FFFFFFF,-0x7FFFFFFF, SQRT2(0x7FFFFFFF), 0x30000000, 0, 25);
    single_atan2_test(-0x7FFFFFFF,0x7FFFFFFF, SQRT2(0x7FFFFFFF), -0x10000000, 0, 25);
    single_atan2_test(-0x7FFFFFFF,-0x7FFFFFFF, SQRT2(0x7FFFFFFF), -0x30000000, 0, 25);
    for(int i = 0; i < 23; i++) {
        single_atan2_test(1200000000, 1600000000, 2000000000, 0x0d1bfaf9, i, 24-i);
    }
    
    for(int i = 0; i < PI2_Q8_24; i += PI2_Q8_24/391) {
        int angle = (((long long) i) << 31) / PI2_Q8_24;
        if (angle >= (1 << 30)) {
            angle -= (1 << 31);
        }
        int s = dsp_math_sin(i);
        int c = dsp_math_cos(i);
        single_atan2_test(s*127, c*127, 127<<24, angle, 0, 24);
        single_atan2_test(s/13, c/13, (1<<24)/13, angle, 0, 22);
    }
    
    for(int i = 0; i < 100; i += 1){
        int x = get_random_number();    //Returns random number from INT_MIN to INT_MAX
        int y = get_random_number();
        if (x < -0x7FFFFFFF) x = -0x7FFFFFFF;   //Clip as arctan2 cannot take INT_MIN
        if (y < -0x7FFFFFFF) y = -0x7FFFFFFF;

        int hypot = (int)( sqrt( (double)x * (double)x + (double)y * (double)y ) );
        int angle = (int)( (atan2((double)y, (double)x)) * (0x40000000 / 3.1415926536f) );
        single_atan2_test(y, x, hypot, angle, 0, 24); //We cannot quite reach 25b on random points so set limit to 24b
    } 
    printf("Atan2: %d out of %d passes\n", atan2_tests-atan2_fails, atan2_tests);
}

int main(void) {
    atan2_test();
    return 0;
}
