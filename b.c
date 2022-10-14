#include<stdio.h>
#include<stdlib.h>

int hash(char ch) {
    return (int) ch % 33;
}

int main() {
    char * filename = "text.txt";
    char cc[256];
    int hashMap[33] = {0};
    FILE *fp;

    if((fp= fopen(filename, "r"))==NULL)
    {
        perror("Error occured while opening file");
        return 1;
    }

    while((fgets(cc, 256, fp))!=NULL) {
        hashMap[hash(cc[0])] += 1;
    }

    for(int i = (int)'a'; i <= (int)'z';i++){
        if(hashMap[hash((char)i)] > 0) {
            printf("count = %d, char = %c\n", hashMap[hash((char)i)], (char)i);
        }
    }
    fclose(fp);
}