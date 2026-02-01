#include "workers/worker.h"
void *mock_routine(void *args)
{
    ThreadPool *threadpool = (ThreadPool *)args;
    while (threadpool->shutdown != 1)
    {
        int status;
        Connection *connection = (Connection *)dequeue_task(threadpool->queue);
        if (connection == NULL)
            continue;
        printf("Worker is handling connection:%d\n", connection->socket_fd);
        // Should process the http request
        // Echo request to the client
        status = io_send(connection);
        if (status == -1)
        {
            printf("Error while sending data\n");
            deregister_connection(connection->handler_context, connection);
        }
        else
        {
            printf("Data was sent completely\n");
        }
        deregister_connection(connection->handler_context, connection);
    }
    return NULL;
}