# ifndef CORE_WORKERPOOL_H
# define CORE_WORKERPOOL_H

# include "cthreads.h"
# include "tasks.h"
# include "jobqueue.h"

typedef struct {
    int                    running;
    size_t                 num_threads;
    struct cthreads_thread *thread;
    job_queue_t            *job_queue;
} worker_pool_t;


int worker_pool_init(worker_pool_t *pool, job_queue_t *queue, size_t num_threads);

int worker_pool_destroy(worker_pool_t *pool);

# endif