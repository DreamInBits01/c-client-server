#include "daemon/index.h"
int daemon_tick(Daemon *daemon)
{
    time_t now;
    time(&now);
    // Tick with a time difference of at least 1
    if (difftime(now, daemon->last_tick) < 1)
    {
        daemon->last_tick = now;
        return -1;
    }
    // Execute tasks
    PeriodicTask *task;
    daemon->last_tick = now;
    LL_FOREACH(daemon->tasks, task)
    {
        if (difftime(now, task->last_run) >= task->interval_seconds)
        {
            printf("Executing task:%s\n", task->name);
            task->handler(task);
            /*
                Must be updated after the handler runs,
                not after each daemon tick because that produces a bug
            */
            task->last_run = now;
        }
    }
    return 0;
}
Daemon *initialize_daemon()
{
    Daemon *daemon = malloc(sizeof(Daemon));
    if (daemon == NULL)
    {
        return NULL;
    };
    memset(daemon, 0, sizeof(Daemon));
    time(&daemon->last_tick);
    daemon->tasks = NULL;
    return daemon;
}
PeriodicTask *daemon_register_task(Daemon *daemon, char *name, void (*handler)(PeriodicTask *ctx), void *handler_context, int interval_seconds, bool enabled)
{
    if (daemon == NULL)
    {
        return NULL;
    }
    PeriodicTask *periodic_task = malloc(sizeof(PeriodicTask));
    if (periodic_task == NULL)
    {
        return NULL;
    };
    memset(periodic_task, 0, sizeof(PeriodicTask));
    // Set values
    periodic_task->enabled = enabled;
    strncpy(periodic_task->name, name, sizeof(periodic_task->name));
    time(&periodic_task->last_run);
    periodic_task->handler = handler;
    periodic_task->handler_context = handler_context;
    periodic_task->interval_seconds = interval_seconds;
    // Append task
    LL_PREPEND(daemon->tasks, periodic_task);
    return periodic_task;
}

int destroy_daemon(Daemon *daemon)
{
    PeriodicTask *task, *tmp;
    LL_FOREACH_SAFE(daemon->tasks, task, tmp)
    {
        LL_DELETE(daemon->tasks, task);
        free(task);
    }
    free(daemon);
    return 0;
}