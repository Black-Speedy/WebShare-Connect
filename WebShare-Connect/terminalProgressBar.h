#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
// terminalProgressBar.h
#ifndef TERMINAL_PROGRESS_BAR_H
#define TERMINAL_PROGRESS_BAR_H

void printProgressBar(int percentage);

void sleep_ms(int milliseconds);

#endif // TERMINAL_PROGRESS_BAR_H
