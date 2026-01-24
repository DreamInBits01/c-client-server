#include "net/utils.h"
int make_socket_nonblocking(int socket_fd)
{
    int status;
    status = fcntl(socket_fd, F_SETFL, O_NONBLOCK);
    if (status == -1)
        return -1;
    return 0;
}
int is_request_completed(Connection *connection)
{
    // Just check for end of headers
    // This works for requests that has no body
    return strstr(connection->request.data, "\r\n\r\n") != NULL ? 0 : -1;
}