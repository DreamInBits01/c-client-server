#ifndef QUEUE_H
#define QUEUE_H
#include <uthash/src/utlist.h>
#include <pthread.h>
#include "connections/structs.h"
#include "structs.h"
RequestsQueue *initialize_queue(int number_of_workers);
int destroy_queue(RequestsQueue *queue);
Request *queue_request(RequestsQueue *queue, Connection *connection);
Connection *dequeue_request(RequestsQueue *queue);
size_t queue_used_capacity(RequestsQueue *queue);
#endif