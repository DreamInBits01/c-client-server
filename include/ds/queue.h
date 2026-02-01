#ifndef QUEUE_H
#define QUEUE_H
#include <uthash/src/utlist.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include "structs.h"
Queue *initialize_queue(int capacity, bool has_capacity_limits);
int destroy_queue(Queue *queue);
Task *queue_task(Queue *queue, void *data);
void *dequeue_task(Queue *queue);
size_t get_queue_tasks_count(Queue *queue);
#endif