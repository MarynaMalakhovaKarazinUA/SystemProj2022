#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_READ 256

int main(int argc, char *argv[]) {
    if(argc != 2 && argc != 3) {
        printf("USAGE: tee [-a] fileName\n");
        return 1;
    }

    char* fileName;
    
    for (int i = 1; i < argc; i++) {
        char *arg = argv[i];
        if (arg[0] != '-') {
            fileName = arg;
        }
    }

    int fd; 
    int append = 0;
    int optc;

    while ((optc = getopt(argc, argv, "a")) != -1) {
        switch (optc) {
            case 'a':
                append = 1;
                break;
            case '?': 
                printf("USAGE: tee [-a] fileName\n");
                return 1;
            default:
                break;
        }
    }

    mode_t filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

    if(append == 0) {
        fd = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, filePerms);
    } else {
        fd = open(fileName, O_WRONLY | O_CREAT | O_APPEND, filePerms);
    }

    if(fd == -1) {
        perror("Error");
        return 5; // IO Error
    }

    char buf[MAX_READ + 1];
    ssize_t numRead;

    while(1) {
        numRead = read(STDIN_FILENO, buf, MAX_READ);

        if(numRead == -1) {
            perror("Error");
            return 5; // IO Error
        }
        
        buf[numRead] = '\0';

        if(write(STDOUT_FILENO, buf, strlen(buf)) == -1) {
            perror("Print error");
            return 5;
        }
        if(write(fd, buf, strlen(buf)) == -1) {
            perror("Write error");
            return 5;
        }
    }

    return 0;
}
