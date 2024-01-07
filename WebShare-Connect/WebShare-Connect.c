#include "WebShare-Connect.h"
#include "server.h"
#include "client.h"
#include "removeQuotes.h"

int userInput(int error) {
	if (error == EOF) {
		printf("Error reading input\n");
		return 1;
	}
	else if (error < 0) {
		printf("No input\n");
		return 1;
	}
	return 0;
}

int main(int argc, char const* argv[])
{
    //if there are no arguments then ask for them
    if (argc < 2) {
        char mode[10];
        int error;
        char port[6];
		char threads[4];
        char filePath[256];
        char* user_argv[5]; // Array of arguments to pass to server_main or client_main
        do {
            printf("Please enter either 'server' or 'client': ");
            error = scanf("%9s", mode);
            if (error != 1) {
                printf("Input error. Please try again.\n");
                scanf("%*[^\n]"); // Clear the input buffer
            }
            else if (strcmp(mode, "server") != 0 && strcmp(mode, "client") != 0) {
                printf("Invalid mode entered. Please try again.\n");
            }
        } while (strcmp(mode, "server") != 0 && strcmp(mode, "client") != 0);

        do {
            printf("Please enter the port (0-65535): ");
            error = scanf("%5s", port);
            if (error != 1) {
                printf("Input error. Please try again.\n");
                scanf("%*[^\n]"); // Clear the input buffer
            }
            else {
                int portNum = atoi(port);
                if (portNum < 0 || portNum > 65535) {
                    printf("Invalid port number. Please enter a number between 0 and 65535.\n");
                    error = 0;
                }
            }
        } while (error != 1);

        do {
            printf("Please enter the number of threads:\n");
            error = scanf("%3s", threads);
            if (error != 1) {
                printf("Input error. Please try again.\n");
                scanf("%*[^\n]"); // Clear the input buffer
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
        scanf("%255s", filePath);

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

        if (strcmp(mode, "server") == 0) {

            return server_main(4, user_argv + 1); // 4 is the number of arguments in user_argv
        }
        else {
            return client_main(argc, user_argv + 1);
        }
    }

    if (strcmp(argv[1], "server") == 0) {
        return server_main(argc - 1, argv + 1); //what does argv + 1 do? a: it is a pointer to the next element in the array of arguments
    }
    else if (strcmp(argv[1], "client") == 0) {
        return client_main(argc - 1, argv + 1);
    }
    else {
        printf("Usage: %s [server|client]\n", argv[0]);
		return 1;
    }
}
