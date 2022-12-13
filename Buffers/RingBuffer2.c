#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio_ext.h>

typedef struct ring_buffer {
    int size;
    int cur_elemnts_amount;
    char *data;
    int RD_index;
    int WR_index;
    bool WR_index_was_behind;
    struct tm time;
    int id;
} ring_buffer;

struct tm getCurrentTime();
ring_buffer* createRB(int);
void resetWrongWRIndex(ring_buffer*);
void writeRBE(ring_buffer*, int);
void resetWrongRBIndex(ring_buffer*);
int readRB(ring_buffer*);
void printRB(ring_buffer*);
char getStrTime(ring_buffer*);
void showCurrentTime(ring_buffer*);
void saveToFile(ring_buffer*);
int isEmpty(ring_buffer*);


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
    ring_buffer *buf = createRB(size);

    int menu = 0;
    while (1) {
        buf->time = getCurrentTime();
        printf("\nMenu");
        printf("\n1. Write to buffer");
        printf("\n2. Read from buffer");
        printf("\n3. Print buffer");
        printf("\n4. Get current time");
        printf("\n0. Save and exit");
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
                char RBE_value;
                printf("Enter the value you want to write in:");
                while (1)
                {
                    printf("\n-> ");
                    __fpurge(stdin);
                    if(scanf("%c", &RBE_value) != 1) {
                        printf("\nEntry error. Try again.\n");
                    } else {
                        break;
                    }
                } 
                writeRBE(buf, RBE_value);
                buf->id++;
                break;
            }
            case 2: {
                if (isEmpty(buf) == 0) {
                    printf("The buffer is empty.\n");
                    break;
                }
                char result = readRB(buf);
                printf("Read element: %c\n", result);
                break;
            }
            case 3: {
                printRB(buf);
                break;
            }
            case 4: {
                showCurrentTime(buf);
                break;
            }   
            case 0: {
                saveToFile(buf);
                exit(0);
                break;
            }
            default:
            {
                printf("There is no such item on the menu. Try again.\n");
                continue;
            }
        }
    }
}

struct tm getCurrentTime() {
    time_t s_time = time(NULL);     //read the system time
    struct tm *m_time;              //pointer to structure with local time
    m_time = localtime(&s_time);    //convert system time to local time
    return *m_time;
}

ring_buffer* createRB(int size) {
    ring_buffer *buf = malloc(sizeof(ring_buffer));
    if(buf == NULL) {
        return NULL;
    }
    buf->size = size;
    buf->data = (char *) malloc(size * sizeof(char));
    if(buf->data == NULL) {
        return NULL;
    }
    buf->RD_index = 0;
    buf->WR_index = 0;
    buf->WR_index_was_behind = false;
    buf->cur_elemnts_amount = 0;
    buf->time = getCurrentTime();
    buf->id = 0;
    return buf;
} 

void resetWrongWRIndex(ring_buffer *buf) {
     if(buf->WR_index >= buf->size) {
        buf->WR_index = 0;
        buf->WR_index_was_behind = true;
    }
}

void writeRBE(ring_buffer *buf, int value) {
    if (buf->RD_index == buf->WR_index && buf->WR_index_was_behind == true) {
        buf->RD_index++;
        resetWrongRBIndex(buf);
    }
    buf->WR_index++;
    buf->data[buf->WR_index - 1] = value;
    if(buf->cur_elemnts_amount < buf->size)
        buf->cur_elemnts_amount++;
    resetWrongWRIndex(buf);
}

void resetWrongRBIndex(ring_buffer *buf) {
     if(buf->RD_index >= buf->size) {
        buf->RD_index = 0;
        buf->WR_index_was_behind = false;
    }
}

int readRB(ring_buffer *buf) {
    int res = buf->data[buf->RD_index];
    buf->data[buf->RD_index] = '\0';
    buf->RD_index++;
    buf->cur_elemnts_amount--;
    if(buf->cur_elemnts_amount == 0)
        buf->WR_index = 0;
    resetWrongRBIndex(buf);
    return res;
}

void printRB(ring_buffer *buf) {
    if (isEmpty(buf) == 0) {
        printf("The buffer is empty.\n");
    } else {
        for (int i = 0; i < buf->size; i++) {
            printf("%c ", buf->data[i]);
        }
        printf("\n");
    }
}

void showCurrentTime(ring_buffer *buf) {
    struct tm *m_time = &buf->time;         //pointer to structure with local time
    char str_t[128] = "";                   //string to save the converted time to text
    strftime(str_t, 128, "%X", m_time);     //convert the local time to a text string
    printf("Current time: %s\n", str_t);
}

void saveToFile(ring_buffer *buf) {
    int outputFd;
    int openFlags = O_CREAT | O_WRONLY | O_TRUNC;
	mode_t filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    outputFd = open("RB2File.txt", openFlags, filePerms);
    if (outputFd < 0) {
		perror("Cannot open destiantion file");
		exit(5);
	}
    
    struct tm *m_time = &buf->time;         //pointer to structure with local time
    char str_t[128] = "";                   //string to save the converted time to text
    strftime(str_t, 128, "%X", m_time);     //convert the local time to a text string
    
    for (int i = 0; i < 128; i++) {
        if (str_t[i] != '\0') {
            write(outputFd, &str_t[i], 1);
        }
    }

    char* tmp = " writer <";

    for (int i = 0; i < 16; i++) {
        if (tmp[i] != '\0') {
            write(outputFd, &tmp[i], 1);
        } else {
            break;
        }
    }

    write(outputFd, &buf->id, 1);

    tmp = ">: ";

    for (int i = 0; i < 16; i++) {
        if (tmp[i] != '\0') {
            write(outputFd, &tmp[i], 1);
        } else {
            break;
        }
    }
    
    for (int i = 0; i < buf->size; i++) {
        if (buf->data[i] != '\0') {
            write(outputFd, &buf->data[i], 1);
        }
    }

    if (close(outputFd) == -1) {
		perror("Close output");
		exit(5);
	}
}

int isEmpty(ring_buffer *buf) {
    if(buf->cur_elemnts_amount == 0) {
        return 0;
    }
    return 1;
}
