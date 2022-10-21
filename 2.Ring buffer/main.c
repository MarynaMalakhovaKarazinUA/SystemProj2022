#include <stdio.h>
#include <stdlib.h>
#include <stdio_ext.h>

typedef struct RingBuffers {
    int size;
    int RD_index;
    int WR_index;
    char *array;
} RingBuffer;

RingBuffer *createRingBuffer(int size);
void readRB(RingBuffer *rb, char *c);
void writeRB(RingBuffer *rb, char c);
void printRB(RingBuffer *rb);

int main() {
    RingBuffer *ringBuffer = NULL;

    while(1) {
        printf("1 - Create ring buffer\n");
        if(ringBuffer != NULL) {
            printf("2 - Read from buffer\n");
            printf("3 - Write to buffer\n");
            printf("4 - Print buffer\n");
        }
        printf(">> ");
        int m = 0;
        __fpurge(stdin);
        int res = scanf("%d", &m);
        if(res == 0) {
            printf("Wrong menu item.\n");
            continue;
        }
        switch(m) {
            case 1: 
                if(ringBuffer != NULL) {
                    free(ringBuffer->array);
                    free(ringBuffer);
                    ringBuffer = NULL;
                }
                printf("\n\nEnter size: ");
                int size = 0;

                __fpurge(stdin);
                if(scanf("%d", &size) != 1 || size <= 0) {
                    printf("\nWrong size. Try again.\n\n");
                    break;
                }

                ringBuffer = createRingBuffer(size);
                printf("\nDone!\n\n");
                break;
            case 2: 
                if(ringBuffer == NULL) 
                    break;
                char c;
                readRB(ringBuffer, &c);
                printf("\n\nOutput: %c\n\n", c);
                break;
            case 3: 
                if(ringBuffer == NULL) 
                        break;
                printf("\n\nEnter char: ");
                char tmp;
                __fpurge(stdin);
                if(scanf(" %c", &tmp) != 1) {
                    printf("\nYou entered wrong char. Try again.\n\n");
                    break;
                }
                writeRB(ringBuffer, tmp);
                printf("\nDone!\n\n");
                break;
            case 4: 
                if(ringBuffer == NULL) 
                    break;
                printf("\n\nBuffer: ");
                printRB(ringBuffer);
                break;
            default: 
                break;
        } 
    }

    return 0;     
}

RingBuffer *createRingBuffer(int size) {
    RingBuffer *tmp = malloc(sizeof(RingBuffer));

    if(tmp == NULL) return NULL;

    tmp->size = size;
    tmp->RD_index = 0;
    tmp->WR_index = 0;
    tmp->array = malloc(size*sizeof(char));

    if(tmp->array == NULL) return NULL; 

    return tmp;
}

void readRB(RingBuffer *rb, char *c) {
    *c = rb->array[rb->RD_index];

    if(rb->RD_index == rb->size - 1) {
        rb->RD_index = 0;
    } else {
        rb->RD_index++;
    }
}

void writeRB(RingBuffer *rb, char c) {
    rb->array[rb->WR_index] = c;

    if(rb->WR_index == rb->size - 1) {
        rb->WR_index = 0;
    } else {
        rb->WR_index++;
    }
}

void printRB(RingBuffer *rb) {
    for(int i = 0; i < rb->size; i++) {
        printf("%c ", rb->array[i]);
    }

    printf("\n\n");
}
