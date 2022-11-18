#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int findFilename(char filename[]) {
    int isFilename = 0;
    if (strchr(filename, '.') != NULL) {
        isFilename = 1;
    }
    return isFilename;
}

char* getFileFromArguments(int size,char *args[]) {
    for(int i = 0; i < size; i++) {
        if (findFilename(args[i]) == 1 && i != 0) {
            return args[i];
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    FILE *fp;
    char *file_name = getFileFromArguments(argc, argv);
    if (file_name == NULL) {
        printf("Incorrect arguments");
    }

    char *opts = "a:";
    char opt = getopt(argc, argv, opts);
    char text[256];
    switch (opt){
        case 'a':
            if((fp= fopen(file_name, "a"))==NULL) {
                perror("Error occured while opening file");
                return 1;
            }
            break;
        case -1:
            if((fp= fopen(file_name, "w"))==NULL) {
                perror("Error occured while opening file");
                return 1;
            }
            break;
    }
    printf("Enter your message: ");
    scanf("%[^\n]%*c", text);
    printf("Your messsage - %s\n", text);
    fprintf(fp,"%s", text);
    exit(0);
}


