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

typedef struct node
{
    char val;
    struct node *next;
    struct node *prev;
    char writer;
} node;

typedef struct ring_buffer
{
    int size;
    int cur_elemnts_amount;

    node *head;
    node *tail;
    int getIndex;
    int putIndex;

    bool putIndex_was_behind;
    bool arrayWasFulledOnce;

    time_t time;
    int cur_writer;
    int user_amount;
} ring_buffer;

time_t getCurrentDate()
{
    time_t t;
    time(&t);
    return t;
}

void resetGetIndexIfOutOfBounds(ring_buffer *buf)
{
    if (buf->getIndex >= buf->cur_elemnts_amount)
    {
        buf->getIndex = 0;
        buf->putIndex_was_behind = false;
    }
}

void resetPutIndexIfOutOfBounds(ring_buffer *buf)
{
    if (buf->putIndex >= buf->size)
    {
        buf->putIndex = 0;
        buf->putIndex_was_behind = true;
    }
}

int isEmpty(ring_buffer *buf)
{
    if (buf->cur_elemnts_amount == 0)
    {
        return 0;
    }
    return 1;
}

void headInit(ring_buffer *buf, char user_value)
{
    buf->head->val = user_value;
    buf->head->writer = buf->cur_writer;
    buf->head->prev = NULL;
    buf->head->next = NULL;
    buf->tail = buf->head;
    buf->cur_elemnts_amount++;
    buf->putIndex++;
}

ring_buffer *buff_init(int size, int user_amount)
{
    ring_buffer *buff = malloc(sizeof(ring_buffer));
    buff->size = size;
    buff->head = (node *)malloc(sizeof(node));
    buff->tail = (node *)malloc(sizeof(node));
    buff->getIndex = 0;
    buff->putIndex = 0;
    buff->putIndex_was_behind = false;
    buff->arrayWasFulledOnce = false;
    buff->cur_elemnts_amount = 0;
    buff->user_amount = user_amount;
    buff->time = getCurrentDate();
    buff->cur_writer = 1;
    return buff;
}

node *getNodeByIndex(ring_buffer *buf, int index)
{
    node *current = buf->head;
    int counter = 0;
    while (counter != index)
    {
        printf("current: %c\n", current->val);
        current = current->next;
        counter++;
    }
    return current;
}

void insertValueAtEnd(ring_buffer *buf, char val)
{
    node *node = malloc(sizeof(node));
    node->val = val;
    node->writer = buf->cur_writer;
    node->next = NULL;

    node->prev = buf->tail;
    buf->tail->next = node;
    buf->tail = node;
}

void setValue(ring_buffer *buf, char val)
{
    node *new_node;
    new_node = malloc(sizeof(node));
    new_node->val = val;
    new_node->writer = buf->cur_writer;
    new_node->next = NULL;
    new_node->prev = NULL;

    node *cur = getNodeByIndex(buf, buf->putIndex);
    printf("cur: %c\n", cur->val);
    printf("node: %c\n", new_node->val);
    if (cur->prev == NULL)
    {
        printf("IN NULL STATE");
        buf->head = new_node;
    }
    else
    {
        cur->prev->next = new_node;
        new_node->prev = cur->prev;
    }
    cur->next->prev = new_node;
    new_node->next = cur->next;
    free(cur);
}

void insertValue(ring_buffer *buf, char val)
{
    node *new_node;
    new_node = malloc(sizeof(node));
    new_node->val = val;
    new_node->writer = buf->cur_writer;
    new_node->next = NULL;

    node *cur = getNodeByIndex(buf, buf->putIndex);
    new_node->prev = cur->prev;
    new_node->next = cur;
    cur->prev->next = new_node;
    cur->prev = new_node;
}

