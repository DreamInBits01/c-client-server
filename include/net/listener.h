#ifndef LISTENER_H
#define LISTENER_H
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
#include "structs.h"
#include "utils.h"
#include "connections/manager.h"
#define BACKLOG 20
/**
 * @brief Creates, binds, and configures a TCP listening socket
 *
 * Creates a non-blocking TCP socket, binds it to the specified host and port,
 * and sets it to listen for incoming connections. Uses getaddrinfo to resolve
 * the address and iterates through available addresses until successful binding.
 *
 * @param host Host address to bind to (currently unused; hardcoded to "127.0.0.1")
 * @param port Port number to bind to (as string)
 * @return Socket file descriptor on success, -1 on failure
 *
 * @note Function exits on bind, listen, or non-blocking configuration failures
 * @note BACKLOG constant determines the maximum queue length for pending connections
 */
int tcp_listener_bind(char *host, char *port);

/**
 * @brief Accepts a new TCP client connection
 *
 * Accepts an incoming connection on the listening socket, creates a TCPClient
 * structure to store client information, and configures the client socket as
 * non-blocking.
 *
 * @param listening_socket File descriptor of the listening socket
 * @return Pointer to newly allocated TCPClient structure on success, NULL on failure
 *
 * @note Caller is responsible for freeing the returned TCPClient structure
 * @note Function exits if non-blocking configuration fails
 * @note Returns NULL if accept fails (e.g., no pending connections)
 */
TCPClient *tcp_listener_accept(int listening_socket);

/**
 * @brief Handler for incoming connections on the listening socket
 *
 * Accepts a new client connection, creates and initializes a Connection structure,
 * records the connection timestamp, and registers it with the connections manager.
 * This function is typically called when the listening socket has pending connections.
 *
 * @param ctx Context pointer, expected to be a ConnectionsManager instance
 * @return 1 on success, -1 on failure (accept error or registration failure)
 *
 * @note Allocates memory for Connection structure; freed during connection cleanup
 * @note Sets connection's last_connection_time to current time
 */
int listening_socket_handler(void *ctx);
#endif