#include <stdio.h>
#include "events/event_loop.h"
#include "connections/manager.h"
#include "net/listener.h"
int main()
{
    printf("Hello world\n");
    ConnectionsManager connections_manager = {0};

    // Initialize connection manager
    int listening_socket = tcp_listener_bind("", "5050");
    connections_manager.listening_socket = listening_socket;
    init_multiplexer(&connections_manager);

    // Register a listening socket
    Connection connection = {0};
    register_connection(
        &connections_manager,
        &connection,
        listening_socket,
        listening_socket_handler);

    // Run event loop
    event_loop_run(&connections_manager);
    return 0;
}