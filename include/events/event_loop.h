#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H
#include "net/io.h"
#include "connections/manager.h"
#include "daemons/daemon.h"
void event_loop_run(ConnectionsManager *connections_manager, Daemon *daemon);
#endif