#include <assert.h>
#include <stdlib.h>
#include "jobqueue.h"

int job_queue_init(job_queue_t *queue, size_t capacity) {
    queue->capacity  = capacity;
    queue->head      = 0;
    queue->tail      = 0;
    queue->currTasks = 0;
    queue->stop      = 0;

    queue->tasks = malloc(capacity * sizeof(task_t *));
    assert(queue->tasks != NULL);

    cthreads_mutex_init(&queue->mutex, NULL);
    cthreads_cond_init(&queue->not_empty, NULL);
    cthreads_cond_init(&queue->not_full, NULL);

    return 0;
}

int job_queue_destroy(job_queue_t *queue) {
    assert(cthreads_mutex_lock(&queue->mutex) == 0);

    queue->stop = 1;

    // Wake up any waiting threads
    assert(cthreads_cond_broadcast(&queue->not_empty) == 0);
    assert(cthreads_cond_broadcast(&queue->not_full) == 0);

    assert(cthreads_mutex_unlock(&queue->mutex) == 0);

    assert(cthreads_mutex_destroy(&queue->mutex) == 0);
    assert(cthreads_cond_destroy(&queue->not_empty) == 0);
    assert(cthreads_cond_destroy(&queue->not_full) == 0);

    free(queue->tasks);
    queue->tasks = NULL;

    return 0;
}

int job_queue_push(job_queue_t *queue, task_t *task) {
    assert(cthreads_mutex_lock(&queue->mutex) == 0);

    // lock the queue until there's space
    while (queue->currTasks == queue->capacity && !queue->stop) {
        assert(cthreads_cond_wait(&queue->not_full, &queue->mutex) == 0);
    }

    if (queue->stop) {
        assert(cthreads_mutex_unlock(&queue->mutex) == 0);
        return -1;
    }

    queue->tasks[queue->tail] = task;
    queue->tail = (queue->tail + 1) % queue->capacity;
    queue->currTasks++;

    assert(cthreads_cond_signal(&queue->not_empty) == 0);
    assert(cthreads_mutex_unlock(&queue->mutex) == 0);

    return 0;
}

int job_queue_pop(job_queue_t *queue, task_t **task) {
    assert(cthreads_mutex_lock(&queue->mutex) == 0);

    // lock the queue until there's a task
    while (queue->currTasks == 0 && !queue->stop) {
        assert(cthreads_cond_wait(&queue->not_empty, &queue->mutex) == 0);
    }

    if (queue->stop && queue->currTasks == 0) {
        assert(cthreads_mutex_unlock(&queue->mutex) == 0);
        return -1;
    }

    *task       = queue->tasks[queue->head];
    queue->head = (queue->head + 1) % queue->capacity;
    queue->currTasks--;

    assert(cthreads_cond_signal(&queue->not_full) == 0);
    assert(cthreads_mutex_unlock(&queue->mutex) == 0);

    return 0;
}

int job_queue_stop(job_queue_t *queue) {
    assert(cthreads_mutex_lock(&queue->mutex) == 0);

    queue->stop = 1;

    // Wake up any waiting threads
    assert(cthreads_cond_broadcast(&queue->not_empty) == 0);
    assert(cthreads_cond_broadcast(&queue->not_full) == 0);

    assert(cthreads_mutex_unlock(&queue->mutex) == 0);

    return 0;
}