#ifndef CONNECTIONS_STRUCTS_H
#define CONNECTIONS_STRUCTS_H
#include "net/structs.h"
#include <sys/time.h>
#include "sys/epoll.h"
#define MAX_CONNECTIONS 4096
#define MAX_EVENTS 512
/*
    Connections will store tcp_client information alongsode the event of that client to be monitored.
    After a period of time of their last connection,
    the client will be removed from the monitored events achieve presestience connection
*/
typedef struct
{
    struct epoll_event event;
    TCPClient *tcp_client;
    struct timeval last_connection_time;
    void(*handler);
} Connection;
/*
    The connection manager will be responsible to manage clients of the provided listening_socket.
    Events will be used inside the event_loop to be monitored.
*/
typedef struct
{
    Connection *connections;
    struct epoll_event events[MAX_EVENTS];
    int epoll_fd;
    int listening_socket;
} ConnectionsManager;
#endif