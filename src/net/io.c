#include "net/io.h"
int io_receive(Connection *connection)
{
    connection->request.bytes_received = 0;
    while (1)
    {
        int bytes_received = recv(
            connection->socket_fd,
            connection->request.data + connection->request.bytes_received,
            sizeof(connection->request.data) - connection->request.bytes_received,
            0);
        if (bytes_received == -1)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                // No more data available right now (non-blocking socket)
                break;
            }
            fprintf(stderr, "[io_receive] error while recveiving from connection:%d: %s\n",
                    connection->socket_fd,
                    strerror(errno));
            memset(connection->request.data, 0, sizeof(connection->request.data));
            return -1;
        };
        if (bytes_received == 0)
        {
            fprintf(stderr, "[io_receive] client close connection:%d\n", connection->socket_fd);
            memset(connection->request.data, 0, sizeof(connection->request.data));
            return -1; // Client closed connection
        };
        connection->request.bytes_received += bytes_received;
        if (connection->request.bytes_received >= sizeof(connection->request.data))
        {
            fprintf(stderr, "[io_receive] buffer full, request too large\n");
            return -1; // Request too large
        }
        // Should check if the request is complete (http utility);
        if (is_request_completed(connection))
        {
            return 0;
        }
    }
    return 0;
}
int io_send(Connection *connection)
{
    return 0;
}