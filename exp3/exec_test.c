// exec_test.c
#include <stdio.h>
#include <stdlib.h>
// #include <error.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char** argv) {
    int result;

    result = fork();
    if (result < 0) {
        printf("Failed to create child");
        exit(1);
    } else if (result == 0) {
        // Child 1
        char *cmd = "./dummy";

        printf("Child process's PID is %d. My parent's PID is %d.\n", getpid(), getppid());
        printf("Child process is about to execute \"%s\"\n\n", cmd);

        result = execlp(cmd, cmd, NULL);
        if (result == -1) {
            printf("In child process, failed to exec a program");
        }

        exit(0);
    } else {
        // parent
        int status;

        printf("Parent process's PID is %d.\n", getpid());
        printf("Parent process is waiting ... \n");
        wait(&status);
        printf("In parent process, status = 0x%x, WEXITSTATUS(status) = %d (i.e. 0x%x)\n", \
                status, WEXITSTATUS(status), WEXITSTATUS(status));
    }

    return (EXIT_SUCCESS);
}
