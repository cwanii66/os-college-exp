#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid, child;
    int *status;
    
    pid = fork(); // 创建子进程
    
    if (pid == 0) {
        // 子进程执行的代码
        printf("current child process is %d, its parent process is %d\n", getpid(), getppid());
        printf("Child process is running\n");
        exit(42); // 子进程退出，退出状态为42
    } else {
        // 父进程执行的代码
        child = wait(status); // 等待子进程结束
        printf("Child process %d exited with status %d\n", child, status);
    }
    
    return 0;
}
