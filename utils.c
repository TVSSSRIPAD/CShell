#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#define SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

void checkHostName(int hostname);
void printDir();
void printPrompt();
int isInBuilt(char *cmd);
int argCount(char **tokens);
char **parseCommand(char *line);

int argCount(char **tokens)
{
    // Returns the number of tokens(arguments)
    if (tokens == NULL)
        return 0;

    int i = 0;
    while (tokens[i] != NULL)
        i++;

    return i;
}
int isInBuilt(char *cmd)
{
    // Check if given Command is inbuilt
    if ((strcmp(cmd, "cd") == 0) || (strcmp(cmd, "pwd") == 0) || (strcmp(cmd, "exit") == 0) || (strcmp(cmd, "about") == 0))
        return 1;
    if ((strcmp(cmd, "history") == 0) || (strcmp(cmd, "kill") == 0) || (strcmp(cmd, "jobs") == 0) || (strcmp(cmd, "help") == 0))
        return 1;
    return 0;
}

void printDir()
{
    /* Current Working Directory */
    char cwd[SIZE];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("Current working dir: %s\n", cwd);
    }
    else
    {
        perror("getcwd() error");
        return;
    }
}
void printPrompt()
{

    // Function to print directory and UserName for each terminal line
    int hostname; /* HostName */
    char hostbuffer[256];
    hostname = gethostname(hostbuffer, sizeof(hostbuffer));
    checkHostName(hostname);
    printf("%s :~", hostbuffer);
    char cwd[SIZE];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("%s", cwd);
    }
    else
    {
        perror("getcwd() error");
        return;
    }
    printf(">>");
    return;
}

void checkHostName(int hostname)
{
    // Check Host COmputer Name
    if (hostname == -1)
    {
        perror("gethostname");
        exit(1);
    }
}
char **parseCommand(char *line)
{
    // Parse the given arguments
    char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
    char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
    int i, tokenIndex = 0, tokenNo = 0;

    for (i = 0; i < strlen(line); i++)
    {
        char readChar = line[i];
        if (readChar == ' ' || readChar == '\n' || readChar == '\t')
        {
            token[tokenIndex] = '\0';
            if (tokenIndex != 0)
            {
                tokens[tokenNo] = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
                strcpy(tokens[tokenNo++], token);
                tokenIndex = 0;
            }
        }
        else
        {
            token[tokenIndex++] = readChar;
        }
    }

    free(token);
    tokens[tokenNo] = NULL;
    return tokens;
}
