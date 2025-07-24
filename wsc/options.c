#include <stdio.h>
#include <string.h>
#include "options.h"
#include <stdlib.h>
#include "version.h"
#include <stdint.h>
#include "variables.h"
#include "errorcodes.h"

char *options[] = {
    "Usage: wsc [options]\n",
    "Options:\n",
    "   -h  | --help                 Show this help message\n",
    "   -ip | --ip-adress <address>  Set the IP address or domain name (default: 127.0.0.1)\n",
    "   -p  | --port <port>          Set the port number (default: 54832)\n",
    "   -v  | --version              Show version information\n",
    "   -V  | --verbose              Enable verbose output\n",
    "   -c  | --config <file>        Specify configuration file\n",
    "\n  Example: wsc --port 8080 --ip-adress 1.1.1.1\n"
};
int help_handler(const char *arg);
int version_handler(const char *arg);

static char ipDescription[100];
__attribute__((constructor))
static void init_ipDescription() {
    snprintf(ipDescription, sizeof(ipDescription),
             "Set the IP address or domain name (default: %s)", DEFAULT_IP);
}

static char portDescription[100];
__attribute__((constructor))
static void init_portDescription() {
    snprintf(portDescription, sizeof(portDescription),
             "Set the port number (default: %d)", DEFAULT_PORT);
}

const OptionContext option_contexts[] = {
    {"-h", "--help", "Show this help message", 0, help_handler},
    {"-ip", "--ip-address", ipDescription, 0, NULL},
    {"-p", "--port", portDescription, 1, NULL},
    {"-V", "--verbose", "Enable verbose output", 0, NULL},
    {"-c", "--config", "Specify configuration file", 1, NULL},
    {"-v", "--version", "Some Version", 0, version_handler},
    {"-V", "--verbose", "Enable verbose output", 0, NULL}
};

const OptionContext *getOptions() {
    return option_contexts;
}

const size_t getOptionsCount() {
    size_t big = sizeof(option_contexts);
    size_t small = sizeof(option_contexts[0]);
    size_t divided = big/small;
    return divided;
}


#define DESCRIPTION_COLUMN 30  // Column where description should start
/**
 * Format the options into a single string.
 * This function concatenates the options array into a single string
 */
char *format_options(OptionContext option) {
    const char *short_op = option.short_opt;
    const char *long_op = option.long_opt;
    const char *description_op = option.description;

    const char *tab = "   ";
    const char *between = " | ";

    size_t option_text_len = strlen(tab) + strlen(short_op) + strlen(between) + strlen(long_op);
    size_t padding_len = (DESCRIPTION_COLUMN > option_text_len)
                         ? DESCRIPTION_COLUMN - option_text_len
                         : 1;  // At least one space

    size_t total_len = option_text_len + padding_len + strlen(description_op) + 1;

    char *line = malloc(total_len);
    if (!line) return NULL;

    sprintf(line, "%s%s%s%s%*s%s",
        tab,
        short_op,
        between,
        long_op,
        (int)padding_len, " ",  // pad to align
        description_op
    );

    return line;
}

int help_handler(const char *arg) {
    for (int i = 0; i < getOptionsCount(); i++) {
        char *formatted = format_options(getOptions()[i]);
        if (formatted) {
            printf("%s\n", formatted);
            free(formatted); // free immediately after use
        }
    }
    return 0;
}


int version_handler(const char *arg) {
    printf("WebShare-Connect Version: %s\n", PROJECT_VERSION);
    return 0;
}

int port_handler(const char *arg) {
    if (arg == NULL) {
        fprintf(stderr, "Error: Port requires a value.\n");
        return ERR_PORT_MISSING_VALUE;
    }
    
    int port = atoi(arg);
    if (port <= 0 || port > 65535) {
        fprintf(stderr, "Error: Invalid port number '%s'. Must be between 1 and 65535.\n", arg);
        return ERR_INVALID_PORT;
    }
    
    // Set the port in the application context (not shown here)
    PORT = port; // Assuming DEFAULT_PORT is a global variable
    
    printf("Port set to %d\n", port);
    return 0;
}