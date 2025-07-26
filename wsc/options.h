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

/**
 * @brief This function handles the help command-line option.
 * 
 * This function handles the help command-line option.
 * It iterates through all defined options and prints their formatted descriptions.
 * If an option does not have a handler, it skips it.
 * 
 * @return Returns 0 on success, or an error code if an error occurs.
 */
int help_handler(const char *arg[]);

/**
 * @brief This function handles the version command-line option.
 * 
 * This function handles the version command-line option.
 * It prints the version of the WebShare-Connect application.
 * 
 * @return Returns 0 on success, or an error code if an error occurs.
 */
int version_handler(const char *arg[]);

/**
 * @brief This function handles the port command-line option.
 * 
 * This function handles the port command-line option.
 * It sets the PORT variable to the specified port number.
 * If the port number is invalid or missing, it returns an error code.
 * 
 * @param args The array of arguments passed to the handler.
 * @return Returns 0 on success, or an error code if an error occurs.
 */
int port_handler(const char *arg[]);

static void init_ipDescription(void);

static void init_portDescription(void);

/**
 * @brief Format the option text into a single string.
 * 
 * Format the option text into a single string.
 * This function concatenates the options array into a single string
 * with proper formatting for short and long options, usage, and description.
 * It ensures that the output is aligned and readable.
 * 
 * @param option The OptionContext structure containing the option details.
 * @return Returns a dynamically allocated string containing the formatted option.
 */
int format_desctription_call_handler(OptionContext option);

/**
 * @brief This function retrieves the array of options defined in the program.
 * 
 * This function retrieves the array of options defined in the program.
 * It returns a pointer to the array of OptionContext structures.
 * 
 * @return Returns a pointer to the array of OptionContext structures.
 */
const OptionContext *getOptions(void);

/**
 * @brief This function returns the number of options defined in the program.
 * 
 * This function returns the number of options defined in the program.
 * It calculates the size of the options array and divides it by the size of a single OptionContext structure.
 * 
 * @return Returns the number of options defined in the program.
 */
size_t getOptionsCount(void);

char *format_options(OptionContext option);

#endif
