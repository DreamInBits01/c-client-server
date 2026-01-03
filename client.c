#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
/*


1-Create a socket
2-Create the address to connect to
3-Connect to the desired address
4-Recieve data
5-close the connection



*/
int main()
{
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
    server_address.sin_port = htons(3000);
    //
    server_address.sin_addr.s_addr = INADDR_ANY;

    int connection_status = connect(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address));
    if (connection_status < 0)
    {
        perror("Error connection failure\n");
        exit(EXIT_FAILURE);
    }
    char server_response[256];
    recv(socket_fd, server_response, sizeof(server_response), 0);
    printf("Received data:%s\n", server_response);
    close(socket_fd);
    return 0;
}