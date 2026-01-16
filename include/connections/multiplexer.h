#ifndef MULTIPLEXER_H
#define MULTIPLEXER_H
#include <stdio.h>
#include <unistd.h>
#include "structs.h"
/**
 * @brief Initializes the epoll multiplexer for the connections manager
 *
 * Creates an epoll instance using epoll_create1 and stores the file descriptor
 * in the connections manager. The epoll instance is used to monitor multiple
 * socket file descriptors for I/O events efficiently.
 *
 * @param connections_manager Pointer to the ConnectionsManager to initialize
 * @return 0 on success, -1 on failure
 *
 * @note Must be called before registering any sockets
 * @note The epoll_fd should be cleaned up with destroy_multiplexer
 */
int init_multiplexer(ConnectionsManager *connections_manager);

/**
 * @brief Registers a socket with the epoll instance for event monitoring
 *
 * Adds a socket file descriptor to the epoll interest list with the specified
 * events (e.g., EPOLLIN for read events, EPOLLOUT for write events).
 *
 * @param epoll_fd File descriptor of the epoll instance
 * @param socket_fd Socket file descriptor to register
 * @param events Bitmask of epoll events to monitor (EPOLLIN, EPOLLOUT, etc.)
 * @return 0 on success, -1 on failure
 *
 * @note Common events: EPOLLIN (readable), EPOLLOUT (writable), EPOLLERR (error)
 * @note Closes epoll_fd on failure (potential bug - see notes)
 */
int register_socket(int epoll_fd, int socket_fd, __uint32_t events);

/**
 * @brief Deregisters a socket from the epoll instance
 *
 * Removes a socket file descriptor from the epoll interest list.
 * Should be called before closing a socket to prevent epoll from
 * monitoring an invalid file descriptor.
 *
 * @param epoll_fd File descriptor of the epoll instance
 * @param socket_fd Socket file descriptor to deregister
 * @return 0 on success, -1 on failure
 *
 * @note Closes epoll_fd on failure (potential bug - see notes)
 */
int deregister_socket(int epoll_fd, int socket_fd);

/**
 * @brief Destroys the epoll multiplexer instance
 *
 * Closes the epoll file descriptor, releasing associated kernel resources.
 * Should be called during shutdown after all sockets have been deregistered.
 *
 * @param epoll_fd File descriptor of the epoll instance to destroy
 * @return 0 on success, -1 on failure
 */
int destroy_multiplexer(int epoll_fd);
#endif