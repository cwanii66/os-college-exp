#include <pthread.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <math.h> 
#include <unistd.h> 
// #include <semaphore.h>
#include <errno.h>
#include <dispatch/dispatch.h>

#define NUM_PRODUCERS 3
#define NUM_CONSUMERS 4
#define BUFFER_SIZE 5
#define PACK_THRESHOLD 10

/**
* 垃圾机构体
* @member int type 垃圾类型
*   0表示厨余垃圾
*   1表示可回收垃圾
*   2表示有害垃圾
*   3表示其他垃圾
*/
typedef struct {
  int type;
} Garbage;

Garbage buffer[BUFFER_SIZE];
int buffer_index = 0; // 缓冲区索引

pthread_mutex_t mutex; // 互斥锁
// sem_t empty, full; // 信号量
dispatch_semaphore_t empty, full; // 信号量

void *producer(void *arg);
void *consumer(void *arg);

int main(int argc, char *argv[]) {
  pthread_mutex_init(&mutex, NULL); // 初始化互斥锁

  // sem_init(&empty, 0, BUFFER_SIZE); // 初始化缓冲区空信号量
  empty = dispatch_semaphore_create(BUFFER_SIZE);
  // sem_init(&full, 0, 0); // 初始化缓冲区满信号量
  full = dispatch_semaphore_create(0);

  pthread_t producer_threads[NUM_PRODUCERS];
  pthread_t consumer_threads[NUM_CONSUMERS];

  int producer_ids[NUM_PRODUCERS];
  int consumer_ids[NUM_CONSUMERS];

  for (int i = 0; i < NUM_PRODUCERS; i++) {
    producer_ids[i] = i;
    pthread_create(&producer_threads[i], NULL, producer, &producer_ids[i]);
  }

  for (int i = 0; i < NUM_CONSUMERS; i++) {
    consumer_ids[i] = i;
    pthread_create(&consumer_threads[i], NULL, consumer, &consumer_ids[i]);
  }

  // 等待所有线程结束
  for (int i = 0; i < NUM_PRODUCERS; i++) {
    pthread_join(producer_threads[i], NULL);
  }

  for (int i = 0; i < NUM_CONSUMERS; i++) {
    pthread_join(consumer_threads[i], NULL);
  }
  
  // 销毁互斥锁和信号量
  pthread_mutex_destroy(&mutex);
  // sem_destroy(&empty);
  // sem_destroy(&full);
  dispatch_release(empty);
  dispatch_release(full);

  return 0;
}

void *producer(void *arg) {
  int producer_id = *(int*)arg;

  while (1) {
    Garbage garbage;
    garbage.type = rand() % 4; // 随机生成垃圾类型

    // sem_wait(&empty); // 等待缓冲区空信号量
    dispatch_semaphore_wait(empty, DISPATCH_TIME_FOREVER);

    pthread_mutex_lock(&mutex); // 加锁
    if (buffer_index < BUFFER_SIZE) {
      buffer[buffer_index++] = garbage; // 垃圾放入缓冲区
      printf("Producer %d produced garbage of type %d\n", producer_id, garbage.type);
    }
    pthread_mutex_unlock(&mutex); // 解锁

    // sem_post(&full); // 发送缓冲区满信号量
    dispatch_semaphore_signal(full);

    usleep(800000); // 随机休眠一段时间
  }
  return NULL;
}

void *consumer(void *arg) {
  int garbage_count = 0; // 垃圾数量
  int consumer_id = *((int*)arg); // 消费者ID

  while (garbage_count < PACK_THRESHOLD) {
    // sem_wait(&full); // 等待缓冲区满信号量
    dispatch_semaphore_wait(full, DISPATCH_TIME_FOREVER);

    pthread_mutex_lock(&mutex); // 加锁
    
    // 处理缓冲区中的垃圾
    for (int i = 0; i <= buffer_index; i++) {
      if (buffer[i].type == consumer_id) {
        garbage_count++;
        printf("Consumer %d processed garbage of type %d (%d/%d)\n", consumer_id, buffer[i].type, garbage_count, PACK_THRESHOLD);
      }
    }

    // 删除缓冲区中已经处理的垃圾
    int j = 0;
    for (int i = 0; i <= buffer_index; i++) {
      if (buffer[i].type != consumer_id) {
        buffer[j] = buffer[i];
        if (i == buffer_index) break; // 保证j在以后不会越界
        j++;
      }
    }
    buffer_index = j;

    pthread_mutex_unlock(&mutex);

    // sem_post(&empty); // 发送缓冲区空信号
    dispatch_semaphore_signal(empty);

    usleep(800000);
  }
  printf("Consumer %d packed %d garbage\n", consumer_id, garbage_count);

  return NULL;
}
