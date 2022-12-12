#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int FindFile(char filename[]);
char* getFile(int size,char *args[]);

int main(int argc, char *argv[]) {
    FILE *File;
    char *FileName = getFile(argc, argv);
    if (FileName == NULL) {
        return 1;
    }


    char *flag ="a:i";
    char opt = getopt(argc, argv, flag);
    char text[256];
    switch (opt){
        case 'a':
            if((File= fopen(FileName, "a"))==NULL) {
            perror("Не вдалося відкрити файл");
                return 1;
            }
            break;
        case -1:
            if((File= fopen(FileName, "w"))==NULL) {
                perror("Не вдалося відкрити файл");
                return 1;
            }
            break;
        case 'i':
          printf("Якщо ви хочете перезаписати строку то не передавайте аргумент\nЯкщо хочете додати строку то передайте аргумент -а\n");
          return 1;
    }
    
    printf("Введіть текст: ");
    scanf("%[^\n]%*c", text);//Регулярний вираз який дає можливість вводити користувачеві цілий рядок і зберігати її в масив чарів
    printf("Перевірте файл\n");
    fprintf(File,"%s", text);
    exit(0);
}

int FindFile(char filename[]) {
    int isFilename = 0;
    if (strchr(filename, '.') != NULL) {
        isFilename = 1;
    }
    return isFilename;
}
//шукає назву файлу в переданих аругументах, якщо знайшло, то повертає його
char* getFile(int size,char *args[]) {
    for(int i = 0; i < size; i++) {
        if (FindFile(args[i]) == 1 && i != 0) {
            return args[i];
        }
    }
    return NULL;
}
