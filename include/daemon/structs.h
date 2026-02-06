#ifndef DAEMON_STRUCTS_H
#define DAEMON_STRUCTS_H
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
typedef struct PeriodicTask
{
    char name[64];
    void (*handler)();
    void *handler_context;
    time_t last_run;
    int interval_seconds;
    bool enabled;
    struct PeriodicTask *next; /* needed for singly- or doubly-linked lists */
} PeriodicTask;
typedef struct Daemon
{
    PeriodicTask *tasks;
    time_t last_tick;
} Daemon;
#endif