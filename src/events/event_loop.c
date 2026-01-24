#include "events/event_loop.h"
void event_loop_run(ConnectionsManager *connections_manager)
{
    printf("Event loop is running...\n");
    // time_t last_timeout_check = time(NULL);
    int ready_events;
    while (1)
    {
        ready_events = epoll_wait(connections_manager->epoll_fd, connections_manager->events, MAX_EVENTS, 3000);
        for (int i = 0; i < ready_events; i++)
        {
            printf("Ready Socket:%d\n", connections_manager->events[i].data.fd);
            // Segmentation fault in HASH_FIND_INT
            Connection *connection = NULL;
            HASH_FIND_INT(
                connections_manager->connections,
                &connections_manager->events[i].data.fd,
                connection);
            if (connection == NULL)
            {
                continue;
            }
            connection->handler(connection);
        }
    }
}