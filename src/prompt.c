/* file:	prompt.c
 * purpose:	enables customisable prompt
 * author:	MA
 * date:	02-Oct-22
 */

void update_prompt(char** oldPrompt, char* newPrompt)
{
	if(newPrompt) { // return if no newPrompt provided
		*oldPrompt = newPrompt;
	}
       return;	
}

