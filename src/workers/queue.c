#include "workers/queue.h"

RequestsQueue *init_queue(int number_of_workers)
{
    RequestsQueue *queue = malloc(sizeof(RequestBuffer));
    if (queue == NULL)
    {
        return NULL;
    };
    memset(queue, 0, sizeof(queue));
    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->not_empty, NULL);
    pthread_cond_init(&queue->not_full, NULL);
    queue->queue_capacity = number_of_workers * 10;
    queue->requests = NULL;
    return queue;
}
Request *queue_request(RequestsQueue *queue, Connection *connection)
{
    Request *request = malloc(sizeof(Request));
    if (request == NULL)
    {
        return NULL;
    }
    memset(request, 0, sizeof(Request));
    request->connection = connection;
    pthread_mutex_lock(&queue->mutex);

    // Queue is full, producer must wait for the consumers to consume and signal that the queue is not_full
    while (queue->used_capacity >= queue->queue_capacity)
        pthread_cond_wait(&queue->not_full, &queue->mutex);

    // Produce
    DL_APPEND(queue->requests, request);
    queue->used_capacity += 1;

    // Signal the queue is not empty for workers
    pthread_cond_broadcast(&queue->not_empty);
    pthread_mutex_unlock(&queue->mutex);
    return request;
}
Connection *dequeue_request(RequestsQueue *queue)
{
    Request *dequeued;
    Connection *connection;
    pthread_mutex_lock(&queue->mutex);

    // Queue is empty, workers must wait for the queue to be not empty
    while (queue->used_capacity <= 0)
        pthread_cond_wait(&queue->not_empty, &queue->mutex);
    // Initialize values
    dequeued = queue->requests;
    // Return value
    connection = dequeued->connection;
    if (queue->used_capacity <= 0)
    {
        pthread_mutex_unlock(&queue->mutex);
        return NULL;
    }
    // Delete from the list
    DL_DELETE(queue->requests, dequeued);

    // Cleanup
    queue->used_capacity -= 1;
    free(dequeued);

    // Signal that the queue is not full so the producer can resume
    pthread_cond_signal(&queue->not_full);
    pthread_mutex_unlock(&queue->mutex);
    return connection;
}
size_t queue_used_capacity(RequestsQueue *queue)
{
    size_t used_capacity;
    pthread_mutex_lock(&queue->mutex);
    used_capacity = queue->used_capacity;
    pthread_mutex_unlock(&queue->mutex);
    return used_capacity;
}
int destroy_queue(RequestsQueue *queue)
{
    if (queue == NULL)
        return -1;
    pthread_mutex_destroy(&queue->mutex);
    pthread_cond_destroy(&queue->not_empty);
    pthread_cond_destroy(&queue->not_full);

    // Empty the list
    Request *tmp, *element;
    DL_FOREACH_SAFE(queue->requests, element, tmp)
    {
        DL_DELETE(queue->requests, element);
        free(element);
    }
    free(queue);
}