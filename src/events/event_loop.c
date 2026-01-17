#include "events/event_loop.h"
void event_loop_run(ConnectionsManager *connections_manager)
{
    int ready_events;
    while (1)
    {
        ready_events = epoll_wait(connections_manager->epoll_fd, connections_manager->events, MAX_EVENTS, 3000);
        printf("Ready events:%d", ready_events);
        for (int i = 0; i < ready_events; i++)
        {
            // Segmentation fault in HASH_FIND_INT
            //  Connection *connection;
            //  HASH_FIND_INT(connections_manager->connections, &connections_manager->events[i].data.fd, connection);
            printf("Reading file descriptor '%d' -- ", connections_manager->events[i].data.fd);
            // connection->handler(connections_manager);
        }
    }
}