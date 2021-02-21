#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <errno.h>
#include <fcntl.h>
#include "runcommand.h"
#include "utils.h"
pid_t bgpid[JOB_SIZE];
int indexInput(char **tokens);
int indexRedirect(char **tokens);
void runCommand(char **tokens)
{
    // Run the commands
    if (tokens == NULL)
    { // Error handling
        perror("NULL command encountered");
        exit(-1);
    }
    char *cmd = tokens[0];
    int posInput = indexInput(tokens);       // Index of '<' token
    int posRedirect = indexRedirect(tokens); // Index of '>' token
    int count = argCount(tokens);

    int runinbg = 0;

    int z;
    if (count >= 2)
    {
        for (z = 0; tokens[count - 2][z] != '\0'; z++)
        {
            if (tokens[count - 2][z] == '&')
            {
                runinbg = 1;
                tokens[count - 2][z] = '\0';
                break;
            }
        }
    }
    else
    {
        for (z = 0; tokens[count - 1][z] != '\0'; z++)
        {
            if (tokens[count - 1][z] == '&')
            {
                runinbg = 1;
                tokens[count - 1][z] = '\0';
                break;
            }
        }
        // printf("%s kk \n",tokens[0]);
    }
    if (tokens[count - 1][0] == '&')
    {
        runinbg = 1; //last token of input is & So run in background
        tokens[count - 1] = NULL;
    }

    if (posRedirect == -2)
    { // More than one redirect token found
        perror("Incorrect IO redirection");
        exit(-1);
    }
    pid_t pid = fork();
    int i, status;
    if (pid == -1)
    {
        printf("\nFailed  while forking child\n");
        return;
    }
    else if (pid == 0)
    {
        char *command = tokens[0];
        if (posInput >= 0)
        {
            int fd0 = open(tokens[posInput + 1], O_RDONLY);
            dup2(fd0, STDIN_FILENO);
            tokens[posInput] = NULL;
            close(fd0);
        }
        if (posRedirect >= 0)
        {
            int fd1 = open(tokens[posRedirect + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            dup2(fd1, STDOUT_FILENO);
            tokens[posRedirect] = NULL;
            close(fd1);
        }
        if (execvp(command, tokens) < 0)
        {
            printf("\nCould not execute command..");
        }
        exit(0);
    }
    else
    {
        //parent
        //wait for child to complete
        if (!runinbg)
            waitpid(pid, &status, 0);
        else
        {
            int j;
            waitpid(pid, &status, WNOHANG);
            //push <pid> of bg process to queue
            bgpid[prev] = pid;
        }
        return;
    }
}

int indexInput(char **tokens)
{
    // Returns first index of '<', -2 if multiple
    if (tokens == NULL)
        return -1;

    int posFound = -1;
    for (int i = 0; tokens[i] != NULL; ++i)
    {
        if (strcmp("<", tokens[i]) == 0)
        {
            if (posFound >= 0)
                return -2;

            posFound = i;
        }
    }
    return posFound;
}

int indexRedirect(char **tokens)
{
    // Returns first index of '>', -2 if multiple
    if (tokens == NULL)
        return -1;

    int posFound = -1;
    for (int i = 0; tokens[i] != NULL; ++i)
    {
        if (strcmp(">", tokens[i]) == 0)
        {
            if (posFound >= 0)
                return -2;

            posFound = i;
        }
    }
    return posFound;
}
