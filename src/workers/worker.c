#include "workers/worker.h"
void *mock_routine(void *args)
{
    ThreadPool *threadpool = (ThreadPool *)args;
    while (threadpool->shutdown != 1)
    {
        int status;
        Connection *connection = (Connection *)dequeue_task(threadpool->queue);
        connection->state = CONN_STATE_PROCESSING;
        if (connection == NULL)
            continue;
        printf("Worker is handling connection:%d\n", connection->socket_fd);
        status = io_receive(connection);
        if (status == -1)
        {
            printf("Error while receiving data\n");
        }
        else
        {
            printf("Data was received completely\n");
        }
        printf("Request:%s, (%ld bytes)\n", connection->request.data, connection->request.bytes_received);
        // Copying data from request to response
        memcpy(connection->response.data, connection->request.data, sizeof(connection->request.data));
        connection->response.bytes_prepared = strlen(connection->response.data);
        printf("Prepared response:%s, (%ld bytes)\n", connection->response.data, connection->response.bytes_prepared);
        // Should process the http request
        // Echo request to the client
        status = io_send(connection);
        if (status == -1)
        {
            printf("Error while sending data\n");
        }
        else
        {
            printf("Data was sent completely\n");
        }
        status = deregister_connection(connection->handler_context->connections_manager, connection);
        if (status == -1)
        {
            fprintf(stderr, "[mock_routine] error while deregistering connection:%d\n", connection->socket_fd);
        }
    }
    return NULL;
}