#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main() {
  pid_t pid1, pid2; // process ID
  pid1 = fork();
  if (pid1 == -1) {
    printf("fork pid1 failed...");
    exit(1);
  }
  if (pid1 == 0) {
    // child process (pid1)
    printf("bbbbb\n");
    exit(0);
  } 
  else {
    // wait(NULL);
    pid2 = fork();
    if (pid2 < 0) {
      printf("fork pid2 faild...");
    }
    if (pid2 == 0) {
      printf("ccccc\n");
      exit(0);
    }
    else {
      // wait(NULL);
      printf("aaaaa\n");
      exit(0);
    }
  }
  return 0;
}
