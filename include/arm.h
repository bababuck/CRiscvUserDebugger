#pragma once

#include <stdint.h>

#include "defines.h"

#ifdef __cplusplus
extern "C" {
#endif

// Return the next PC for the given instruction
void get_next_instructions(uint8_t *target_count, addr_t **_targets, uint32_t instruction, addr_t pc, pid_t pid);

// Fetch an instruction from the given address
unsigned int fetch_instruction(addr_t addr, pid_t pid);

#ifdef __cplusplus
}
#endif
