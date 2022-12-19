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

typedef struct node {
    char value;
    struct node *next;
    struct node *prev;
    char writer;
    time_t time;
} node;

typedef struct ring_buffer {
    int size;
    int AmountElem;

    node *head;
    node *tail;
    int getIndex;
    int putIndex;

    bool IndexBehind;
    bool arrayFull;

    int currentWriter;
    int uAmount;
} ring_buffer;

void printError() {
    printf("Помилка! Ви ввели невірне значення!\n");
    exit(1);
}

time_t getCurrentDate() {
    time_t t;
    time(&t);

    return t;
}

void resetGetIndex(ring_buffer *temp) {
     if(temp->getIndex >= temp->AmountElem) {
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

void headInit(ring_buffer *temp, char uValue) {
    temp->head->value = uValue;
    temp->head->writer = temp->currentWriter;
    temp->head->prev = NULL;
    temp->head->next = NULL;
    temp->tail = temp->head;
    temp->head->time = getCurrentDate();
    temp->AmountElem++;
    temp->putIndex++;
}

ring_buffer* buff_init(int size, int uAmount) {
    ring_buffer *buff = malloc(sizeof(ring_buffer));

    buff->size = size;
    buff->head = (node*) malloc(sizeof(node));
    buff->tail = (node*) malloc(sizeof(node));
    buff->getIndex = 0;
    buff->putIndex = 0;
    buff->IndexBehind = false;
    buff->arrayFull = false;
    buff->AmountElem = 0;
    buff->uAmount = uAmount;
    buff->currentWriter = 1;

    return buff;
}

node* getNode(ring_buffer* temp, int index) {
    node* current = temp->head;

    int counter = 0;

    while (counter != index) {
        current = current->next;
        counter++;
    }

    return current;
}

void insertValueIntoEnd(ring_buffer* temp, char value) {
    node *node = malloc(sizeof(node));

    node->value = value;
    node->writer = temp->currentWriter;
    node->next = NULL;
    node->time = getCurrentDate();

    node->prev = temp->tail;
    temp->tail->next = node;
    temp->tail = node;
}

void insertValue(ring_buffer* temp, char value) {
    node *new_node;

    new_node = malloc(sizeof(node));
    new_node->value = value;
    new_node->writer = temp->currentWriter;
    new_node->next = NULL;
    new_node->time = getCurrentDate();

    node *cur = getNode(temp, temp->putIndex);

    if(cur->prev == NULL) {
        temp->head = new_node;
    } else {
        new_node->prev = cur->prev;
        cur->prev->next = new_node;
    }

    new_node->next = cur;
    cur->prev = new_node;
}

void push(ring_buffer* temp, char value) {
    if(temp->putIndex == temp->AmountElem) {
        insertValueIntoEnd(temp, value);
    } else if(temp->AmountElem == temp->size) {
        printf("Помилка! Перезаписати необробленні данні неможливо!\n");
        return;
    } else {
        insertValue(temp, value);
    }

    if(temp->AmountElem < temp->size)
        temp->AmountElem++;

    if(temp->getIndex == temp->putIndex && temp->IndexBehind == true) {
        temp->getIndex++;
        resetGetIndex(temp);
    }  

    temp->putIndex++;
    resetPutIndex(temp);    
}

node* removeFromEnd(ring_buffer* temp) {
    node *pop_node = temp->tail;

    temp->tail = pop_node->prev;
    temp->tail->next = NULL;
    
    return pop_node;
}

node* removeFromList(ring_buffer* temp) {
    node *pop_node = getNode(temp, temp->getIndex);

    if(pop_node->prev == NULL) {
        pop_node->next->prev = NULL;
        temp->head = pop_node->next;
    } else {
        pop_node->prev->next = pop_node->next;
        pop_node->next->prev = pop_node->prev;
    }

    return pop_node;
}

void pop(ring_buffer* temp) {
   node *pop_node;

    if(temp->getIndex == temp->AmountElem - 1) {
        pop_node = removeFromEnd(temp);
    } else {
        pop_node = removeFromList(temp);
    }

    if(temp->IndexBehind == false) {
        temp->putIndex--;
        resetPutIndex(temp);
    }

    temp->AmountElem--;

    if(temp->AmountElem == 0)
        temp->putIndex = 0;

    if(temp->getIndex > temp->AmountElem)
        temp->getIndex = temp->AmountElem; 

    resetGetIndex(temp);

    time_t now = getCurrentDate();

    printf("Значення: %c\n", pop_node->value);
    printf("Час введення: %s", ctime(&pop_node->time));
    printf("Час виведення: %s", ctime(&now));

    free(pop_node);
}

void getAll(ring_buffer *temp) {
    node * current = temp->head;

    while (current != NULL) {
        printf("%c ", current->value);
        current = current->next;
    }

    printf("\n");
}

void saveIntoFile(ring_buffer *temp) {
    int file;

    file = open("task.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);

    if(file < 0) {
		printf("Помилка! Не вдалося відкрити файл:\n%s\n", strerror(errno));
		return;
	}

    node * current = temp->head;

    while (current != NULL) {
        write(file, &current->value, 1);
        current = current->next;
    }

    close(file);
}

void changeUser(ring_buffer *temp) {
    int ch;

    printf("Оберіть письменника:\n");

    for (int i = 1; i <= temp->uAmount; i++) {
        printf("Письменник#%d\n", i);
    }

    printf(" >>> ");

    if(scanf("%d", &ch) < 0)
        printError();

    if(ch < 1 || ch > temp->uAmount) {
        printf("Помилка! Такого користувача не існує!\n");
        return;
    }

    temp->currentWriter = ch;
}

int main() {
    int size;
    int uAmount;

    printf("Введіть розмір буфера:\n >>> ");

    if(scanf("%d", &size) < 0)
        printError();

    printf("Введіть номер користувача:\n >>> ");

    if(scanf("%d", &uAmount) < 0)
        printError();

    ring_buffer *buff = buff_init(size, uAmount);

    int ch = 0;

    while(1) {
        printf("Поточний письменник:\nПисьменник#%d\n", buff->currentWriter);

        printf("\tГОЛОВНЕ МЕНЮ:\n1-Додати значення\n2-Показати значення\n3-Показати буфер\n4-Обрати письменника\n0-Вихід\n >>> ");

        scanf("%d", &ch);

        switch(ch) {
            case 1:
                char uValue;

                printf("Введіть значення:\n >>> ");

                if(scanf(" %c", &uValue) < 0)
                    printError();

                if(buff->head->value == '\0') {
                    headInit(buff, uValue);
                } else {   
                    push(buff, uValue);
                }

                break;

            case 2:
                pop(buff);
                break;

            case 3:
                getAll(buff);
                break;

            case 4:
                changeUser(buff);
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