#ifndef MANAGER_H
#define MANAGER_H
#include <stdio.h>
#include <stdbool.h>
#include "structs.h"
bool init_multiplexer(ConnectionsManager *connections_manager);
bool register_socket(int epoll_fd, int socket_fd, __uint32_t events);
bool unregister_socket(int epoll_fd, int socket_fd);
#endif