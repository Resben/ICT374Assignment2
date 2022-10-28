/*
 * file:	wildcard.c
 * aim:		return a list of filenames that match the wildcard
 * author:	BH
 * date: 	16-Oct-22
 */

#include "../include/wildcard.h"

void wildcard(char *file_array[], char* wildcard)
{
	glob_t glob_item;

	glob(wildcard, GLOB_NOCHECK, 0, &glob_item);

	file_array = glob_item.gl_pathv;

	globfree(&glob_item);
}