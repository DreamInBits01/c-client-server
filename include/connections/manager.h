#ifndef MANAGER_H
#define MANAGER_H
#include <stdio.h>
#include <unistd.h>
#include "structs.h"
#include "multiplexer.h"
#include <uthash/src/uthash.h>
int register_connection(ConnectionsManager *connections_manager, Connection *connection);
int dereigster_connection(ConnectionsManager *connections_manager, Connection *connection);
int cleanup_connection(Connection *connection);
int cleanup_connections_manager(ConnectionsManager *connections_manager);
#endif