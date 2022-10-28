#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int result[26];
    char buf;
    ssize_t nr;

    for(int i = 0; i < 26; i++) {
        result[i] = 0;
    }

    int fd = open("./output.txt", O_RDONLY);

    if(fd == -1) {
        return 5;
    }

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
