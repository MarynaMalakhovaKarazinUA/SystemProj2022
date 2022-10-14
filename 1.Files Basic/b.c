#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int result[26];

    for(int i = 0; i < 26; i++) {
        result[i] = 0;
    }

    int fd;

    fd = open("./output.txt", O_RDONLY);

    if(fd == -1) {
        return 1;
    }

    char buf;
    ssize_t nr;
    while((nr = read(fd, &buf, sizeof(char))) != 0) {
        if(buf >= 'A' && buf <= 'Z') {
            result[buf - 'A'] += 1;
        } else if(buf >= 'a' && buf <= 'z') {
            result[buf - 'a'] += 1;
        }
    }

    for(int i = 0; i < 26; i++) {
        printf("%d. %c (%d) - %d times\n", i+1, i+'A', i+'A', result[i]);
    }

    return 0;
}