#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H
#include "connections/manager.h"
void event_loop_run(ConnectionsManager *connections_manager);
#endif