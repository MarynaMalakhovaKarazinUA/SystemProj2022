#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

int getIndex = 0;
int putIndex = 0;
int curElements = 0;

void addFunction(int buff[],int size, int value) {
    if (!(putIndex == 0 && getIndex == 0)) {
        if (putIndex == getIndex) {
            getIndex++;
            if (getIndex >= size) {
                getIndex = 0;
            }
        }
    }
    putIndex++;
    if (putIndex > size) {
        putIndex = 1;
        if ((putIndex - 1) == getIndex) {
            getIndex++;
            if (getIndex >= size) {
                getIndex = 0;
            }
        }
    }
    buff[putIndex - 1] = value;
}

int getFunction(int buff[],int size) {
    int res = buff[getIndex];
    buff[getIndex] = 0;
    getIndex++;
    if (getIndex >= size) {
        getIndex = 0;
    }
    return res; 
}

void getAll(int buff[]) {
    for (int i = 0; i < curElements; i++) {
        printf("%d ", buff[i]);
    }
    printf("\n");
}

void mysigabort() {
    printf("Wrong input!\n");
    printf("The program was terminated.\n");
}

int main() {
    signal(SIGABRT, &mysigabort);

    int size;
    printf("Enter size of buffer: ");
    if (scanf(" %d", &size) < 0)
        abort();    
    if (size < 0)
        abort();         
    int buff[size];

    int choice = 0;

    while(1) {
        printf("putIndex: %d\n", putIndex);
        printf("getIndex: %d\n", getIndex);
        printf("1.Add value\n");
        printf("2.Get value\n");
        printf("3.Show and clean the buffer\n");
        printf("4.Exit\n");
        printf(">> ");
        if (scanf(" %d", &choice) < 0)
            abort();

        switch(choice) {
            case 1: ;
                int value;
                printf("Enter the value: ");
                if (scanf(" %d", &value) < 0)
                    abort();   
                addFunction(buff, size, value);
                curElements++;
                break;
            case 2: ;
                int res = getFunction(buff, size);
                printf("Value: %d\n", res);
                curElements--;
                break;
            case 3:
                getAll(buff);
                break;
            case 4:
                exit(1);
                break;                  
        }
    }
}
