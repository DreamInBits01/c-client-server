#include "ds/queue.h"

Queue *initialize_queue(int capacity, bool has_capacity_limits)
{
    Queue *queue = malloc(sizeof(Queue));
    if (queue == NULL)
    {
        return NULL;
    };
    memset(queue, 0, sizeof(Queue));
    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->not_empty, NULL);
    pthread_cond_init(&queue->not_full, NULL);
    queue->has_capacity_limits = has_capacity_limits;
    queue->queue_capacity = has_capacity_limits ? capacity : -1;
    queue->tasks = NULL;
    return queue;
}
Task *queue_task(Queue *queue, void *data)
{
    Task *task = malloc(sizeof(Task));
    if (task == NULL)
    {
        return NULL;
    }
    memset(task, 0, sizeof(Task));
    task->data = data;
    pthread_mutex_lock(&queue->mutex);

    // Queue is full, producer must wait for the consumers to consume and signal that the queue is not_full
    while (queue->has_capacity_limits && queue->tasks_count >= queue->queue_capacity)
        pthread_cond_wait(&queue->not_full, &queue->mutex);

    // Produce
    DL_APPEND(queue->tasks, task);
    queue->tasks_count += 1;
    // Signal the queue is not empty for workers
    pthread_cond_broadcast(&queue->not_empty);
    pthread_mutex_unlock(&queue->mutex);
    return task;
}

/*
To calculate the number of active workers
-Use pthread self to get the thread id
-Push it into an array of active workers, if it doesn't exist
-In the empty condition, delete it from the array
    -Swap with the last element in the array
    -Decrease the size of the array
*/
void *dequeue_task(Queue *queue)
{
    Task *dequeued;
    void *data;
    pthread_mutex_lock(&queue->mutex);

    // Queue is empty, workers must wait for the queue to be not empty
    while (queue->tasks_count <= 0)
    {
        // Decrease the number of workers
        pthread_cond_wait(&queue->not_empty, &queue->mutex);
    }
    // Initialize values
    dequeued = queue->tasks;
    // Return value
    data = dequeued->data;
    if (queue->tasks_count <= 0)
    {
        pthread_mutex_unlock(&queue->mutex);
        return NULL;
    }
    // Increase the number of workers
    //  Delete from the list
    DL_DELETE(queue->tasks, dequeued);
    // Cleanup
    queue->tasks_count -= 1;
    free(dequeued);

    // Signal that the queue is not full so the producer can resume
    pthread_cond_signal(&queue->not_full);
    pthread_mutex_unlock(&queue->mutex);
    return data;
}
size_t get_queue_tasks_count(Queue *queue)
{
    size_t tasks_count;
    pthread_mutex_lock(&queue->mutex);
    tasks_count = queue->tasks_count;
    pthread_mutex_unlock(&queue->mutex);
    return tasks_count;
}
int destroy_queue(Queue *queue)
{
    if (queue == NULL)
        return -1;
    pthread_mutex_destroy(&queue->mutex);
    pthread_cond_destroy(&queue->not_empty);
    pthread_cond_destroy(&queue->not_full);

    // Empty the list
    Task *tmp, *element;
    DL_FOREACH_SAFE(queue->tasks, element, tmp)
    {
        DL_DELETE(queue->tasks, element);
        free(element);
    }
    free(queue);
    return 0;
}