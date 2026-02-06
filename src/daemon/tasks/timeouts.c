#include "daemon/tasks/timeouts.h"
void timeouts_handler(PeriodicTask *task)
{
    // Loop over connections
    // Check their last request_time if it's larger than 10s
    // Check their state, if it's in the receving then the connection must be close
    Connection *current_connection, *tmp;
    ConnectionsManager *connections_manager = (ConnectionsManager *)task->handler_context;
    time_t now;
    time(&now);
    HASH_ITER(hh, connections_manager->connections, current_connection, tmp)
    {

        time_t time_diff = difftime(now, current_connection->request.request_start_time);
        if (current_connection->socket_fd != connections_manager->listening_socket && time_diff >= 10 && current_connection->state == CONN_STATE_RECEIVING)
        {
            printf("Deregistering connection:%d...\n", current_connection->socket_fd);
            deregister_connection(connections_manager, current_connection);
        }
    }
}