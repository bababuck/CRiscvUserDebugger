#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char** argv) {
  int user_pid = fork();
  if (user_pid == 0) {
    // Debugger
  } else if (user_pid > 0) {
    // Forked Process
  } else {
    perror("fork() failed");
  }
  return 0;
}
