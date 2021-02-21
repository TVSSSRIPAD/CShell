#ifndef RUN_COMMAND_H
#define RUN_COMMAND_H
#define JOB_SIZE 10
#include <stdio.h>
extern int prev;
extern pid_t bgpid[JOB_SIZE];
void runCommand(char **tokens);
#endif
