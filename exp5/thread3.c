#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#define NUM 200
int stack[NUM][2];
int size = 0;
sem_t sem1, sem2, sem3, sem4;

void read1(void)
{ //读入data0中数据存入缓冲区
  FILE *fp = fopen("data0.txt", "r");
  while (!feof(fp))
  {
    sem_wait(&sem1);
    if (!fscanf(fp, "%d %d", &stack[size][0], &stack[size][1]))
      return;
    size++;
    sem_post(&sem2);
  }
  fclose(fp);
}

void read2(void)
{ //读入data1中数据存入缓冲区
  FILE *fp = fopen("data1.txt", "r");
  while (!feof(fp))
  {
    sem_wait(&sem2);
    if (!fscanf(fp, "%d %d", &stack[size][0], &stack[size][1]))
      return;
    size++;
    sem_post(&sem3);
  }
  fclose(fp);
}
void plus1(void)
{ //乘法运算
  while (1)
  {
    sem_wait(&sem3);
    if (size == 0)
    {
      return;
    }
    size--;
    printf("Plus: %d+%d=%d\n", stack[size][0], stack[size][1], stack[size][0] + stack[size][1]);
    sem_post(&sem4);
  }
}
void multi2(void)
{ //除法运算
  while (1)
  {
    sem_wait(&sem4);
    if (size == 0)
    {
      return;
    }
    size--;
    printf("Multiply: %d*%d=%d\n", stack[size][0], stack[size][1], stack[size][0] * stack[size][1]);
    sem_post(&sem1);
  }
}

int main()
{ //线程创建与程序执行
  pthread_t t1, t2, t3, t4;
  sem_init(&sem1, 0, 1);
  sem_init(&sem2, 0, 0);
  sem_init(&sem3, 0, 0);
  sem_init(&sem4, 0, 0);
  pthread_create(&t1, NULL, (void *)read1, NULL);
  pthread_create(&t2, NULL, (void *)read2, NULL);
  pthread_create(&t3, NULL, (void *)plus1, NULL);
  pthread_create(&t4, NULL, (void *)multi2, NULL);
  pthread_join(t1, NULL);
}
