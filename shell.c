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
#define SIZE 1024
#define MAX_INPUT_SIZE 1024

#define JOB_SIZE 10 //for queue

void numRun(char *cmd, char line[]);
void runInbuilt(char **tokens, char *cmd);
void terminate(int n);
void pushToBG(char line[]);
void runCommand(char **tokens);

int numBGP = 0;
static pid_t bgPGID = 0;
int front = -1, rear = -1, size = 0, start = JOB_SIZE;
int fjobs = -1, rjobs = -1, sjobs = 0, prev;

char cmds[JOB_SIZE][MAX_INPUT_SIZE]; //queue for storing history

char bgjobs[JOB_SIZE][MAX_INPUT_SIZE]; //stores list of background processes

int main(int argc, char **argv)
{
	//By: T.V.S.S.Sripad

	char line[MAX_INPUT_SIZE];
	char **tokens;
	int i, j;
	char *cmdhistory[JOB_SIZE];

	while (1)
	{
		printPrompt();
		bzero(line, MAX_INPUT_SIZE); // clear previous inputs if any
		gets(line);
		//printf("Got command %s\n", line);

		line[strlen(line)] = '\n';	 //terminate with new line
		tokens = parseCommand(line); //parse the commands

		char *cmd = tokens[0]; // Get first command
							   //printf("%s",cmd);
		int isinbuilt = isInBuilt(cmd);
		if (isinbuilt == 1)
		{
			runInbuilt(tokens, tokens[0]);
		}
		else
		{
			if (cmd[0] == '!')
			{

				numRun(cmd, line);
			}
			else
			{
				if (tokens[argCount(tokens) - 1][0] == '&')
					pushToBG(line);
				runCommand(tokens);
			}
		}

		//push to queue
		if (rear == -1 && rear == front)
		{
			//queue is empty
			++rear;
			++front;
			//printf("%d %d",front,rear);
			for (j = 0; line[j] != '\n'; j++)
			{
				cmds[rear][j] = line[j];
			}
			cmds[rear][j] = '\0';
			size++;
		}
		else if (size < JOB_SIZE)
		{
			//queue is not full
			++rear;
			//printf("%d %d",front,rear);
			for (j = 0; line[j] != '\n'; j++)
			{
				cmds[rear][j] = line[j];
			}
			cmds[rear][j] = '\0';
			size++;
		}
		else
		{
			//queue is full. Dequeue first and then push again
			for (j = 0; line[j] != '\0' && line[j] != '\n'; j++)
			{
				cmds[front][j] = line[j];
			}
			cmds[front][j] = '\0';
			rear = (rear + 1) % JOB_SIZE;
			front = (front + 1) % JOB_SIZE;
			//printf("%d %d",front,rear);
		}

		// Freeing the allocated memory
		for (i = 0; tokens[i] != NULL; i++)
		{
			free(tokens[i]);
		}
		free(tokens);
	}
	return 0;
}
void pushToBG(char line[])
{
	// Store the process in list of background processes
	int j;
	//push to queue
	if (rjobs == -1 && rjobs == fjobs)
	{
		//queue is empty
		++rjobs;
		++fjobs;
		//printf("%d %d",front,rear);
		for (j = 0; line[j] != '\n'; j++)
		{
			bgjobs[rjobs][j] = line[j];
		}
		bgjobs[rjobs][j] = '\0';
		prev = rjobs;
		sjobs++;
	}
	else if (size < JOB_SIZE)
	{
		//queue is not full
		++rjobs;
		//printf("%d %d",front,rear);
		for (j = 0; line[j] != '\n'; j++)
		{
			bgjobs[rjobs][j] = line[j];
		}
		bgjobs[rjobs][j] = '\0';
		prev = rjobs;
		sjobs++;
	}
	else
	{
		//queue is full. Dequeue first and then push again
		for (j = 0; line[j] != '\0' && line[j] != '\n'; j++)
		{
			bgjobs[fjobs][j] = line[j];
		}
		bgjobs[fjobs][j] = '\0';
		prev = fjobs;
		rjobs = (rjobs + 1) % JOB_SIZE;
		fjobs = (fjobs + 1) % JOB_SIZE;
		//printf("%d %d",front,rear);
	}
}
void numRun(char *cmd, char line[])
{

	// Run the Command of type !<no>
	char **tokens;
	int i, j;
	if (cmd[0] == '!')
	{
		char num[SIZE];
		for (j = 1; cmd[j] != '\0'; j++)
		{
			num[j - 1] = cmd[j];
		}
		int n = atoi(num);
		if (size == JOB_SIZE)
		{
			int f = 0;
			for (j = front; j < JOB_SIZE; j++)
			{
				if (j - front + 1 == n)
				{
					n = j;
					f = 1;
				};
			}
			if (0 != front && f == 0)
			{
				for (j = 0; j <= rear; j++)
				{
					if (n == JOB_SIZE + j - rear)
						;
					{
						n = j;
					}
				}
			}
			// n++;
		}
		else
			n--;
		char newcmd[MAX_INPUT_SIZE];
		bzero(newcmd, MAX_INPUT_SIZE);
		for (j = 0; j < strlen(cmds[n]); j++)
		{
			newcmd[j] = cmds[n][j];
		}
		newcmd[strlen(newcmd)] = '\n';
		tokens = parseCommand(newcmd);

		// for(i=0;tokens[i]!=NULL;i++){
		// 		printf("%s\n",tokens[i]);
		// 	}
		if (isInBuilt(tokens[0]) == 1)
		{
			runInbuilt(tokens, tokens[0]);
		}
		else
		{
			if (tokens[0][0] == '!')
				numRun(tokens[0], line);
			else
			{
				if (tokens[argCount(tokens) - 1][0] == '&')
					pushToBG(line);
				runCommand(tokens);
			}
		}
	}
	else
		runCommand(tokens);
}
void runInbuilt(char **tokens, char *cmd)
{
	// Run the command if it is Inbuilt
	int i, j;
	if (strcmp(cmd, "cd") == 0)
	{ // Change Directory cd
		if (argCount(tokens) != 2)
		{								  // Check no.of arguments
			perror("Usage: cd <path>\n"); //raise error if no.of args is incorrect
		}
		else
		{
			if (chdir(tokens[1]) < 0)
			{ // Handle Errors
				char *errorMessage;
				if (errno == ENOENT)
					errorMessage = "No such file or directory";
				else if (errno == ENAMETOOLONG)
					errorMessage = "Path too long";
				else if (errno == EACCES)
					errorMessage = "Permission denied";
				else if (errno == ENOTDIR)
					errorMessage = "Not a directory";
				else if (errno == ELOOP)
					errorMessage = "Loop in symbolic link";

				printf("cd: %s", tokens[1]);
				printf("\n%s\n", errorMessage);
			}
		}
	}
	else if (strcmp(cmd, "pwd") == 0)
	{
		if (argCount(tokens) == 1)
		{
			printDir();
		}
		else
		{
			perror("Usage: pwd\n");
		}
	}
	else if (strcmp(cmd, "jobs") == 0)
	{
		int count = 0;
		if (sjobs < JOB_SIZE)
		{
			for (j = 0; j < sjobs; j++)
			{
				// printf("101L: %c\n",bgjobs[j][0]);
				if (bgjobs[j][0] != '\0')
				{
					printf("%d ", count + 1);
					for (i = 0; bgjobs[j][i]; i++)
					{
						printf("%c", bgjobs[j][i]);
					}
					printf(" pid : %u", bgpid[j]);
					printf("\n");
					count++;
				}
			}
		}
		else
		{
			for (j = fjobs; j < JOB_SIZE; j++)
			{
				if (bgjobs[j][0])
				{
					printf("%d ", count + 1);
					for (i = 0; bgjobs[j][i] != '\0'; i++)
					{
						printf("%c", bgjobs[j][i]);
					}
					printf(" pid : %u", bgpid[j]);
					printf("\n");
					count++;
				}
			}
			if (0 != front)
			{
				for (j = 0; j <= rjobs; j++)
				{
					if (bgjobs[j][0])
					{
						printf("%d ", count + 1);
						for (i = 0; bgjobs[j][i] != '\0'; i++)
						{
							printf("%c", bgjobs[j][i]);
						}
						printf(" pid : %u", bgpid[j]);
						printf("\n");
						count++;
					}
				}
			}
		}
	}
	else if (strcmp(cmd, "kill") == 0)
	{
		if (argCount(tokens) == 2)
		{
			// printf("%s\n",tokens[1]);
			pid_t tokill = (atoi)(tokens[1]);
			// printf("%u\n", tokill);
			kill(tokill, SIGKILL);
			printf("Killed the process with pid %u\n", tokill);
			int x;
			for (x = 0; x < sjobs; x++)
			{
				if (tokill == bgpid[x])
				{
					bgpid[x] = 0;
					for (i = 0; bgjobs[x][i] != '\0'; i++)
						bgjobs[x][i] = '\0';
				}
			}
		}
		else
		{
			perror("Usage: kill <pid_no>\n");
		}
	}
	else if (strcmp(cmd, "exit") == 0)
	{
		terminate(SIGINT); // Terminate processes

		exit(0);
	}
	else if (strcmp(cmd, "history") == 0)
	{
		if (size < JOB_SIZE)
		{
			for (j = 0; j < size; j++)
			{
				printf("%d ", j + 1);
				for (i = 0; cmds[j][i]; i++)
					printf("%c", cmds[j][i]);
				printf("\n");
			}
		}
		else
		{
			for (j = front; j < JOB_SIZE; j++)
			{
				printf("%d ", j - front + 1);
				for (i = 0; cmds[j][i] != '\0'; i++)
					printf("%c", cmds[j][i]);
				printf("\n");
			}
			if (0 != front)
			{

				for (j = 0; j <= rear; j++)
				{
					printf("%d ", JOB_SIZE + j - rear);
					for (i = 0; cmds[j][i] != '\0'; i++)
						printf("%c", cmds[j][i]);
					printf("\n");
				}
			}
		}
	}
	else if (strcmp(cmd, "about") == 0)
	{

		int i, j;
		char x;
		for (i = 0; i < 10; i++)
		{
			x = (i == 0 || i == 9) ? '+' : ' ';
			for (j = 0; j < 10; j++)
				printf("+");
			for (j = 0; j < 15; j++)
				printf("%c", x);

			if (i == 4)
			{
				printf("This is a Simple Shell Written in C!");
				for (j = 0; j < 4; j++)
					printf("%c", x);
			}
			else if (i == 5)
			{
				printf("By: @TVSSSRIPAD");
				for (j = 15; j < 40; j++)
					printf("%c", x);
			}
			else if (i == 8)
			{
				printf("Run help command to know about inbuilt commands");
				// for (j = 15; j < 40; j++)
				// 	printf("%c", x);
			}
			else
			{
				for (j = 0; j < 40; j++)
					printf("%c", x);
			}
			if (i != 8)
			{
				for (j = 0; j < 20; j++)
					printf("%c", x);
			}
			else
			{
				for (j = 0; j < 13; j++)
					printf("%c", x);
			}
			for (j = 0; j < 10; j++)
				printf("+");
			printf("\n");
		}
	}
	else if (strcmp(cmd, "help") == 0)
	{
		int i, j;
		char x;
		for (i = 0; i < 14; i++)
		{
			x = (i == 0 || i == 13) ? '+' : ' ';
			for (j = 0; j < 10; j++)
				printf("+");
			for (j = 0; j < 15; j++)
				printf("%c", x);
			if (i == 2)
			{
				printf("The following inbuilt commands are supported:");
			}
			else if (i == 4)
			{
				printf("pwd : prints current working directory");
				for (j = 0; j < 4; j++)
					printf("%c", x);
			}
			else if (i == 5)
			{
				printf("cd : for changing directory ");
				for (j = 0; j < 12; j++)
					printf("%c", x);
			}
			else if (i == 6)
			{
				printf("about : shows basic details about this shell ");
			}
			else if (i == 7)
			{
				printf("help : shows this help text ");
				for (j = 0; j < 12; j++)
					printf("%c", x);
			}
			else if (i == 8)
			{
				printf("history : gives list of recently ran commands");
				// for (j = 15; j < 40; j++)
				printf("  ");
			}
			else if (i == 9)
			{
				printf("jobs : gives list of processes running in background");
				// for (j = 15; j < 40; j++)
				// 	printf("%c", x);
			}
			else if (i == 10)
			{
				printf("kill <pid> : !kills a process with pid = <pid>");
				// for (j = 15; j < 40; j++)
				// 	printf("%c", x);
			}
			else if (i == 11)
			{
				printf("exits : exits the shell cleanly");
				for (j = 0; j < 9; j++)
					printf("%c", x);
			}
			else
			{
				for (j = 0; j < 40; j++)
					printf("%c", x);
			}
			if (i == 2)
			{
				for (j = 0; j < 15; j++)
					printf("%c", x);
			}
			else if (i == 4)
			{
				for (j = 0; j < 18; j++)
					printf("%c", x);
			}
			else if (i == 6)
			{
				for (j = 0; j < 15; j++)
					printf("%c", x);
			}
			else if (i == 8)
			{
				for (j = 0; j < 13; j++)
					printf("%c", x);
			}
			else if (i == 9)
			{
				for (j = 0; j < 8; j++)
					printf("%c", x);
			}
			else if (i == 10)
			{
				for (j = 0; j < 14; j++)
					printf("%c", x);
			}
			else
			{
				for (j = 0; j < 20; j++)
					printf("%c", x);
			}
			for (j = 0; j < 10; j++)
				printf("+");
			printf("\n");
		}
	}
}

void terminate(int signum)
{
	// Terminate all foreground processes
	if (kill(0, signum) < 0)
	{ // Kill all processes with PGID == PGID of caller (shell)
		perror("Error ");
	}
	while (1)
	{
		if (waitpid(0, NULL, 0) <= 0)
			if (errno == ECHILD)
				break;
	}
}
