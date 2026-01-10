#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#define REQUEST_SIZE 1024
#define RESPONSE_SIZE 1024
#define PORT 4002
/*
Some computer architectures use big endian or little endain, however networks are configured to use big endian.
Bytes sent from the host to the network must be converted to big endian first, then should be transmitted over
Bytes received from the network must be converted back to the machine's byte ordering


When two processes wish to communicate,
the sending socket (client) must know the server's ip and the open socket on that machine,
thus a number is needed to asign to that socket.


uint32_t htonl(uint32_t hostlong);  //"Host to network long"
uint16_t htons(uint16_t hostshort); //"Host to network short"
uint32_t ntohl(uint32_t netlong);   //"Network to host long"
uint16_t ntohs(uint16_t netshort);  //"Network to host short"
*/
typedef struct sockaddr SocketAddress;
void check(int value, char *error_message)
{
    if (value < 0)
    {
        printf("%s", error_message);
        exit(EXIT_FAILURE);
    }
}
int create_server_socket(int port)
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    check(socket_fd, "Error while opening a socket\n");
    struct sockaddr_in server_address;
    // Zero out structs
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_port = htons(PORT);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    // Bind the socket to an address
    check(
        bind(socket_fd, (SocketAddress *)&server_address, sizeof(server_address)),
        "Bind faild\n");
    check(
        listen(socket_fd, 1),
        "Listen failed\n");
    printf("Listening on port 3000\n");
    return socket_fd;
}
int handle_client(int client_fd)
{
    char response[RESPONSE_SIZE] = "HTTP/1.1 200 OK\r\n\n";

    char request[REQUEST_SIZE];
    memset(request, 0, sizeof(request));
    int bytes_received = recv(client_fd, request, REQUEST_SIZE, 0);
    if (bytes_received == 0)
    {
        printf("Client closed connection\n");
        exit(EXIT_FAILURE);
    }
    else if (bytes_received == -1)
    {
        printf("Error while recieving data\n");
        exit(EXIT_FAILURE);
    };
    request[bytes_received] = '\0';
    printf("Request body:%s\n", request);
    send(client_fd, response, RESPONSE_SIZE, 0);
    close(client_fd);
}
int accept_client(int server_fd)
{
}
int main()
{
    printf("--- Server ---\n");
    int socket_fd = create_server_socket(PORT);
    struct sockaddr_storage client_address;
    socklen_t client_address_size;
    memset(&client_address, 0, sizeof(client_address));

    while (1)
    {
        int client_fd = accept(socket_fd, (SocketAddress *)&client_address, &client_address_size);
        handle_client(client_fd);
    }
    check(
        close(socket_fd), "Error while closing server\n");
    printf("Server closed\n");
    close(socket_fd);
    return EXIT_SUCCESS;
}