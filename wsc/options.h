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
    const char *usage;
    const char *description;
    int expects_argument;             // 0 or 1
    int (*handler)(const char *arg);  // NULL if no handler
} OptionContext;

extern const OptionContext option_contexts[];

int help_handler(const char *arg);

int version_handler(const char *arg);

int port_handler(const char *arg);

static void init_ipDescription(void);

static void init_portDescription(void);


int format_desctription_call_handler(OptionContext option);

const OptionContext *getOptions(void);
size_t getOptionsCount(void);

char *format_options(OptionContext option);

#endif // OPTIONS_H
