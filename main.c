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
#include "include/prompt.h"

#define BUFF_SIZE 256

int main(void)
{
	char *token[MAX_NUM_TOKENS]; // max is 1000
	char input[BUFF_SIZE]; 
	char *prompt = "% "; // default prompt

	while (1) {
		printf("%s", prompt);
		fgets(input, BUFF_SIZE, stdin);
		if (strcmp(input, "exit\n") == 0) { // this needs to be modified to be a built-in command 
			break;	
		} 

		tokenise(input, token); // separate input into tokens

		for (int i = 0; token[i] != NULL; i++) { 
			// check if prompt need to be updated
			if (strcmp(token[i], "prompt") == 0) {
				update_prompt(&prompt, token[i+1]);
			}
		}
	}
	
	exit(0);
}

