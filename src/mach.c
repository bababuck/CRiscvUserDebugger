#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <mach/mach.h>
#include <mach/mach_types.h>
#include <mach/thread_status.h>

#include "../include/logger.h"
#include "../include/mach.h"

task_t get_task(pid_t pid) {
  task_t task;
  if (task_for_pid(mach_task_self(), pid, &task))
    perror("task_for_pid() failed");

  char* string;
  if(asprintf(&string, "Found task=%x for pid=%x\n", task, pid) < 0)
    perror("asprintf() formatting failed");
  logger("debug_proc", string);
  free(string);
  return task;
}

arm_thread_state64_t get_state(pid_t pid){
  task_t task = get_task(pid);
  long thread = 0;
  thread_act_port_array_t thread_list;
  mach_msg_type_number_t thread_count;
  mach_msg_type_number_t sc = ARM_THREAD_STATE64_COUNT;
  arm_thread_state64_t arm_state;

  if (task_threads(task, &thread_list, &thread_count))
    perror("task_threads() failed");

  if (thread_get_state(thread_list[0],
                       ARM_THREAD_STATE64,
                       (thread_state_t)&arm_state,
                       &sc) ||
      sc != ARM_THREAD_STATE64_COUNT)
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

void write_mem(pid_t pid, uint8_t *data, unsigned int bytes, addr_t addr) {
  task_t task = get_task(pid);
  kern_return_t error;

  if ((error = vm_protect(task, addr, bytes, FALSE, VM_PROT_READ | VM_PROT_WRITE | VM_PROT_COPY)))
    fprintf(stderr, "vm_protect() failed with code= %d\n", error);

  if ((error = vm_write(task, (vm_address_t) addr, (vm_offset_t) data, (mach_msg_type_number_t) bytes)))
    fprintf(stderr, "vm_write() failed with code= %d\n", error);

  if ((error = vm_protect(task, addr, bytes, FALSE, VM_PROT_READ | VM_PROT_EXECUTE)))
    fprintf(stderr, "vm_protect() failed with code= %d\n", error);

  for (int i=0; i<bytes; ++i) {
    char* string;
    if(asprintf(&string, "Data= %hhx written to Addr= %lx\n", data[i], (vm_address_t) (addr + 1)) < 0)
      perror("asprintf() formatting failed");
    logger("debug_proc", string);
    free(string);
  }
}

void read_mem(pid_t pid, uint8_t *buffer, unsigned int bytes, addr_t addr) {
  get_pc(pid);
  task_t task = get_task(pid);
  kern_return_t error;
  vm_offset_t local_buffer;
  if ((error = vm_read(task, (vm_address_t) addr, bytes, (vm_offset_t*) &local_buffer, (mach_msg_type_number_t*) &bytes)))
    fprintf(stderr, "vm_read() failed with code= %d\n", error);

  memcpy(buffer, (const void *) local_buffer, bytes);

  for (int i=0; i<bytes; ++i) {
    char* string;
    if(asprintf(&string, "Data= %hhx read from Addr= %lx\n", buffer[i], (vm_address_t) (addr + i)) < 0)
      perror("asprintf() formatting failed");
    logger("debug_proc", string);
    free(string);
  }
}