void push(ring_buffer *buf, char val)
{
    if (buf->putIndex == buf->cur_elemnts_amount)
    {
        insertValueAtEnd(buf, val);
    }
    else if (buf->cur_elemnts_amount == buf->size)
    {
        setValue(buf, val);
    }
    else
    {
        insertValue(buf, val);
    }
    if (buf->cur_elemnts_amount < buf->size)
        buf->cur_elemnts_amount++;

    if (buf->getIndex == buf->putIndex && buf->putIndex_was_behind == true)
    {
        buf->getIndex++;
        resetGetIndexIfOutOfBounds(buf);
    }
    buf->putIndex++;
    resetPutIndexIfOutOfBounds(buf);
}

char removeFromEnd(ring_buffer *buf)
{
    node *pop_node = buf->tail;
    buf->tail = pop_node->prev;
    buf->tail->next = NULL;
    char res = pop_node->val;
    free(pop_node);
    return res;
}

char removeFromList(ring_buffer *buf)
{
    node *pop_node = getNodeByIndex(buf, buf->getIndex);
    if (pop_node->prev == NULL)
    {
        pop_node->next->prev = NULL;
        buf->head = pop_node->next;
    }
    else
    {
        pop_node->prev->next = pop_node->next;
        pop_node->next->prev = pop_node->prev;
    }
    char res = pop_node->val;
    free(pop_node);
    return res;
}

char pop(ring_buffer *buf)
{
    char res;
    if (buf->getIndex == buf->cur_elemnts_amount - 1)
    {
        res = removeFromEnd(buf);
    }
    else
    {
        res = removeFromList(buf);
    }
    if (buf->putIndex_was_behind == false)
    {
        buf->putIndex--;
        resetPutIndexIfOutOfBounds(buf);
    }

    buf->cur_elemnts_amount--;
    if (buf->cur_elemnts_amount == 0)
        buf->putIndex = 0;
    if (buf->getIndex > buf->cur_elemnts_amount)
        buf->getIndex = buf->cur_elemnts_amount;
    resetGetIndexIfOutOfBounds(buf);
    return res;
}

void getAll(ring_buffer *buf)
{
    node *current = buf->head;
    while (current != NULL)
    {
        printf("%c ", current->val);
        current = current->next;
    }
    printf("\n");
}

void showCurrentTime(ring_buffer *buf)
{
    time_t t = buf->time;
    printf("Current date and time is : %s", ctime(&t));
}

void saveToFile(ring_buffer *buf)
{
    int file;
    file = open("bufferFile.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
    if (file < 0)
    {
        printf("Failed to open destiantion file: %s\n", strerror(errno));
        return;
    }
    node *current = buf->head;
    while (current != NULL)
    {
        write(file, &current->val, 1);
        current = current->next;
    }
    close(file);
}

void changeUser(ring_buffer *buf)
{
    int choice;
    printf("Choose a writer\n");
    for (int i = 1; i <= buf->user_amount; i++)
    {
        printf("writer#%d\n", i);
    }
    printf(">> ");
    scanf(" %d", &choice);

    if (choice < 1 || choice > buf->user_amount)
    {
        printf("There isn`t such user\n");
        return;
    }
    buf->cur_writer = choice;
}

int main()
{
    int size;
    int user_amount;
    printf("Enter size of buffer: ");
    scanf(" %d", &size);
    printf("Enter the number of users: ");
    scanf(" %d", &user_amount);
    ring_buffer *buff = buff_init(size, user_amount);

    int choice = 0;

    while (1)
    {
        buff->time = getCurrentDate();
        printf("Current writter: writter#%d\n", buff->cur_writer);
        printf("1.Add value\n");
        printf("2.Get value\n");
        printf("3.Show the buffer\n");
        printf("4.Get current time\n");
        printf("5.Change writter\n");
        printf("0.Save and exit\n");
        printf(">> ");
        scanf(" %d", &choice);

        switch (choice)
        {
        case 1:
            char user_value;
            printf("Enter the value: ");
            scanf(" %c", &user_value);
            if (buff->head->val == '\0')
            {
                headInit(buff, user_value);
            }
            else
            {
                push(buff, user_value);
            }
            break;
        case 2:
            char res = pop(buff);
            printf("Value: %c\n", res);
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
            saveToFile(buff);
            exit(0);
            break;
        default:
            printf("There isn`t such option\n");
        }
    }
}