#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#include "variables.h"
#include "handlearguments.h"
#include "errorcodes.h"
#include "version.h"
#include "options.h"

int handle_cli_singular_argument(char *input_option, char *rest_of_input[]) {
    int handler_result;
    const OptionContext *options = getOptions();
    size_t count = getOptionsCount();
    // Rest of input is a pointer to the next argument after the option
    for (size_t i = 0; i < count; i++) {
        const OptionContext current_option = options[i];
        if (current_option.handler == NULL) {
            // Skip options without a handler
            continue;
        }
        const char *input_for_option[current_option.expects_argument + 1];
        for (size_t j = 0; j < current_option.expects_argument; j++) {
            // Get the next argument if it exists
            if (i + j + 1 < count) {
                input_for_option[j] = rest_of_input[j];
            } else {
                input_for_option[j] = NULL;  // No more arguments available
            }
        }

        if (strcmp(input_option, current_option.short_opt) == 0 || 
            strcmp(input_option, current_option.long_opt) == 0) {
            if (current_option.expects_argument > 0 && 
               (*input_for_option == NULL || input_for_option[0] == NULL)) {

                fprintf(stderr, "Error: %s requires a value.\n", current_option.long_opt);
                return ERR_NO_INPUT;
            }
            handler_result = current_option.handler(input_for_option);
            if (handler_result < 0) {
                return handler_result;  // Return error code from handler
            }
            return current_option.expects_argument;
        }
    }
    fprintf(stderr, "Error: No valid option provided was given '%s'. Use -h or --help for options.\n", input_option);
    return ERR_INVALID_INPUT;
}

int handle_cli_arguments(int count, char *arguments[]) {
    char **rest_of_input;
    if (count < 1) {
        // can be changed later if we want different behavior
        fprintf(stderr, "Error: No option provided. Use -h or --help for options.\n");
        return ERR_NO_INPUT;
    }

    for (int i = 0; i < count; i++) {
        char *argument;
        int result;
        argument = arguments[i];
        // Check if the argument is NULL or empty (for perfomance) and we know we have at least one argument
        if (argument == NULL || strlen(argument) == 0) {
            return ERR_INVALID_INPUT;
        }
        rest_of_input = &arguments[i + 1]; // Pointer to the rest of the input after the current argument

        result = handle_cli_singular_argument(argument, rest_of_input);

        if (result < 0) {
            return result;
        }
        i += result; // Skip the number of values consumed by the argument
    }
    return 0;
}