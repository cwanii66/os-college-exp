#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

struct menber
{
  int a;
  char *s;
};

void *num_type(void *arg)
{
  int *num;
  num = (int *)arg;
  printf("create param is %d\n", *num);
  return (void *)0;
}
void *ch_type(void *arg)
{
  char *ch;
  ch = (char *)arg;
  printf("create param is %c\n", *ch);
  return (void *)0;
}
void *struct_type(void *arg)
{
  struct menber *temp;
  temp = (struct menber *)arg;
  printf("create menber -> a %d\n", temp->a);
  printf("create menber -> s %s\n", temp->s);
  return (void *)0;
}

int main(int argc, char *argv[])
{
  pthread_t id1, id2, id3;
  int ret1, ret2, ret3;
  int num = 1;
  char ch = 'a';
  struct menber *b;
  b = (struct menber *)malloc(sizeof(struct menber));
  b->a = 6;
  b->s = "cwanii66";

  int *p_num = &num;
  char *p_ch = &ch;

  ret1 = pthread_create(&id1, NULL, (void *)num_type, (void *)p_num);
  if (ret1 != 0)
  {
    printf("create thread1 failed...\n");
    exit(1);
  }
  ret2 = pthread_create(&id2, NULL, (void *)ch_type, (void *)p_ch);
  if (ret2 != 0)
  {
    printf("create thread2 failed...\n");
    exit(1);
  }
  ret3 = pthread_create(&id3, NULL, (void *)struct_type, (void *)b);
  if (ret3 != 0)
  {
    printf("create thread3 failed...\n");
    exit(1);
  }

  pthread_join(id1, NULL);
  pthread_join(id2, NULL);
  pthread_join(id3, NULL);

  return 0;
}
