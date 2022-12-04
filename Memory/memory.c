#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TERM_WORD "end"
#define SIZE_INCREMENT 10

void main()
{
    char **words;

    char buffer[64];
    //Счётчик слов
    unsigned wordCounter = 0;

    unsigned length;

    unsigned size = SIZE_INCREMENT;
    int i;

    words = (char **)malloc(size * sizeof(char *));
    do
    {
        printf("%d: ", wordCounter);
        scanf("%63s", buffer);

        if (strcmp(TERM_WORD, buffer) == 0)
        {
            break;
        }

        length = strlen(buffer);

        if (wordCounter >= size)
        {
            size += SIZE_INCREMENT;
            words = (char **)realloc(words, size * sizeof(char *));
        }

        words[wordCounter] = (char *)malloc(length + 1);

        strcpy(words[wordCounter], buffer);
        wordCounter++;
    } while (1);

    for (i = 0; i < wordCounter; i++)
    {
        printf("%s\n", words[i]);
    }
    _getch();

    for (i = 0; i < wordCounter; i++)
    {
        free(words[i]);
    }
    free(words);
}