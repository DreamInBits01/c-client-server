#ifndef NET_STRUCTS_H
#define NET_STRUCTS_H
#include <sys/socket.h>
typedef struct
{
    struct sockaddr_storage client_information;
    socklen_t client_information_len;
    int socket_fd;
} TCPClient;
#endif