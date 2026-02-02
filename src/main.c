#include <stdio.h>
#include "events/event_loop.h"
#include "connections/manager.h"
#include "connections/handlers.h"
#include "daemons/daemon.h"
int main()
{
    // Initialize a connections manager
    ConnectionsManager *connections_manager = initialize_connections_manager();
    if (connections_manager == NULL)
    {
        fprintf(stderr, "[main]: initialize_connections_manager failed");
        return EXIT_FAILURE;
    }
    // Initialize a connection
    Connection *connection = initialize_connection(
        connections_manager->listening_socket,
        NULL,
        listening_socket_handler,
        connections_manager);

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
    daemon_register_task(daemon, "Mock task", mock_handler, connections_manager, 5, true);
    // Run event loop with the connection's manager
    event_loop_run(connections_manager, daemon);

    // Cleanup
    destroy_daemon(daemon);
    destroy_connections_manager(connections_manager);
    return 0;
}