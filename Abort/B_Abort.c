#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <signal.h>

void my_signal_abort();

int main() {
    int result[26];
    int outputFd;
    char buffer;
    ssize_t numRead;

    signal(SIGABRT, &my_signal_abort);

    outputFd = open("./outputA.txt", O_RDONLY);
    if(outputFd == -1) {
        abort();
    }

    for(int i = 0; i < 26; i++) {
        result[i] = 0;
    }

    while((numRead = read(outputFd, &buffer, sizeof(char))) != 0) {
        if(buffer >= 'A' && buffer <= 'Z') {
            result[buffer - 'A'] += 1;
        } else if(buffer >= 'a' && buffer <= 'z') {
            result[buffer - 'a'] += 1;
        }
    }

    for(int i = 0; i < 26; i++) {
        printf("%d)\t%c (%d)\t:\t%d times\n", i + 1, i + 'A', i + 'A', result[i]);
    }

    if (close(outputFd) == -1) {
		abort();
	}

    return 0;
}

void my_signal_abort() {
    printf("File error!\n");
    printf("Unable to open source file or close output file.\n");
    printf("The program was terminated..\n");
}