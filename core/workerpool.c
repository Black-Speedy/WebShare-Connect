#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "workerpool.h"

// placeholder for task handling function
static void handle_encrypt(void *data) {
    printf("Handling encryption task with data: %p\n", data);
}

static void worker_loop(void *arg) {
    #ifdef _WIN32
        struct cthreads_args *args = (struct cthreads_args *)arg;
        worker_pool_t        *pool = (worker_pool_t *)args->data;
        free(args);
    #else
        worker_pool_t *pool = (worker_pool_t *)arg;
    #endif

    task_t *task;

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

    for (size_t i = 0; i < num_threads; ++i) {
        // need to test on windows and POSIX
        #ifdef _WIN32
            struct cthreads_args *args = malloc(sizeof(struct cthreads_args));
            if (!args) return -1;

            args->func = worker_loop;
            args->data = pool;
            cthreads_thread_create(&pool->thread[i], NULL, worker_loop, pool, args);
        #else
            // args is ignored on POSIX
            cthreads_thread_create(&pool->thread[i], NULL, worker_loop, pool, NULL);
        #endif
    }

    return 0;
}

int worker_pool_destroy(worker_pool_t *pool) {
    pool->running = 0;

    job_queue_stop(pool->job_queue);

    // signal threads to stop
    for (size_t i = 0; i < pool->num_threads; ++i) {
        cthreads_thread_join(pool->thread[i], NULL);
    }

    free(pool->thread);
    return 0;
}

// TODO: add tests to ensure worker pool functionality
// maybe a simple fibonacci task or similar?