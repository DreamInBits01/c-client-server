#include "connections/manager.h"
ConnectionsManager *initialize_connections_manager()
{
    int status;
    // Create a listening socket
    int listening_socket = tcp_listener_bind("", "5050");
    if (listening_socket == -1)
    {
        fprintf(stderr, "[initialize_connections_manager] tcp_listener_bind failed\n");
        return NULL;
    }
    // Create a connections manager
    ConnectionsManager *connections_manager = malloc(sizeof(ConnectionsManager));
    if (connections_manager == NULL)
    {
        fprintf(stderr, "[initialize_connections_manager] malloc failed\n");
        return NULL;
    }
    memset(connections_manager, 0, sizeof(ConnectionsManager));
    connections_manager->listening_socket = listening_socket;
    // Create the multiplexer
    int epoll_fd = initialize_multiplexer(connections_manager);
    if (epoll_fd == -1)
    {
        free(connections_manager);
        fprintf(stderr, "[initialize_connections_manager] initialize_multiplexer failed\n");
        return NULL;
    }
    // Assign multiplexer
    connections_manager->epoll_fd = epoll_fd;
    // // Create threadpool
    // ThreadPool *threadpool = initialize_threadpool();
    // if (threadpool == NULL)
    // {
    //     destroy_multiplexer(connections_manager->epoll_fd);
    //     free(connections_manager);
    //     fprintf(stderr, "[initialize_connections_manager] initialize_threadpool failed\n");
    //     return NULL;
    // }
    // Assign threadpool
    // connections_manager->threadpool = threadpool;
    return connections_manager;
}
Connection *initialize_connection(int socket_fd, TCPClient *tcp_client, void (*handler)(Connection *), ConnectionHandlerContext *handler_context)
{
    // Create a connection
    if (socket_fd <= handler_context->connections_manager->listening_socket)
        return NULL;
    Connection *connection = malloc(sizeof(Connection));
    if (connection == NULL)
    {
        fprintf(stderr, "[initialize_connection] malloc failed\n");
        return NULL;
    }
    memset(connection, 0, sizeof(Connection));
    // SET VALUES
    connection->tcp_client = tcp_client;
    connection->socket_fd = socket_fd;
    connection->handler_context = handler_context;
    connection->handler = handler;
    connection->request.data[0] = '\0';
    connection->response.data[0] = '\0';
    gettimeofday(&connection->last_connection_time, NULL);
    return connection;
}
int register_connection(ConnectionsManager *connections_manager, Connection *connection)
{
    if (connections_manager == NULL || connection == NULL)
        return -1;
    int status;
    // Register socket to the connections' manager multiplexer
    status = register_socket(connections_manager->epoll_fd, connection->socket_fd, EPOLLIN);
    if (status == -1)
    {
        fprintf(stderr, "[register_connection] register_socket failed\n");
        return -1;
    }
    // Add connection to the connections' list
    HASH_ADD_INT(connections_manager->connections, socket_fd, connection);
    return 0;
}
int deregister_connection(ConnectionsManager *connections_manager, Connection *connection)
{
    int status;
    // Deregister socket from the connections manager multiplexer
    HASH_DEL(connections_manager->connections, connection);
    status = deregister_socket(connections_manager->epoll_fd, connection->socket_fd);
    if (status == -1)
    {
        fprintf(stderr, "[deregister_connection] deregister_socket failed\n");
        return -1;
    }
    // Cleanup the connection
    status = destroy_connection(connection);
    if (status == -1)
    {
        fprintf(stderr, "[deregister_connection] destroy_connection failed\n");
        return -1;
    }
    return 0;
}
int destroy_connection(Connection *connection)
{
    if (connection == NULL)
        return -1;
    int status;
    // Close socket
    status = close(connection->socket_fd);
    if (status == -1)
        return -1;
    // Free tcp client
    if (connection->tcp_client)
    {
        free(connection->tcp_client);
    }
    // Free connection
    free(connection);
    return 0;
}
int destroy_connections_manager(ConnectionsManager *connections_manager)
{
    // Close listening socket
    close(connections_manager->listening_socket);
    // Destroy the multiplexer
    destroy_multiplexer(connections_manager->epoll_fd);
    Connection *current_connection, *tmp;
    // Delete All cnonnections
    HASH_ITER(hh, connections_manager->connections, current_connection, tmp)
    {
        HASH_DEL(connections_manager->connections, current_connection); /* delete; advances to next */
        destroy_connection(current_connection);                         /* free connection  */
    }
    // Free connections manager
    free(connections_manager);
    return 0;
}