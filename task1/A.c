#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main() {
    char text[1024];

    printf("\nВведіть текст, який буде складатись лише з латинських літер, без пробілів та цифр:\n >>> ");
    scanf("%s", text);

    for(int i = 0; i < strlen(text); i++) {
        if(!((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z'))) {
            printf("\nВи ввели текст, що не відповідає умовам!\n");
            return 0;
        }
    }

    for(int i = 0; i < strlen(text); i++) {
        for(int j = strlen(text) - 1; j > i; j--) {
            int char_now = (text[j] >= 'a') ? text[j] - 32 : text[j];
            int last_char = (text[j-1] >= 'a') ? text [j-1] - 32 : text[j-1];

            if(char_now >= last_char) {
                continue;
            }

            char tmp = text [j-1];
            text[j-1] = text [j];
            text[j] = tmp;
        }
    }

    int wr_f = open("./result.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);

    if(wr_f == -1) {
        fprintf(stderr, "%s", strerror(errno));
        return 5;
    }
    if(write(wr_f, &text, strlen(text)) == strlen(text)) {
        printf("\nРезультат був записан в файл result.txt\n");
        close(wr_f);
    } else {
        printf("\nПомилка запису результату в файл result.txt\n");
        fprintf(stderr, "%s", strerror(errno));
        close(wr_f);
        return 5;
    }
    return 0;
}