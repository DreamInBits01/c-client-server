#include "connections/handlers.h"
void client_socket_handler(Connection *connection)
{
    /*
        -Client handler should submit the connection to the thread pool (provided by connections manager)
        -Submission function shuld worry about rotating the threads an distributing the load
    */
    int status = 0;
    printf("Handling client %d...\n", connection->socket_fd);
    status = io_receive(connection);
    if (status == -1)
    {
        printf("Error while receiving data\n");
    }
    else
    {
        printf("Data was received completely\n");
    }
    // Copying data from request to response
    memcpy(connection->response.data, connection->request.data, sizeof(connection->request.data));
    connection->response.bytes_prepared = strlen(connection->response.data);
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
void listening_socket_handler(Connection *connection)
{
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
    int status;
    TCPClient *tcp_client = tcp_listener_accept(connections_manager->listening_socket);
    if (tcp_client == NULL)
    {
        fprintf(stderr, "[listening_socket_handler] register_connection failed\n");
        return;
    }
    Connection *new_connection = initialize_connection(tcp_client->socket_fd, tcp_client, client_socket_handler, connections_manager);
    if (new_connection == NULL)
    {
        free(tcp_client);
        fprintf(stderr, "[listening_socket_handler] register_connection failed\n");
        return;
    }
    status = register_connection(connections_manager, new_connection);
    if (status == -1)
    {
        free(new_connection);
    }
    return;
}
