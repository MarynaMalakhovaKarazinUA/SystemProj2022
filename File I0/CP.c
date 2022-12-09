#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <fcntl.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int main(int argc, char *argv[])
{
	int inputFd, outputFd;
	int openFlags = O_CREAT | O_WRONLY | O_TRUNC;
	mode_t filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
	ssize_t numRead;
	char buffer[BUF_SIZE];
	unsigned long long holeSize = 0;

	if (argc != 3 || strcmp(argv[1], "--help") == 0) {
        printf("Help: %s old-file new-file\n", argv[0]);
		return 1;
	}
	
	inputFd = open(argv[1], O_RDONLY);
	if (inputFd == -1) {
		perror("Cannot open source file");
		return -1;
	}
	
	outputFd = open(argv[2], openFlags, filePerms);
	if (outputFd == -1) {
		close(inputFd);
		perror("Cannot open destiantion file");
		return -1;
	}
	
	while ((numRead = read(inputFd, buffer, BUF_SIZE)) > 0) {
		if (write(outputFd, buffer, numRead) != numRead) {
			perror("write() returned error or partial write occurred");
			return -1;
		}
		for (int i = 0; i < numRead; i++) {
			if (buffer[i] == '\0') {
				holeSize++;
			} else if (holeSize > 0) {
				lseek(outputFd, holeSize, SEEK_CUR);
				write(outputFd, &buffer[i], 1);
				holeSize = 0;
			} else { write(outputFd, &buffer[i], 1); }
		}
	}
	
	if (numRead == -1) {
		perror("Read");
		return -1;
	}
	if (close(inputFd) == -1) {
		perror("Close input");
		return -1;
	}
	if (close(outputFd) == -1) {
		perror("Close output");
		return -1;
	}

	return 0;
}