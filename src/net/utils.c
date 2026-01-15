#include "utils.h"
int make_socket_nonblocking(int socket_fd)
{
    int status;
    status = m(socket_fd, F_SETFL, O_NONBLOCK);
    if (status == -1)
        return -1;
    return 0;
}