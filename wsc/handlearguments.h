#include "options.h"
/**
 * @brief Handles a single command-line argument and its associated value.
 * 
 * This function processes a single command-line argument and its associated value.
 * It checks if the argument matches any defined options and calls the corresponding handler.
 * If the option requires an argument, it checks if the value is provided. On success, it skips
 * the number of arguments consumed by the option.
 * 
 * @param input_option The command-line option to process (e.g., "-p" or "--port").
 * @param rest_of_input A pointer to the next argument after the option.
 * @return Returns the number of arguments consumed on success, or an error code if an error occurs.
 */
int handle_cli_singular_argument(char *input_option, char *rest_of_input[]);

/**
 * @brief Handles command-line arguments based on the defined options.
 * 
 * This function processes command-line arguments based on the defined options.
 * It iterates through the provided arguments, and calls the helper function
 * `handle_cli_argument_smart` for each argument to determine if it matches any
 * defined options along with the rest of the provided input. If an argument
 * fails in the helper function it returns the error code.
 * 
 * @param count The number of arguments passed to the program.
 * @param arguments The array of arguments passed to the program.
 * @return Returns 0 on success, or an error code if an error occurs.
 */
int handle_cli_arguments(int count, char *arguments[]);
