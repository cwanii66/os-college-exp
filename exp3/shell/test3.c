#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

void Execute(int argc, char *argv[]);
void Operate(char *buf);

int main()
{
  char buf[50]; // 输入缓冲区
  while (1)
  {                                 // 在输入 quit 之前，可以实现多次执行
    memset(buf, 0x00, sizeof(buf)); // 初始化内存空间
    scanf("%[^\n]%*c", buf); // 读取一行字符，利用正则表达式，遇到\n 则结束读入
    // *c 表示跳过换行符，不把换行符存入 buf

    if (strcmp(buf, "quit") == 0)
    {
      exit(0); // 输入 quit，退出
    }

    Operate(buf); //处理输入的字符串
  }

  return 0;
}

void Execute(int argc, char *argv[])
{
  pid_t pid;
  int result;
  pid = fork();
  if (pid < 0)
  {
    perror("Failed to create child");
    exit(1);
  }
  else if (pid == 0)
  {
    printf("Child process's PID is %d. My parent's PID is %d.\n", getpid(), getppid());
    printf("Child process is about to execute \"%s\"\n\n", argv[0]);
    result = execvp(argv[0], &argv[0]); //执行命令

    if (result == -1)
    {
      perror("In child process, failed to exec a program");
      exit(1);
    }
    exit(0);
  }
  else
  {
    // parent
    int status;
    printf("Parent process's PID is %d , PPID is %d.\n", getpid(), getppid());
    printf("Parent process is waiting ... \n");
    wait(&status);
    printf("In parent process, status = 0x%x, WEXITSTATUS(status) = %d (i.e.0x%x)\n", status, WEXITSTATUS(status), WEXITSTATUS(status));
  }
}

void Operate(char *buf)
{
  char *argv[10]; //命令参数
  int argc = 0;   //命令个数

  char *temp = NULL;   // 保存每次执行 strtok_r 被拆分下来的参数
  char *space = " ";   // 拆分标志
  char *remain = NULL; // 保存每次执行 strtok_r 之后剩余的字符串
  temp = strtok_r(buf, space, &remain);

  while (temp)
  {
    // printf("temp[%s]; remain[%s]\n",temp,remain); //执行本句可以查看每次字符串的改变情况
    argv[argc] = temp;
    argc++;
    temp = strtok_r(NULL, space, &remain); // 切割字符串，返回字符串首指针
  }

  argv[argc] = NULL; //命令行参数结尾必须为 NULL

  Execute(argc, argv); //执行子程序
}
