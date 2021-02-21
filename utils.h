#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

void checkHostName(int hostname);
void printDir();
void printPrompt();
int argCount(char **tokens);
int isInBuilt(char *cmd);
char **parseCommand(char *line);
#endif
