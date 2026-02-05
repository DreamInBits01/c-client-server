#ifndef EVENT_LOOP_STRUCTS_H
#define EVENT_LOOP_STRUCTS_H
#include "connections/manager.h"
#include "workers/threadpool.h"
#include "daemon/index.h"
typedef struct EventLoopContext
{
    ConnectionsManager *connections_manager;
    ThreadPool *threadpool;
    Daemon *daemon;
} EventLoopContext;
#endif