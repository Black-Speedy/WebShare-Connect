#include <stdio.h>
#include <string.h>
#include "options.h"
#include <stdlib.h>
#include "version.h"
#include <stdint.h>
#include "variables.h"
#include "errorcodes.h"
#include "allocatecopystring.h"

int help_handler(const char *arg[]);
int version_handler(const char *arg[]);
int address_handler(const char *args[]);
int filepath_handler(const char *args[]);

static char ipDescription[100];
// is a constructor
__attribute__((constructor))
static void init_ipDescription(void) {
    snprintf(ipDescription, sizeof(ipDescription),
             "Set the IP address or domain name (default: %s)", DEFAULT_IP);
}

static char portDescription[100];
// is a constructor
__attribute__((constructor))
static void init_portDescription() {
    snprintf(portDescription, sizeof(portDescription),
             "Set the port number (default: %d)", DEFAULT_PORT);
}

const OptionContext help = {
    .short_opt        = "-h",
    .long_opt         = "--help",
    .usage            = NULL,
    .description      = "Show this help message",
    .expects_argument =                        0,
    .handler          = help_handler
};
const OptionContext version = {
    .short_opt        = "-v",
    .long_opt         = "--version",
    .usage            = NULL,
    .description      = "Display the version of the application",
    .expects_argument =                                        0,
    .handler          = version_handler
};
const OptionContext port = {
    .short_opt        = "-p",
    .long_opt         = "--port",
    .usage            = "<port>",
    .description      = portDescription,
    .expects_argument =               1,
    .handler          = port_handler
};
const OptionContext address = {
    .short_opt        = "-a",
    .long_opt         = "--address",
    .usage            = "<address>",
    .description      = ipDescription,
    .expects_argument =             1,
    .handler          = address_handler
};
const OptionContext filepath = {
    .short_opt        = "-f",
    .long_opt         = "--file",
    .usage            = "<file>",
    .description      = "Specify input file path",
    .expects_argument =                         1,
    .handler          = filepath_handler
};
const OptionContext config = {
    .short_opt        = "-c",
    .long_opt         = "--config",
    .usage            = "<file>",
    .description      = "Specify configuration file",
    .expects_argument =                            1,
    .handler          = NULL
};
const OptionContext verbose = {
    .short_opt        = "-V",
    .long_opt         = "--verbose",
    .usage            = NULL,
    .description      = "Enable verbose output",
    .expects_argument =                       0,
    .handler          = NULL
};

/**
 * @brief This function formats the description of an option into a string.
 */
const OptionContext option_contexts[] = {
    help,
    version,
    verbose,
    address,
    port,
    filepath,
    config
};

const OptionContext *getOptions(void) {
    return option_contexts;
}

size_t getOptionsCount(void) {
    const size_t big     = sizeof(option_contexts);
    const size_t small   = sizeof(option_contexts[0]);
    const size_t divided = big / small;
    return divided;
}

#define BETWEEN_COLUMN        0  // Space between short and long option
#define DESCRIPTION_COLUMN    30 // Column where description should start
char *format_options(OptionContext option) {
    const char *short_op       = option.short_opt;
    const char *long_op        = option.long_opt;
    const char *description_op = option.description;
    const char *usage_op       = option.usage ? option.usage : "";


    size_t option_text_len = strlen(short_op) +
                             strlen(long_op) +
                             strlen(" ") +
                             strlen(usage_op);
    size_t length_upto_between = strlen(short_op);
    size_t between_padding     = (BETWEEN_COLUMN > length_upto_between)
                             ? BETWEEN_COLUMN - length_upto_between
                             : 1;  // At least one space
    size_t padding_len = (DESCRIPTION_COLUMN > option_text_len)
                         ? DESCRIPTION_COLUMN - option_text_len
                         : 1;  // At least one space

    int required_len = snprintf(NULL, 0, "   %s%*s| %s %s%*s%s",
                                short_op,
                                (int)between_padding, "",
                                long_op,
                                usage_op,
                                (int)padding_len, "", // pad to align
                                description_op
                                );
    char *line = malloc(required_len + 1);  // +1 for null terminator
    if (!line) return NULL;

    sprintf(line, "   %s%*s| %s %s%*s%s",
            short_op,
            (int)between_padding, "",
            long_op,
            usage_op,
            (int)padding_len, "", // pad to align
            description_op
            );

    return line;
}

