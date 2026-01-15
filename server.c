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
#define REQUEST_BUFFER 1024
#define RESPONSE_BUFFER 1024
typedef struct
{
    struct pollfd client;
    struct timeval last_request_time;
} Connection;
// int accept_new_client(int listening_socket, struct sockaddr_storage *client_information, socklen_t *client_information_length);
bool handle_client(int client_fd);
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
                close(poll_fds[i].fd);
            }
        }
    };
    close(listening_socket);
    return EXIT_SUCCESS;
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
