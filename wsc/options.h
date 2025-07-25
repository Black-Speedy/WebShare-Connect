#ifndef OPTIONS_H
#define OPTIONS_H
#include <stdio.h>
#include <string.h>

typedef struct OptionContext {
    const char *short_opt;
    const char *long_opt;
    const char *usage;                // Usage string, can be NULL if no input is expected
    const char *description;
    size_t expects_argument;
    int (*handler)(const char *arg[]);  // NULL if no handler
} OptionContext;

extern const OptionContext option_contexts[];

int help_handler(const char *arg[]);

int version_handler(const char *arg[]);

int port_handler(const char *arg[]);

static void init_ipDescription(void);

static void init_portDescription(void);


int format_desctription_call_handler(OptionContext option);

const OptionContext *getOptions(void);
size_t getOptionsCount(void);

char *format_options(OptionContext option);

#endif
