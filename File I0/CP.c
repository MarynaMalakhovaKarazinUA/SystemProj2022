#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <fcntl.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif
#define ERR_PARAM 87	//unacceptable parameters
#define ERR_FILE 5		//file not available
#define SUCCESS 0		//successful completion of the programme

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
		return ERR_PARAM;
	}
	
	inputFd = open(argv[1], O_RDONLY);
	if (inputFd == -1) {
		perror("Cannot open source file");
		return ERR_FILE;
	}
	
	outputFd = open(argv[2], openFlags, filePerms);
	if (outputFd == -1) {
		close(inputFd);
		perror("Cannot open destiantion file");
		return ERR_FILE;
	}
	
	while ((numRead = read(inputFd, buffer, BUF_SIZE)) > 0) {
		if (write(outputFd, buffer, numRead) != numRead) {
			perror("write() returned error or partial write occurred");
			return ERR_FILE;
		}
		for (int i = 0; i < numRead; i++) {
			if (buffer[i] == '\0') {
				holeSize++;
			} else if (holeSize > 0) {
				lseek(outputFd, holeSize, SEEK_CUR);
				if(write(outputFd, &buffer[i], 1) == -1) {
					perror("write() returned error or partial write occurred");
					return ERR_FILE;
				}
				holeSize = 0;
			} else {
				if(write(outputFd, &buffer[i], 1) == -1) {
					perror("write() returned error or partial write occurred");
					return ERR_FILE;
				}
			}
		}
	}
	
	if (numRead == -1) {
		perror("Read");
		return ERR_FILE;
	}
	if (close(inputFd) == -1) {
		perror("Close input");
		return ERR_FILE;
	}
	if (close(outputFd) == -1) {
		perror("Close output");
		return ERR_FILE;
	}

	return SUCCESS;
}
