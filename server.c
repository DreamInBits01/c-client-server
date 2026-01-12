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
#define POLL_CAPACITY 5
#define BACKLOG 5
#define REQUEST_BUFFER 1024
#define RESPONSE_BUFFER 1024
int create_listening_socket(char *port);
// int accept_new_client(int listening_socket, struct sockaddr_storage *client_information, socklen_t *client_information_length);
int accept_new_client(int listening_socket);
bool handle_client(int client_fd);
bool add_to_poll(int fd, struct pollfd **poll_fds, int *poll_index, int *poll_size);
bool delete_from_poll();

int main(int argc, char **argv)
{
    /*
        -Initialize the poll of sockets
        -Append server socket
        -Monitor POLLIN sockets
    */
    if (argc != 2)
    {
        fprintf(stderr, "usage: ./server <PORT>\n");
        exit(EXIT_FAILURE);
    }
    int listening_socket = create_listening_socket(argv[1]);
    int poll_size = POLL_CAPACITY;
    int poll_index = 0;
    struct pollfd *poll_fds = calloc(POLL_CAPACITY + 1, sizeof(struct pollfd));
    if (poll_fds == NULL)
    {
        fprintf(stderr, "Error while allocating poll_fds\n");
        exit(EXIT_FAILURE);
    }
    // Add listening socket
    add_to_poll(listening_socket, &poll_fds, &poll_index, &poll_size);
    while (1)
    {
        // Poll sockets
        int status = poll(poll_fds, poll_index, 2000);
        if (status == -1)
        {
            // ERROR
            fprintf(stderr, "[Server] Poll error: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        else if (status == 0)
        {
            // NO SOCKET IS READY
            printf("[Server] Waiting...\n");
            continue;
        }
        for (size_t i = 0; i < poll_index; i++)
        {
            if (!(poll_fds[i].revents & POLLIN))
            {
                continue;
            }
            printf("Ready for I/O operation...\n");
            if (poll_fds[i].fd == listening_socket)
            {
                // New client to accept
                int client_fd = accept_new_client(listening_socket);
                add_to_poll(client_fd, &poll_fds, &poll_index, &poll_size);
            }
            else
            {
                // Data to read from clients
                handle_client(poll_fds[i].fd);
                // close(poll_fds[i].fd);
            }
        }
        // struct sockaddr_storage client_information;
        // socklen_t client_information_length;
        // int client_fd = accept_new_client(listening_socket);
    };
    close(listening_socket);
    return EXIT_SUCCESS;
}
int create_listening_socket(char *port)
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
        fprintf(stderr, "Bind failed!\n");
        exit(EXIT_FAILURE);
    }
    if (listen(socket_fd, BACKLOG) == -1)
    {
        fprintf(stderr, "Listen failed!\n");
        exit(EXIT_FAILURE);
    };
    printf("Listening on port:%s\n", port);
    return socket_fd;
}
bool handle_client(int client_fd)
{
    char request_buffer[REQUEST_BUFFER];
    memset(request_buffer, 0, REQUEST_BUFFER);
    int bytes_received = recv(client_fd, request_buffer, REQUEST_BUFFER - 1, 0);
    if (bytes_received == 0)
    {
        printf("Client close connection\n");
        return false;
    }
    else if (bytes_received <= -1)
    {
        printf("Error while receiving data\n");
        return false;
    }
    // Success
    request_buffer[bytes_received] = '\0';
    printf("Server got:%s, %d bytes\n", request_buffer, bytes_received);
    char *response = "msg received from poll server";
    send(client_fd, response, strlen(response), 0);
    close(client_fd);
    return true;
}
int accept_new_client(int listening_socket)
{
    /*
        Must be added to the poll of fds!
    */
    // memset(client_information, 0, sizeof(client_information));
    int client_fd = accept(listening_socket, NULL, NULL);
    return client_fd;
}
bool add_to_poll(int fd, struct pollfd **poll_fds, int *poll_index, int *poll_size)
{
    if (*poll_index == *poll_size)
    {
        *poll_size *= 2;
        *poll_fds = realloc(*poll_fds, *poll_size * sizeof(struct pollfd));
        printf("Reallocating...\n");
        if (*poll_fds == NULL)
        {
            fprintf(stderr, "Error while reallocating poll_fds\n");
            exit(EXIT_FAILURE);
        }
    }
    (*poll_fds)[*poll_index].fd = fd;
    (*poll_fds)[*poll_index].events = POLLIN;
    (*poll_index)++;
}