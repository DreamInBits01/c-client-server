#ifndef DAEMON_H
#define DAEMON_H
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <uthash/src/utlist.h>
#include "structs.h"
#include "tasks/timeouts.h"
Daemon *initialize_daemon();
PeriodicTask *daemon_register_task(Daemon *daemon, char *name, void (*handler)(PeriodicTask *ctx), void *handler_context, int interval_seconds, bool enabled);
int daemon_tick(Daemon *daemon);
int destroy_daemon(Daemon *daemon);
#endif