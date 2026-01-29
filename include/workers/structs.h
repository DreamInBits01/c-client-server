#ifndef WORKERS_STRUCTS_H
#define WORKERS_STRUCTS_H
#include <unistd.h>
#include <pthread.h>
#include "connections/structs.h"

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

typedef struct Request
{
    Connection *connection;
    struct Request *prev; /* needed for a doubly-linked list only */
    struct Request *next; /* needed for singly- or doubly-linked lists */
} Request;
typedef struct RequestsQueue
{
    Request *requests;
    // Number of workers * 10
    size_t queue_capacity;
    size_t used_capacity;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} RequestsQueue;
typedef struct
{
    pthread_t *threads;
    size_t number_of_workers;
    RequestsQueue *queue;
    volatile int shutdown;
    volatile int active_workers;
} ThreadPool;
#endif