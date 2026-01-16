#include "net/listener.h"

int tcp_listner_bind(char *host, char *port)
{
    // To check the status at each procedure
    printf("Host:%s", host);
    int status;
    struct addrinfo hints, *response;
    memset(&hints, 0, sizeof(hints));
    // Set hints
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // Get address information
    status = getaddrinfo("127.0.0.1", port, &hints, &response);
    if (status != 0)
    {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return -1;
    }
    struct addrinfo *current_response = response;
    int socket_fd;
    // Loop to get a valid response
    while (current_response != NULL)
    {
        socket_fd = socket(current_response->ai_family, current_response->ai_socktype, current_response->ai_protocol);
        if (socket_fd == -1)
            continue;
        if (bind(socket_fd, current_response->ai_addr, current_response->ai_addrlen) == 0)
        {
            status = 0;
        };
        current_response = current_response->ai_next;
    }
    // Cleanup
    freeaddrinfo(response);
    if (status == -1)
    {
        fprintf(stderr, "[tcp_listner_bind] Bind failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (listen(socket_fd, BACKLOG) == -1)
    {
        fprintf(stderr, "[tcp_listner_bind] Listen failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    };
    // Make socket non-blocking
    status = make_socket_nonblocking(socket_fd);
    if (status == -1)
    {
        fprintf(stderr, "[tcp_listner_bind] make_socket_nonblocking failed");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    return socket_fd;
}
TCPClient *tcp_listner_accept(int listening_socket)
{
    TCPClient *tcp_client = malloc(sizeof(TCPClient));
    memset(tcp_client, 0, sizeof(TCPClient));
    int socket_fd = accept(listening_socket, (struct sockaddr *)&tcp_client->client_information, &tcp_client->client_information_len);
    if (socket_fd == -1)
    {
        fprintf(stderr, "[tcp_listner_accept] error: %s\n", strerror(errno));
        return NULL;
    }
    int status = make_socket_nonblocking(socket_fd);
    if (status == -1)
    {
        fprintf(stderr, "[tcp_listner_accept] make_socket_nonblocking failed");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    tcp_client->socket_fd = socket_fd;
    return tcp_client;
}
int listening_socket_handler(void *ctx)
{
    int status;
    ConnectionsManager *connections_manager = (ConnectionsManager *)ctx;
    TCPClient *tcp_client = tcp_listner_accept(connections_manager->listening_socket);
    if (tcp_client == NULL)
        return -1;
    Connection *connection = malloc(sizeof(Connection));
    memset(connection, 0, sizeof(Connection));
    connection->tcp_client = tcp_client;
    connection->socket_fd = tcp_client->socket_fd;
    gettimeofday(&connection->last_connection_time, NULL);
    status = register_connection(connections_manager, connection);
    if (status == -1)
    {
        free(tcp_client);
        fprintf(stderr, "[listening_socket_handler] register_connection failed\n");
        return -1;
    }
    return 1;
}
