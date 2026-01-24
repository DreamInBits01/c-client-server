#include "net/listener.h"

int tcp_listener_bind(char *host, char *port)
{
    // To check the status at each procedure
    printf("Host:%s\n", host);
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
        fprintf(stderr, "[tcp_listener_bind] Bind failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (listen(socket_fd, BACKLOG) == -1)
    {
        fprintf(stderr, "[tcp_listener_bind] Listen failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    };
    // Make socket non-blocking
    status = make_socket_nonblocking(socket_fd);
    if (status == -1)
    {
        fprintf(stderr, "[tcp_listener_bind] make_socket_nonblocking failed");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    return socket_fd;
}
TCPClient *tcp_listener_accept(int listening_socket)
{
    TCPClient *tcp_client = malloc(sizeof(TCPClient));
    memset(tcp_client, 0, sizeof(TCPClient));
    int socket_fd = accept(listening_socket, (struct sockaddr *)&tcp_client->client_information, &tcp_client->client_information_len);
    if (socket_fd == -1)
    {
        fprintf(stderr, "[tcp_listener_accept] error: %s\n", strerror(errno));
        return NULL;
    }
    // Make socket non-blocking
    int status = make_socket_nonblocking(socket_fd);
    if (status == -1)
    {
        fprintf(stderr, "[tcp_listener_accept] make_socket_nonblocking failed");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    tcp_client->socket_fd = socket_fd;
    return tcp_client;
}
/*
client_socket_handle Should be implemented else where and imorted here
*/
void client_socket_handle(Connection *connection)
{
    printf("Handling client %d...\n", connection->socket_fd);
    io_receive(connection);
    int total_btes_sent = 0;
    int bytes_sent = send(connection->socket_fd, connection->request.data, sizeof(connection->request.data), 0);
    if (bytes_sent == -1)
    {
        printf("Error while sending message\n");
    }
    else if (bytes_sent == sizeof(connection->request.data))
    {
        printf("Message was sent completely\n");
    }
    else
    {
        printf("Message was sent partially (%d bytes)\n", bytes_sent);
    }
    close(connection->socket_fd);
}
void listening_socket_handler(Connection *connection)
{
    if (connection == NULL)
    {
        fprintf(stderr, "[listening_socket_handler] need connection\n");
    }
    ConnectionsManager *connections_manager = (ConnectionsManager *)connection->handler_context;
    if (connections_manager == NULL)
    {
        fprintf(stderr, "[listening_socket_handler] need handler context\n");
    }
    int status;
    TCPClient *tcp_client = tcp_listener_accept(connections_manager->listening_socket);
    if (tcp_client == NULL)
    {
        fprintf(stderr, "[listening_socket_handler] register_connection failed\n");
        return;
    }
    /*
        -Should attach a handler to the clients sockets
        -The handler should distrbute the workload to the workers
        -Workers should parse each request and handle the client
    */
    Connection *new_connection = initialize_connection(tcp_client->socket_fd, tcp_client, client_socket_handle, connections_manager);
    if (new_connection == NULL)
    {
        free(tcp_client);
        fprintf(stderr, "[listening_socket_handler] register_connection failed\n");
        return;
    }
    status = register_connection(connections_manager, new_connection);
    if (status == -1)
    {
        free(new_connection);
    }
    return;
}
