#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#include "variables.h"
#include "handlearguments.h"
#include "errorcodes.h"
#include "argumentvalueamount.h"
#include "version.h"
#include "options.h"

/** Takes an option and possible value from the command line arguments.
 *  Handles options like --help, -h, --port, -p.
 *  Returns either an error code (negative numbers), or a success with how many values to skip (as in how many values were consumed).
 */
// int handle_cli_argument(char *option, char *value[]) {
//     if (strcmp(option, "--help") == 0 || strcmp(option, "-h") == 0) {
//         printf("%s", format_options());
//         return 0;
//     } else if (strcmp(option, "--port") == 0 || strcmp(option, "-p") == 0) {
//         if (*value != NULL) {
//             if (strlen(*value) == 0) {
//                 fprintf(stderr, "Error: Port option requires a value.\n");
//                 return PortOptionRequiresValue;
//             }
//             int port_value = atoi(*value);
//             if (port_value <= 0 || port_value > 65535) {
//                 fprintf(stderr, "Error: Invalid port number. Must be between 1 and 65535.\n");
//                 return InvalidPortNumber;
//             }
//             printf("Setting PORT to %d\n", port_value);
//             PORT = port_value;
//             return PORT_VALUE_AMOUNT; 
//         } else {
//             fprintf(stderr, "Error: Port option requires a value.\n");
//             return PortOptionRequiresValue;
//         }
//     } else if (strcmp(option, "--version") == 0 || strcmp(option, "-v") == 0) {
//         printf("WebShare-Connect Version: %s\n", PROJECT_VERSION);
//         return 0;
//     }
    
//     printf("Error: No valid option provided was given '%s'. Use -h or --help for options.\n", option);
//     return NoInputProvided;
// }

int getOptionsAmount(char *input) {
    if (input == NULL || strlen(input) == 0) {
        return 0;
    }
    
    int count = 0;
    char *token = strtok(input, " ");
    while (token != NULL) {
        count++;
        token = strtok(NULL, " ");
    }
    
    return count;

}

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
                    return NoInputProvided;
                }
            }
            return current_option.handler(NULL);
        }
    }
    
    printf("Error: No valid option provided was given '%s'. Use -h or --help for options.\n", input_option);
    return InvalidInputProvided;
}

int handle_cli_arguments(int count, char *arguments[]) {
    if (count < 1) {
        // can be changed later if we want different behavior
        fprintf(stderr, "Error: No option provided. Use -h or --help for options.\n");
        return NoInputProvided;
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