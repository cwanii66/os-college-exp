#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

void thread_1(void) {
  printf("this is thread1...\n");
}
void thread_2(void) {
  printf("this is thread2...\n");
}

int main() {
  pthread_t id1, id2;
  int ret1, ret2;

  ret1 = pthread_create(&id1, NULL, (void*)thread_1, NULL);
  if (ret1 != 0) {
    printf("create thread1 failed...\n");
    exit(1);
  }
  ret2 = pthread_create(&id1, NULL, (void*)thread_2, NULL);
  if (ret2 != 0) {
    printf("create thread2 failed...\n");
    exit(1);
  }

  pthread_join(id1, NULL);
  pthread_join(id2, NULL);
  
  return 0;
}
