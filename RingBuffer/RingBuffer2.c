#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int size;
    printf("Enter size of buffer: ");
    scanf(" %d", &size);
    ring_buffer *buff = buff_init(size);

    int choice = 0;

    while (1)
    {
        buff->time = getCurrentDate();
        printf("1.Add value\n");
        printf("2.Get value\n");
        printf("3.Show the buffer\n");
        printf("4.Get current time\n");
        printf("0.Save and exit\n");
        printf(">> ");
        scanf(" %d", &choice);

        switch (choice)
        {
        case 1:
            char user_value;
            printf("Enter the value: ");
            scanf(" %c", &user_value);
            pushToBuff(buff, user_value);
            buff->record_id++;
            break;
        case 2:
            if (isEmpty(buff) == 0)
            {
                printf("No elements in buffer\n");
                break;
            }
            char res = popFromBuff(buff);
            printf("Value: %c\n", res);
            break;
        case 3:
            getAll(buff);
            break;
        case 4:
            showCurrentTime(buff);
            break;
        case 0:
            saveToFile(buff);
            exit(0);
            break;
        default:
            printf("There isn`t such option\n");
        }
    }
}

typedef struct ring_buffer {
    int size;
    int cur_elemnts_amount;

    char *data;
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
    buf->data[buf->getIndex] = '\0';
    buf->getIndex++;
    buf->cur_elemnts_amount--;
    if(buf->cur_elemnts_amount == 0)
        buf->putIndex = 0;
    resetGetIndexIfOutOfBounds(buf);
    return res;
}

void getAll(ring_buffer *buf) {
    for (int i = 0; i < buf->size; i++) {
        printf("%c ", buf->data[i]);
    }
    printf("\n");
}

time_t getCurrentDate() {
    time_t t;
    time(&t);
    return t;
}

void showCurrentTime(ring_buffer *buf) {
    time_t t = buf->time;
    printf("Current date and time is : %s", ctime(&t));
}

void saveToFile(ring_buffer *buf) {
    int file;
    file = open("bufferFile.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
    if (file < 0) {
		printf("Failed to open destiantion file: %s\n", strerror(errno));
		return;
	}
    for (int i = 0; i < buf->size; i++) {
        if (buf->data[i] != '\0') {
            write(file, &buf->data[i], 1);
        }
    }   
    close(file);
}

ring_buffer* buff_init(int size) {
    ring_buffer *buff = malloc(sizeof(ring_buffer));
    buff->size = size;
    buff->data = (char *) malloc(size * sizeof(char));
    buff->getIndex = 0;
    buff->putIndex = 0;
    buff->putIndex_was_behind = false;
    buff->cur_elemnts_amount = 0;
    buff->time = getCurrentDate();
    buff->record_id = 0;
    return buff;
} 
