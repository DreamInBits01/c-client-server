#include "connections/handlers.h"
void client_socket_handler(Connection *connection)
{
    /*
        -Client handler should submit the connection to the thread pool (provided by connections manager)
        -Submission function shuld worry about rotating the threads an distributing the load
    */
    printf("Handling client %d...\n", connection->socket_fd);
    io_receive(connection);
    size_t total_bytes_sent = 0;
    while (1)
    {
        int bytes_sent = send(connection->socket_fd, connection->request.data + total_bytes_sent, strlen(connection->request.data) - total_bytes_sent, 0);
        if (bytes_sent == -1)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                // No more data available right now (non-blocking socket)
                break;
            }
            printf("Error while sending message\n");
        }
        total_bytes_sent += bytes_sent;
        if (total_bytes_sent == connection->request.bytes_received)
        {
            printf("Message was sent completely\n");
            break;
        }
        else
        {
            printf("Message was sent partially (%d bytes)\n", bytes_sent);
        }
    }
    close(connection->socket_fd);
}
void listening_socket_handler(Connection *connection)
{
    if (connection == NULL)
    {
        fprintf(stderr, "[listening_socket_handler] need connection\n");
    }
    ConnectionsManager *connections_manager = (ConnectionsManager *)connection->handler_context;
    if (connections_manager == NULL)
    {
        fprintf(stderr, "[listening_socket_handler] need handler context\n");
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
