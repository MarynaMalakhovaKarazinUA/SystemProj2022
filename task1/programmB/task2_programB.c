#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
  int i, j, count = 0;
  char str[20];
  char findLetter;
  FILE *file;

  file = fopen("text_file_prog.txt", "r");
  fscanf(file, "%s", str);
  fclose(file);

  printf("String from file: %s\n\nStatistics:\n", str);

  for(i = 0; i < strlen(str); i++){
    if(str[i] == str[i+1]) continue;
      findLetter = str[i];
        for(j = 0; j < strlen(str); j++){
          if(str[j] == findLetter) {
            count++;
            if(str[j] == str[j+1]){
              continue;
            }
            printf("%c: %d\n", findLetter, count);
            count = 0;
        }
      }
  }

  return 0;
}
