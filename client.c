#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#define RESPONSE_SIZE 1024
#define RESEND_ATTEMTS 4
/*


1-Create a socket
2-Create the address to connect to
3-Connect to the desired address
4-Recieve data
5-close the connection



*/
int is_response_completed(char *buffer)
{
    // Just check for end of headers
    // This works for requests that has no body
    return strstr(buffer, "\r\n\r\n") != NULL ? 0 : -1;
}
int main(int ac, char **argv)
{
    if (ac != 3)
    {
        printf("usage: ./client <PORT> <MESSAGE>\n");
        exit(EXIT_FAILURE);
    }
    printf("--- CLIENT ---\n");
    // Create the socket (stream of bytes exposed over the network)
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        perror("Error socket failure\n");
        exit(1);
    }
    // Put the information of the server to connect to
    struct sockaddr_in server_address;
    // AF_INET = TCP server
    server_address.sin_family = AF_INET;
    // Port to connect to
    server_address.sin_port = htons(atoi(argv[1]));
    //
    server_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    int connection_status = connect(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address));
    if (connection_status == -1)
    {
        perror("Error connection failure\n");
        exit(EXIT_FAILURE);
    }
    int message_length = strlen(argv[2]);
    int total_bytes_sent = 0;
    while (1)
    {
        int bytes_sent = send(socket_fd, argv[2] + total_bytes_sent, message_length - total_bytes_sent, 0);
        total_bytes_sent += bytes_sent;
        if (bytes_sent == -1)
        {
            printf("Error while sending message\n");
            break;
        }
        if (total_bytes_sent == message_length)
        {
            printf("Message was sent completely (%d bytes)\n", total_bytes_sent);
            break;
        }
        // Retry
        printf("Message was sent partially (%d bytes)\n", bytes_sent);
    }

    int bytes_received = 1;
    int total_bytes_received = 0;
    char server_response[RESPONSE_SIZE];
    // Wait for a response via while
    while (1)
    {
        bytes_received = recv(socket_fd, server_response + total_bytes_received, RESPONSE_SIZE - total_bytes_received, 0);
        total_bytes_received += bytes_received;
        if (bytes_received == 0)
        {
            printf("Server closed connection\n");
            break;
        }
        if (bytes_received == -1)
        {
            printf("Error while receiving bytes\n");
            break;
        }
        if (total_bytes_received >= sizeof(server_response))
        {
            printf("Response buffer is full\n");
            break;
        }
        if (is_response_completed(server_response) == 0)
        {
            printf("Received data:%s\n", server_response);
            server_response[total_bytes_received] = '\0';
            break;
        }
        else
        {
            printf("Received data partially:%d\n", total_bytes_received);
        }
    }
    printf("Received data:%s\n", server_response);
    printf("Closing socket\n");
    close(socket_fd);
    return 0;
}