#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

int main() {
    struct timespec start, end;
    char* mem;

    // ___________ 2^x ____________
    printf("____________ 2^x ____________\n");
    // malloc

    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i <= 64; i++) {
        mem = (char *) malloc((int) pow(2, i));
        free(mem);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("malloc >> %ld ns\n", (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec));

    // calloc

    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i <= 64; i++) {
        mem = (char *) calloc((int) pow(2, i), 1);
        free(mem);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("calloc >> %ld ns\n", (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec));

    // alloca

    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i <= 64; i++) {
        mem = (char *) alloca((int) pow(2, i));
        free(mem);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("alloca >> %ld ns\n", (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec));

    // ___________ 2^x+1 ____________
    printf("____________ 2^x+1 ____________\n");
    // malloc

    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i <= 64; i++) {
        mem = (char *) malloc((int) pow(2, i+1));
        free(mem);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("malloc >> %ld ns\n", (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec));

    // calloc

    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i <= 64; i++) {
        mem = (char *) calloc((int) pow(2, i+1), 1);
        free(mem);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("calloc >> %ld ns\n", (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec));

    // alloca

    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i <= 64; i++) {
        mem = (char *) alloca((int) pow(2, i+1));
        free(mem);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("alloca >> %ld ns\n", (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec));

    return 0;
}