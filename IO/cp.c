#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

void print_usage();

int main(int argc, char *argv[])
{
    int input, output, flags;
    mode_t perms;
    ssize_t numRead;
    char buf[BUF_SIZE];

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
    {
        print_usage();
        return 1;
    }

    flags = (O_CREAT | O_WRONLY | O_TRUNC);
    perms = (S_IRUSR | S_IWUSR | S_IRGRP |
                 S_IROTH | S_IWOTH);

    input = open(argv[1], O_RDONLY);

    if (input < 0)
    {
        printf("Failed to open input file: %s\n", strerror(errno));
        print_usage();
        return 1;
    }

    output = open(argv[2], flags, perms);

    if (output < 0)
    {
      printf("Failed to open output file: %s\n", strerror(errno));
      print_usage();
      return 1;
    }

    int i, err;
    unsigned long holeSize = 0;
    while ((numRead = read(input, buf, BUF_SIZE)) > 0)
    {
      for (i = 0; i < numRead; i++)
      {
        if (buf[i] == '\0')
        {
            holeSize++;
        }
        else if (holeSize > 0)
        {
            lseek(output, holeSize, SEEK_CUR);
            write(output, &buf[i], 1);
            holeSize = 0;
        }
        else
        {
            write(output, &buf[i], 1);
        }
      }
    }

    off_t lastApparentByte = lseek(input, -1, SEEK_END);
    lseek(output, lastApparentByte, SEEK_SET);
    write(output, "", 1);

    close(input);
    close(output);

    return 0;
}

void print_usage()
{
    printf("Usage: ./prog_cpholes <src> <dst>\n");
}
