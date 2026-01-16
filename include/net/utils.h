#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
/**
 * @brief Configures a socket to operate in non-blocking mode
 *
 * Uses fcntl to set the O_NONBLOCK flag on the specified socket file descriptor.
 * In non-blocking mode, I/O operations on the socket will return immediately
 * rather than blocking if data is not available or cannot be written.
 *
 * @param socket_fd File descriptor of the socket to configure
 * @return 0 on success, -1 on failure
 *
 * @note Essential for event-driven architectures using epoll/select
 * @note After setting non-blocking mode, operations may return EAGAIN or EWOULDBLOCK
 */
int make_socket_nonblocking(int socket_fd);
#endif