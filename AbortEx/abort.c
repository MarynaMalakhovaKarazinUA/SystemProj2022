#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#define BUF_SIZE 4096

int main(int argc, char const *argv[])
{
    int fd, flags, opt;
    ssize_t nread, nwrite, nwrite2;
    char buf[BUF_SIZE + 1];
    int has_file = 0;
    if (argc > 1)
        has_file = 1;

    if (has_file)
    {
        flags = O_WRONLY | O_CREAT | O_TRUNC;
        while ((opt = getopt(argc, argv, "a")) != -1)
        {
            switch (opt)
            {
            case 'a':
                flags = O_WRONLY | O_APPEND | O_CREAT;
                break;
            default:
                fprintf(stderr, "Usage: %s filename [-a]\n", argv[0]);
                abort();
            }
        }

        fd = open(argv[argc - 1], flags, 0644);

        if (fd == -1)
        {
            perror("open");
            return EXIT_FAILURE;
        }
    }

    while ((nread = read(STDIN_FILENO, buf, BUF_SIZE)) != 0)
    {
        if (nread == -1)
        {
            if (errno == EINTR)
                continue;
            perror("read");
            break;
        }
        nwrite = write(STDOUT_FILENO, buf, nread);
        if (nwrite == -1)
        {
            perror("write");
            abort();
        }
        if (has_file)
        {
            nwrite2 = write(fd, buf, nread);
            if (nwrite2 == -1)
            {
                perror("write");
                abort();
            }
        }
    }

    return 0;
}