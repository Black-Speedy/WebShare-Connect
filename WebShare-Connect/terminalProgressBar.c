#include "terminalProgressBar.h"

void applyStatusColor(int is_complete) {
    #ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if (is_complete) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Green color for completion
        } else {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);       // Yellow color for progress
        }

    #else
        if (is_complete) {
            printf("\033[0;32m"); // Green color for completion on Unix
        } else {
            printf("\033[0;33m"); // Yellow color for progress on Unix
        }

    #endif
}

void resetConsoleColor() {
    printf("\033[0m"); // Reset color on Unix
}

void printProgressBar(int percentage, int current_chunk, int total_chunks, int is_complete) {
    int width = 50; // Width of the progress bar

    // Set color
    applyStatusColor(is_complete);

    printf("\r["); // Carriage return to start of line without new line

    // Calculate the number of filled and unfilled segments
    int filled = (percentage * width) / 100;
    for (int i = 0; i < width; ++i) {
        if (i < filled)
            #ifdef _WIN32
                printf("#"); // Use # for filled part on Windows
            #else
                printf("█"); // Use Unicode solid block on Linux
            #endif
        else
            printf("-");
    }

    // Display the percentage and chunk information
    printf("] %d%% | Chunk %d/%d", percentage, current_chunk, total_chunks);

    fflush(stdout); // Display immediately

    // Reset color
    resetConsoleColor();
}