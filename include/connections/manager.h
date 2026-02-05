#ifndef MANAGER_H
#define MANAGER_H
#include <stdio.h>
#include <unistd.h>
#include "structs.h"
#include "multiplexer.h"
#include "net/listener.h"
#include <uthash/src/uthash.h>
#include "workers/threadpool.h"
/**
 * Initializes a new ConnectionsManager instance.
 *
 * Creates a TCP listening socket on port 5050, allocates the manager structure,
 * and initializes the event multiplexer (epoll). Exits on failure.
 *
 * @return Pointer to the newly created ConnectionsManager
 */
ConnectionsManager *initialize_connections_manager();

/**
 * Initializes a new Connection object.
 *
 * Allocates and zeroes a Connection structure, setting the socket descriptor,
 * TCP client, handler function, and current timestamp. Exits on malloc failure.
 *
 * @param socket_fd File descriptor of the connection socket
 * @param tcp_client Pointer to the TCPClient associated with this connection
 * @param handler Callback function to handle events for this connection
 * @return Pointer to the newly created Connection
 */
Connection *initialize_connection(int socket_fd, TCPClient *tcp_client, void (*handler)(Connection *), ConnectionHandlerContext *handler_context);
/**
 * Registers a connection with the connections manager.
 *
 * Adds the connection to the manager's hash table and registers its socket
 * with epoll for incoming events (EPOLLIN).
 *
 * @param connections_manager Pointer to the ConnectionsManager instance
 * @param connection Pointer to the Connection to register
 * @return 0 on success, -1 on failure
 */
int register_connection(ConnectionsManager *connections_manager, Connection *connection);

/**
 * Deregisters and cleans up a connection.
 *
 * Removes the connection from the manager's hash table, deregisters its socket
 * from epoll, and performs cleanup (closes socket, frees resources).
 *
 * @param connections_manager Pointer to the ConnectionsManager instance
 * @param connection Pointer to the Connection to deregister
 * @return 0 on success, -1 on failure
 */
int deregister_connection(ConnectionsManager *connections_manager, Connection *connection);

/**
 * Cleans up a connection's resources.
 *
 * Closes the connection's socket and frees the associated TCP client.
 *
 * @param connection Pointer to the Connection to clean up
 * @return 0 on success, -1 on failure
 */
int destroy_connection(Connection *connection);

/**
 * Cleans up the connections manager and all its connections.
 *
 * Destroys the epoll multiplexer, iterates through all registered connections
 * to clean them up, and frees the manager structure.
 *
 * @param connections_manager Pointer to the ConnectionsManager to clean up
 * @return 1 on success
 */
int destroy_connections_manager(ConnectionsManager *connections_manager);
#endif