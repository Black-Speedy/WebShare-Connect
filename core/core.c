#include <stdio.h>
#include <zmq.h>
#include <string.h>
#include "core.h"
#include "cthreads.h"

void *print_hello(void *arg) {
    const char *msg = arg;
    for (int i = 0; i < 3; i++) {
        printf("%s: %d\n", msg, i);
    }

    return NULL;
}

int core_test() {
    struct cthreads_thread thread1, thread2;
    struct cthreads_args   args1, args2;

    // No custom attributes
    struct cthreads_thread_attr *attr = NULL;

    // Create first thread
    if (cthreads_thread_create(&thread1, attr, print_hello, "Thread 1", &args1) != 0) {
        perror("Failed to create thread 1");
        return 1;
    }

    // Create second thread
    if (cthreads_thread_create(&thread2, attr, print_hello, "Thread 2", &args2) != 0) {
        perror("Failed to create thread 2");
        return 1;
    }

    // Join both threads
    cthreads_thread_join(thread1, NULL);
    cthreads_thread_join(thread2, NULL);

    printf("All threads completed.\n");
    return 0;
}