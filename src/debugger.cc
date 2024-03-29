#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <vector>

#include <sys/types.h>
#include <sys/ptrace.h>

#include <../include/debugger.h>
#include "../include/logger.h"
#include "../include/exit_codes.h"


/**
 * Handle all debug functionality.
 */
class debugger_t {
private:
  int child_pid;
  std::string program_name;

  void execute(const std::string &command);
  void resume_child_process();
  void step_child_process();
  std::vector<std::string> parse_command(const std::string &line);
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
  if (waitpid(child_pid, &status, WUNTRACED) == -1) {
    perror("waitpid() failed");
    exit(DBG_SYSCALL_FAILURE);
  }
  logger("debug_proc", "Child stopped");
}

void debugger_t::run() {
  logger("debug_proc", "Debugger run() called");

  std::string command;
  while (1) {
    printf("> "); // Prompt
    std::getline(std::cin, command);
    execute(command);
    int status;
    if (waitpid(child_pid, &status, 0) == -1)
      perror("waitpid() failed");
    else
      break;
  }
}

void debugger_t::execute(const std::string &command_line) {
  auto args = parse_command(command_line);
  std::string command = args[0];
  int argc = args.size();

  if (command == "continue" || command == "c") {
    if (argc == 1) {
      logger("debug_proc", "Continue child program");
      resume_child_process();
    } else {
      std::cerr << "No arguments expected with 'continue'" << std::endl;
    }
  } else if (command == "step" || command == "s") {
    if (argc == 1) {
      logger("debug_proc", "Step child program");
      step_child_process();
    } else {
      std::cerr << "No arguments expected with 'step'" << std::endl;
    }
  } else {
    std::cerr << "Unknown command" << std::endl;
  }
}

std::vector<std::string> debugger_t::parse_command(const std::string &line){
  std::vector<std::string> parsed = {};
  std::stringstream ss(line);
  std::string arg;
  while (std::getline(ss, arg,' ')) {
    parsed.push_back(arg);
  }
  return parsed;
}

void debugger_t::resume_child_process(){
  if (ptrace(PT_CONTINUE, child_pid, (caddr_t) 1, 0)) {
    perror("ptrace() failed");
    exit(DBG_SYSCALL_FAILURE);
  }

  // Wait until process continues
  int status;
  if (waitpid(child_pid, &status, 0) == -1) {
    perror("waitpid() failed");
    exit(DBG_SYSCALL_FAILURE);
  }
}

void debugger_t::step_child_process(){
  if (ptrace(PT_STEP, child_pid, (caddr_t) 1, 0)) {
    perror("ptrace(PT_STEP) failed");
    exit(DBG_SYSCALL_FAILURE);
  }

  // Wait until process steps
  int status;
  if (waitpid(child_pid, &status, 0) == -1) {
    perror("waitpid() failed");
    exit(DBG_SYSCALL_FAILURE);
  }
}
