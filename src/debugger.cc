#include <string>

#include <../include/debugger.h>


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

debugger_t::debugger_t(int child_pid, char *program_name): child_pid(child_pid), program_name(program_name) {}

void debugger_t::run() {}
