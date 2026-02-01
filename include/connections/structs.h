#ifndef CONNECTIONS_STRUCTS_H
#define CONNECTIONS_STRUCTS_H
#include <sys/time.h>
#include <sys/epoll.h>
#include <uthash/src/uthash.h>
// #include "workers/structs.h"
#include "net/structs.h"
#define MAX_CONNECTIONS 4096
#define MAX_EVENTS 512
#define REQUEST_BUFFER_SIZE 8192   // 8 KB
#define RESPONSE_BUFFER_SIZE 16384 // 16 KB
typedef struct ThreadPool ThreadPool;
typedef enum
{
    CONN_STATE_RECEIVING,
    CONN_STATE_PROCESSING,
    CONN_STATE_WRITING,
    CONN_STATE_CLOSED
} ConnectionState;
typedef struct
{
    size_t bytes_prepared; // Total response prepared
    size_t bytes_sent;     // Total bytes sent
    char data[RESPONSE_BUFFER_SIZE];
} ResponseBuffer;
typedef struct
{
    size_t bytes_received;
    time_t request_start_time;
    char data[REQUEST_BUFFER_SIZE];
} RequestBuffer;
/*
    Connections will store tcp_client information alongsode the event of that client to be monitored.
    After a period of time of their last connection,
    the client will be removed from the monitored events achieve presestience connection
*/
typedef struct Connection
{
    struct epoll_event event;
    TCPClient *tcp_client;
    int socket_fd;
    struct timeval last_connection_time;
    ConnectionState state;

    RequestBuffer request;
    ResponseBuffer response;

    // Handler and its context
    void (*handler)(struct Connection *);
    void *handler_context;
    UT_hash_handle hh; /* makes this structure hashable */
} Connection;
/*
    The connection manager will be responsible to manage clients of the provided listening_socket.
    Events will be used inside the event_loop to be monitored.
*/
typedef struct
{
    Connection *connections;
    ThreadPool *threadpool;
    struct epoll_event events[MAX_EVENTS];
    int epoll_fd;
    int listening_socket;
} ConnectionsManager;
#endif