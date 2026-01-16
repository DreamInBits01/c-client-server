#include "connections/multiplexer.h"
int init_multiplexer(ConnectionsManager *connections_manager)
{
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1)
    {
        fprintf(stderr, "[init_multiplexer] failed\n");
        return -1;
    }
    connections_manager->epoll_fd = epoll_fd;
    return 0;
}
int register_socket(int epoll_fd, int socket_fd, __uint32_t events)
{
    struct epoll_event event;
    event.data.fd = socket_fd;
    event.events = events;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &event) == -1)
    {
        fprintf(stderr, "[register_socket] failed\n");
        return -1;
    }
    return 0;
}
int deregister_socket(int epoll_fd, int socket_fd)
{
    if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, socket_fd, NULL) == -1)
    {
        fprintf(stderr, "[deregister_socket] failed\n");
        close(epoll_fd);
        return -1;
    }
    return 0;
}
int destroy_multiplexer(int epoll_fd)
{
    int status;
    status = close(epoll_fd);
    if (status == -1)
    {
        fprintf(stderr, "[destroy_multiplexer] failed\n");
        return -1;
    }
    return 0;
}
