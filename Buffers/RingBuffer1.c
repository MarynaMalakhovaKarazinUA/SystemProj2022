#include <stdio.h>
#include <stdlib.h>
#include <stdio_ext.h>

void writeRBE(int*, int, int*, int, int*);
int readRB(int*, int*, int);
void printRB(int*, int*, int, int*);

int main() {

    int size;
    printf("Enter ring buffer size:");
    while (1)
    {
        size = 0;
        printf("\n-> ");
        __fpurge(stdin);
        if(scanf("%d", &size) != 1 || size <= 0) {
            printf("\nWrong size. Try again.\n");
        } else {
            break;
        }
    }
    
    int ringBuffer[size];
    int last_RBE_index =- 1, last_read_RBE_index = 0;
    int RBE_value;
    int reloadFlag = 0;
    
    int menu;
    while (1) {
        printf("\nMenu");
        printf("\n1. Write to buffer");
        printf("\n2. Read from buffer");
        printf("\n3. Print buffer");
        printf("\n0. Exit");
        printf("\n-> ");
        scanf("%d", &menu);
        printf("\n");
        switch (menu) {
            case 1: {
                printf("Enter the number you want to write in:");
                while (1)
                {
                    printf("\n-> ");
                    __fpurge(stdin);
                    if(scanf("%d", &RBE_value) != 1) {
                        printf("\nEntry error. Try again with the numbers.\n");
                    } else {
                        break;
                    }
                }
                writeRBE(ringBuffer, RBE_value, &last_RBE_index, size, &reloadFlag);
                break;
            }
            case 2: {
                if (last_RBE_index == -1) {
                    printf("The buffer is empty.\n");
                    break;
                }
                int readElement = readRB(ringBuffer, &last_read_RBE_index, size);
                printf("Read element: %d", readElement);
                break;
            }
            case 3: {
                printRB(ringBuffer, &last_RBE_index, size, &reloadFlag);
                break;
            }
            case 0: {
                return 0;
            }
            default: {
                printf("There is no such item on the menu. Try again.\n");
                continue;
            }
        }
    }
}

int readRB(int *ringBuffer, int *last_read_RBE_index, int size) {
    int RBE_value = ringBuffer[*last_read_RBE_index];
    if (*last_read_RBE_index == size - 1) {
        (*last_read_RBE_index) = 0;
    } else {
        (*last_read_RBE_index)++;
    }
    return RBE_value; 
}

void writeRBE(int *ringBuffer, int RBE_value, int *last_RBE_index, int size, int *reloadFlag) {
    if (*last_RBE_index == size - 1) {
        *last_RBE_index = -1;
        *reloadFlag = 1;
    }
    (*last_RBE_index)++;
    ringBuffer[*last_RBE_index] = RBE_value;
    printf("\nElement entered: %d", RBE_value);
    printf("\n");
}


void printRB(int *ringBuffer, int *last_RBE_index, int size, int *reloadFlag) {
    if (*last_RBE_index == -1) { printf("The buffer is empty.\n"); }
    else {
        int tmp;
        if(*reloadFlag == 0) { tmp = *last_RBE_index + 1; }
        else { tmp = size; }
        for (int i = 0 ; i < tmp; i++) {
            printf("ELEMENT[%d] = %d\n", i, ringBuffer[i]);
        }
    }
}