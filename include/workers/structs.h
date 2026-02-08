#ifndef WORKERS_STRUCTS_H
#define WORKERS_STRUCTS_H
#include <unistd.h>
#include <pthread.h>
#include "connections/structs.h"
#include "ds/structs.h"
/*
Number of workers is the number of cors + 1 on the system
*/
/*
-if the queue is not empty, workers will consume untill the queue becomes empty.
If the queue is empty, workers will be stopped.

-if the queue is not full, the submitting function (producer) will keep working.
If the queue is full, the producer will stop
-
*/

typedef struct ThreadPool
{
    pthread_t *threads;
    Queue *queue;
    size_t number_of_workers;
    volatile int shutdown;
} ThreadPool;
#endif