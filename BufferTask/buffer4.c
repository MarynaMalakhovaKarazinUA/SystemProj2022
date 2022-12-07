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
    time_t time;
}node;

typedef struct ring_buffer {
    int size;
    int cur_elemnts_amount;

    node *head;
    node *tail;
    int getIndex;
    int putIndex;
    
    bool putIndex_was_behind;
    bool arrayWasFulledOnce;

    int cur_writer;
    int user_amount;
}ring_buffer;

void wrongInputError() {
    printf("Wrong input!\n");
    exit(1);
}

time_t getCurrentDate() {
    time_t t;
    time(&t);
    return t;
}

void resetGetIndexIfOutOfBounds(ring_buffer *buf) {
     if(buf->getIndex >= buf->cur_elemnts_amount) {
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

void headInit(ring_buffer *buf, char user_value) {
    buf->head->val = user_value;
    buf->head->writer = buf->cur_writer;
    buf->head->prev = NULL;
    buf->head->next = NULL;
    buf->tail = buf->head;
    buf->head->time = getCurrentDate();
    buf->cur_elemnts_amount++;
    buf->putIndex++;
}

ring_buffer* buff_init(int size, int user_amount) {
    ring_buffer *buff = malloc(sizeof(ring_buffer));
    buff->size = size;
    buff->head = (node*) malloc(sizeof(node));
    buff->tail = (node*) malloc(sizeof(node));
    buff->getIndex = 0;
    buff->putIndex = 0;
    buff->putIndex_was_behind = false;
    buff->arrayWasFulledOnce = false;
    buff->cur_elemnts_amount = 0;
    buff->user_amount = user_amount;
    buff->cur_writer = 1;
    return buff;
}

node* getNodeByIndex(ring_buffer* buf, int index) {
    node* current = buf->head;
    int counter = 0;
    while (counter != index) {
        current = current->next;
        counter++;
    }
    return current;
}

void insertValueAtEnd(ring_buffer* buf, char val) {
    node *node = malloc(sizeof(node));
    node->val = val;
    node->writer = buf->cur_writer;
    node->next = NULL;
    node->time = getCurrentDate();

    node->prev = buf->tail;
    buf->tail->next = node;
    buf->tail = node;
}

void insertValue(ring_buffer* buf, char val) {
    node *new_node;
    new_node = malloc(sizeof(node));
    new_node->val = val;
    new_node->writer = buf->cur_writer;
    new_node->next = NULL;
    new_node->time = getCurrentDate();

    node *cur = getNodeByIndex(buf, buf->putIndex);
    if (cur->prev == NULL) {
        buf->head = new_node;
    }
    else {
        new_node->prev = cur->prev;
        cur->prev->next = new_node;
    }
    new_node->next = cur;
    cur->prev = new_node;
}

void push(ring_buffer* buf, char val) {
    if (buf->putIndex == buf->cur_elemnts_amount){
        insertValueAtEnd(buf, val);
    }
    else if(buf->cur_elemnts_amount == buf->size){
        printf("You can't overwrite raw data\n");
        return;
    }
    else {
        insertValue(buf, val);
    }
    if(buf->cur_elemnts_amount < buf->size)
        buf->cur_elemnts_amount++;
        
    if (buf->getIndex == buf->putIndex && buf->putIndex_was_behind == true) {
        buf->getIndex++;
        resetGetIndexIfOutOfBounds(buf);
    }  
    buf->putIndex++;
    resetPutIndexIfOutOfBounds(buf);    
}

node* removeFromEnd(ring_buffer* buf) {
    node *pop_node = buf->tail;
    buf->tail = pop_node->prev;
    buf->tail->next = NULL;
    return pop_node;
}

node* removeFromList(ring_buffer* buf) {
    node *pop_node = getNodeByIndex(buf, buf->getIndex);
    if (pop_node->prev == NULL) {
        pop_node->next->prev = NULL;
        buf->head = pop_node->next;
    }
    else {
        pop_node->prev->next = pop_node->next;
        pop_node->next->prev = pop_node->prev;
    }
    return pop_node;
}

void pop(ring_buffer* buf) {
   node *pop_node;
    if (buf->getIndex == buf->cur_elemnts_amount - 1) {
        pop_node = removeFromEnd(buf);
    }
    else {
        pop_node = removeFromList(buf);
    }

    if (buf->putIndex_was_behind == false) {
        buf->putIndex--;
        resetPutIndexIfOutOfBounds(buf);
    }
    buf->cur_elemnts_amount--;
    if(buf->cur_elemnts_amount == 0)
        buf->putIndex = 0;
    if (buf->getIndex > buf->cur_elemnts_amount)
        buf->getIndex = buf->cur_elemnts_amount;    
    resetGetIndexIfOutOfBounds(buf);
    time_t now = getCurrentDate();
    printf("Value: %c\n", pop_node->val);
    printf("Input time: %s", ctime(&pop_node->time));
    printf("Output time: %s", ctime(&now));
    free(pop_node);
}

void getAll(ring_buffer *buf) {
    node * current = buf->head;
    while (current != NULL) {
        printf("%c ", current->val);
        current = current->next;
    }
    printf("\n");
}

void saveToFile(ring_buffer *buf) {
    int file;
    file = open("bufferFile.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
    if (file < 0) {
		printf("Failed to open destiantion file: %s\n", strerror(errno));
		return;
	}
    node * current = buf->head;
    while (current != NULL) {
        write(file, &current->val, 1);
        current = current->next;
    }
    close(file);
}

void changeUser(ring_buffer *buf) {
    int choice;
    printf("Choose a writer\n");
    for (int i = 1; i <= buf->user_amount; i++) {
        printf("writer#%d\n", i);
    }
    printf(">> ");
    if (scanf(" %d", &choice) < 0)
        wrongInputError();

    if (choice < 1 || choice > buf->user_amount) {
        printf("There isn`t such user\n");
        return;
    }
    buf->cur_writer = choice;
}

int main() {
    int size;
    int user_amount;
    printf("Enter size of buffer: ");
    if (scanf(" %d", &size) < 0)
        wrongInputError();
    printf("Enter the number of users: ");
    if (scanf(" %d", &user_amount) < 0)
        wrongInputError();
    ring_buffer *buff = buff_init(size, user_amount);

    int choice = 0;

    while(1) {
        printf("Current writter: writter#%d\n", buff->cur_writer);
        printf("1.Add value\n");
        printf("2.Get value\n");
        printf("3.Show the buffer\n");
        printf("4.Change writter\n");
        printf("0.Save and exit\n");
        printf(">> ");
        if (scanf(" %d", &choice) < 0)
            wrongInputError();

        switch(choice) {
            case 1:
                char user_value;
                printf("Enter the value: ");
                if (scanf(" %c", &user_value) < 0)
                    wrongInputError();
                if(buff->head->val == '\0') {
                    headInit(buff, user_value);
                } 
                else {   
                    push(buff, user_value);
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
                saveToFile(buff);
                exit(0);
                break;
            default:
                printf("There isn`t such option\n");                    
        }
    }
}
