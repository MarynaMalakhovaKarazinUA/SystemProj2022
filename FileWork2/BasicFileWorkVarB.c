#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h> 

int main() {
    // Variables
    int res[26]; 
    char buff;
    ssize_t nr;
    // Initialization variable
    for(int i = 0; i <26; i++)
        res[i] = 0;
    // Open file 
    int fd = open("./results.txt", O_RDONLY); 
    if(fd == -1)
        return 5; 
    // Number of occurrences
    while((nr = read(fd, &buff, sizeof(char))) != 0) {
        if(buff >= 'A' && buff <= 'Z') {
            res[buff - 'A'] += 1;
        } else if (buff >= 'a' && buff <= 'z') {
            res[buff - 'a'] += 1;
        }
    }
    // Display info 
    for(int i = 0; i < 26; i++)
        printf("%d. %c (%d) - %d count\n", i+1, i+'A', i+'A', res[i]);
    // Exit programm 
    return 0;
}