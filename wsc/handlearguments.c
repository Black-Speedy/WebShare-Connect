#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#include "variables.h"
#include "handlearguments.h"
#include "errorcodes.h"
#include "argumentvalueamount.h"
#include "version.h"
#include "options.h"

int handle_cli_argument_smart(char *input_option, char *value[], const OptionContext options[]) {
    for (int i = 0; i < getOptionsCount(); i++) {
        const OptionContext current_option = options[i];
        if (current_option.handler == NULL) {
            // Skip options without a handler
            continue;
        }
        
        if (strcmp(input_option, current_option.short_opt) == 0 || strcmp(input_option, current_option.long_opt) == 0) {
            if (current_option.expects_argument > 0) {
                if (value == NULL || *value == NULL) {
                    fprintf(stderr, "Error: %s requires a value.\n", current_option.long_opt);
                    return ERR_NO_INPUT;
                }
            }
            return current_option.handler(NULL);
        }
    }
    
    printf("Error: No valid option provided was given '%s'. Use -h or --help for options.\n", input_option);
    return ERR_INVALID_INPUT;
}

int handle_cli_arguments(int count, char *arguments[]) {
    if (count < 1) {
        // can be changed later if we want different behavior
        fprintf(stderr, "Error: No option provided. Use -h or --help for options.\n");
        return ERR_NO_INPUT;
    }

    const OptionContext *options = getOptions();
    for (int i = 0; i < count+1; i++) {
        char *arg = arguments[i];
        // Check if the argument is NULL or empty (for perfomance)
        if (arg == NULL || strlen(arg) == 0) {
            return 0;
        }
        char **value = arguments + i + 1; // Get the next argument as the value
        // printf("Processing argument: %s\n", arg);
        // printf("Value: %s\n", *value ? *value : "NULL");
        int result = handle_cli_argument_smart(arg, value, options);
        if (result < 0) {
            return result;
        }
        i += result; // Skip the number of values consumed by the argument
    }
    return 0;
}