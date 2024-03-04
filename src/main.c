#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../include/exit_codes.h"

int main(int argc, char** argv) {
  int user_pid = fork();
  if (user_pid > 0) {
    // Debugger
  } else if (user_pid == 0) {
    // Child Process
  } else {
    perror("fork() failed");
    exit(DBG_SYSCALL_FAILURE);
  }
  return DBG_SUCCESS;
}
