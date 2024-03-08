#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../include/child.h"
#include "../include/exit_codes.h"
#include "../include/logger.h"


int main(int argc, char** argv) {
  open_log();
  if (argc < 2) {
    perror("User program not specified");
    exit(DBG_ILLEGAL_ARGUMENTS);
  }

  int user_pid = fork();
  if (user_pid > 0) {
    // Debugger
    logger("debug_proc", "Debug process started");
    logger("debug_proc", "Debug process completed");
  } else if (user_pid == 0) {
    // Child Process
    logger("child_proc", "Child process started");
    int child_argc = argc - 1;
    char** child_argv = argv + 1;
    launch_child(child_argc, child_argv);
    logger("child_proc", "Child process completed");
  } else {
    perror("fork() failed");
    exit(DBG_SYSCALL_FAILURE);
  }
  close_log();
  return DBG_SUCCESS;
}
