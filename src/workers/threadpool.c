#include "workers/threadpool.h"

ThreadPool *initialize_threadpool()
{
    // Allocate a thread pool
    ThreadPool *thread_pool = malloc(sizeof(ThreadPool));
    if (thread_pool == NULL)
    {
        return NULL;
    };
    memset(thread_pool, 0, sizeof(ThreadPool));
    // Initialize the values
    thread_pool->number_of_workers = sysconf(_SC_NPROCESSORS_ONLN);
    thread_pool->threads = malloc(sizeof(pthread_t) * thread_pool->number_of_workers);
    if (thread_pool->threads == NULL)
    {
        free(thread_pool);
        return NULL;
    }

    thread_pool->queue = initialize_queue(thread_pool->number_of_workers);
    if (thread_pool->queue == NULL)
    {
        free(thread_pool->threads);
        free(thread_pool);
        return NULL;
    }
    // Create workers
    for (size_t i = 0; i < thread_pool->number_of_workers; i++)
    {
        pthread_create(&thread_pool->threads[i], NULL, mock_routine, thread_pool);
    };
    return thread_pool;
};
int destroy_threadpool(ThreadPool *threadpool)
{
    if (threadpool == NULL)
    {
        return -1;
    }
    // Signal shutdown
    threadpool->shutdown = 1;
    // Wake all threads up
    pthread_mutex_lock(&threadpool->queue->mutex);
    // Wake all threads up
    pthread_cond_broadcast(&threadpool->queue->not_empty);
    pthread_mutex_unlock(&threadpool->queue->mutex);
    // Wait for all workers to finish
    for (size_t i = 0; i < threadpool->number_of_workers; i++)
    {
        pthread_join(threadpool->threads[i], NULL);
    }
    // Destroy queue after all the workers have stopped
    destroy_queue(threadpool->queue);
    free(threadpool->threads);
    free(threadpool);
    return 0;
};
int submit_to_threadpool(ThreadPool *threadpool, Connection *connection)
{
    Request *request = queue_request(threadpool->queue, connection);
    if (request == NULL)
        return -1;
    return 0;
};