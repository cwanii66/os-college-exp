#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main()
{
  pid_t pid; // process ID
  printf("Before fork, PID = %d, PPID = %d. ", getpid(), getppid());
  printf("\n");

  pid = fork(); // to create a child process
  if (pid == -1)
  {
    perror("Failed in calling fork");
    exit(1);
  }
  else if (pid == 0)
  {
    /* the child process */
    printf("In child process, PID = %d, PPID = %d. ", getpid(), getppid());
    printf("\n");

    // ----- A -----
    sleep(3); // 程序暂停3秒
    // ----- B -----
    // exit(0); // 退出进程
  }
  else
  {
    /* the parent process */
    printf("In parent process, child's PID = %d. ", pid);
    printf("\n");

    printf("In parent process, PID = %d, PPID = %d. ", getpid(), getppid());
    printf("\n");

    // ----- C -----
    // wait(NULL); // 父进程等待子进程中断或结束
  }

  printf("Before return in main(), PID = %d, PPID = %d. ", getpid(), getppid());
  printf("\n");

  return (EXIT_SUCCESS);
}
