/**
 * @file WebShare-Connect.c
 * @brief Main program file containing the entry point.
 */

#include "WebShare-Connect.h"
#include "sender.h"
#include "receiver.h"
#include "removeQuotes.h"

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
    //if there are no arguments then ask for them
    if (argc < 2) {
        char mode[10];
        int error;
        char port[6];
		char threads[4];
        char filePath[256];
        char* user_argv[5]; // Array of arguments to pass to sender_main or receiver_main
        do {
            printf("Please enter either 'sender' or 'receiver': ");
            error = scanf("%9s", mode);
            if (error != 1) {
                printf("Input error. Please try again.\n");
                error = scanf("%*[^\n]"); // Clear the input buffer
            }
            else if (strcmp(mode, "sender") != 0 && strcmp(mode, "receiver") != 0) {
                printf("Invalid mode entered. Please try again.\n");
            }
        } while (strcmp(mode, "sender") != 0 && strcmp(mode, "receiver") != 0);

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

        printf("Please enter the file path: ");
        error = scanf("%255s", filePath);

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

        if (strcmp(mode, "sender") == 0) {

            return sender_main(4, user_argv + 1); // 4 is the number of arguments in user_argv
        }
        else {
            return receiver_main(4, user_argv + 1);
        }
    }

    if (strcmp(argv[1], "sender") == 0) {
        return sender_main(argc - 1, argv + 1); //what does argv + 1 do? a: it is a pointer to the next element in the array of arguments
    }
    else if (strcmp(argv[1], "receiver") == 0) {
        return receiver_main(argc - 1, argv + 1);
    }
    else {
        printf("Usage: %s [sender|receiver]\n", argv[0]);
		return 1;
    }
}
