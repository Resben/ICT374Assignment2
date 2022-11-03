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

void executePipe(Command* cmd1, Command* cmd2);
void execute(Command* command); 
void catch(int signo);

int main(void)
{
	char input[BUFF_SIZE];             // unprocessed input from stdin
	char *token[MAX_NUM_TOKENS];	   // holds tokenised form of input string
	Command command[MAX_NUM_COMMANDS]; // tokens have been converted into command objects
	int total_cmds;
	char *prompt = "%"; // default prompt
	char *prompt_out; // output prompt

	struct sigaction act;
	act.sa_handler = catch;
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGALRM, &act, NULL);
	sigaction(SIGCHLD, &act, NULL);

	signal(SIGTSTP, SIG_IGN); /* Disable CTRL-Z */
	signal(SIGINT, SIG_IGN);	/* Disable CTRL-C */
	//signal(SIGQUIT, SIG_IGN); /* Disable CTRL-\ */

	// get paths for executables pwd and exit
	char *path = getenv("PWD"); // get the executable path on startup
	char pwdPath[100], exitPath[100];
	strcpy(pwdPath, path);
	strcat(pwdPath, "/src/pwd");
	strcpy(exitPath, path);
	strcat(exitPath, "/src/exit");
	
	while (1) {
		prompt_out = replace_placeholders(prompt);
		printf("%s ", prompt_out);
		fgets(input, BUFF_SIZE, stdin);
	
		tokenise(input, token);
		total_cmds = separateCommands(token, command); // separates cmd_token by commands and fills
	       					               // command with each separate command
		for (int i = 0; i < total_cmds; ++i) { // run through each command
			if (strcmp(command[i].path, "pwd") == 0) {
				command[i].path = pwdPath;
				execute(&command[i]);
			} else if(strcmp(command[i].path, "exit") == 0) {	
				command[i].path = exitPath;
				execute(&command[i]);
			} else if(strcmp(command[i].path, "cd") == 0) {
				cd(command[i].argv[1]);
			} else if(strcmp(command[i].path, "prompt") == 0) {
				update_prompt(&prompt, command[i].argv[1]);
			} else if(strcmp(command[i].path, "ls") == 0) {
				glob_t globbuf;
				if (strchr(command[i].argv[command->argc-1], '*') != NULL || 
				    strchr(command[i].argv[command->argc-1], '?') != NULL) {
					if (glob(command[i].argv[command->argc-1], GLOB_DOOFFS, NULL, &globbuf) == GLOB_NOMATCH) {
						printf("No files matching %s\n", command[i].argv[command->argc-1]);
					} else {
						while(globbuf.gl_pathv[i]) {
							printf("%s\n", globbuf.gl_pathv[i]);
							++i;
						}
					}
				} else {
					execute(&command[i]);
				}
			} else if(strcmp(command[i].sep, pipeSep) == 0) {
				if(strcmp(command[i + 1].sep, pipeSep) != 0) {
					executePipe(&command[i], &command[i + 1]);
				} else {
					printf("Command '%s' failed\n", command[i].path); // report execlp failure
				}
				i++; // skip both commands
			} else {
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

	// redirections if-else block
	if (command->stdout_file != NULL) { // 1 for stdout redirect 2 for stdin redirect 0 for no redirection
		rd = 1;
		ofd = open(command->stdout_file, O_CREAT | O_WRONLY | O_TRUNC, 0666);
		if (!ofd) { 
			perror("open");
			exit(1);
		}
	} else if (command->stdin_file != NULL) { // stdin redirection
		rd = 2;
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
		// redirections if-else block
		if (rd == 1) {
			dup2(ofd, 1); // replaces stdout with ofd
			close(ofd);
		} else if (rd == 2) {
			dup2(ifd, 0); // replaces stdin with ifd
			close(ifd);
		}

		// execute command
		if (execvp(command->path, command->argv) < 0) { 
			printf("Command '%s' failed\n", command->path); // report execlp failure
			kill(cldPid, SIGKILL); // child process isn't left hanging after execlp failure
		}

	} else { // parent
		// redirections if-else block
		if (rd == 1) {
			close(ofd);
			command->stdout_file = NULL;
		} else if (rd == 2) {
			close(ifd);
			command->stdin_file = NULL;
		}

		// Wait for process (for ; or |)
		if(strcmp(command->sep, seqSep) == 0 || strcmp(command->sep, pipeSep) == 0) {
			wait((int*)0);
		}

		// Run in background (for &)
		if(strcmp(command->sep, conSep) == 0) {
			printf("\n[%d] Waiting\n\n", cldPid);
		}
		return;
	}
}

void executePipe(Command* cmd1, Command* cmd2)
{
	pid_t pid1, pid2;
	int fd[2];

	if (pipe(fd) == -1) { // open pipe & check success
		perror("pipe");
	}
	

	if ((pid1 = fork()) < 0) {
		perror("fork\n");
		exit(1);
	}

	if (pid1 == 0) {
		dup2(fd[1], 1); // duplicate stdin into fd[1]
		close(fd[0]);
		close(fd[1]);
		if (execvp(cmd1->path, cmd1->argv) < 0) {
			printf("Command %s failed\n", cmd1->path);
			kill(pid1, SIGKILL); // kill process if command fails so not left hanging
		}
	} 
	
	if ((pid2 = fork()) < 0) {
		perror("fork\n");
		exit(1);
	}

	if(pid2 ==  0) {
		dup2(fd[0], 0); // duplicate stdout into fd[0]
		close(fd[0]);
		close(fd[1]);
		if (execvp(cmd2->path, cmd2->argv) < 0) {
			printf("Command %s failed\n", cmd2->path);
			kill(pid2, SIGKILL); // kill process if command fails so not left hanging
		}
	}

	// final close is required
	close(fd[0]);
	close(fd[1]); 
	
	wait((int*)0); // wait for all child processes to finish before returning
	return;
}

void catch(int signo)
{
	pid_t pid;
	int status;

	if(signo == SIGCHLD) {
		wait((int*)0);
	}
}
