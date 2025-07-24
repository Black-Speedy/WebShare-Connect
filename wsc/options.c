#include <stdio.h>
#include <string.h>
#include "options.h"
#include <stdlib.h>
#include "version.h"
#include <stdint.h>

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


static const OptionContext option_contexts[] = {
    {"-h", "--help", "Show this help message", 0, help_handler},
    {"-v", "--version", "Some Version", 0, version_handler}
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

/**
 * Format the options into a single string.
 * This function concatenates the options array into a single string
 */
char *format_options(OptionContext option) {
    const char *short_op = option.short_opt;
    const char *long_op = option.long_opt;
    const char *description_op = option.description;
    int expects_argument = option.expects_argument;
    int (*handler)(const char *) = option.handler;
    
    const int tab_size = 3; // Number of spaces for alignment
    const int formatting_between_options_length = 3;
    const int description_space_length = 17; // Length of the description field
    
    const char *tab = "   ";
    const char *formatting_between_options = " | ";
    const char *description_space = "                 ";
    
    size_t prefix_len = strlen(short_op) +
                        strlen(long_op) +
                        strlen(description_op) +
                        strlen(tab) +
                        strlen(formatting_between_options) +
                        description_space_length +
                        1; // null terminator

    char *prefix = malloc(prefix_len);
    
    sprintf(prefix, "%s%s%s%s%-*s%s",
    tab,
    short_op,
    formatting_between_options,
    long_op,
    description_space_length, "", // width = description_space_length, fill with ""
    description_op);
    
    return prefix;
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