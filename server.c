#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
int main()
{
    // Create the socket (stream of bytes exposed over the network)
    char server_response[256] = "You have connected to the server\r\n";
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        perror("Error while opening socket\n");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in socket_address;
    socket_address.sin_family = AF_INET;
    socket_address.sin_port = htons(3000);
    socket_address.sin_addr.s_addr = INADDR_ANY;
    // We bind the socket to an address instead of connecting to it
    bind(socket_fd, (struct sockaddr *)&socket_address, sizeof(socket_address));

    // Listen to that address
    listen(socket_fd, 1);

    // Accept clients
    while (1)
    {
        struct sockaddr client_address;
        int client_fd = accept(socket_fd, &client_address, NULL);
        send(client_fd, server_response, sizeof(server_response), 0);
    }
    close(socket_fd);
    return 0;
}