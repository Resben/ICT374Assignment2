/*
 * file:	pwd.c
 * aim:		print the filepath of the current directory 
 * author:	MA
 * date: 	2-Oct-22
 */

#include <stdlib.h>
#include <stdio.h>
#include "../include/pwd.h"

void pwd()
{
	char *s;
	
	if ((s=getenv("PWD")) != NULL) {
		printf("%s\n", s);
	}
}

int main(void)
{
	pwd();
	return(0);
}
