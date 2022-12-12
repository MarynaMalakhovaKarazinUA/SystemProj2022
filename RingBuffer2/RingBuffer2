#include<stdio.h>
#include <stdlib.h>
#include<malloc.h>
#include<memory.h>
#include <time.h>

typedef struct ringBuff{
    time_t * buff;
    time_t start;
    time_t end;
    int size;
    int count;
} ringBufft;

ringBufft * ringBuff_new(int size)
{
  ringBufft * cb = (ringBufft*)malloc(sizeof(ringBufft));
  memset(cb, 0, sizeof(ringBufft));
  cb->size = size;
  cb->buff = (time_t*)malloc(sizeof(time_t)*size);

  return cb;
}

void ringBuff_add(ringBufft * cb)
{
  time_t now;
  time_t end = cb->end;
  time(&now);
  printf("Today is %s", ctime(&now));
  if(cb->count && (end % cb->size) == cb->start){
    printf("\nThe buffer is full, the element [%d] %s has been replaced.\n", cb->start, ctime(&cb->buff[cb->start]));
    cb->start = (cb->start + 1 ) %cb->size;
    cb->count --;
  }

  cb->buff[cb->end] = now;
  cb->end = (cb->end+1) % cb->size;
  cb->count ++;
  }

int ringBuff_remove(ringBufft * cb)
{
  time_t start = cb->start ;
  int ret = -1;
  if(cb->count <= 0) {
    printf("Buffer is empty\n");
    return ret;
  }
  if(cb->count || (start % cb->size) != cb->end) {
    printf("Removed Elem[%d] = %d\n",cb->start, cb->buff[cb->start]);
    ret = cb->buff[cb->start];
    cb->start = (cb->start + 1 ) % cb->size;
    cb->count--;
  } else {
    printf("Buffer is empty\n");
  }
  return ret;
}

void ringBuff_print(ringBufft * cb)
{
  time_t start = cb->start ;
  time_t end = cb->end ;
  int i, count = 0;
  for(i = start; count < cb->count; i = (i + 1)%cb->size){

    printf("Elem[%d] = %s\n", i, ctime(&cb->buff[i]));
    count++;
    if(i == (end - 1)) {
      break;
    }
  }
}

void ringBuff_delete(ringBufft * cb)
{
  free(cb->buff);
  free(cb);
}

int main()
{
  int key;
  int elem;
  int size;
  char * filename = "file.dat";

  printf("Enter the buffer size\n\n > ");

  while (scanf("%d", &size) == 0) {
      printf("You can only enter numbers here!\n\n > ");
          while(getchar()!='\n');
  }

  ringBufft *cb = ringBuff_new(size);

  while(1) {
    printf("\nRing buffer\n1. Add number\n2. Remove number\n3. Print buffer\n4. Create new buffer\n5. Exit\n\n > ");
    fflush(stdin);
    while (scanf("%d", &key) == 0) {
        printf("You can only enter numbers here!\n\n > ");
            while(getchar()!='\n');
    }
    printf("\n");
    switch(key){
      case 1:
      {
        ringBuff_add(cb);
        break;
      }
      case 2:
      {
        ringBuff_remove(cb);
        break;
      }
      case 3:
      {
        ringBuff_print(cb);
        break;
      }
      case 4:
      {
        ringBuff_delete(cb);

        printf("Enter the buffer size\n\n > ");

        while (scanf("%d", &size) == 0) {
            printf("You can only enter numbers here!\n\n > ");
                while(getchar()!='\n');
        }

        ringBufft * cb = ringBuff_new(size);
        break;
      }
      case 5:
      {
        ringBuff_delete(cb);
      }
      default:
      {
        printf("Try again!\n\n");
      }
    }
  }
  return 0;
}
