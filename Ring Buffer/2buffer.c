#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>

typedef struct ring_buffer {
    int size;
    int cur_elemnts_amount;

    int *data;
    int getIndex;
    int putIndex;
    
    bool putIndex_was_behind;

    time_t time;
    int record_id;
}ring_buffer;

void resetGetIndexIfOutOfBounds(ring_buffer *buf) {
     if(buf->getIndex >= buf->size) {
        buf->getIndex = 0;
        buf->putIndex_was_behind = false;
    }
}

void resetPutIndexIfOutOfBounds(ring_buffer *buf) {
     if(buf->putIndex >= buf->size) {
        buf->putIndex = 0;
        buf->putIndex_was_behind = true;
    }
}

int isEmpty(ring_buffer *buf) {
    if(buf->cur_elemnts_amount == 0) {
        return 0;
    }
    return 1;
}

void pushToBuff(ring_buffer *buf, int value) {
    if (buf->getIndex == buf->putIndex && buf->putIndex_was_behind == true) {
        buf->getIndex++;
        resetGetIndexIfOutOfBounds(buf);
    }
    buf->putIndex++;
    buf->data[buf->putIndex - 1] = value;
    if(buf->cur_elemnts_amount < buf->size)
        buf->cur_elemnts_amount++;
    resetPutIndexIfOutOfBounds(buf);
}

int popFromBuff(ring_buffer *buf) {
    int res = buf->data[buf->getIndex];
    buf->data[buf->getIndex] = 0;
    buf->getIndex++;
    buf->cur_elemnts_amount--;
    if(buf->cur_elemnts_amount == 0)
        buf->putIndex = 0;
    resetGetIndexIfOutOfBounds(buf);
    return res;
}

void getAll(ring_buffer *buf) {
    for (int i = 0; i < buf->size; i++) {
        printf("%d ", buf->data[i]);
    }
    printf("\n");
}

time_t getCurrentDate() {
    time_t t;
    time(&t);
    return t;
}

ring_buffer* buff_init(int size) {
    ring_buffer *buff = malloc(sizeof(ring_buffer));
    buff->size = size;
    buff->data = (int *) malloc(size * sizeof(int));
    buff->getIndex = 0;
    buff->putIndex = 0;
    buff->putIndex_was_behind = false;
    buff->cur_elemnts_amount = 0;
    buff->time = getCurrentDate();
    buff->record_id = 0;
    return buff;
} 

int main() {
    int size;
    printf("Enter size of buffer: ");
    scanf(" %d", &size);
    ring_buffer *buff = buff_init(size);

    int choice = 0;

    while(1) {
        printf("1.Add value\n");
        printf("2.Get value\n");
        printf("3.Show the buffer\n");
        printf("4.Exit\n");
        scanf(" %d", &choice);

        switch(choice) {
            case 1:
                buff->time = getCurrentDate();
                int user_value;
                printf("Enter the value: ");
                scanf(" %d", &user_value);   
                pushToBuff(buff, user_value);
                buff->record_id++;
                break;
            case 2:
                buff->time = getCurrentDate();
                if (isEmpty(buff) == 0) {
                    printf("No elements in buffer\n");
                    break;
                }
                int res = popFromBuff(buff);
                printf("Value: %d\n", res);
                break;
            case 3:
                buff->time = getCurrentDate();
                getAll(buff);
                break;
            case 4:
                buff->time = getCurrentDate();
                exit(1);
                break;
            default:                    
        }
    }
