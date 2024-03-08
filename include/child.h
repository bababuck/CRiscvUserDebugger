/**
 * Module for handling the child process side.
 *
 * Single entry point that should be called by the child process
 */

#pragma once

/**
 * Launch the child process with the specified program and arguments.
 *
 * @params argc: Number of arguments for the child program
 * @params argv: Arguments supplied for the child program.
 */
void launch_child(int argc, char** argv);
