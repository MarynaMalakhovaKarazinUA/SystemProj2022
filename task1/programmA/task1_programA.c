#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void){
  int count, i, j;
  char str[20];
  FILE *file;
  scanf("%s", str);
  count = strlen(str);

  for (i = 0; i < count; ++i) {
    for (j = count - 1; j > i; --j) {
      if (str[j] < str[j-1]) {
      char t = str[j - 1];
      str[j - 1] = str[j];
      str[j] = t;
      }
    }
  }

  printf("The string '%s' was successfully written \n", str);

  file = fopen("text_file_prog.txt", "w");
  fprintf(file, "%s", str);
  fclose(file);

  return 0;
}
