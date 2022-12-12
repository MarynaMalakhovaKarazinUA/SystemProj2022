#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    char buff[1024];
    int CounFiles[2];
    ssize_t count_s;

    if (argc =! 3) {
		printf("Помилка! Необхідна кількість аргументів - 3!");
     return -1;
	}
        
    CounFiles[0] = open(argv[1], O_RDONLY);

    if (CounFiles[0] == -1) {
		printf("Помилка! Файл не вдалося відкрити!");
     return -1;
	} 
       
    CounFiles[1] = open(argv[2], O_WRONLY | O_CREAT | S_IRUSR | S_IWUSR);

    if (CounFiles[1] == -1) {
		printf("Помилка! Файл не вдалося створити!");
        close(CounFiles[0]);
        return -1;
    }

    while ((count_s = read(CounFiles[0], buff, sizeof(buff))) != 0)
        write(CounFiles[1], buff, count_s);

    return 0;
}