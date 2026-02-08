#ifndef DAEMON_CHECK_HEALTH_TASK_H
#define DAEMON_CHECK_HEALTH_TASK_H
#include "daemon/structs.h"
#include "events/structs.h"
#include "ds/queue.h"
void check_health_handler(PeriodicTask *task);
#endif