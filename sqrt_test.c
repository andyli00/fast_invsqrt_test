#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NUM 1000

float Q_rsqrt(float number) {
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y = number;
    i = * (long *) &y;                      // evil floating point bit level hacking
    i = 0x5f3759df - (i >> 1);
    y = * (float *) &i;
    y = y * (threehalfs - (x2 * y * y));    // 1st iteration
    y = y * (threehalfs - (x2 * y * y));    // 2nd iteration, can be removed

    return y;
}

int main() {
    float rands[NUM];
    float results_approx[NUM];
    float results_accurate[NUM];
    float rel_errs[NUM];
    long fast_times[NUM];
    long slow_times[NUM];

    srand(time(NULL));

    for (int i = 0; i < NUM; i++) {
        rands[i] = (float)rand() / RAND_MAX;

        struct timespec before, after;

        clock_gettime(CLOCK_REALTIME, &before);
        results_accurate[i] = 1 / sqrt(rands[i]);
        clock_gettime(CLOCK_REALTIME, &after);
        slow_times[i] = after.tv_nsec - before.tv_nsec;
        
        clock_gettime(CLOCK_REALTIME, &before);
        results_approx[i] = Q_rsqrt(rands[i]);
        clock_gettime(CLOCK_REALTIME, &after);
        fast_times[i] = after.tv_nsec - before.tv_nsec;

        rel_errs[i] = fabs((results_accurate[i] - results_approx[i]) / results_accurate[i]);
    }

    long fast_avg = 0;
    long slow_avg = 0;
    float rel_err_avg = 0.0;

    for (int i = 0; i < NUM; i++) {
        fast_avg += fast_times[i];
        slow_avg += slow_times[i];
        rel_err_avg += rel_errs[i];
    }

    fast_avg /= NUM;
    slow_avg /= NUM;
    rel_err_avg /= NUM;

    printf("Average time for algorithm: %ld\n", fast_avg);
    printf("Average time for traditional approach: %ld\n", slow_avg);
    printf("Average relative error: %f\n", rel_err_avg);

    return 0;
}