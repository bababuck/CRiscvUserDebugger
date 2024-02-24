#include <stdint.h>

#include <mach/thread_status.h>

#pragma once

typedef int pid_t;
typedef unsigned long pc_t;
typedef unsigned long addr_t;

#ifdef __cplusplus
extern "C" {
#endif

arm_thread_state64_t get_state(pid_t pid);

pc_t get_pc(pid_t pid);

void write_mem(pid_t pid, uint8_t *data, unsigned int bytes, addr_t addr);

void read_mem(pid_t pid, uint8_t *buffer, unsigned int bytes, addr_t addr);

#ifdef __cplusplus
}
#endif
