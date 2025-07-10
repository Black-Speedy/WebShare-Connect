#include <stdio.h>
#include <string.h>
// char *options[] = {
//     "Usage: wsc [options]\n",
//     "Options:\n",
//     "\t--help | -h        Show this help message\n",
//     "\t--ip-adress | -i <address>        Set the IP address (default:"
//     "\t--port | -p <port>        Set the port number (default: 54832)\n",
//     "\t--version | -v        Show version information\n",
//     "\t--verbose | -V        Enable verbose output\n",
//     "\t--config | -c <file>        Specify configuration file\n"
// };


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

/**
 * Format the options into a single string.
 * This function concatenates the options array into a single string
 */
char *format_options() {
    size_t total_length = 0;
    size_t count = sizeof(options) / sizeof(options[0]);

    for (size_t i = 0; i < count; ++i) {
        total_length += strlen(options[i]);
    }

    char *result = malloc(total_length + 1);
    if (!result) return NULL;

    result[0] = '\0';

    for (size_t i = 0; i < count; ++i) {
        #ifdef _WIN32
        if (strcat_s(result, total_length + 1, options[i]) != 0) {
            free(result);
            return NULL;
        }
        #else
        // Linux/macOS: use strncat with length check
        if (strlen(result) + strlen(options[i]) >= total_length + 1) {
            free(result);
            return NULL; // overflow would occur
        }
        strncat(result, options[i], total_length - strlen(result));
        #endif
    }

    return result;
}