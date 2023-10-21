#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>

struct msg_buffer {
  long msg_type;
  char msg_text[1024];
};

int main() {
  key_t key = 75; // 消息队列的键值
  int msgid;
  struct msg_buffer message;

  msgid = msgget(key, 0666 | IPC_CREAT); // 创建消息队列

  pid_t client_pid, server_pid;

  // fork Server子进程
  server_pid = fork();
  if (server_pid == 0) {
    // Server子进程
    while (1) {
      if (msgrcv(msgid, &message, sizeof(message.msg_text), 0, 0) == -1) {
        perror("msgrcv");
        exit(1);
      }
      if (message.msg_type == 1) {
        msgctl(msgid, IPC_RMID, NULL);
        printf("\nServer: Received message type of \"%ld\", remove the queue.\n", message.msg_type);
        break;
      }

      printf("(Server) received: %s | type: %ld\n", message.msg_text, message.msg_type);
    }
    exit(0);
  }
  else if (server_pid > 0) {
      // 父进程fork Client子进程
      client_pid = fork();
      if (client_pid == 0) {
        // Client子进程
        for (int i = 10; i > 0; i--) {
          message.msg_type = i;
          strcpy(message.msg_text, "A man is not old as long as he is seeking something. A man is not old until regrets take the place of dreams.");

          if (msgsnd(msgid, &message, sizeof(message.msg_text), 0) == -1) {
            perror("msgsnd");
            exit(1);
          }

          printf("(Client) sent: %s | type: %ld\n", message.msg_text, message.msg_type);
        }

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

  return 0;
}
