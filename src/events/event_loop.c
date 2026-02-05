#include "events/event_loop.h"

void event_loop_run(EventLoopContext *ctx)
{
    printf("Event loop is running...\n");
    int ready_events;
    while (1)
    {
        ready_events = epoll_wait(ctx->connections_manager->epoll_fd, ctx->connections_manager->events, MAX_EVENTS, 3000);
        for (int i = 0; i < ready_events; i++)
        {
            printf("Ready Socket:%d\n", ctx->connections_manager->events[i].data.fd);
            // Segmentation fault in HASH_FIND_INT
            Connection *connection = NULL;
            HASH_FIND_INT(
                ctx->connections_manager->connections,
                &ctx->connections_manager->events[i].data.fd,
                connection);
            if (connection == NULL)
            {
                continue;
            }
            connection->handler(connection);
        }
        // Should run on a separate thread
        daemon_tick(ctx->daemon);
    }
}