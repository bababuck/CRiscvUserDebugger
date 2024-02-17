#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ptrace.h>

#include "../include/child.h"


void launch_child(int argc, char** argv) {
  char *user_program = argv[0];

  if (ptrace(PT_TRACE_ME, 0, 0, 0)) {
    // errno = 3;
    perror("PT_TRACE_ME request failed");
    exit(3);
  }

  execv(user_program, argv);
}
