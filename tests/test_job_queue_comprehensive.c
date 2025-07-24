#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "jobqueue.h"
#include "tasks.h"
#include "cthreads.h"

// Test data structure
typedef struct {
    job_queue_t *queue;
    int         thread_id;
    int         num_tasks;
} thread_data_t;

void dummy_task_free(task_t *task) {
    free(task->data);
    free(task);
}

task_t *create_test_task(int value) {
    task_t *task = malloc(sizeof(task_t));
    task->type         = TASK_HASH;
    task->data         = malloc(sizeof(int));
    *(int *)task->data = value;
    return task;
}

// Test 1: Basic functionality (your current test)
int test_basic_operations() {
    printf("Running basic operations test...\n");

    job_queue_t q;
    assert(job_queue_init(&q, 4) == 0);

    // Push a task
    task_t *t = create_test_task(42);
    assert(job_queue_push(&q, t) == 0);

    // Pop and verify
    task_t *out = NULL;
    assert(job_queue_pop(&q, &out) == 0);
    assert(out->type == TASK_HASH);
    assert(*(int *)out->data == 42);

    dummy_task_free(out);
    assert(job_queue_destroy(&q) == 0);

    printf("Basic operations test PASSED\n");
    return 0;
}

// Test 2: Queue capacity limits
int test_capacity_limits() {
    printf("Running capacity limits test...\n");

    job_queue_t q;
    assert(job_queue_init(&q, 2) == 0); // Small queue

    // Fill the queue
    task_t *t1 = create_test_task(1);
    task_t *t2 = create_test_task(2);

    assert(job_queue_push(&q, t1) == 0);
    assert(job_queue_push(&q, t2) == 0);

    // Queue should be full now
    // Note: Testing blocking behavior would require threading

    // Empty the queue
    task_t *out;
    assert(job_queue_pop(&q, &out) == 0);
    assert(*(int *)out->data == 1);
    dummy_task_free(out);

    assert(job_queue_pop(&q, &out) == 0);
    assert(*(int *)out->data == 2);
    dummy_task_free(out);

    assert(job_queue_destroy(&q) == 0);

    printf("Capacity limits test PASSED\n");
    return 0;
}

// Test 3: Multiple tasks in sequence
int test_multiple_tasks() {
    printf("Running multiple tasks test...\n");

    job_queue_t q;
    assert(job_queue_init(&q, 10) == 0);

    const int num_tasks = 5;

    // Push multiple tasks
    for (int i = 0; i < num_tasks; i++) {
        task_t *task = create_test_task(i * 10);
        assert(job_queue_push(&q, task) == 0);
    }

    // Pop and verify order (FIFO)
    for (int i = 0; i < num_tasks; i++) {
        task_t *out;
        assert(job_queue_pop(&q, &out) == 0);
        assert(*(int *)out->data == i * 10);
        dummy_task_free(out);
    }

    assert(job_queue_destroy(&q) == 0);

    printf("Multiple tasks test PASSED\n");
    return 0;
}

// Test 4: Stop functionality
int test_stop_functionality() {
    printf("Running stop functionality test...\n");

    job_queue_t q;
    assert(job_queue_init(&q, 4) == 0);

    // Add a task
    task_t *task = create_test_task(100);
    assert(job_queue_push(&q, task) == 0);

    // Stop the queue
    assert(job_queue_stop(&q) == 0);

    // Try to push after stop (should fail)
    task_t *task2 = create_test_task(200);
    assert(job_queue_push(&q, task2) == -1);
    dummy_task_free(task2); // Clean up since push failed

    // Should still be able to pop existing task
    task_t *out;
    assert(job_queue_pop(&q, &out) == 0);
    assert(*(int *)out->data == 100);
    dummy_task_free(out);

    // Pop from empty stopped queue should fail
    assert(job_queue_pop(&q, &out) == -1);

    assert(job_queue_destroy(&q) == 0);

    printf("Stop functionality test PASSED\n");
    return 0;
}

// Producer thread function
void *producer_thread(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;

    for (int i = 0; i < data->num_tasks; i++) {
        task_t *task = create_test_task(data->thread_id * 1000 + i);
        if (job_queue_push(data->queue, task) != 0) {
            dummy_task_free(task); // Clean up on failure
            break;
        }
    }

    return NULL;
}

// Consumer thread function
void *consumer_thread(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;

    for (int i = 0; i < data->num_tasks; i++) {
        task_t *task;
        if (job_queue_pop(data->queue, &task) == 0) {
            // Process task (just verify it's valid)
            assert(task->type == TASK_HASH);
            assert(task->data != NULL);
            dummy_task_free(task);
        } else {
            break; // Queue stopped
        }
    }

    return NULL;
}

// Test 5: Basic threading (producer-consumer)
int test_basic_threading() {
    printf("Running basic threading test...\n");

    job_queue_t q;
    assert(job_queue_init(&q, 10) == 0);

    const int tasks_per_thread = 5;

    // Create thread data
    thread_data_t producer_data = {&q, 1, tasks_per_thread};
    thread_data_t consumer_data = {&q, 2, tasks_per_thread};

    // Create threads
    struct cthreads_thread producer, consumer;
    struct cthreads_args   producer_args, consumer_args;

    assert(cthreads_thread_create(&producer, NULL, producer_thread, &producer_data, &producer_args) == 0);
    assert(cthreads_thread_create(&consumer, NULL, consumer_thread, &consumer_data, &consumer_args) == 0);

    // Wait for threads to complete
    assert(cthreads_thread_join(producer, NULL) == 0);
    assert(cthreads_thread_join(consumer, NULL) == 0);

    assert(job_queue_destroy(&q) == 0);

    printf("Basic threading test PASSED\n");
    return 0;
}

int main() {
    printf("=== Comprehensive Job Queue Tests ===\n\n");

    int failed = 0;

    failed += test_basic_operations();
    failed += test_capacity_limits();
    failed += test_multiple_tasks();
    failed += test_stop_functionality();
    failed += test_basic_threading();

    if (failed == 0) {
        printf("\n=== ALL TESTS PASSED ===\n");
        return 0;
    } else {
        printf("\n=== %d TESTS FAILED ===\n", failed);
        return 1;
    }
}