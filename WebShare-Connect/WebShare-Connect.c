/**
 * @file WebShare-Connect.c
 * @brief Main program file containing the entry point.
 */
#include <stdio.h>
#include "WebShare-Connect.h"
#include "sender.h"
#include "receiver.h"
#include "removeQuotes.h"
#include <nice.h>

 /**
  * @brief The main function. This is where the program starts.
  *
  * @param argc The number of arguments passed to the program.
  * @param argv The arguments passed to the program.
  *             - argv[0]: Program name
  *             - argv[1]: Operation mode (sender/receiver)
  *             - argv[2]: Port number
  *             - argv[3]: Number of threads
  *             - argv[4]: File path
  *
  * @return 0 if the program exits successfully, 1 otherwise.
  */
int main(int argc, char const* argv[])
{
    printf("How many arguments: %d\n", argc);
    char* user_argv[5]; // Array of arguments to pass to sender_main or receiver_main
    if (5 == argc) {
        printf("IM HERE\n");
        // set user_argv to argv
        for (int i = 0; i < argc; i++) {
            user_argv[i] = strdup(argv[i]);
        }
    }
    //if there are no arguments then ask for them
    // else if (argc < 2) {
    else {
        char mode[10];
        int error;
        char port[6];
        char threads[4];
        char filePath[256];
        do {
            printf("Please enter either 'sender / server' or 'receiver / client': ");
            error = scanf("%9s", mode);
            if (error != 1) {
                printf("Input error. Please try again.\n");
                error = scanf("%*[^\n]"); // Clear the input buffer
            }
            else if (strcmp(mode, "sender") != 0 && strcmp(mode, "receiver") != 0 
                    && strcmp(mode, "server") != 0 && strcmp(mode, "client") != 0) {
                printf("Invalid mode entered. Please try again.\n");
            }
        } while (strcmp(mode, "sender") != 0 && strcmp(mode, "receiver") != 0 
                && strcmp(mode, "server") != 0 && strcmp(mode, "client") != 0);

        do {
            printf("Please enter the port (0-65535): ");
            error = scanf("%5s", port);
            if (error != 1) {
                printf("Input error. Please try again.\n");
                error = scanf("%*[^\n]"); // Clear the input buffer
            }
            else {
                int portNum = atoi(port);
                if (portNum < 0 || portNum > 65535) {
                    printf("Invalid port number. Please enter a number between 0 and 65535: ");
                    error = 0;
                }
            }
        } while (error != 1);

        do {
            printf("Please enter the number of threads: ");
            error = scanf("%3s", threads);
            if (error != 1) {
                printf("Input error. Please try again.\n");
                error = scanf("%*[^\n]"); // Clear the input buffer
            }
            //else {
            int threadNum = atoi(threads);
            //if (threadNum < 1 || threadNum > MAX_THREADS) {
            //    printf("Invalid number of threads. Please enter a number between 1 and %d.\n", MAX_THREADS);
            //    error = 0;
            //}
        //}
        } while (error != 1);

        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        printf("Please enter the file path: ");
        fgets(filePath, 256, stdin);
        filePath[strcspn(filePath, "\n")] = 0; // Remove trailing newline

        user_argv[0] = strdup(argv[0]); // Program name
        user_argv[1] = strdup(mode);
        user_argv[2] = strdup(port);
        user_argv[3] = strdup(threads);

        if (containsQuotes(filePath)) {
            printf("contains quotes and removing them\n");
            char* newFilePath = removeQuotes(filePath);
            user_argv[4] = strdup(newFilePath);
        }
        else user_argv[4] = strdup(filePath);

    if (strcmp(user_argv[1], "sender") == 0 || strcmp(user_argv[1], "server") == 0) {
        return sender_main(argc - 1, user_argv + 1);
    }
    else if (strcmp(user_argv[1], "receiver") == 0 || strcmp(user_argv[1], "client") == 0) {
        return receiver_main(argc - 1, user_argv + 1);
    }
    else {
        printf("Usage: %s [sender|receiver]\n", user_argv[0]);
        return 1;
    }
}
