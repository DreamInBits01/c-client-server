#include <stdio.h>
#include "events/event_loop.h"
#include "connections/manager.h"
#include "net/listener.h"
int main()
{
    printf("Hello world\n");

    ConnectionsManager connections_manager;
    Connection connection;
    memset(&connections_manager, 0, sizeof(connections_manager));
    memset(&connection, 0, sizeof(connection));
    // Initialize connection manager
    int listening_socket = tcp_listener_bind("", "5050");
    connections_manager.listening_socket = listening_socket;
    init_multiplexer(&connections_manager);
    // Register a connection
    connection.socket_fd = listening_socket;
    register_connection(&connections_manager, &connection);
    event_loop_run(&connections_manager);
    return 0;
}