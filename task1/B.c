#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int result[26];
    char tmp;
    ssize_t read_f;

    for(int i = 0; i < 26; i++) {
        result[i] = 0;
    }

    int fd = open("./result.txt", O_RDONLY);

    if(fd == -1) {
        return 5;
    }

    while((read_f = read(fd, &tmp, sizeof(char))) != 0) {
        if(tmp >= 'A' && tmp <= 'Z') {
            result[tmp - 'A'] += 1;
        } else if(tmp >= 'a' && tmp <= 'z') {
            result[tmp - 'a'] += 1;
        }
    }

    printf("\nСтатистика символів в файлі result.txt\n");
    for(int i = 0; i < 26; i++) {
        printf("%2.d. %c (%d)  - %3d разів.\n", i+1, i+'A', i+'A', result[i]);
    }

    return 0;
}