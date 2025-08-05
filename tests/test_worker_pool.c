#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "jobqueue.h"
#include "tasks.h"

int fib(int n) {
    if (n == 0) return 0;

    if (n == 1) return 1;

    return fib(n - 1) + fib(n - 2);
}

int main() {
    printf("Running worker pool tests...\n");
    return 0;
}