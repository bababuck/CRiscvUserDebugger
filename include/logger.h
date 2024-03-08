#pragma once

void open_log();

void close_log();

#ifdef __cplusplus
extern "C" {
#endif

void logger(const char* tag, const char* message);

#ifdef __cplusplus
}
#endif
