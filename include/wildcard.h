/*
 * file:	wildcard.h
 * aim:		return a list of filenames that match the wildcard
 * author:	BH
 * date: 	16-Oct-22
 */

// Todo: proper testing

#define MAX_WILDCARDS 100

#include <glob.h>

    // Return a list of filenames
void wildcard(char *file_array[], char* wildcard);
