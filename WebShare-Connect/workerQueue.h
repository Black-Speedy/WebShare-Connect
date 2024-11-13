// worker_queue.h
#ifndef WORKER_QUEUE_H
#define WORKER_QUEUE_H

#include <stddef.h>

typedef void (*task_function)(void *arg);

typedef struct WorkerQueue WorkerQueue;

WorkerQueue* worker_queue_init(int num_threads);

int worker_queue_submit(WorkerQueue *queue, task_function func, void *arg);

void worker_queue_destroy(WorkerQueue *queue);

#endif // WORKER_QUEUE_H
