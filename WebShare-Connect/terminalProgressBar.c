#include "terminalProgressBar.h"

void printProgressBar(int percentage) {
    int width = 50; // Width of the progress bar

    // Print the beginning of the progress bar
    printf("\r["); // '\r' brings the cursor to the beginning of the line

    int pos = (percentage * width) / 100;

    for (int i = 0; i < width; ++i) {
        if (i < pos)
            printf("=");
        else if (i == pos)
            printf(">");
        else
            printf(" ");
    }

    printf("] %d%%", percentage);

    fflush(stdout); // Force the output to be displayed immediately
}

void sleep_ms(int milliseconds) {
    #ifdef _WIN32
        Sleep(milliseconds); // Windows sleep in milliseconds
    #else
        usleep(milliseconds * 1000); // usleep takes microseconds
    #endif
}