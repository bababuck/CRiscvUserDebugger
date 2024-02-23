#include <string>

#include <../include/debugger.h>
#include "../include/logger.h"


/**
 * Handle all debug functionality.
 */
class debugger_t {
private:
  int child_pid;
  std::string program_name;

public:
  debugger_t(int child_pid, char *program_name);
  void run();
};

extern "C" {

void launch_debugger(int child_pid, char *program_name) {
  debugger_t debugger(child_pid, program_name);
  debugger.run();
}

}

debugger_t::debugger_t(int child_pid, char *program_name): child_pid(child_pid), program_name(program_name) {
  int status;
  logger("debug_proc", "Waiting for child to stop");
  if (waitpid(child_pid, &status, WUNTRACED) == -1)
    perror("waitpid() failed");
  logger("debug_proc", "Child stopped");
}

void debugger_t::run() {}
