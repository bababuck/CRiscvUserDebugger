#include <mach/thread_status.h>

#pragma once

typedef int pid_t;
typedef unsigned long pc_t;


#ifdef __cplusplus
extern "C" {
#endif

arm_thread_state64_t get_state(pid_t pid);

pc_t get_pc(pid_t pid);

#ifdef __cplusplus
}
#endif
