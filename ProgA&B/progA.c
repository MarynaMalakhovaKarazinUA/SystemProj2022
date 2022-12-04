#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main()
{
    bool latinLetterCheck = false;
    int i = 0;
    int j = 0;
    
    int sizeOfYourArray;
    printf("Enter size of your array: ");
    scanf("%d", &sizeOfYourArray);
    char arrayOfYourLetters[sizeOfYourArray];

    
    while (i < sizeOfYourArray)
    {
       	char enterLetter;
        printf("Enter a letter: ");
        scanf(" %c", &enterLetter);

        if ((enterLetter >= 97 && enterLetter <= 122) || (enterLetter >= 65 && enterLetter <= 90))
        {
            latinLetterCheck = true;
        } else {
            latinLetterCheck = false;
        }

        if (latinLetterCheck == true)
        {
            arrayOfYourLetters[i] = (char)enterLetter;
            i++;
        } else {
            printf("You entered a non-latin letter\n");
        }
    }

    for (i = 0; i < sizeOfYourArray; i++)
    {
        for (j = 0; j < sizeOfYourArray - 1; j++)
        {
            if (arrayOfYourLetters[i] < arrayOfYourLetters[j])
            {
                char temp = arrayOfYourLetters[i];
                arrayOfYourLetters[i] = arrayOfYourLetters[j];
                arrayOfYourLetters[j] = temp;
            }
        }
    }
    
	char *filename = "useless.txt";
    FILE *file;
    if ((file = fopen(filename, "w")) == NULL)
    {
        perror("Error opening file");
      	return(-1);
   }

    for (i = 0; i < sizeOfYourArray; i++)
    {
        fprintf(file, "%c\n", arrayOfYourLetters[i]);
    }

    fclose(file);
    return 0;
}
