#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

void Operate(char *buf);
void Execute(int argc, char *argv[]);

int main(int argc, char *argv[]) {
  char buf[50];

  while(1) {
    memset(buf, 0x00, sizeof(buf));
    scanf("%[^\n]%*c", buf); // 读取一行字符，利用正则表达式，遇到\n 则结束读入
    
    // handle quit input
    if (strcmp(buf, "quit") == 0) {
      exit(0);
    }

    Operate(buf);
  }

  return 0;
}

void Operate(char *buf) {
  char *argv[10]; // 命令参数
  int argc = 0; // 输入命令索引

  char *token = NULL;
  char *delim = " ";
  char **remain = NULL;

  token = strtok_r(buf, delim, remain); // 按定界符切割出来的字符

  while (token) {
    argv[argc] = token; // collect the token
    argc += 1;
    token = strtok_r(NULL, delim, remain); // cut input repeatedly
  }

  Execute(argc, argv);
}

void Execute(int argc, char **argv) {
  pid_t pid;
  int result;

  pid = fork();

  if (pid < 0) {
    printf("failed to create child process");
    exit(1);
  }
  else if (pid == 0) {
    // child process
    printf("Child process %d is running which parent is %d\n", getpid(), getppid());
    printf("program %s is about to running...\n", argv[0]);
    result = execvp(argv[0], &argv[0]);
    if (result == -1) {
      perror("In child process, failed to exec a program");
      exit(1);
    }
    exit(0);
  }
  else {
    // parent
    int status;
    printf("Parent process's PID is %d , PPID is %d.\n", getpid(), getppid());
    printf("Parent process is waiting ... \n");
    wait(&status);
    printf("In parent process, status = 0x%x, WEXITSTATUS(status) = %d (i.e.0x%x)\n", status, WEXITSTATUS(status), WEXITSTATUS(status));
  }
}
