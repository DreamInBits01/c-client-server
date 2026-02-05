#ifndef DAEMON_TIMEOUTS_TASK_H
#define DAEMON_TIMEOUTS_TASK_H
#include "daemon/structs.h"
#include "connections/manager.h"
void timeouts_handler(PeriodicTask *task);
#endif