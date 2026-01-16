#ifndef MANAGER_H
#define MANAGER_H
#include <stdio.h>
#include <unistd.h>
#include "structs.h"
#include "multiplexer.h"
#include <uthash/src/uthash.h>
/**
 * @brief Registers a new connection with the connections manager
 *
 * Adds the connection to the manager's hash table and registers its socket
 * with the epoll instance for monitoring incoming data (EPOLLIN events).
 *
 * @param connections_manager Pointer to the ConnectionsManager instance
 * @param connection Pointer to the Connection to register
 * @return 1 on success, -1 on failure (null parameters or socket registration error)
 */
int register_connection(ConnectionsManager *connections_manager, Connection *connection);

/**
 * @brief Deregisters and cleans up a connection
 *
 * Removes the connection from the manager's hash table, deregisters its socket
 * from epoll, closes the socket, and frees associated resources.
 *
 * @param connections_manager Pointer to the ConnectionsManager containing the connection
 * @param connection Pointer to the Connection to deregister
 * @return 1 on success
 */
int deregister_connection(ConnectionsManager *connections_manager, Connection *connection);

/**
 * @brief Cleans up resources associated with a single connection
 *
 * Closes the connection's socket file descriptor and frees the tcp_client structure.
 * Does not free the Connection structure itself.
 *
 * @param connection Pointer to the Connection to clean up
 * @return 1 on success, -1 if closing the socket fails
 */
int cleanup_connection(Connection *connection);

/**
 * @brief Destroys the connections manager and all registered connections
 *
 * Performs complete cleanup by destroying the epoll multiplexer, iterating through
 * all connections to clean them up, and freeing the ConnectionsManager structure.
 * Should be called during application shutdown.
 *
 * @param connections_manager Pointer to the ConnectionsManager to destroy
 * @return 1 on success
 */
int cleanup_connections_manager(ConnectionsManager *connections_manager);
#endif