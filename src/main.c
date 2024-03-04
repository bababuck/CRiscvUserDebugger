#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../include/child.h"
#include "../include/exit_codes.h"


int main(int argc, char** argv) {
  if (argc < 2) {
    perror("User program not specified");
    exit(DBG_ILLEGAL_ARGUMENTS);
  }

  int user_pid = fork();
  if (user_pid > 0) {
    // Debugger
  } else if (user_pid == 0) {
    // Child Process
    int child_argc = argc - 1;
    char** child_argv = argv + 1;
    launch_child(child_argc, child_argv);
  } else {
    perror("fork() failed");
    exit(DBG_SYSCALL_FAILURE);
  }
  return DBG_SUCCESS;
}
