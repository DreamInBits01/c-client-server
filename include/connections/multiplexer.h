#ifndef MULTIPLEXER_H
#define MULTIPLEXER_H
#include <stdio.h>
#include <unistd.h>
#include "structs.h"
int init_multiplexer(ConnectionsManager *connections_manager);
int register_socket(int epoll_fd, int socket_fd, __uint32_t events);
int deregister_socket(int epoll_fd, int socket_fd);
int destroy_multiplexer(int epoll_fd);
#endif