#include "daemon/index.h"
void daemon_tick(void *ctx)
{
    Daemon *daemon = (Daemon *)ctx;
    while (daemon->is_running)
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
        pthread_mutex_lock(&daemon->mutex);
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
        pthread_mutex_unlock(&daemon->mutex);

        nanosleep(&daemon->delay, NULL);
    }
    return NULL;
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
    daemon->is_running = 1;
    struct timespec delay = {
        .tv_sec = 0,
        .tv_nsec = 1000000000};
    daemon->delay = delay;
    pthread_mutex_init(&daemon->mutex, NULL);
    pthread_create(&daemon->thread_id, NULL, daemon_tick, daemon);
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
    pthread_mutex_lock(&daemon->mutex);
    LL_APPEND(daemon->tasks, periodic_task);
    pthread_mutex_unlock(&daemon->mutex);
    return periodic_task;
}

int destroy_daemon(Daemon *daemon)
{
    daemon->is_running = 0;
    PeriodicTask *task, *tmp;
    // Wait for the thread to finishes work
    pthread_join(daemon->thread_id, NULL);
    pthread_mutex_lock(&daemon->mutex);
    LL_FOREACH_SAFE(daemon->tasks, task, tmp)
    {
        LL_DELETE(daemon->tasks, task);
        free(task);
    }
    pthread_mutex_unlock(&daemon->mutex);
    pthread_mutex_destroy(&daemon->mutex);
    free(daemon);
    return 0;
}