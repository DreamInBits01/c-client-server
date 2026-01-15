#ifndef MANAGER_H
#define MANAGER_H
#include <stdio.h>
#include <unistd.h>
#include "structs.h"
int init_multiplexer(ConnectionsManager *connections_manager);
int register_socket(int epoll_fd, int socket_fd, __uint32_t events);
int unregister_socket(int epoll_fd, int socket_fd);
#endif