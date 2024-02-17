#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/logger.h"

FILE* global_log = NULL;

void open_log() {
  if (global_log == NULL)
    global_log = fopen ("log.txt", "w+");
}

void close_log() {
  fclose(global_log);
}

void logger(const char* tag, const char* message) {
  time_t now;
  time(&now);
  fprintf(global_log, "%s [%s]: %s\n", ctime(&now), tag, message);
  printf("%s [%s]: %s\n", ctime(&now), tag, message);
}
