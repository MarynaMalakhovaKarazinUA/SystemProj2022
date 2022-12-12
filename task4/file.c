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

typedef struct ring_buffer {
    int size;
    int AmountElem;

    char *data;
    int getIndex;
    int putIndex;

    bool IndexBehind;

    time_t time;
    int EntryId;
} ring_buffer;

void resetGetIndex(ring_buffer *temp) {
     if(temp->getIndex >= temp->size) {
        temp->getIndex = 0;
        temp->IndexBehind = false;
    }
}

void resetPutIndex(ring_buffer *temp) {
     if(temp->putIndex >= temp->size) {
        temp->putIndex = 0;
        temp->IndexBehind = true;
    }
}

int isEmpty(ring_buffer *temp) {
    if(temp->AmountElem == 0) {
        return 0;
    }
    return 1;
}

void pushBuffer(ring_buffer *temp, int value) {
    if(temp->getIndex == temp->putIndex && temp->IndexBehind == true) {
        temp->getIndex++;
        resetGetIndex(temp);
    }

    temp->putIndex++;
    temp->data[temp->putIndex - 1] = value;

    if(temp->AmountElem < temp->size)
        temp->AmountElem++;
    
    resetPutIndex(temp);
}

int fromBuffer(ring_buffer *temp) {
    int res = temp->data[temp->getIndex];
    temp->data[temp->getIndex] = '\0';
    temp->getIndex++;
    temp->AmountElem--;

    if(temp->AmountElem == 0)
        temp->putIndex = 0;
    
    resetGetIndex(temp);
    return res;
}

void getAll(ring_buffer *temp) {
    for(int i = 0; i < temp->size; i++) {
        printf("%c ", temp->data[i]);
    }

    printf("\n");
}

time_t getCurrentDate() {
    time_t t;
    time(&t);
    
    return t;
}

void showCurrentTime(ring_buffer *temp) {
    time_t t = temp->time;
    printf("Поточні дата та час: %s", ctime(&t));
}

void saveIntoFile(ring_buffer *temp) {
    int file;

    file = open("task.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);

    if(file < 0) {
		printf("Помилка! Не вдалося відкрити файл:\n%s\n", strerror(errno));
		return;
	}

    for(int i = 0; i < temp->size; i++) {
        if(temp->data[i] != '\0') {
            write(file, &temp->data[i], 1);
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
    buff->IndexBehind = false;
    buff->AmountElem = 0;
    buff->time = getCurrentDate();
    buff->EntryId = 0;

    return buff;
} 

int main() {
    int size;

    printf("Введіть розмір буфера:\n >>> ");

    if(scanf("%d", &size) < 0) 
        ErrorInput();
    
    ring_buffer *buff = buff_init(size);

    int ch = 0;

    while(1) {
        buff->time = getCurrentDate();

        printf("\tГОЛОВНЕ МЕНЮ:\n1-Додати значення\n2-Показати значення\n3-Показати буфер\n4-Показати поточну дату та час\n0-Вихід\n >>> ");
        scanf("%d", &ch);

        switch(ch) {
            case 1: ;
                char user_value;
                printf("Введіть значення:\n >>> ");

                if(scanf("%c", &user_value) < 0) {
                    printf("Помилка! Введено невірне значення!\n");
                    exit(1);
                }

                pushBuffer(buff, user_value);
                buff->EntryId++;
                break;

            case 2: ;
                if(isEmpty(buff) == 0) {
                    printf("Буфер пустий!\n");
                    break;
                }

                char res = fromBuffer(buff);
                printf("Отримане значення: %c\n", res);
                break;

            case 3:
                getAll(buff);
                break;

            case 4:
                showCurrentTime(buff);
                break;   

            case 0:
                saveIntoFile(buff);
                exit(0);
                break;

            default:
                printf("Помилка! Ви ввели значення, якого немає в меню!\n");                    
        }
    }
}