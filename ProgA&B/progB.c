#include<stdio.h>
#include<stdlib.h>

#define int justRoute[33] = {0};
#define char myArray[256];

int main() {

    char * filename = "useless.txt";
    FILE *file;

	file = fopen(filename, "r");
    if(file == NULL)
    {
        return perror("Error opening file");
   }

    while((fgets(myArray, 256, file))!=NULL) {
        justRoute[(int) myArray[0] % 33] += 1;
    }

	int i;
    for(i = (int)'a'; i <= (int)'z'; i++){
        if(justRoute[(char) i % 33] > 0) {
            printf("count = %d, char = %c\n", justRoute[(char) i % 33], (char)i);
        }
    }
    fclose(file);
}
