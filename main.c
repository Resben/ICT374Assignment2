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
#include "include/token.h"
#include "include/prompt.h"

#define BUFF_SIZE 256

void execute(char *executablePath, char *executable, pid_t pid);

int main(void)
{
	char *token[MAX_NUM_TOKENS]; // max is 1000
	char input[BUFF_SIZE]; 
	char *prompt = "% "; // default prompt
	pid_t pid;

	while (1) {
		printf("%s", prompt);
		fgets(input, BUFF_SIZE, stdin);
		if (strcmp(input, "exit\n") == 0) { // this needs to be modified to be a built-in command 
			break;	
		} 

		tokenise(input, token); // separate input into tokens

		for(int i = 0; token[i] != NULL; i++) {
			// check if prompt need to be updated
			if (strcmp(token[i], "prompt") == 0) {
				update_prompt(&prompt, token[i+1]);
			}
			if (strcmp(token[i], "pwd") == 0) {
				execute("./src/pwd", "pwd", pid);
			}
		}
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

