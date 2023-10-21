#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

struct shared_data_st {
  int written;
  char text[1024];
};

void dt_shm(void *shm_ptr) {
  if (shmdt(shm_ptr) == -1) { // 断开共享内存连接
    perror("shmdt");
    exit(1);
  }
}
void rm_shm(int shmid) {
  if (shmctl(shmid, IPC_RMID, NULL) == -1) { // 删除共享内存
    perror("shmctl");
    exit(1);
  }
}

int main() {
  key_t key = 75;
  char *message = "A man is not old as long as he is seeking something. A man is not old until regrets take the place of dreams.";
  pid_t server_pid, client_pid;

  // 父进程fork Server子进程
  server_pid = fork();
  if (server_pid == 0) {
    // Server子进程
    struct shared_data_st *sdt;
    int shmid;
    void *shm_ptr;

    shmid = shmget(key, sizeof(*sdt), IPC_CREAT | 0666); // 创建共享内存
    if (shmid == -1) {
      perror("shmget");
      exit(1);
    }

    shm_ptr = shmat(shmid, NULL, 0); // 连接共享内存到当前进程
    sdt = (struct shared_data_st *)shm_ptr;

    sdt->written = 0; // 可写

    while (1) {
      while (sdt->written == 0) {
        sleep(1); // 睡眠1秒，等待Client子进程写入共享内存
        printf("Server: waiting for Client write...\n");
      }
      
      printf("(Server) received: %s\n", sdt->text);
      sdt->written = 0; // 可写

      if (strcmp(sdt->text, "exit") == 0) break;

      sleep(1);
    }

    rm_shm(shmid); // 删除共享内存
    dt_shm(shm_ptr); // 断开共享内存连接

    exit(0);
  }
  else if (server_pid > 0) {
    // 父进程fork Client子进程
    client_pid = fork();
    if (client_pid == 0) {
      // Client子进程
      sleep(2); // 睡眠2秒，等待Server子进程初始化

      struct shared_data_st *sdt;
      int shmid;
      void *shm_ptr;

      shmid = shmget(key, sizeof(*sdt), 0666); // 连接共享内存
      if (shmid == -1) {
        perror("shmget");
        exit(1);
      }

      shm_ptr = shmat(shmid, NULL, 0); // 连接共享内存到当前进程
      sdt = (struct shared_data_st *)shm_ptr;

      for (int i = 10; i > 0; i--) {
        while (sdt->written == 1) {
          sleep(1); // 睡眠1秒，等待Server子进程读取共享内存
          printf("Client: waiting for Server read...\n");
        }
        if (i == 1) {
          strcpy(sdt->text, "exit");
        }
        else {
          strcpy(sdt->text, message);
        }
        sdt->written = 1; // 可读

        printf("(Client) sent: %s\n", sdt->text);
      }

      dt_shm(shm_ptr); // 断开共享内存连接

      exit(0);
    }
  }
  else {
    perror("fork");
    exit(1);
  }

  // Wait for both child processes to finish
  int status;
  waitpid(server_pid, &status, 0);
  waitpid(client_pid, &status, 0);

  exit(0);
}
