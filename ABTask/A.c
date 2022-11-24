#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

int main() {
   int sizeOfText;
   printf("Enter size of text: ");
   scanf(" %d", &sizeOfText);
   char array [sizeOfText];
   bool engLetter = false;
   char *filename = "Task_text.txt";
   FILE *file;
   int i = 0;
   while (i < sizeOfText) {
      char letter;
      printf("Enter the letter: ");
      scanf(" %c", &letter);
      if ((letter <= 90 && letter >= 65)|| (letter <= 122 && letter >= 97) )
      {
        engLetter = true;
      }
      else engLetter = false;
      if(engLetter == true)
      {
        array[i] = (char) letter;
        i++;
      }
      else
      {
        printf("ERROR! Incorrect data. Enter only latin letters.\n");
        return 2;
      } 
   }
   for (int i = 0; i < sizeOfText; i++)
   {
      for (int j = 0; j < sizeOfText - 1; j++)
      {
          if (array[i] < array[j])
          {
            char tmp = array[i];
            array[i] = array[j];
            array[j] = tmp;
          }
      }
   }
   if ((file= fopen(filename, "w")) == NULL)
   {
      perror("ERROR! File cannot be opened.");
      return 1;
   }
   for(int i = 0; i < sizeOfText; i++)
   {
     fprintf(file,"%c\t", array[i]);
   }  
   printf("Success!\n");
   fclose(file);
   return 0;
}
