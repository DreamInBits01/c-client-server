#include "connections/handlers.h"
void client_socket_handler(Connection *connection)
{
    /*
        -Client handler should submit the connection to the thread pool (provided by connections manager)
        -Submission function shuld worry about rotating the threads an distributing the load
    */
    int status = 0;
    ConnectionsManager *connections_manager = (ConnectionsManager *)connection->handler_context;
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
    submit_to_threadpool(connections_manager->threadpool, connection);
    // Echo request to the client
    // status = io_send(connection);
    // if (status == -1)
    // {
    //     printf("Error while sending data\n");
    //     deregister_connection(connection->handler_context, connection);
    // }
    // else
    // {
    //     printf("Data was sent completely\n");
    // }
    // deregister_connection(connection->handler_context, connection);
}
void listening_socket_handler(Connection *connection)
{
    // Make sure connection & connections_manager are provided
    if (connection == NULL)
    {
        fprintf(stderr, "[listening_socket_handler] need connection\n");
        return;
    }
    ConnectionsManager *connections_manager = (ConnectionsManager *)connection->handler_context;
    if (connections_manager == NULL)
    {
        fprintf(stderr, "[listening_socket_handler] need handler context\n");
        return;
    }
    // Start the accepting process
    int status;
    // Accept new socket
    TCPClient *tcp_client = tcp_listener_accept(connections_manager->listening_socket);
    if (tcp_client == NULL)
    {
        fprintf(stderr, "[listening_socket_handler] register_connection failed\n");
        return;
    }
    // Create a new connection for that socket
    Connection *new_connection = initialize_connection(tcp_client->socket_fd, tcp_client, client_socket_handler, connections_manager);
    if (new_connection == NULL)
    {
        free(tcp_client);
        fprintf(stderr, "[listening_socket_handler] register_connection failed\n");
        return;
    }
    // Register socket
    status = register_connection(connections_manager, new_connection);
    if (status == -1)
    {
        free(new_connection);
    }
    return;
}
