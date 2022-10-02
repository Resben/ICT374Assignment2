/* file:	prompt.c
 * purpose:	enables customisable prompt
 * author:	MA
 * date:	02-Oct-22
 */

#include <string.h>
#include <stdio.h>//to delete

void update_prompt(char** oldPrompt, char* newPrompt)
{
	if(newPrompt) { // return if no newPrompt provided
		strcat(newPrompt, " "); // add a space after the prompt
		*oldPrompt = newPrompt;
	} 
}

