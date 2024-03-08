#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../include/child.h"


void launch_child(int argc, char** argv) {
  char *user_program = argv[0];
  execv(user_program, argv);
}
