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
#include <fcntl.h>
#include <sys/wait.h>
#include "include/command.h"
#include "include/token.h"
#include "include/prompt.h"
#include "include/cd.h"
#include "include/placeholder.h"
#include "include/wildcard.h"

#define BUFF_SIZE 256

void execute(int rd, char *file, char *executablePath, char *executable, pid_t pid); 

int main(void)
{
	char input[BUFF_SIZE];             // unprocessed input from stdin
	char *token[MAX_NUM_TOKENS];	   // holds tokenised form of input string
	Command command[MAX_NUM_COMMANDS]; // tokens have been converted into command objects
	int total_cmds;
	char *prompt = "%"; // default prompt
	char *prompt_out; // output prompt
	char *wildcard_array[MAX_WILDCARDS];
	pid_t pid;

	while (1) {
		prompt_out = replace_placeholders(prompt);
		printf("%s ", prompt_out);
		fgets(input, BUFF_SIZE, stdin);
		
		tokenise(input, token);
		total_cmds = separateCommands(token, command); // separates cmd_token by commands and fills								      // command with each separate command
		for (int i = 0; i < total_cmds; ++i) { // run through each command
			for (int j = command[i].first; j < command[i].last; ++j) { // run through each token of a command
				if (command[i].last > 2 && strcmp(token[j+1], command->stdout_file) == 0) {
					execute(1, token[j+1], token[j-1], token[j-1], pid);
					j += 2;
				} else if (command[i].last > 2 && strcmp(token[j+1], command->stdout_file) == 0) {
					execute(2, token[j+1], token[j-1], token[j-1], pid);
					j += 2;
				} else if (strcmp(token[j], "prompt") == 0) {
					update_prompt(&prompt, token[j+1]);
					++j;
				} else if (strcmp(token[j], "cd") == 0) {
					cd(token[j+1]);
					++j;
				} else if (strcmp(token[j], "pwd") == 0) {
					execute(0, NULL, "./src/pwd", "pwd", pid);
				} else if (strcmp(token[j], "exit") == 0) {
					execute(0, NULL, "./src/exit", "exit", pid);
        			} else {
					execute(0, NULL, token[j], token[j], pid);
				}
			} // end for commands
		} // end for tokens
	} // end while
  
	exit(0);
}

void execute(int rd, char *file, char *executablePath, char *executable, pid_t pid) 
{
	int ofd; // stdout redirection
	int ifd; // stdin redirection

	if (rd == 1) { // 1 for stdout redirect 2 for stdin redirect 0 for no redirection
		ofd = open(file, O_CREAT|O_WRONLY|O_TRUNC, 0666);
		if (!ofd) { 
			perror("open");
			exit(1);
		}
	} else if (rd == 2) { // stdin redirection
		ifd = open(file, O_RDONLY);
		if (!ifd) {
			perror("open");
			exit(1);
		}
	}
			
	if ((pid = fork()) < 0) {
		perror("fork\n");
		exit(1);
	}

	pid_t pPid = getppid(); // parent pid
	pid_t cldPid = getpid();// child pid
	
	if (pid == 0) { // child
		if (rd == 1) {
			dup2(ofd, 1); // replaces stdout with ofd
			close(ofd);
		} else if (rd == 2) {
			dup2(ifd, 0); // replaces stdin with ifd
			close(ifd);
		}

		if (execlp(executablePath, executable, NULL) < 0) { // execute commnd
			printf("Command '%s' failed\n", executable); // report execlp failure
			kill(cldPid, SIGKILL); // child process isn't left hanging after execlp failure
		} 
	} else { // parent
			if (rd == 1) {
				close(ofd);
			} else if (rd == 2) {
				close(ifd);
			}

			wait((int*)0); // wait for child process to finish
			return;
	}
}
