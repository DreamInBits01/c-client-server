#ifndef DAEMON_H
#define DAEMON_H
#include <stdbool.h>
#include <time.h>
typedef struct PeriodTask
{
    char name[64];
    void (*handler)(void *ctx);
    void *ctx;
    time_t last_run;
    time_t interval_seconds;
    bool enabled;
    struct PeriodTask *prev; /* needed for a doubly-linked list only */
    struct PeriodTask *next; /* needed for singly- or doubly-linked lists */
} PeriodTask;
typedef struct Daemon
{
    PeriodTask *tasks;
    time_t last_tick;
} Daemon;

void initialize_daemon();
void daemon_register_task();
void daemon_tick();
void destroy_daemon();
#endif