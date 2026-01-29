#include "workers/worker.h"
void *mock_routine(void *args)
{
    ThreadPool *threadpool = (ThreadPool *)args;
    while (threadpool->shutdown != 1)
    {
        Connection *connection = dequeue_request(threadpool->queue);
        if (connection == NULL)
            continue;
        printf("Handle connection:%d\n", connection->socket_fd);
        // Should process the http request
    }
}