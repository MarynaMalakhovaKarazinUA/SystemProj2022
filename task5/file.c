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
    char val;
    struct node *next;
    struct node *prev;
    char writer;
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

    time_t time;

    int currentWriter;
    int uAmount;
} ring_buffer;

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

void headInit(ring_buffer *temp, char user_value) {
    temp->head->val = user_value;
    temp->head->writer = temp->currentWriter;
    temp->head->prev = NULL;
    temp->head->next = NULL;
    temp->tail = temp->head;
    temp->AmountElem++;
    temp->putIndex++;
}

void printError() {
    printf("Помилка! Ви ввели невірне значення!\n");
    exit(1);
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
    buff->time = getCurrentDate();
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

void insertValueIntoEnd(ring_buffer* temp, char val) {
    node *node = malloc(sizeof(node));

    node->val = val;
    node->writer = temp->currentWriter;
    node->next = NULL;
    node->prev = temp->tail;
    temp->tail->next = node;
    temp->tail = node;
}

void setValue(ring_buffer* temp, char val) {
    node *new_node;

    new_node = malloc(sizeof(node));
    new_node->val = val;
    new_node->writer = temp->currentWriter;
    new_node->next = NULL;
    new_node->prev = NULL;

    node *cur = getNode(temp, temp->putIndex);

    if(cur->prev == NULL) {
        temp->head = new_node;
    } else {
        cur->prev->next = new_node;
        new_node->prev = cur->prev;
    }

    cur->next->prev = new_node;
    new_node->next = cur->next;

    free(cur);
}

void insertValue(ring_buffer* temp, char val) {
    node *new_node;

    new_node = malloc(sizeof(node));
    new_node->val = val;
    new_node->writer = temp->currentWriter;
    new_node->next = NULL;

    node *cur = getNode(temp, temp->putIndex);

    new_node->prev = cur->prev;
    new_node->next = cur;
    cur->prev->next = new_node;
    cur->prev = new_node;
}

void push(ring_buffer* temp, char val) {
    if(temp->putIndex == temp->AmountElem) {
        insertValueIntoEnd(temp, val);
    } else if(temp->AmountElem == temp->size) {
        setValue(temp, val);
    } else {
        insertValue(temp, val);
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

char removeFromEnd(ring_buffer* temp) {
    node *pop_node = temp->tail;

    temp->tail = pop_node->prev;
    temp->tail->next = NULL;
    char res = pop_node->val;
    free(pop_node); 

    return res;
}

char removeFromList(ring_buffer* temp) {
    node *pop_node = getNode(temp, temp->getIndex);

    if(pop_node->prev == NULL) {
        pop_node->next->prev = NULL;
        temp->head = pop_node->next;
    } else {
        pop_node->prev->next = pop_node->next;
        pop_node->next->prev = pop_node->prev;
    }

    char res = pop_node->val;
    free(pop_node);

    return res;
}

char pop(ring_buffer* temp) {
    char res;

    if(temp->getIndex == temp->AmountElem - 1) {
        res = removeFromEnd(temp);
    } else {
        res = removeFromList(temp);
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

    return res;
}

void getAll(ring_buffer *temp) {
    node * current = temp->head;

    while (current != NULL) {
        printf("%c ", current->val);
        current = current->next;
    }

    printf("\n");
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

    node * current = temp->head;

    while (current != NULL) {
        write(file, &current->val, 1);
        current = current->next;
    }

    close(file);
}

void changeUser(ring_buffer *temp) {
    int ch;

    printf("Оберіть письменника\n");

    for (int i = 1; i <= temp->uAmount; i++) {
        printf("Письменник#%d\n", i);
    }

    printf(" >>> ");

    if(scanf(" %d", &ch) < 0)
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

    printf("Введіть розмір буферу:\n >>> ");

    if(scanf(" %d", &size) < 0)
        printError();
    
    printf("Введіть номер користувача:\n >>> ");

    if(scanf(" %d", &uAmount) < 0)
        printError();
    
    ring_buffer *buff = buff_init(size, uAmount);

    int ch = 0;

    while(1) {
        buff->time = getCurrentDate();
        printf("Поточний письменник:\nПисьменник#%d\n", buff->currentWriter);

        printf("\tГОЛОВНЕ МЕНЮ:\n1-Додати значення\n2-Показати значення\n3-Показати буфер\n4-Показати поточну дату та час\n0-Вихід\n >>> ");

        scanf("%d", &ch);

        switch(ch) {
            case 1: ;
                char user_value;
                printf("Введіть значення:\n >>> ");
                scanf(" %c", &user_value);

                if(buff->head->val == '\0') {
                    headInit(buff, user_value);
                } else {   
                    push(buff, user_value);
                }

                break;

            case 2: ;
                char res = pop(buff);
                printf("Значення: %c\n", res);
                break;

            case 3:
                getAll(buff);
                break;

            case 4:
                showCurrentTime(buff);
                break;

            case 5:
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