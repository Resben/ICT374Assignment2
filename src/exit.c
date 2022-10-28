/*
 * file:	exit.c
 * aim:		exit the shell when input as a command. Return 1 on error. 
 * author:	MA
 * date: 	15-Oct-22
 */

#include <stdlib.h>
#include <unistd.h>
#include <sys/signal.h>
#include "../include/exit.h"

int exit_shell()
{
	return(kill(getppid(), SIGKILL)); 
}

int main(void)
{
	exit_shell();
	exit(0);
}
