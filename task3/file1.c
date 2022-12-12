#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int Search_File(char name[]);
char* getFile(int size, char *args[]);

int main(int argc, char *argv[]) {
    FILE *File;
    char *NameOfFile = getFile(argc, argv);

    if (NameOfFile == NULL) {
        return 1;
    }

    char *flag ="a:i";
    char opt = getopt(argc, argv, flag);
    char text[256];

    switch (opt) {
        case 'a':
            if((File= fopen(NameOfFile, "a"))==NULL) {
            perror("Помилка! Файл не відкрився!");
                return 1;
            }
            break;

        case -1:
            if((File= fopen(NameOfFile, "w"))==NULL) {
                perror("Помилка! Файл не відкрився!");
                return 1;
            }
            break;

        case 'i':
          printf("При перезапису строки - не передавайте аргумент\nПри додавання строки - передайте аргумент -а\n");
          return 1;
    }
    
    printf("Введіть текст:\n >>> ");
    scanf("%[^\n]%*c", text);
    printf("Данні було записано в файл\n");
    fprintf(File,"%s", text);
    exit(0);
}

int Search_File(char name[]) {
    int CurrName = 0;
    if (strchr(name, '.') != NULL) {
        CurrName = 1;
    }
    
    return CurrName;
}

char* getFile(int size,char *args[]) {
    for(int i = 0; i < size; i++) {
        if (Search_File(args[i]) == 1 && i != 0) {
            return args[i];
        }
    }

    return NULL;
}