int help_handler(const char *_[]) {
    for (size_t i = 0; i < getOptionsCount(); i++) {
        if (i == 0) {
            printf("Usage: wsc [options]\n");
            printf("Program Control Options:\n");
        } else if (strcmp(getOptions()[i].short_opt, "-a") == 0) {
            printf("\nExecution Configuration Options:\n");
        }

        if (getOptions()[i].handler == NULL) {
            goto Example;  // Skip options without handler
        }

        char *formatted = format_options(getOptions()[i]);
        if (formatted) {
            printf("%s\n", formatted);
            free(formatted);
        } else {
            fprintf(stderr, "Error: Memory allocation failed while formatting options.\n");
            return ERR_MEMORY_ALLOCATION;
        }

        // Print example usage for the -c option
Example:
        if (strcmp(getOptions()[i].short_opt, "-c") == 0) {
            printf("Example: wsc --port 8080 --ip-adress 1.1.1.1\n");
        }
    }

    return 0;
}

int version_handler(const char *_[]) {
    printf("WebShare-Connect Component Versions:\n  Overall Version: %s\n  CLI Version: %s\n  Core Version: %s\n",
           PROJECT_VERSION, CLI_PROJECT_VERSION, CORE_PROJECT_VERSION);
    return 0;
}

int port_handler(const char *args[]) {
    const char *arg = args[0];
    if (arg == NULL || arg[0] == '\0') {
        fprintf(stderr, "Error: Port requires a value.\n");
        return ERR_PORT_MISSING_VALUE;
    }

    int port = atoi(arg);
    if (port == 0) {
        fprintf(stderr, "Error: Port cannot be zero. Or you have inserted a non-integer value.\n");
        return ERR_INVALID_PORT;
    } else if (port < 1 || port > 65535) {
        fprintf(stderr, "Error: Invalid port number '%s'. Must be between 1 and 65535.\n", arg);
        return ERR_INVALID_PORT;
    }

    // Set the port in the application context (not shown here)
    PORT = (uint16_t)port; // Assuming DEFAULT_PORT is a global variable

    printf("Port set to %d\n", port);
    return 0;
}

int address_handler(const char *args[]) {
    int        error;
    const char *ip_address = args[0];
    if (ip_address == NULL || ip_address[0] == '\0') {
        fprintf(stderr, "Error: IP address requires a value.\n");
        return ERR_IP_MISSING_VALUE;
    }

    if (strcmp(ip_address, IP_ADDRESS) == 0) {
        printf("IP address is already set to %s\n", IP_ADDRESS);
        return 0; // No change needed
    }

    if (strlen(ip_address) > sizeof(IP_ADDRESS)) {
        // reallocate memory for IP_ADDRESS
        char *temp = (char *)realloc(IP_ADDRESS, strlen(ip_address) + 1);
        if (temp == NULL) {
            fprintf(stderr, "Error: Memory allocation failed for IP address.\n");
            return ERR_MEMORY_ALLOCATION;
        }

        IP_ADDRESS = temp;
    }

    IP_ADDRESS = malloc(strlen(ip_address) + 1); // Allocate memory for IP address
    if (IP_ADDRESS == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for IP address.\n");
        return ERR_MEMORY_ALLOCATION;
    }

    error = snprintf(IP_ADDRESS, strlen(ip_address) + 1, "%s", ip_address);
    if (error < 0) {
        free(IP_ADDRESS);  // Free allocated memory on error
        IP_ADDRESS = NULL; // Set to NULL to avoid dangling pointer
        fprintf(stderr, "Error: Failed to copy IP address.\n");
        return error;
    }

    IP_ADDRESS[strlen(ip_address)] = '\0'; // Ensure null termination
    if (IP_ADDRESS == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for IP address.\n");
        return ERR_MEMORY_ALLOCATION;
    }

    printf("IP address set to %s\n", IP_ADDRESS);
    return 0;
}

int filepath_handler(const char *args[]) {
    int        error;
    size_t     file_path_length;
    const char *file_path = args[0];
    if (file_path == NULL || file_path[0] == '\0') {
        fprintf(stderr, "Error: File path requires a value.\n");
        return ERR_FILE_MISSING_VALUE;
    }

    // Here you would typically set the file path in your application context
    // For demonstration, we just print it
    printf("File path set to: %s\n", file_path);

    // error = allocate_and_copy_string(&FILE_PATH, file_path);
    // if (error < 0) {
    //     fprintf(stderr, "Error: Failed to allocate and copy file path.\n");
    //     return error;
    // }

    file_path_length = strlen(file_path);

    FILE_PATH = malloc(file_path_length + 1); // Allocate memory for file path
    if (FILE_PATH == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for file path.\n");
        return ERR_MEMORY_ALLOCATION;
    }

    error = snprintf(FILE_PATH, file_path_length + 1, "%s", file_path); // Copy the file path into FILE_PATH
    if (error < 0) {
        free(FILE_PATH);                                                // Free allocated memory on error
        fprintf(stderr, "Error: Failed to copy file path.\n");
        return error;
    }

    FILE_PATH[strlen(file_path)] = '\0'; // Ensure null termination
    return 0;
}