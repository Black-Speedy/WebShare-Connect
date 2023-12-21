#include <stdlib.h>
#include <stdio.h>
#include "removeQuotes.h"


int containsQuotes(const char* str) {
    while (*str != '\0') {
        if (*str == '"' || *str == '\'') {
            return 1;
        }
        str++;
    }
    return 0;
}

char* removeQuotes(const char* str) {
    size_t len = strlen(str);
    char* newStr = (char*)malloc(len + 1); // Allocate memory for the modified string
    if (newStr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    const char* current = str; // Pointer for iteration
    char* newStrPtr = newStr; // Pointer to build the modified string

    while (*current != '\0') {
        if (*current != '"' && *current != '\'') {
            *newStrPtr = *current;
            newStrPtr++;
        }
        current++;
    }
    *newStrPtr = '\0'; // Null-terminate the modified string

    return newStr;
}