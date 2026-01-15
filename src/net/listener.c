#include "listener.h"
int tcp_listner_bind(char *host, char *port)
{
    struct addrinfo hints, *response;
    memset(&hints, 0, sizeof(hints));
    // Set hints
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // Get address information
    int getaddrinfo_status = getaddrinfo("127.0.0.1", port, &hints, &response);
    if (getaddrinfo_status != 0)
    {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(getaddrinfo_status));
        return -1;
    }
    struct addrinfo *current_response = response;
    int bind_status = -1;
    int socket_fd;
    // Loop to get a valid response
    while (current_response != NULL)
    {
        socket_fd = socket(current_response->ai_family, current_response->ai_socktype, current_response->ai_protocol);
        if (socket_fd == -1)
            continue;
        if (bind(socket_fd, current_response->ai_addr, current_response->ai_addrlen) == 0)
        {
            bind_status = 0;
        };
        current_response = current_response->ai_next;
    }
    // Cleanup
    freeaddrinfo(response);
    if (bind_status == -1)
    {
        fprintf(stderr, "[tcp_listner_bind] Bind failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (listen(socket_fd, BACKLOG) == -1)
    {
        fprintf(stderr, "[tcp_listner_bind] Listen failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    };
    return socket_fd;
}
TCPClient *tcp_listner_accept(int listening_socket)
{
    TCPClient *tcp_client = malloc(sizeof(TCPClient));
    int client_fd = accept(listening_socket, (struct sockaddr *)&tcp_client->client_information, &tcp_client->client_information_len);
    if (client_fd == -1)
    {
        fprintf(stderr, "[tcp_listner_accept] error: %s\n", strerror(errno));
        return NULL;
    }
    tcp_client->socket_fd = client_fd;
    return tcp_client;
}
