#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int isFileName(char[]);
char* retrieveFileFromArguments(int,char*[]);

int main(int argc, char *argv[])
{
    FILE *fp;

    if (argc != 3 || strcmp(argv[1], "--help") == 0) {
        printf("Help:\n");
        printf("%s destiantion-file - to write text to the file with the given name and overwrite it\n", argv[0]);
        printf("%s -a destiantion-file - to append text to the end of a file with the given name if it already exists\n", argv[0]);
		return 1;
    }

    char *file_name = retrieveFileFromArguments(argc, argv);
    if (file_name == NULL) {
        printf("Incorrect arguments");
    }

    char *options = "a:";
    char option = getopt(argc, argv, options);
    char text[256];
    switch (option) {
        case 'a': {
            if((fp = fopen(file_name, "a")) == NULL) {
                perror("Cannot open destiantion file");
                return -1;
            }
            break;
        }
        case -1: {
            if((fp = fopen(file_name, "w")) == NULL) {
                perror("Cannot open destiantion file");
                return -1;
            }
            break;
        }
        default: {
            return -1;
        }
    }

    printf("Enter your text: ");
    scanf("%[^\n]%*c", text);               //regular expression to enter a string and store it as a char type array
    printf("Entered text: %s\n", text);
    fprintf(fp, "%s", text);

    return 0;
}

int isFileName(char fileName[]) {
    int isFileName = 0;
    if (strchr(fileName, '.') != NULL) { isFileName = 1; }
    return isFileName;
}

char* retrieveFileFromArguments(int size,char *args[]) {
    for(int i = 0; i < size; i++) {
        if (isFileName(args[i]) == 1 && i != 0) { return args[i]; }
    }
    return NULL;
}