#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

void print_usage()
{
    printf("Usage: ./prog_cpholes <src> <dst>\n");
}

int main(int argc, char *argv[])
{
    int inputFd, outputFd, openFlags;
    mode_t filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE];

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
    {
        print_usage();
        return 1;
    }

    openFlags = (O_CREAT | O_WRONLY | O_TRUNC);
    filePerms = (S_IRUSR | S_IWUSR | S_IRGRP |
                 S_IROTH | S_IWOTH);

    inputFd = open(argv[1], O_RDONLY);
    if (inputFd < 0)
    {
        printf("Failed to open input file: %s\n", strerror(errno));
        print_usage();
        return 1;
    }

    outputFd = open(argv[2], openFlags, filePerms);
    if (outputFd < 0)
    {
        printf("Failed to open output file: %s\n", strerror(errno));
        print_usage();
        return 1;
    }

    int i, err;
    unsigned long holeSize = 0;
    while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0)
    {
        for (i = 0; i < numRead; i++)
        {
            if (buf[i] == '\0')
            {
                holeSize++;
            }
            else if (holeSize > 0)
            {
                lseek(outputFd, holeSize, SEEK_CUR);
                write(outputFd, &buf[i], 1);
                holeSize = 0;
            }
            else
            {
                write(outputFd, &buf[i], 1);
            }
        }
    }

    off_t lastApparentByte = lseek(inputFd, -1, SEEK_END);
    lseek(outputFd, lastApparentByte, SEEK_SET);
    write(outputFd, "", 1);

    /* close fds */
    close(inputFd);
    close(outputFd);

    return 0;
}