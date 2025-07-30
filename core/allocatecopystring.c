#include "allocatecopystring.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
int allocate_and_copy_string(char **dest, const char *src) {
    if (src == NULL) {
        *dest = NULL;
        return 0; // No string to copy
    }

    size_t len   = strlen(src);
    char   *temp = malloc(len + 1); // +1 for null terminator
    if (temp == NULL) {
        return -1;                  // Memory allocation failed
    }

    snprintf(temp, len + 1, "%s", src);
    *dest = temp;
    return 0; // Success
}