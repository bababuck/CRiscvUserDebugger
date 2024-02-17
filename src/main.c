#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../include/child.h"


int main(int argc, char** argv) {
  if (argc < 2) {
    perror("User program not specified");
  }

  int user_pid = fork();
  if (user_pid == 0) {
    // Forked Process
    launch_child(argc - 1, argv + 1);
  } else if (user_pid > 0) {
    // Debugger
  } else {
    perror("fork() failed");
  }
  return 0;
}
