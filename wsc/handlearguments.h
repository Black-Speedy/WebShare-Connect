#include "options.h"
int handle_cli_argument(char *argument, char *value[]);
int handle_cli_argument_smart(char *input_option, char *value[], const OptionContext options[]);
int handle_cli_arguments(int count, char *arguments[]);