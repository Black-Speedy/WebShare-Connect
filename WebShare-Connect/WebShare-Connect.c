#include "WebShare-Connect.h"
#include "server.h"
#include "client.h"
#include "removeQuotes.h"

int main(int argc, char const* argv[])
{
    //if there are no arguments then ask for them
    if (argc < 2) {
        char* user_argv[5];
        printf("Which one? server or client?\n");
        char mode[10]; // mode[10] is an array of 10 characters
        scanf("%9s", mode); // scanf("%9s", mode) reads a string of up to 9 characters from stdin and stores it in mode

        if (strcmp(mode, "server") == 0 || strcmp(mode, "client") == 0) {
            printf("Which port?\n");
            char port[6];
            scanf("%5s", port);

            printf("How many threads?\n");
            char threads[4];
            scanf("%3s", threads);

            printf("What is the file path?\n");
            char filePath[256];
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
                return client_main(argc, port, threads, filePath);
            }
        }
        else {
            printf("Invalid mode entered.\n");
            return 1;
        }
            
    }

    if (strcmp(argv[1], "server") == 0) {
        return server_main(argc - 1, argv + 1); //what does argv + 1 do? a: it is a pointer to the next element in the array of arguments
    }
    else if (strcmp(argv[1], "client") == 0) {
        return client_main(argc - 1, argv + 1);
    }
    else {
        printf("---------------------\n");
        printf("Usage: %s [server|client]\n", argv[0]);
		return 1;
    }
}
