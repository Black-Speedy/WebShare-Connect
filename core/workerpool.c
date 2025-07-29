# include <stdlib.h>
# include <stdio.h>
#include <assert.h>

# include "workerpool.h"

// placeholder for task handling function
static void handle_encrypt(void *data) {
    printf("Handling encryption task with data: %p\n", data);
}

static void worker_loop(void *arg) {
    worker_pool_t *pool = (worker_pool_t *)arg;
    task_t        *task;

    while (job_queue_pop(pool->job_queue, (task_t **)&task) == 0) {
        switch (task->type) {
        case TASK_ENCRYPT:
            handle_encrypt(task->data);
            break;
        default:
            fprintf(stderr, "[worker] Unknown task type: %d\n", task->type);
            break;
        }

        free(task->data);
        free(task);
    }
}

int worker_pool_init(worker_pool_t *pool, job_queue_t *queue, uint16_t num_threads) {
    pool->num_threads = num_threads;
    pool->job_queue   = queue;
    pool->running     = 1;

    pool->thread = calloc(num_threads, sizeof(struct cthreads_thread));
    if (!pool->thread) return -1;

    return 0;
}