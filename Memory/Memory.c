#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

int main() {
    struct timespec start, end;
    char* mem;
    int menu = 0;
    signed char return_back;
    long double pow_result = 0;
    while (1) {
        printf("\nMenu");
        printf("\n1. Allocate memory buffers x = 0..64, whose sizes are equal to 2^x");
        printf("\n2. Allocate memory buffers x = 0..64, whose sizes are equal to 2^(x + 1)");
        printf("\n0. Exit");
        while (1)
        {
            printf("\n-> ");
            __fpurge(stdin);
            if(scanf("%d", &menu) != 1) {
                printf("\nEntry error. Try again with the numbers.\n");
            } else {
                break;
            }
        }
        printf("\n");
        return_back = 0;
        switch (menu) {
            case 1: {
                while (!return_back) {
                        printf("Menu");
                        printf("\n1. Using the malloc function");
                        printf("\n2. Using the calloc function");
                        printf("\n3. Using the alloca function");
                        printf("\n0. Return back");
                        while (1)
                        {
                            printf("\n-> ");
                            __fpurge(stdin);
                            if(scanf("%d", &menu) != 1) {
                                printf("\nEntry error. Try again with the numbers.\n");
                            } else {
                                break;
                            }
                        }
                        printf("\n");
                        switch (menu) {
                            case 1: {
                                clock_gettime(CLOCK_REALTIME, &start);
                                for(int i = 0; i <= 64; i++) {
                                    pow_result = pow(2, i);
                                    mem = (char *) malloc((int) pow_result);
                                    free(mem);
                                }
                                clock_gettime(CLOCK_REALTIME, &end);
                                printf("malloc -> %ld ns\n", (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec));
                                break;
                            }
                            case 2: {
                                clock_gettime(CLOCK_REALTIME, &start);
                                for(int i = 0; i <= 64; i++) {
                                    pow_result = pow(2, i);
                                    mem = (char *) calloc((int) pow_result, 1);
                                    free(mem);
                                }
                                clock_gettime(CLOCK_REALTIME, &end);
                                printf("calloc -> %ld ns\n", (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec));
                                break;
                            }
                            case 3: {
                                clock_gettime(CLOCK_REALTIME, &start);
                                for(int i = 0; i <= 64; i++) {
                                    pow_result = pow(2, i);
                                    mem = (char *) alloca((int) pow_result);
                                    free(mem);
                                }
                                clock_gettime(CLOCK_REALTIME, &end);
                                printf("alloca -> %ld ns\n", (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec));
                                break;
                            }
                            case 0: {
                                return_back = 1;
                                break;
                            }
                            default:
                            {
                                printf("There is no such item on the menu. Try again.\n");
                                continue;
                            }
                        }
                }
                break;
            }
            case 2: {
                while (!return_back) {
                        printf("Menu");
                        printf("\n1. Using the malloc function");
                        printf("\n2. Using the calloc function");
                        printf("\n3. Using the alloca function");
                        printf("\n0. Return back");
                        while (1)
                        {
                            printf("\n-> ");
                            __fpurge(stdin);
                            if(scanf("%d", &menu) != 1) {
                                printf("\nEntry error. Try again with the numbers.\n");
                            } else {
                                break;
                            }
                        }
                        printf("\n");
                        switch (menu) {
                            case 1: {
                                clock_gettime(CLOCK_REALTIME, &start);
                                for(int i = 0; i <= 64; i++) {
                                    pow_result = pow(2, i+1);
                                    mem = (char *) malloc((int) pow_result);
                                    free(mem);
                                }
                                clock_gettime(CLOCK_REALTIME, &end);
                                printf("malloc -> %ld ns\n", (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec));
                                break;
                            }
                            case 2: {
                                clock_gettime(CLOCK_REALTIME, &start);
                                for(int i = 0; i <= 64; i++) {
                                    pow_result = pow(2, i+1);
                                    mem = (char *) calloc((int) pow_result, 1);
                                    free(mem);
                                }
                                clock_gettime(CLOCK_REALTIME, &end);
                                printf("calloc -> %ld ns\n", (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec));
                                break;
                            }
                            case 3: {
                                clock_gettime(CLOCK_REALTIME, &start);
                                for(int i = 0; i <= 64; i++) {
                                    pow_result = pow(2, i+1);
                                    mem = (char *) alloca((int) pow_result);
                                    free(mem);
                                }
                                clock_gettime(CLOCK_REALTIME, &end);
                                printf("alloca -> %ld ns\n", (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec));
                                break;
                            }
                            case 0: {
                                return_back = 1;
                                break;
                            }
                            default:
                            {
                                printf("There is no such item on the menu. Try again.\n");
                                continue;
                            }
                        }
                }
                break;
            }
            case 0: {
                return 0;
            }
            default:
            {
                printf("There is no such item on the menu. Try again.\n");
                continue;
            }
        }
    }
}
