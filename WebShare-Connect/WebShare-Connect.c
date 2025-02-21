/**
 * @file WebShare-Connect.c
 * @brief Main program file containing the entry point.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "WebShare-Connect.h"
#include "sender.h"
#include "receiver.h"
#include "removeQuotes.h"

#ifndef MAX_THREADS
#define MAX_THREADS       100
#endif

#define MAX_INPUT_SIZE    256

/**
 * @brief Gets user input and ensures it's valid.
 * @param prompt The message to display to the user.
 * @param buffer The buffer to store the user input.
 * @param size The maximum size of the input.
 */
void get_user_input(const char *prompt, char *buffer, int size) {
    int error = 0;
    do {
        printf("%s", prompt);
        if (fgets(buffer, size, stdin) == NULL) {
            printf("Input error. Please try again.\n");
            continue;
        }

        buffer[strcspn(buffer, "\n")] = '\0'; // Remove trailing newline
        error = (strlen(buffer) > 0);
        if (!error) {
            printf("Invalid input. Please try again.\n");
        }
    } while (!error);
}

/**
 * @brief Gets a valid integer input within a specified range.
 * @param prompt The message to display to the user.
 * @param min The minimum acceptable value.
 * @param max The maximum acceptable value.
 * @return A valid integer within the given range.
 */
int get_valid_int(const char *prompt, int min, int max) {
    char buffer[MAX_INPUT_SIZE];
    int  value, error;
    do {
        get_user_input(prompt, buffer, sizeof(buffer));
        error = sscanf(buffer, "%d", &value);
        if (error != 1 || value < min || value > max) {
            printf("Invalid input. Please enter a number between %d and %d.\n", min, max);
            error = 0;
        }
    } while (!error);

    return value;
}

/**
 * @brief Handles user input for the required arguments.
 * @param user_argv Array to store user inputs.
 */
void handle_user_input(char *user_argv[]) {
    char mode[MAX_INPUT_SIZE];
    char port[MAX_INPUT_SIZE];
    char threads[MAX_INPUT_SIZE];
    char filePath[MAX_INPUT_SIZE];

    // Get mode input
    do {
        get_user_input("Enter 'sender' / 'server' or 'receiver' / 'client': ", mode, sizeof(mode));
    } while (strcmp(mode, "sender") != 0 && strcmp(mode, "receiver") != 0 &&
             strcmp(mode, "server") != 0 && strcmp(mode, "client") != 0);

    // Get port input
    int portNum = get_valid_int("Enter the port (1-65535): ", 1, 65535);
    snprintf(port, sizeof(port), "%d", portNum);

    // Get threads input
    int threadNum = get_valid_int("Enter the number of threads (1-100): ", 1, MAX_THREADS);
    snprintf(threads, sizeof(threads), "%d", threadNum);

    // Get file path input
    get_user_input("Enter the file path: ", filePath, sizeof(filePath));

    // Remove quotes if present
    if (containsQuotes(filePath)) {
        printf("File path contains quotes. Removing them...\n");
        char *newFilePath = removeQuotes(filePath);
        strncpy(filePath, newFilePath, sizeof(filePath) - 1);
        filePath[sizeof(filePath) - 1] = '\0';
    }

    // Populate user_argv
    printf("Name: %s\n", user_argv[0]);
    user_argv[1] = strdup(mode);
    user_argv[2] = strdup(port);
    user_argv[3] = strdup(threads);
    user_argv[4] = strdup(filePath);
}

int main(int argc, char const *argv[]) {
    char *user_argv[5];
    user_argv[0] = strdup(argv[0]);
    if (5 == argc) {
        for (int i = 1; i < argc; i++) {
            user_argv[i] = strdup(argv[i]);
        }
    } else {
        handle_user_input(user_argv);
    }

    // finding the mode
    int result = 1;
    if (0 == strcmp(user_argv[1], "sender") || 0 == strcmp(user_argv[1], "server")) {
        result = sender_main(4, user_argv + 1);
    } else if (0 == strcmp(user_argv[1], "receiver") || 0 == strcmp(user_argv[1], "client"))  {
        result = receiver_main(4, user_argv + 1);
    } else {
        printf("Usage: %s [sender|receiver]\n", user_argv[0]);
    }

    for (int i = 0; i < 5; i++) {
        free(user_argv[i]);
    }

    return result;
}