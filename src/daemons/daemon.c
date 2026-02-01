#include "daemons/daemon.h"
Daemon *initialize_daemon()
{
    Daemon *daemon = malloc(sizeof(Daemon));
    if (daemon == NULL)
    {
        return NULL;
    };
    memset(daemon, 0, sizeof(daemon));
    daemon->last_tick = NULL;
    daemon->tasks = NULL;
    return daemon;
}
PeriodicTask *daemon_register_task(Daemon *daemon, char *name, void (*handler)(void *ctx), void *ctx, int interval_seconds, bool enabled)
{
    PeriodicTask *periodic_task = malloc(sizeof(PeriodicTask));
    if (periodic_task == NULL)
    {
        return NULL;
    };
    // Set values
    periodic_task->enabled = enabled;
    strncpy(periodic_task->name, name, sizeof(periodic_task->name));
    periodic_task->last_run = NULL;
    periodic_task->handler = handler;
    periodic_task->ctx = ctx;
    periodic_task->interval_seconds = interval_seconds;
    // Append task
    LL_APPEND(daemon->tasks, periodic_task);
    return periodic_task;
}
int daemon_tick()
{
}
int destroy_daemon()
{
}