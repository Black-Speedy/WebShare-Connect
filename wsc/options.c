#include <stdio.h>
#include <string.h>
#include "options.h"
#include <stdlib.h>
#include "version.h"
#include <stdint.h>
#include "variables.h"
#include "errorcodes.h"

int help_handler(const char *arg[]);
int version_handler(const char *arg[]);
int address_handler(const char *args[]);

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

/**
 * @brief This function formats the description of an option into a string.
 */
const OptionContext option_contexts[] = {
    {"-h", "--help",    NULL,        "Show this help message",     0, help_handler   },
    {"-v", "--version", NULL,        "Some Version",               0, version_handler},
    {"-V", "--verbose", NULL,        "Enable verbose output",      0, NULL           },
    {"-a", "--address", "<address>", ipDescription,                1, address_handler},
    {"-p", "--port",    "<port>",    portDescription,              1, port_handler   },
    {"-c", "--config",  "<file>",    "Specify configuration file", 1, NULL           }
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
    printf("WebShare-Connect Version: %s\n", PROJECT_VERSION);
    return 0;
}

int port_handler(const char *args[]) {
    const char *arg = args[0];
    if (arg == NULL || arg[0] == '\0') {
        fprintf(stderr, "Error: Port requires a value.\n");
        return ERR_PORT_MISSING_VALUE;
    }

    printf("Setting port to: %s\n", arg);

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
    return 0; //Number of arguments consumed
}

int address_handler(const char *args[]) {
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

    #ifdef _WIN32
        strncpy_s(IP_ADDRESS, strlen(ip_address) + 1, ip_address, _TRUNCATE);
    #else
        strncpy(IP_ADDRESS, ip_address, strlen(ip_address) + 1);
    #endif
    IP_ADDRESS[strlen(ip_address)] = '\0'; // Ensure null termination
    if (IP_ADDRESS == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for IP address.\n");
        return ERR_MEMORY_ALLOCATION;
    }

    printf("IP address set to %s\n", IP_ADDRESS);
    return 0; //Number of arguments consumed
}