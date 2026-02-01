#ifndef QUEUE_STRUCTS_H
#define QUEUE_STRUCTS_H
#include <pthread.h>
#include <stdbool.h>
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
typedef struct Task
{
    void *data;
    struct Task *prev; /* needed for a doubly-linked list only */
    struct Task *next; /* needed for singly- or doubly-linked lists */
} Task;
typedef struct Queue
{
    Task *tasks;
    size_t queue_capacity;
    size_t tasks_count;
    bool has_capacity_limits;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} Queue;
#endif