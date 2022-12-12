#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

void getDirectory() {
    char cur_dir[PATH_MAX];

    if(getcwd(cur_dir, sizeof(cur_dir)) != NULL) {
        printf("\n%s\n", cur_dir);
    } else {
        perror("Помилка функції getcwd()!");
        return;
    }
}

void getDirectoryWithFile(char *nameOfFile) {
    char cur_dir[PATH_MAX];

    if(getcwd(cur_dir, sizeof(cur_dir)) != NULL) {
        strcat(cur_dir, nameOfFile);
        printf("\n%s\n", cur_dir);
    } else {
        perror("Помилка функції getcwd()!");
        return;
    }
}

void cutName(char *dir, char *nameOfFile) {
    const char ch = '/';
    char *realp;

    realp = strrchr(dir, ch);

    if(realp == NULL)
        return;

    strcpy(nameOfFile, realp);

    int lengthOfDir = strlen(dir);
    int lengthNameFile = strlen(realp);

    dir[lengthOfDir - lengthNameFile] = '\0';
}

void getPath(char *dir) {
    DIR *dp;

    char nameOfFile[40] = "\0";

    if((dp = opendir(dir)) == NULL) {
        cutName(dir, nameOfFile);
        if((dp = opendir(dir)) == NULL) {
            fprintf(stderr,"Помилка! Неможливо відкрити дерикторію:\n%s\n", dir);
            return;
        }
    }
    chdir(dir);

    if(strcmp(nameOfFile, "\0") != 0)
        getDirectoryWithFile(nameOfFile);
    else 
        getDirectory();

    closedir(dp);
}

int main() {
    char str[] = "../Docker";

    getPath(str);
    exit(0);
}