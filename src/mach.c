#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <mach/mach.h>
#include <mach/mach_types.h>
#include <mach/thread_status.h>

#include "../include/logger.h"
#include "../include/mach.h"

arm_thread_state64_t get_state(pid_t pid){
  task_t task;
  long thread = 0;
  thread_act_port_array_t thread_list;
  mach_msg_type_number_t thread_count;
  mach_msg_type_number_t sc = ARM_THREAD_STATE_COUNT;
  arm_thread_state64_t arm_state;

  if (task_for_pid(mach_task_self(), pid, &task))
    perror("task_for_pid() failed");

  if (task_threads(task, &thread_list, &thread_count))
    perror("task_threads() failed");

  if (thread_get_state(thread_list[0],
                       ARM_THREAD_STATE64,
                       (thread_state_t)&arm_state,
                       &sc) ||
      sc != ARM_THREAD_STATE_COUNT)
    perror("thread_get_state() failed");
  return arm_state;
}

pc_t get_pc(pid_t pid) {
  arm_thread_state64_t arm_state = get_state(pid);

  pc_t pc = arm_state.__pc;
  char* string;
  if(asprintf(&string, "PC= %lx\n", pc) < 0)
    perror("asprintf() formatting failed");
  logger("debug_proc", string);
  free(string);
  return pc;
}
