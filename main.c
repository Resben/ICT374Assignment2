/*
 * file:        main.c
 * aim:         get std input and tokenise
 * author:      MA
 * date:        2-Oct-22
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/token.h"

#define BUFF_SIZE 256

int main(void)
{
	char *token[MAX_NUM_TOKENS];
	char input[BUFF_SIZE];

	while (1) {
		printf("$ ");
		fgets(input, BUFF_SIZE, stdin);
		if(strcmp(input, "exit\n") == 0) {
			break;	
		}

		int total = tokenise(input, token);

		for(int i = 0; token[i] != NULL; i++) { 
			printf("%d. %s\n",i, token[i]);
		}
	}
	
	exit(0);
}

