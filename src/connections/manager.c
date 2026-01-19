#include "connections/manager.h"
int initialize_connections_manager(ConnectionsManager *connections_manager)
{
    int status;
    int listening_socket = tcp_listener_bind("", "5050");
    connections_manager->listening_socket = listening_socket;
    status = init_multiplexer(connections_manager);
    if (status == -1)
    {
        fprintf(stderr, "[initialize_connections_manager] init_multiplexer failed");
        exit(EXIT_FAILURE);
    }
    return 0;
}
int register_connection(ConnectionsManager *connections_manager, Connection *connection, int socket_fd, void (*handler)(void *ctx))
{
    int status;
    if (connections_manager == NULL || connection == NULL)
        return -1;
    // Set socket values
    connection->socket_fd = socket_fd;
    connection->handler = handler;
    HASH_ADD_INT(connections_manager->connections, socket_fd, connection);
    status = register_socket(connections_manager->epoll_fd, socket_fd, EPOLLIN);
    if (status == -1)
    {
        fprintf(stderr, "[register_connection] register_socket failed");
        return -1;
    }
    return 0;
}
int deregister_connection(ConnectionsManager *connections_manager, Connection *connection)
{
    int status;
    HASH_DEL(connections_manager->connections, connection);
    status = deregister_socket(connections_manager->epoll_fd, connection->socket_fd);
    if (status == -1)
    {
        fprintf(stderr, "[deregister_connection] deregister_socket failed");
        return -1;
    }
    status = cleanup_connection(connection);
    if (status == -1)
    {
        fprintf(stderr, "[deregister_connection] cleanup_connection failed");
        return -1;
    }
    return 0;
}
int cleanup_connection(Connection *connection)
{
    int status;
    status = close(connection->socket_fd);
    if (status == -1)
        return -1;
    free(connection->tcp_client);
    return 0;
}
int cleanup_connections_manager(ConnectionsManager *connections_manager)
{
    // Closing the listening socket should be in the http cleanup
    destroy_multiplexer(connections_manager->epoll_fd);
    // connections_manager.
    Connection *current_connection, *tmp;
    // Delete All cnonnections
    HASH_ITER(hh, connections_manager->connections, current_connection, tmp)
    {
        HASH_DEL(connections_manager->connections, current_connection); /* delete; advances to next */
        cleanup_connection(current_connection);                         /* free connection  */
    }
    free(connections_manager);
    return 1;
}