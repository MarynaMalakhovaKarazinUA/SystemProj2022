#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main() {
    char input[1024];

    printf("\nEnter text: ");
    scanf("%s", input);

    for(int i = 0; i < strlen(input); i++) {
        if(!((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z'))) {
            printf("One or more letter aren't latin. Exit..");
            return 1;
        }
    }

    for(int i = 0; i < strlen(input); i++) {
        for(int j = strlen(input) - 1; j > i; j--) {
            int current_char = (input[j] >= 'a') ? input[j] - 32 : input[j];
            int previous_char = (input[j-1] >= 'a') ? input [j-1] - 32 : input[j-1];
            if(current_char >= previous_char) {
                continue;
            }
            char tmp = input [j-1];
            input[j-1] = input [j];
            input[j] = tmp;
        }
    }

    int fd = open("./output.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);

    if(fd == -1) {
        fprintf(stderr, "%s", strerror(errno));
        return 5;
    }

    if(write(fd, &input, strlen(input)) == strlen(input)) {
        printf("Result wrote in output.txt\n");
        close(fd);
    } else {
        printf("Error writing to output.txt\n");
        fprintf(stderr, "%s", strerror(errno));
        close(fd);
        return 5;
    }
    
    return 0;
}
