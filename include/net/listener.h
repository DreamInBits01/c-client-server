#ifndef LISTENER_H
#define LISTENER_H
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <poll.h>
#include "structs.h"
#include "utils.h"
#define BACKLOG 20

int tcp_listner_bind(char *host, char *port);
TCPClient *tcp_listner_accept(int listening_socket);
#endif