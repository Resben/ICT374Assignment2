/* file:	placeholder.c
 * purpose:	replaces defined placeholders
 * author:	BH
 * date:	16-Oct-22
 */
#include <stdio.h>
#include "../include/placeholder.h"

int find(const char* input, const char* replaced_word);
char* replace(const char* input, const char* replaced_word, const char* replace_with);

char* replace_placeholders(const char* input)
{
    int i = 0;
    char* placeholders[] = {pwd, NULL};
    char* newInput;
    char path[100];

    while (placeholders[i] != NULL) {
        if(placeholders[i] == pwd) {
            newInput = replace(input, placeholders[i], getcwd(path, 100));
        }
        //Add more if statements for more placeholders
        i++;
    }

    return newInput;
}

char* replace(const char* input, const char* replaced_word, const char* replace_with)
{
    int start = find(input, replaced_word);

        // Return the normal input string if not found
    if(start < 0) {
        char* nullString = (char*)malloc(strlen(input) * sizeof(char));
        nullString[0] = '\0';
        strcat(nullString, input);
        return nullString;
    }

        // Get the end index of the placeholder
    int end = start + strlen(replaced_word);
        // Size of the new string + malloc new string
    int size = strlen(input) - strlen(replaced_word) + strlen(replace_with) + 1;
    char* newString = (char*)malloc(size * sizeof(char));

    newString[0] = '\0';

    strncat(newString, input, start); // Get the original string up to the placeholder
    strcat(newString, replace_with); // Append the new word to the end
    strncat(newString, &input[end], strlen(&input[end])); // Get the end + end size of the string and add it to new string

    return newString;
}

    // Loop through until the placeholder is found or
    // end of inputted string is reached
    // returns the starting index of the placeholder
int find(const char* input, const char* replaced_word)
{
    int sizeInput = strlen(input);
    int sizeReplace = strlen(replaced_word);

    int inc = 0, start = 0, flag = 0;

    for(int i = 0; i < sizeInput; i++) {
        if(input[i] == replaced_word[inc]) {
            if(flag == 0) {
                start = i;
                flag = 1;
            }
            inc++;
            if(inc == sizeReplace) {
                return start;
            }
        }
        else {
            flag = 0;
            inc = 0;
        }
    }
    return -1;
}
