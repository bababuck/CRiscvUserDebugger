/**
 * Handle enabling and disabling breakpoints.
 */

#pragma once

#include <stdint.h>

#include "mach.h"


/**
 * Hold data about breakpoint location and old data.
 */
class breakpoint_t {
 public:
  breakpoint_t(const addr_t addr, const int child_pid);
  breakpoint_t();

  void enable();
  void disable();
  bool enabled;
 private:
  addr_t addr;
  int child_pid;
  uint8_t saved_inst[4];
};
