#include "connections/manager.h"
int register_connection(ConnectionsManager *connections_manager, Connection *connection)
{
    int status;
    if (connections_manager == NULL || connection == NULL)
        return -1;

    HASH_ADD_INT(connections_manager->connections, socket_fd, connection);
    status = register_socket(connections_manager->epoll_fd, connection->socket_fd, EPOLLIN);
    if (status == -1)
    {
        fprintf(stderr, "[register_connection] register_socket failed");
        return -1;
    }
    return 1;
}
int deregister_connection(ConnectionsManager *connections_manager, Connection *connection)
{
    HASH_DEL(connections_manager->connections, connection);
    deregister_socket(connections_manager->epoll_fd, connection->socket_fd);
    cleanup_connection(connection);
    return 1;
}
int cleanup_connection(Connection *connection)
{
    int status;
    status = close(connection->socket_fd);
    if (status == -1)
        return -1;
    free(connection->tcp_client);
    return 1;
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
        HASH_DEL(connections_manager->connections, current_connection); /* delete; users advances to next */
        cleanup_connection(current_connection);                         /* free connection  */
    }
    free(connections_manager);
    return 1;
}