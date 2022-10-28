#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void enQueue(int,int*,int*,int,int*);
void display();

int main(){
  int size;
  int check;
    
  do {
    printf("Введите размер массива\n");
    scanf("%*s %d", &size);
  } while (size==NULL || size<=0);

    int qu[size];
    int rear=-1, frnt=-1;
    int t;
    int s=1;
   
    int value;
    int m=0;
    while (s==1){
      printf("Что вы хотите сделать?\n1-Добавить элемент\n2-Посмотреть список\n0-Вийти\n");
      scanf("%*s %d",&t);
      switch (t){
        case 1:
          printf("Введите число которое хотите вписать:");
          scanf("%d",&value);
          enQueue(value,qu,&rear,size,&m);
          break;
        case 2:
          display(qu,&rear,size,&m);
          break;
        case 0:
          break;
        default:continue;
      }

      if(t==0) {
        return 0;
      }
    }
  

    return 0;
}
void enQueue(int value,int *items,int *rear,int size,int *m) {
  if (*rear == size - 1){
    *rear=-1; 
    *m=1;
  }
    
  if(*rear!=size-1) {
    (*rear)++;
    items[*rear] = value;
    printf("\nДобавлено значение -> %d", value);
  }
}


  void display(int *items,int *rear,int size,int *m) {
    if (*rear == -1)
      printf("\nОчередь пуста");
    else {
      if(*m ==0){
        for(int i=0;i<=*rear;i++){
          printf("qu[%d]=%d\n",i,items[i]);
     }
    }
      else{
        for(int i=0;i<size;i++){
          printf("qu[%d]=%d\n",i,items[i]);
     }
    }
  }
            printf("\n");
}
