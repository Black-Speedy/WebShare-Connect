/**
 * @file removeQuotes.h
 * @brief Functions to handle double quotes in strings.
 */

/**
* @brief      Checks if a string contains double quotes.
* @param[in]  str   The string to check.
* @return     1 if the string contains double quotes, 0 otherwise.
*/
int containsQuotes(const char* str);
/**
* @brief      Removes double quotes from a string.
* @param[in]  str   The string to remove double quotes from.
* @return     A pointer to the modified string.
*/
char* removeQuotes(const char* str);