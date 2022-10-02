/*
 * file:	pwd.h
 * aim:		print the filepath of the current directory as a string
 * author:	MA
 * date: 	2-Oct-22
 */

extern char **environ;

/* prints the filepath of the current directory to stdout if not null
 * does nothing and returns if environment varaible PWD is null
 */
void pwd();
