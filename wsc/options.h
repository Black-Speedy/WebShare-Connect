#ifndef OPTIONS_H
#define OPTIONS_H
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
typedef struct OptionContext {
    const char *short_opt;
    const char *long_opt;
    const char *description;
    int expects_argument;             // 0 or 1
    int (*handler)(const char *arg);  // NULL if no handler
} OptionContext;


int help_handler(const char *arg);

int format_desctription_call_handler(OptionContext option);

const OptionContext *getOptions();
const size_t getOptionsCount();

char *format_options();

#endif // OPTIONS_H