/*
 * file:        token.h
 * aim:         break an input line into a series of tokens
 * author:      MA
 * date:        2-Oct-22
 */

#define MAX_NUM_TOKENS 100
#define tokenSeparators " \t\n"

int tokenise (char *inputLine, char *token[]);
