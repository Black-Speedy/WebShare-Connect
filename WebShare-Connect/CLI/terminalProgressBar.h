#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void applyStatusColor(int is_complete);
void resetConsoleColor();
void printProgressBar(int percentage, int current_chunk, int total_chunks, int is_complete);