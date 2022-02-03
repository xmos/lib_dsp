#include <stdio.h>
#include <stdint.h>

float f[1000];
float g[1000];
extern float dsp_vect_dot_prod_xs3(float *ptr1, float *ptr2, uint32_t N);
extern float dsp_vect_dot_prod_fast_xs3(float *ptr1, float *ptr2, uint32_t N);

float dot_ref(float *ptr1, float *ptr2, uint32_t N) {
    float sum = 0;
    for(int i = 0; i < N; i++) {
        sum += ptr1[i] * ptr2[i];
    }
    return sum;
}

void test(float *ptr1, float *ptr2, int N) {
    for(int i = 0; i < N; i++) {
        ptr1[i] = i + 10.0;
    }
    for(int i = 0; i < N; i++) {
        ptr2[i] = i + 13.0;
    }
    int t0, t1, t2, t3;
    asm volatile("gettime %0" : "=r" (t0));
    float z1 = dsp_vect_dot_prod_xs3(ptr1, ptr2, N);
    asm volatile("gettime %0" : "=r" (t1));
    float z2 = dsp_vect_dot_prod_fast_xs3(ptr1, ptr2, N);
    asm volatile("gettime %0" : "=r" (t2));
    float z0 = dot_ref(ptr1, ptr2, N);
    asm volatile("gettime %0" : "=r" (t3));
    printf("%f %f %f %g %g\n", z0, z1, z2, z0-z1, z0 - z2);
    printf("%d %d %d\n", t1-t0, t2-t1, t3-t2);
}

int main(void) {
    test(f, g, 600);
    test(f, g, 601);
    test(f+1, g, 600);
    test(f+1, g, 601);
    test(f, g+1, 600);
    test(f, g+1, 601);
    test(f+1, g+1, 600);
    test(f+1, g+1, 601);
    return 0;
}
