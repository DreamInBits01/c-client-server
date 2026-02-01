#ifndef DAEMON_H
#define DAEMON_H
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "ds/queue.h"
typedef struct PeriodicTask
{
    char name[64];
    void (*handler)(void *ctx);
    void *ctx;
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
PeriodicTask *daemon_register_task();
int daemon_tick();
int destroy_daemon();
#endif