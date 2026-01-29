#ifndef CONNECTION_HANDLERS
#define CONNECTION_HANDLERS
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "structs.h"
#include "net/io.h"
#include "net/listener.h"
#include "net/structs.h"
#include "workers/threadpool.h"
/**
 * @brief Handler for incoming connections on the listening socket
 *
 * Accepts a new client connection, creates and initializes a Connection structure,
 * records the connection timestamp, and registers it with the connections manager.
 * This function is typically called when the listening socket has pending connections.
 *
 * @param ctx Context pointer, expected to be a ConnectionsManager instance
 * @return 0 on success, -1 on failure (accept error or registration failure)
 *
 * @note Allocates memory for Connection structure; freed during connection cleanup
 * @note Sets connection's last_connection_time to current time
 */
void listening_socket_handler(Connection *connection);
void client_socket_handler(Connection *connection);
#endif