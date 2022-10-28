/*
 * file:        main.c
 * aim:         get std input and tokenise
 * author:      MA
 * date:        2-Oct-22
 */

#include <signal.h>
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

void execute(Command* command); 

int main(void)
{
	char input[BUFF_SIZE];             // unprocessed input from stdin
	char *token[MAX_NUM_TOKENS];	   // holds tokenised form of input string
	Command command[MAX_NUM_COMMANDS]; // tokens have been converted into command objects
	int total_cmds;
	char shellCmd[BUFF_SIZE] = "/usr/bin/";
	char *prompt = "%"; // default prompt
	char *prompt_out; // output prompt
	char *wildcard_array[MAX_WILDCARDS];
	pid_t pid;

	signal(SIGTSTP, SIG_IGN); /*	Disable CTRL-Z */
	signal(SIGINT, SIG_IGN);	/* Disable CTRL-C */
	//signal(SIGQUIT, SIG_IGN); /* Disable CTRL-\ */

	while (1) {
		prompt_out = replace_placeholders(prompt);
		printf("%s ", prompt_out);
		fgets(input, BUFF_SIZE, stdin);
		
		tokenise(input, token);
		total_cmds = separateCommands(token, command); // separates cmd_token by commands and fills								      // command with each separate command
		for (int i = 0; i < total_cmds; ++i) { // run through each command

			if(strcmp(command[i].path, "pwd") == 0)
			{
				command[i].path = "./src/pwd";
				execute(&command[i]);
			}
			else if(strcmp(command[i].path, "exit") == 0)
			{
				command[i].path = "./src/exit";
				execute(&command[i]);
			}
			else if(strcmp(command[i].path, "cd") == 0)
			{
				cd(command->argv[0]);
			}
			else if(strcmp(command[i].path, "prompt") == 0)
			{
				update_prompt(&prompt, command->argv[0]);
			}
			else
			{
				strcat(shellCmd, command[i].path);
				command->path = shellCmd;
				execute(&command[i]);
			}
		}
	}
	exit(0);
}

void execute(Command* command) 
{
	int rd = 0;
	pid_t pid;
	int ofd; // stdout redirection
	int ifd; // stdin redirection

	if (command->stdout_file != NULL) { // 1 for stdout redirect 2 for stdin redirect 0 for no redirection
		rd = 1;
		ofd = open(command->stdout_file, O_CREAT|O_WRONLY|O_TRUNC, 0666);
		if (!ofd) { 
			perror("open");
			exit(1);
		}
	} else if (command->stdin_file != NULL) { // stdin redirection
		ifd = open(command->stdin_file, O_RDONLY);
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
		if(command->argv == NULL) {
			command->argv[0] = "";
		}
		if(command->argv == NULL) {
			printf("Did");
		}
		if (execv(command->path, command->argv) < 0) { // execute commnd
			printf("Command '%s' failed\n", command->path); // report execlp failure
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
