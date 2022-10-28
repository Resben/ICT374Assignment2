/* file:	placeholder.h
 * purpose:	replaces defined placeholders
 * author:	BH
 * date:	16-Oct-22
*/
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define pwd "${pwd}"        // The following string is replaced with the current path

char* replace_placeholders(const char* input);
