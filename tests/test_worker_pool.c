#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "jobqueue.h"
#include "tasks.h"
#include "workerpool.h"


int main() {
    printf("Running worker pool test with Fibonacci task...\n");

    job_queue_t   queue;
    worker_pool_t pool;
    size_t        capacity    = 64;
    int           num_threads = 15;

    assert(job_queue_init(&queue, capacity) == 0);
    assert(worker_pool_init(&pool, &queue, num_threads) == 0);

    for (int i = 40; i <= 50; ++i) {
        uint64_t *arg = malloc(sizeof(uint64_t));
        *arg = i;

        task_t *task = malloc(sizeof(task_t));

        task->type = TASK_FIB;
        task->data = arg;

        assert(job_queue_push(&queue, task) == 0);
    }

    clock_t start = clock();
    assert(worker_pool_destroy(&pool) == 0);
    clock_t end = clock();

    printf("elapsed %.3f s using %d thread(s)\n",
           (double)(end - start) / CLOCKS_PER_SEC,
           num_threads);

    assert(job_queue_destroy(&queue) == 0);

    printf("test_worker_pool passed\n");
    return 0;
}