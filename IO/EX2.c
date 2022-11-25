#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <fcntl.h>

int main(int argc, char **argv)
{
    char buffer[1024];
    int files[2];
    ssize_t count;

    /*Перевірка на достатність аргументів*/
    if (argc < 3 || argc >3){
		printf("Помилка:аргументів повинно бути 3");
     return -1;
	}
        
    files[0] = open(argv[1], O_RDONLY);
	/*Перевірка на відкриття файлу*/
    if (files[0] == -1){
		printf("Помилка:не вдалося відкрити файл");
     return -1;
	} 
       
    files[1] = open(argv[2], O_WRONLY | O_CREAT | S_IRUSR | S_IWUSR);
    if (files[1] == -1) /* Перевірка на відкриття файлу */
    {
		printf("Помилка:не вдалося створити файл");
        close(files[0]);
        return -1;
    }

    while ((count = read(files[0], buffer, sizeof(buffer))) != 0)
        write(files[1], buffer, count);

    return 0;
}