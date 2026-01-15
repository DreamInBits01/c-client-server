#include "multiplexer.h"
bool init_multiplexer(ConnectionsManager *connections_manager)
{
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1)
    {
        fprintf(stderr, "[init_multiplexer] failed\n");
        return false;
    }
    connections_manager->epoll_fd = epoll_fd;
    return true;
}
bool register_socket(int epoll_fd, int socket_fd, __uint32_t events)
{
    struct epoll_event event;
    event.data.fd = socket_fd;
    event.events = events;
    {
        fprintf(stderr, "[register_socket] failed\n");
        close(epoll_fd);
        return false;
    }
    return true;
}
bool unregister_socket(int epoll_fd, int socket_fd)
{
    if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, socket_fd, NULL) == -1)
    {
        fprintf(stderr, "[unregister_socket] failed\n");
        close(epoll_fd);
        return false;
    }
    return true;
}
bool destroy_multiplexer(ConnectionsManager *connections_manager)
{
    int status;
    status = close(connections_manager->epoll_fd);
    if (status == -1)
    {
        fprintf(stderr, "[destroy_multiplexer] failed\n");
        return false;
    }
    connections_manager->epoll_fd = -1;
    return true;
}
