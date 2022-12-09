#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

void getCurDir() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf(" %s\n", cwd);
    } else {
        perror("getcwd() error");
        return;
    }
}

void getCurDirWithFile(char *filename) {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        strcat(cwd, filename);
        printf(" %s\n", cwd);
    } else {
        perror("getcwd() error");
        return;
    }
}

void cutFilenameFromPath(char *dir, char *filename) {
    const char ch = '/';
    char *ret;

    ret = strrchr(dir, ch);
    if (ret == NULL)
        return;
    strcpy(filename, ret);

    int dir_len = strlen(dir);
    int filename_len = strlen(ret);
    
    dir[dir_len - filename_len] = '\0';
}

void getRealPath(char *dir) {
    DIR *dp;
    char filename[40] = "\0";
    if((dp = opendir(dir)) == NULL) {
        cutFilenameFromPath(dir, filename);
        if((dp = opendir(dir)) == NULL) {
            fprintf(stderr,"cannot open directory: %s\n", dir);
            return;
        }
    }
    chdir(dir);
    if (strcmp(filename, "\0") != 0)
        getCurDirWithFile(filename);
    else 
        getCurDir();
    closedir(dp);
}

int main() {
    char str[] = "../SystemCourse";
    getRealPath(str);
    exit(0);
}