/*
 * file:        main.c
 * aim:         get std input and tokenise
 * author:      MA
 * date:        2-Oct-22
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "include/command.h"
#include "include/token.h"
#include "include/prompt.h"

#define BUFF_SIZE 256

void execute(char *executablePath, char *executable, pid_t pid);

int main(void)
{
	char input[BUFF_SIZE];             // unprocessed input from stdin
	char *token[MAX_NUM_TOKENS];	   // holds tokenised form of input string
	Command command[MAX_NUM_COMMANDS]; // tokens have been converted into command objects
	int total_cmds;
	char *prompt = "%"; // default prompt
	pid_t pid;

	while (1) {
		printf("%s ", prompt);
		fgets(input, BUFF_SIZE, stdin);
		if (strcmp(input, "exit\n") == 0) { // this needs to be modified to be a built-in command 
			break;	
		}

		tokenise(input, token);

		total_cmds = separateCommands(token, command); // separates cmd_token by commands and fills									  // command with each separate command
		for (int i = 0; i < total_cmds; ++i) { // run through each command
			for (int j = command[i].first; j < command[i].last; ++j) { // run through each token of a command
				if (strcmp(token[j], "prompt") == 0) {
					update_prompt(&prompt, token[j+1]);
				}
				if (strcmp(token[j], "pwd") == 0) {
					execute("./src/pwd", "pwd", pid);
				}
			} // end for
		} // end for
	}
	
	exit(0);
}

void execute(char *executablePath, char *executable, pid_t pid)
{
	if ((pid = fork()) < 0) {
		perror("fork\n");
		exit(1);
	}

	if (pid == 0) { // child
		if (execlp(executablePath, executable, NULL) < 0) { // execute commnd
			printf("execlp error\n"); // report error if command fails to execute
		}
		return;
	} else {
		wait((int*)0); // wait for child process to finish
		return;
	}
}

