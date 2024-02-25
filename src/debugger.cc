#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <unordered_map>
#include <vector>

#include <sys/types.h>
#include <sys/ptrace.h>

#include <../include/breakpoint.h>
#include <../include/debugger.h>
#include "../include/logger.h"


/**
 * Handle all debug functionality.
 */
class debugger_t {
private:
  int child_pid;
  std::string program_name;
  std::unordered_map<addr_t, breakpoint_t> breakpoints;

  void execute(const std::string &command);
  void resume_child_process();
  void step_child_process();
  std::vector<std::string> parse_command(const std::string &line);
  void add_breakpoint_at(addr_t addr);
  void delete_breakpoint(breakpoint_id_t id);
  void list_breakpoints();
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

debugger_t::debugger_t(int child_pid, char *program_name): child_pid(child_pid), program_name(program_name), breakpoints() {
  int status;
  logger("debug_proc", "Waiting for child to stop");
  if (waitpid(child_pid, &status, WUNTRACED) == -1)
    perror("waitpid() failed");
  logger("debug_proc", "Child stopped");
}

void debugger_t::run() {
  logger("debug_proc", "Debugger run() called");

  std::string command;
  while (1) {
    printf("Stopped at 0x%lx > ", get_pc(child_pid));
    std::getline(std::cin, command);
    execute(command);
    int status;
    /*    if (waitpid(child_pid, &status, 0) == -1)
      perror("waitpid() failed");
    else
    break;*/
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
  } else if (command == "break" || command == "b") {
    if (argc == 2) {
      addr_t addr = stoull(args[1], nullptr, 16);
      add_breakpoint_at(addr);
    } else {
      std::cerr << "Expected one argument with 'break'" << std::endl;
    }
  } else if (command == "del" || command == "d") {
    if (argc == 2) {
      breakpoint_id_t id = stoul(args[1], nullptr, 10);
      delete_breakpoint(id);
    } else {
      std::cerr << "Expected one argument with 'del'" << std::endl;
    }
  } else if (command == "info" || command == "i") {
    if (argc == 2) {
      auto query = args[1];
      if (query == "break" || query == "b") {
        list_breakpoints();
      } else {
        std::cout << "Unknown command" << std::endl;
      }
    } else {
      std::cerr << "Expected one argument with 'ifno'" << std::endl;
    }
  } else {
    std::cout << "Unknown command" << std::endl;
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
  ptrace(PT_CONTINUE, child_pid, (caddr_t) 1, 0);

  // Wait until process continues
  int status;
  if (waitpid(child_pid, &status, 0) == -1)
    perror("waitpid() failed");
}

void debugger_t::step_child_process(){
  ptrace(PT_STEP, child_pid, (caddr_t) 1, 0);

  // Wait until process steps
  int status;
  if (waitpid(child_pid, &status, 0) == -1)
    perror("waitpid() failed");
}

void debugger_t::add_breakpoint_at(addr_t addr){
  breakpoint_t breakpoint(addr, child_pid);
  breakpoint.enable();
  breakpoints[addr] = breakpoint;
}

void debugger_t::delete_breakpoint(breakpoint_id_t id){
  for (auto it = breakpoints.begin(); it != breakpoints.end(); ++it) {
    breakpoint_t breakpoint = it->second;
    if (breakpoint.id == id) {
      if (breakpoint.enabled)
        breakpoint.disable();

      breakpoints.erase(it);
      return;
    }
  }
  printf("Breakpoint not found with ID= %d\n", id);
}

void debugger_t::list_breakpoints() {
  std::cout << "  Num    Type      Enb  Address\n";
  for (auto breakpoint_pair : breakpoints) {
    std::cout << breakpoint_pair.second.list_string() << "\n";
  }
  std::cout << std::endl;
}
