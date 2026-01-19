#include "events/event_loop.h"
void event_loop_run(ConnectionsManager *connections_manager)
{
    printf("Event loop is running...\n");
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
                continue;
            /*
                NOTE:For testing purposes I'll check the socket type;
                NOTE:Should only dispatched the connection's handler
            */
            if (connection->socket_fd == connections_manager->listening_socket)
            {
                // Listening socket handler will add connected sockets
                printf("Listening socket\n");
                connection->handler(connections_manager);
            }
            else
            {
                printf("Client socket\n");
                // A connected client
                int total_bytes_sent = 0;
                char *mock_message = "Message\n";

                int bytes_sent = send(connection->socket_fd, mock_message, sizeof(mock_message), 0);
                total_bytes_sent += bytes_sent;
                if (total_bytes_sent == -1)
                {
                    printf("Error while sending message\n");
                }
                else if (total_bytes_sent == sizeof(mock_message))
                {
                    printf("Message was sent completely\n");
                }
                else
                {
                    printf("Message was sent partially (%d bytes)\n", total_bytes_sent);
                }
                close(connection->socket_fd);
            }
        }
    }
}