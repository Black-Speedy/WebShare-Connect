#include <stdlib.h>
#include <stdio.h>
#include "removeQuotes.h"

/**
 * @file removeQuotes.c
 * @brief Functions to handle double quotes in strings.
 */

/**
* @brief      Checks if a string contains double quotes.
* @param[in]  str   The string to check.
* @return     1 if the string contains double quotes, 0 otherwise.
*/
int containsQuotes(const char* str) {
    int singleQuoteAtBeginning = 0;
    if (*str == '\'') {
        singleQuoteAtBeginning = 1;
    }

    while (*str != '\0') {
        if (*str == '"') {
            return 1;
        }
        str++;
    }

    return singleQuoteAtBeginning;
}

/**
* @brief      Removes double quotes from a string.
* @param[in]  str   The string to remove double quotes from.
* @return     A pointer to the modified string.
*/
char* removeQuotes(const char* str) {
    size_t len = strlen(str);
    char* newStr = (char*)malloc(len + 1); // Allocate memory for the modified string
    if (newStr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    const char* current = str; // Pointer for iteration
    char* newStrPtr = newStr; // Pointer to build the modified string

    int singleQuoteAtBeginning = 0;
    if (*current == '\'') {
        const char* end = str + len - 1;
        if (*end == '\'') {
            singleQuoteAtBeginning = 1;
            current++;
            len -= 2; // Reduce the length to exclude the single quotes at the beginning and the end
        }
    }

    while (*current != '\0' && len > 0) {
        if (*current != '"') {
            *newStrPtr = *current;
            newStrPtr++;
        }
        current++;
        len--;
    }
    *newStrPtr = '\0'; // Null-terminate the modified string

    if (singleQuoteAtBeginning) {
        // If single quote was at the beginning, remove it from the end as well
        size_t newLen = strlen(newStr);
        if (newLen > 0 && newStr[newLen - 1] == '\'') {
            newStr[newLen - 1] = '\0';
        }
    }

    return newStr;
}
