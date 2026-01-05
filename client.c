#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#define RESPONSE_SIZE 256
#define PORT 4002

/*


1-Create a socket
2-Create the address to connect to
3-Connect to the desired address
4-Recieve data
5-close the connection



*/
int main(int ac, char **argv)
{
    if (ac != 2)
    {
        printf("Please Provide the correct number of args\n");
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
    server_address.sin_port = htons(PORT);
    //
    server_address.sin_addr.s_addr = INADDR_ANY;

    int connection_status = connect(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address));
    if (connection_status < 0)
    {
        perror("Error connection failure\n");
        exit(EXIT_FAILURE);
    }
    int message_length = strlen(argv[1]);
    int bytes_sent = send(socket_fd, argv[1], message_length, 0);
    if (bytes_sent == -1)
    {
        printf("Error while sending message\n");
    }
    else if (bytes_sent == message_length)
    {
        printf("Message was sent completely\n");
    }
    else
    {
        printf("Message was sent partially (%d bytes)\n", bytes_sent);
    }
    int bytes_read = 1;
    char server_response[RESPONSE_SIZE];
    // Wait for a response via while
    while (bytes_read >= 0)
    {
        bytes_read = recv(socket_fd, server_response, RESPONSE_SIZE, 0);
        if (bytes_read == 0)
        {
            printf("Server closed connection\n");
            break;
        }
        else if (bytes_read == -1)
        {
            printf("Error while receiving bytes\n");
            break;
        }
        else
        {
            // Message got
            server_response[bytes_read] = '\0';
            printf("Received data:%s\n", server_response);
            break;
        }
    }
    printf("Closing socket\n");
    close(socket_fd);
    return 0;
}