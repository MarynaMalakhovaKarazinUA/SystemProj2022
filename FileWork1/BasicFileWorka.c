#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
int main(){
    char input[1024]; 
    // Input letter in mass
    printf("\nEnter a text: ");
    scanf("%s", input);
    // Сhecking for Latin alphabet
    for (int i = 0; i < strlen(input); i++) {
        if(!((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z'))) {
            printf("letters weren't latin.\n Exit.");
            return 1; 
        }
    }
    // Sorting on Latin alphabet 
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
    // Opening file if file doesn't exist, we'll create this file with special permissions
    int fd = open("./results.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
    // Сhecking for bugs
    if (fd == -1) 
        return 4; 
    // Writing result into file results.txt
    if(write(fd, &input, strlen(input)) == strlen(input)) {
        // Show script completion message
        printf("Result wrote in results.txt\n");
        // Close fd 
        close(fd);
    } else {
        // Show script uncompletion message we had error 
        printf("Error writing to file\n");
        fprintf(stderr, "%s", strerror(errno));
        // Close fd 
        close(fd);
        return 4;
    }
    // Close fd 
    close(fd);
    // Exit programm 
    return 0;
}

