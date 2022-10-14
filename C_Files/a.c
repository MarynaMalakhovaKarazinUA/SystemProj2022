#include<stdio.h>
#include<stdbool.h>

int main() {
   int size;
   printf("Enter size: ");
   scanf(" %d", &size);
   char arr [size];
   bool isLatin = false;
   char * filename = "text.txt";
   FILE *fp;
   int i = 0;
   while (i < size) {
      char ch;
      printf("Enter the letter: ");
      scanf(" %c", &ch);

      if ((ch >= 97 && ch <= 122) || (ch >= 65 && ch <= 90)){
        isLatin = true;
      }
      else {
        isLatin = false;
      }

      if(isLatin == true){
        arr[i] = (char) ch;
        i++;
      }
      else {
        printf("Wrong input!\n");
      }
   }

   for (int i = 0; i < size; i++) {
      for (int j = 0; j < size - 1; j++) {
          if (arr[i] < arr[j]) {
            char temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
          }
      }
   }
   if ((fp= fopen(filename, "w"))==NULL) {
      perror("Error occured while opening file");
      return 1;
   }

    for(int i = 0; i < size; i++) {
      fprintf(fp,"%c\n", arr[i]);
    }
    
    fclose(fp);
    return 0;
}
