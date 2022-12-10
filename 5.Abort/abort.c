#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>

#define MAX_READ 256

void abort_handler();

int main(int argc, char *argv[]) {
    signal(SIGABRT, &abort_handler);

    if(argc == 1) {
        printf("USAGE: cp [FROM] [TO]\n");
        return 1;
    }

    char* fromFile = argv[1];
    char* toFile = argv[2];

    int fd[2];

    fd[0] = open(fromFile, O_RDONLY);

    if(fd[0] == -1) {
        abort();
    }

    fd[1] = open(toFile, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

    if(fd[1] == -1) {
        abort();
    }

    char buf[1024];
    ssize_t numRead;

    while ((numRead = read(fd[0], buf, sizeof(buf))) != 0) {
        if(write(fd[1], buf, numRead) == -1) {
            abort();
        }
    }

    return 0;
}

void abort_handler() {
    printf("Oops.. You reached SIGABRT signal :(\n");
}