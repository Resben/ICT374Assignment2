/*
 * file:	pwd.c
 * aim:		directory walk functions
 * author:	BH
 * date: 	16-Oct-22
 */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/cd.h"

int cd(char *path)
{
	char s[100];

	char *newPath;

	if(path) {
		newPath = path;
	}
	else {
		newPath = getenv("HOME");
	}

	chdir(newPath);
	return setenv("PWD", getcwd(s, 100), 1);
}