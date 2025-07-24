#ifndef CORE_TASKS_H
#define CORE_TASKS_H

typedef enum {
    TASK_ENCRYPT,
    TASK_DECRYPT,
    TASK_HASH,
    TASK_STITCH_CHUNKS,
    TASK_VERIFY_CHECKSUM,
} task_type_t;

typedef struct {
    task_type_t type;
    void        *data; // cast depending on task
} task_t;

#endif // CORE_TASKS_H