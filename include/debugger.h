/**
 * Handle all debug related functionality.
 *
 * Only a single entry point, the rest is internally handled.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Launch the debugger.
 *
 * @params child_pid: Process that is being debugged.
 * @params program_name: Name of the program being debugged, for tracing purposes.
 */
void launch_debugger(int child_pid, char *program_name);

#ifdef __cplusplus
}
#endif
