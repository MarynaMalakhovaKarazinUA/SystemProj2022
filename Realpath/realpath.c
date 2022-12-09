#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <sys/stat.h>

static int my_resolve_path(char*, char*, char*);
char *my_realpath(const char*, char*);

int main() {
    char buf[PATH_MAX];
    char *res = my_realpath("realpath.c", buf);
    if (res) {
        printf("This source is at %s.\n", buf);
    } else {
        char* errStr = strerror(errno);
        printf("Error string: %s\n", errStr);
        perror("realpath");
        exit(EXIT_FAILURE);
    }
    return 0;
}

static int my_resolve_path(char *path, char *result, char *pos)
{
    if (*path == '/') {
	*result = '/';
	pos = result+1;
	path++;
    }
    *pos = 0;
    if (!*path) return 0;
    while (1) {
        char *slash;
        struct stat st;
        slash = *path ? strchr(path,'/') : NULL;
        if (slash) {
            *slash = 0;
        }
        if (!path[0] || (path[0] == '.' &&
        (!path[1] || (path[1] == '.' && !path[2])))) {
            pos--;
            if (pos != result && path[0] && path[1])
            while (*--pos != '/');
        }
        else {
            strcpy(pos,path);
            if (lstat(result,&st) < 0) {
                return -1;
            }
            if (S_ISLNK(st.st_mode)) {
                char buf[PATH_MAX];
                if (readlink(result,buf,sizeof(buf)) < 0) {
                    return -1;
                }
                *pos = 0;
                if (slash) {
                    *slash = '/';
                    strcat(buf,slash);
                }
                strcpy(path,buf);
                if (*path == '/') {
                    result[1] = 0;
                }
                pos = strchr(result,0);
                continue;
            }
            pos = strchr(result,0);
        }
        if (slash) {
            *pos++ = '/';
            path = slash+1;
        }
        *pos = 0;
        if (!slash) {
            break;
        }
    }
    return 0;
}

char *my_realpath(const char *path, char *resolved_path)
{
    char cwd[PATH_MAX];
    char *path_copy;
    int res;
    if (!*path) {
        errno = ENOENT;
        return NULL;
    }
    if (!getcwd(cwd,sizeof(cwd))) {
        return NULL;
    }
    strcpy(resolved_path,"/");
    if (my_resolve_path(cwd,resolved_path,resolved_path)) {
        return NULL;
    }
    strcat(resolved_path,"/");
    path_copy = strdup(path);
    if (!path_copy) {
        return NULL;
    }
    res = my_resolve_path(path_copy,resolved_path,strchr(resolved_path,0));
    free(path_copy);
    if (res) {
        return NULL;
    }
    return resolved_path;
}