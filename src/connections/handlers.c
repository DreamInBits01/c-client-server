#include "connections/handlers.h"
void client_socket_handler(Connection *connection)
{
    /*
        -Client handler should submit the connection to the thread pool (provided by connections manager)
        -Submission function shuld worry about rotating the threads an distributing the load
    */
    int status = 0;
    ConnectionHandlerContext *handler_context = connection->handler_context;
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
    status = submit_to_threadpool(handler_context->threadpool, connection);
    if (status == -1)
    {
        fprintf(stderr, "[client_socket_handler]: Error while queuing the request\n");
        return;
    }
    printf("[client_socket_handler]: Connection was queued successfully\n");
}
void listening_socket_handler(Connection *connection)
{
    // Make sure connection & connections_manager are provided
    if (connection == NULL)
    {
        fprintf(stderr, "[listening_socket_handler] need connection\n");
        return;
    }
    ConnectionHandlerContext *handler_context = connection->handler_context;
    if (handler_context == NULL)
    {
        fprintf(stderr, "[listening_socket_handler] need handler context\n");
        return;
    }
    // Start the accepting process
    int status;
    // Accept new socket
    TCPClient *tcp_client = tcp_listener_accept(handler_context->connections_manager->listening_socket);
    if (tcp_client == NULL)
    {
        fprintf(stderr, "[listening_socket_handler] register_connection failed\n");
        return;
    }
    // Create a new connection for that socket
    Connection *new_connection = initialize_connection(tcp_client->socket_fd, tcp_client, client_socket_handler, handler_context);
    if (new_connection == NULL)
    {
        free(tcp_client);
        fprintf(stderr, "[listening_socket_handler] register_connection failed\n");
        return;
    }
    // Register socket
    status = register_connection(handler_context->connections_manager, new_connection);
    if (status == -1)
    {
        free(new_connection);
    }
    return;
}
