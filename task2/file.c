#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void addQueue(int, int*, int*, int, int*);
void prQueue();

int main(){
    int size;

    printf("\nВведіть розмір масива\nЩоб вийти введіть значення що меньше, або дорінює нулю!\n\n >>> ");
    scanf("%d", &size);

    if(size <= 0) {
        return 0;
    }

    int qu[size];
    int rear = -1;
    int ch;
    int inf = 1;
   
    int value;
    int m = 0;
    
    while (inf == 1) {
        printf("\n\tГОЛОВНЕ МЕНЮ\n1-Додати елемент до масива\n2-Вивести список\n0-Вийти\n\n >>> ");
        
        scanf("%d", &ch);

        switch (ch) {
            case 1:
                printf("\nВведіть число, яке хочете додати до масиву: ");
                scanf("%d", &value);
                addQueue(value, qu, &rear, size, &m);
                break;

            case 2:
                prQueue(qu, &rear, size, &m);
                break;

            case 0:
                exit(0);
                break;

            default:
                printf("\nВи ввели невірне знаення, якого немає в меню.\nПовторіть спробу!\n");
                continue;
        }
    }   
    return 0;
}

