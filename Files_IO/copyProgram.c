#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
	int sourceFl, destFl, openFlags;
	ssize_t nr;
	char buf[BUF_SIZE];


	if (argc != 3 || strcmp(argv[1], "--help") == 0) {
        printf("Invalid arguments");
		return 1;
	}
	
	openFlags = (O_CREAT | O_WRONLY | O_TRUNC);
	
	sourceFl = open(argv[1], O_RDONLY);
	if (sourceFl < 0) {
		printf("Failed to open source file: %s\n", strerror(errno));
		return 1;
	}
	
	destFl = open(argv[2], openFlags);
	if (destFl < 0) {
		printf("Failed to open destiantion file: %s\n", strerror(errno));
		return 1;
	}
	
	unsigned long holeSize = 0;
	while ((nr = read(sourceFl, buf, BUF_SIZE)) > 0) {
		for (int i = 0; i < nr; i++) {
			if (buf[i] == '\0') {
				holeSize++;
			} else if (holeSize > 0) {
				lseek(destFl, holeSize, SEEK_CUR);
				write(destFl, &buf[i], 1);
				holeSize = 0;
			} else {
				write(destFl, &buf[i], 1);
			}
		}
	}
	
	close(sourceFl);
	close(destFl);

	return 0;
}