#ifndef DAEMON_H
#define DAEMON_H
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include "ds/queue.h"
typedef struct PeriodicTask
{
    char name[64];
    void (*handler)(void *ctx);
    void *handler_context;
    time_t last_run;
    time_t interval_seconds;
    bool enabled;
    struct PeriodicTask *next; /* needed for singly- or doubly-linked lists */
} PeriodicTask;
typedef struct Daemon
{
    PeriodicTask *tasks;
    time_t last_tick;
} Daemon;

Daemon *initialize_daemon();
void mock_handler(PeriodicTask *task);
PeriodicTask *daemon_register_task(Daemon *daemon, char *name, void (*handler)(PeriodicTask *ctx), void *handler_context, int interval_seconds, bool enabled);
int daemon_tick(Daemon *daemon);
int destroy_daemon(Daemon *daemon);
#endif