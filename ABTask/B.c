#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
int main() {
    int letter[99];
    ssize_t t_size;
    char buffer;
    for(int i = 0; i < 99; i++)
    {
        letter[i] = 0;
    }
    int file = open("Task_text.txt", O_RDONLY);
    if(file == -1)
    {
        return 1;
    }
    while((t_size = read(file, &buffer, sizeof(char))) != 0)
    {
        if((buffer <= 90 && buffer >= 65)|| (buffer <= 122 && buffer >= 97))
        {
            letter[buffer - 97] += 1;
        } else if(buffer >= 97 && buffer <= 122)
        {
            letter[buffer - 97] += 1;
        }
    }
    for(int i = 0; i < 99; i++)
    {
        if(letter[i] >= 1)
        {
            printf("Letter |%c| - %d times in file\n", i + 97, letter[i]);
        }     
    }
    return 0;
}
