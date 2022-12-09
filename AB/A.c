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
    int outputFd;
    int openFlags = O_CREAT | O_WRONLY | O_TRUNC;
	mode_t filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

    printf("\n");
    printf("Enter text: ");
    scanf("%s", input);

    for(int i = 0; i < strlen(input); i++) {
        if(!((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z'))) {
            printf("Error. There are non-Latin letters in the text.");
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

    outputFd = open("./outputA.txt", openFlags, filePerms);
    if(outputFd == -1) {
        perror("Cannot open output file");
        return 5;
    }

    if(write(outputFd, &input, strlen(input)) == strlen(input)) {
        printf("Result wrote in outputA.txt\n");
        close(outputFd);
    } else {
        perror("Error writing to outputA.txt");
        if (close(outputFd) == -1) {
            perror("Close output");
        }
        return 5;
    }
    
    return 0;
}