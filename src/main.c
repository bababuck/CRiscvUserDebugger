#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../include/child.h"
#include "../include/debugger.h"
#include "../include/logger.h"


int main(int argc, char** argv) {
  open_log();
  if (argc < 2) {
    perror("User program not specified");
  }

  int user_pid = fork();
  if (user_pid == 0) {
    // Forked Process
    logger("child_proc", "Child process started");
    launch_child(argc - 1, argv + 1);
    logger("child_proc", "Child process completed");
  } else if (user_pid > 0) {
    // Debugger
    logger("debug_proc", "Debug process started");
    launch_debugger(user_pid, argv[1]);
    logger("debug_proc", "Debug process completed");
  } else {
    perror("fork() failed");
  }
  close_log();
  return 0;
}
