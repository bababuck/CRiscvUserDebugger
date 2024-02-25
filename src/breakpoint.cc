/**
 * Handle enabling and disabling breakpoints.
 */

#include <iomanip>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../include/breakpoint.h"
#include "../include/logger.h"
#include "../include/mach.h"

breakpoint_id_t breakpoint_t::count = 0;

breakpoint_t::breakpoint_t(const addr_t addr, const int child_pid): enabled(false), addr(addr), child_pid(child_pid), id(count) {++count;}

breakpoint_t::breakpoint_t(): enabled(false), addr(0), child_pid(0), id(0) {}

void breakpoint_t::enable() {
  read_mem(child_pid, saved_inst, /*bytes*/4, addr);
  uint8_t zero_inst[4] = {0, 0, 0, 0};
  write_mem(child_pid, zero_inst, /*bytes*/4, addr);

  enabled = true;
  char* string;
  if(asprintf(&string, "Breakpoint enabled at Addr= %lx\n", addr) < 0)
    perror("asprintf() formatting failed");
  logger("debug_proc", string);
  free(string);
}

void breakpoint_t::disable() {
  uint8_t zero_inst[4] = {0, 0, 0, 0};
  uint8_t found_inst[4];
  read_mem(child_pid, found_inst, /*bytes*/4, addr);
  if ((uint32_t) *zero_inst != (uint32_t) *found_inst) {
    fprintf(stderr, "Breakpoint was modified.");
    exit(4);
  }
  write_mem(child_pid, /*data*/saved_inst, /*bytes*/4, addr);

  enabled = false;
}

std::string breakpoint_t::list_string() {
  std::stringstream ss;
  ss << " " << std::setw(5) << id << "  breakpoint  " << (enabled ? "y" : "n") << "  0x" << std::setw(10) << addr;
  return ss.str();
}
