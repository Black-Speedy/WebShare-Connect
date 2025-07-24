#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "jobqueue.h"
#include "tasks.h"

void dummy_task_free(task_t *task) {
    free(task->data);
    free(task);
}

int main() {
    job_queue_t q;
    assert(job_queue_init(&q, 4) == 0);

    // Push a dummy task
    task_t *t = malloc(sizeof(task_t));
    t->type = TASK_HASH;
    t->data = malloc(1); // fake data
    assert(job_queue_push(&q, t) == 0);

    // Pop and verify
    task_t *out = NULL;
    assert(job_queue_pop(&q, &out) == 0);
    assert(out->type == TASK_HASH);

    dummy_task_free(out);
    job_queue_destroy(&q);

    printf("test_job_queue passed\n");
    return 0;
}