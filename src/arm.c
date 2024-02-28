#include "../include/arm.h"
#include "../include/defines.h"
#include "../include/mach.h"

static addr_t targets[2];

// Is a given instruction a branch, if so what is it's target
uint8_t is_branch(uint32_t instruction, addr_t *branch_target, addr_t pc, pid_t pid);

void get_next_instructions(uint8_t *target_count, addr_t **_targets, uint32_t instruction, addr_t pc, pid_t pid) {
  targets[0] = pc + 4; // Always have option of incrementiting

  addr_t branch_target;
  if (is_branch(instruction, &branch_target, pc, pid)) {
    targets[1] = branch_target;
    *target_count = 2;
  } else {
    *target_count = 1;
  }
  *_targets = targets;
}

uint8_t is_branch(uint32_t instruction, addr_t *branch_target, addr_t pc, pid_t pid) {
  // B and Bl
  if ((instruction & 0x7C000000) == 0x14000000) {
    *branch_target = pc + (((instruction & 0x03FFFFFF) + 2/*PC-offset*/) << 2);
    return 1;
  }
  // B.cond
  if ((instruction & 0xFF000000) == 0x54000000) {
    *branch_target = pc + (((((instruction & 0x00FFFFE0) >> 5)) + 2/*PC-offset*/) << 2);
    return 1;
  }
  // Blr and Br
  if ((instruction & 0xFFDFC01F) == 0xD51F0000) {
    uint8_t reg_num = (instruction >> 5) & 0x1F;
    arm_thread_state64_t state = get_state(pid);
    *branch_target = state.__x[reg_num];
    return 1;
  }
  // Ret
  if ((instruction & 0xFFFFC01F) == 0xD5FF0000) {
    uint8_t reg_num = (instruction >> 5) & 0x1F;
    arm_thread_state64_t state = get_state(pid);
    *branch_target = state.__x[reg_num];
    return 1;
  }
  return 0;
}

// Arm is Little Endian
unsigned int fetch_instruction(addr_t addr, pid_t pid) {
  uint8_t buffer[4];
  read_mem(pid, buffer, 4, addr);
  return *((unsigned int*) buffer);
}
