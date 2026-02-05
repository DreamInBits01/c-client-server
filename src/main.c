#include <stdio.h>
#include "events/event_loop.h"
#include "connections/manager.h"
#include "connections/handlers.h"
#include "daemon/index.h"
int main()
{
    // Initialize a connections manager
    ConnectionsManager *connections_manager = initialize_connections_manager();
    if (connections_manager == NULL)
    {
        fprintf(stderr, "[main]: initialize_connections_manager failed");
        return EXIT_FAILURE;
    }
    // Threadpool
    ThreadPool *threadpool = initialize_threadpool();
    if (threadpool == NULL)
    {
        destroy_multiplexer(connections_manager->epoll_fd);
        free(connections_manager);
        fprintf(stderr, "[initialize_connections_manager] initialize_threadpool failed\n");
        return EXIT_FAILURE;
    }
    // Initialize a connection
    ConnectionHandlerContext connection_handler_context = {0};
    connection_handler_context.connections_manager = connections_manager;
    connection_handler_context.threadpool = threadpool;
    Connection *connection = initialize_connection(
        connections_manager->listening_socket,
        NULL,
        listening_socket_handler,
        &connection_handler_context);

    if (connection == NULL)
    {
        fprintf(stderr, "[main]: initialize_connection failed");
        return EXIT_FAILURE;
    }
    // Register a connection for the listening socket
    if (register_connection(connections_manager, connection) == -1)
    {
        fprintf(stderr, "[main]: register_connection failed");
        return EXIT_FAILURE;
    }
    // Daemon
    Daemon *daemon = initialize_daemon();
    daemon_register_task(daemon, "Check health", mock_handler, connections_manager, 5, true);
    daemon_register_task(daemon, "Timeout handling", mock_handler, connections_manager, 10, true);

    // Event loop context
    EventLoopContext ctx = {0};
    ctx.connections_manager = connections_manager;
    ctx.daemon = daemon;
    ctx.threadpool = threadpool;
    // Run event loop with the context
    event_loop_run(&ctx);

    // Cleanup
    destroy_daemon(daemon);
    destroy_connections_manager(connections_manager);
    destroy_threadpool(threadpool);
    return 0;
}