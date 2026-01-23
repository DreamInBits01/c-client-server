#include <stdio.h>
#include "events/event_loop.h"
#include "connections/manager.h"
#include "net/listener.h"
int main()
{
    // Initialize a connections manager
    ConnectionsManager *connections_manager = initialize_connections_manager();
    if (connections_manager == NULL)
    {
        fprintf(stderr, "[main]: initialize_connections_manager failed");
        return EXIT_FAILURE;
    }
    // Initialize a conneection
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
    // Run event loop with the connection's manager
    event_loop_run(connections_manager);
    return 0;
}