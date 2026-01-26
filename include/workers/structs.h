#ifndef WORKERS_STRUCTS_H
#define WORKERS_STRUCTS_H
#include "connections/structs.h"
#include <pthread.h>
#define NUMBER_OF_WORKERS 5

// typedef struct
// {
//     pthread_t thread_id;

// } Worker;
typedef struct
{
    pthread_t threads[NUMBER_OF_WORKERS];
    int number_of_workers;

    // Queue implementation should be thread-safe
    // Queue should provide a count function to be used in the condition
    pthread_cond_t work_available;
    pthread_cond_t queue_empty;

    volatile int shutdown;
    volatile int active_workers;
} ThreadPool;
#endif