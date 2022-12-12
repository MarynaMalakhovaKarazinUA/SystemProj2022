#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

int getIndex = 0;
int putIndex = 0;
int AmountElem = 0;

void functAdd(int buff[], int size, int value) {
    if(!(putIndex == 0 && getIndex == 0)) {
        if(putIndex == getIndex) {
            getIndex++;
            if(getIndex >= size) {
                getIndex = 0;
            }
        }
    }

    putIndex++;

    if(putIndex > size) {
        putIndex = 1;
        if((putIndex - 1) == getIndex) {
            getIndex++;
            if(getIndex >= size) {
                getIndex = 0;
            }
        }
    }

    buff[putIndex - 1] = value;
}

int functGet(int buff[], int size) {
    int res = buff[getIndex];

    buff[getIndex] = 0;
    getIndex++;

    if(getIndex >= size) {
        getIndex = 0;
    }

    return res; 
}

void getAll(int buff[]) {
    for (int i = 0; i < AmountElem; i++) {
        printf("%d ", buff[i]);
    }

    printf("\n");
}

void printError() {
    printf("Помилка! Ви ввели невірне значення!\n");
}

int main() {
    signal(SIGABRT, &printError);

    int size;

    printf("Введіть розмір буферу:\n >>> ");

    if(scanf(" %d", &size) < 0)
        abort();    

    if(size < 0)
        abort(); 

    int buff[size];

    int ch = 0;

    while(1) {
        printf("Введений Індекс: %d\n", putIndex);
        printf("Отриманий Індекс: %d\n", getIndex);

        printf("\tГОЛОВНЕ МЕНЮ:\n1-Додати значення\n2-Показати значення\n3-Показати та очистити буфер\n0-Вихід\n >>> ");

        scanf("%d", &ch);

        switch(ch) {
            case 1: ;
                int value;

                printf("Введіть значення:\n >>> ");

                if(scanf("%d", &value) < 0)
                    abort();   

                functAdd(buff, size, value);
                AmountElem++;
                break;

            case 2: ;
                int temp = functGet(buff, size);

                printf("Значення: %d\n", temp);
                AmountElem--;
                break;

            case 3:
                getAll(buff);
                break;

            case 0:
                exit(1);
                break;    

            default:
                printf("Помилка! Ви ввели значення, якого немає в меню!\n");                 
        }
    }
}