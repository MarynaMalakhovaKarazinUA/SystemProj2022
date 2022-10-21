#include <stdlib.h>
#include <stdio.h>

int getIndex = 0;
int putIndex = 0;

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
        if ((putIndex - 1)  == getIndex) {
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

void getAll(int buff[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", buff[i]);
    }
    printf("\n");
}

int main() {
    int size;
    printf("Enter size of buffer: ");
    scanf(" %d", &size);
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
        scanf(" %d", &choice);

        switch(choice) {
            case 1:
                int value;
                printf("Enter the value: ");
                scanf(" %d", &value);   
                addFunction(buff, size, value);
                break;
            case 2:
                int res = getFunction(buff, size);
                printf("Value: %d\n", res);
                break;
            case 3:
                getAll(buff, size);
                break;
            case 4:
                exit(1);
                break;
            default:                    
        }
    }
}
