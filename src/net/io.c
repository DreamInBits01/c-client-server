#include "net/io.h"
int io_receive(Connection *connection)
{
    connection->request.bytes_received = 0;
    connection->state = CONN_STATE_RECEIVING;
    time(&connection->request.request_start_time);
    while (1)
    {
        int bytes_received = recv(
            connection->socket_fd,
            connection->request.data + connection->request.bytes_received,
            sizeof(connection->request.data) - connection->request.bytes_received,
            0);
        // ERROR FROM RECV
        if (bytes_received == -1)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                // No more data available right now (non-blocking socket)
                continue;
                // return 0;
            }
            fprintf(stderr, "[io_receive] error while recveiving from connection:%d: %s\n",
                    connection->socket_fd,
                    strerror(errno));
            memset(connection->request.data, 0, sizeof(connection->request.data));
            return -1;
        };
        // Client closed connection
        if (bytes_received == 0)
        {
            fprintf(stderr, "[io_receive] client close connection:%d\n", connection->socket_fd);
            memset(connection->request.data, 0, sizeof(connection->request.data));
            return -1; // Client closed connection
        };
        connection->request.bytes_received += bytes_received;
        // Client overflowed the request's buffer
        if (connection->request.bytes_received >= sizeof(connection->request.data))
        {
            fprintf(stderr, "[io_receive] buffer full, request too large\n");
            return -1; // Request too large
        }
        // Should check if the request is complete (http utility);
        if (is_request_completed(connection) == 0)
        {
            printf("[io_receive]: message was received completely\n");
            return 0;
        }
    }
    return 0;
}
int io_send(Connection *connection)
{
    int total_bytes_sent = 0;
    while (1)
    {
        int bytes_sent = send(connection->socket_fd, connection->response.data + total_bytes_sent, strlen(connection->response.data) - total_bytes_sent, 0);
        total_bytes_sent += bytes_sent;
        if (bytes_sent == -1)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                // No more data available right now (non-blocking socket)
                return 0;
            }
            fprintf(stderr, "[io_send] error while recveiving from connection:%d: %s\n",
                    connection->socket_fd,
                    strerror(errno));
            // Error while sending message
            return -1;
        }
        if (bytes_sent == 0)
        {
            fprintf(stderr, "[io_send] client closed connection:%d\n", connection->socket_fd);
            memset(connection->response.data, 0, sizeof(connection->response.data));
            return -1; // Client closed connection
        };
        if (total_bytes_sent == connection->response.bytes_prepared)
        {
            // Message was sent completely
            return 0;
        }
    }
}