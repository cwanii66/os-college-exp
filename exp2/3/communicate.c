#include <stdio.h>
#include <unistd.h> //添加fork()函数头文件
#include <signal.h> //signal头文件
#include <stdlib.h> //exit需头文件

int main()
{
  int fd[3], pid1, pid2;
  char OutPipe[100], InPipe[100];
  pipe(fd); //创建管道，fd[0]指向管道的读端，fd[1]指向管道的写端
  while ((pid1 = fork()) == -1);
  if (pid1 == 0)
  {
    printf("child process1\n");
    sprintf(OutPipe, "child1 is sending a message!"); //给OutPipe赋值
    write(fd[1], OutPipe, 50);
    exit(0);
  }
  else
  {
    while ((pid2 = fork()) == -1);
    if (pid2 == 0)
    {
      printf("child process2\n");
      sprintf(OutPipe, "child2 is sending a message!");
      write(fd[1], OutPipe, 50);
      exit(0);
    }
    else
    {
      printf("parent process\n");
      wait(0);
      read(fd[0], InPipe, 50);
      printf("%s\n", InPipe);
      wait(0);
      read(fd[0], InPipe, 50);
      printf("%s\n", InPipe);
      exit(0);
    }
  }
  return 0;
}
