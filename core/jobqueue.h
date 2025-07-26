#ifndef CORE_JOBQUEUE_H
#define CORE_JOBQUEUE_H

#include "cthreads.h"
#include "tasks.h"

typedef struct {
    int                   head;
    int                   tail;
    int                   currTasks; // Current number of tasks
    int                   stop;      // Flag to stop the queue

    struct cthreads_mutex mutex;
    struct cthreads_cond  not_empty;
    struct cthreads_cond  not_full;

    task_t                **tasks;   // Array of tasks
    size_t                capacity;  // Maximum number of tasks
} job_queue_t;

int job_queue_init(job_queue_t *queue, size_t capacity);

int job_queue_destroy(job_queue_t *queue);

int job_queue_push(job_queue_t *queue, task_t *task);

int job_queue_pop(job_queue_t *queue, task_t **task);

int job_queue_stop(job_queue_t *queue);

#endif // CORE_JOBQUEUE_H