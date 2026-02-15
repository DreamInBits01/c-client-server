#include "daemon/tasks/check_health.h"
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

void check_health_handler(PeriodicTask *task)
{
    EventLoopContext *event_loop_context = (EventLoopContext *)task->handler_context;

    // Get current timestamp
    time_t now = time(NULL);
    char timestamp[26];
    memcpy(timestamp, ctime(&now), sizeof(timestamp));
    timestamp[24] = '\0'; // Remove newline

    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                    SYSTEM HEALTH CHECK                     ║\n");
    printf("║                  %s                  ║\n", timestamp);
    printf("╠════════════════════════════════════════════════════════════╣\n");
    // Connections Manager Metrics
    printf("║ Connections Manager                                        ║\n");
    printf("║   Active Connections         : %-27d ║\n",
           HASH_COUNT(event_loop_context->connections_manager->connections));

    // printf("║   Max Connections (limit)    : %-27d ║\n", max_connections);
    // printf("║   Failed Connections (total) : %-27ld ║\n", failed_connection_count);
    // printf("║   Avg Connection Duration    : %-24.2f ms ║\n", avg_duration);

    // Threadpool Metrics
    printf("║                                                            ║\n");
    printf("║ Threadpool                                                 ║\n");
    printf("║   Worker Threads             : %-27ld ║\n",
           event_loop_context->threadpool->number_of_workers);
    printf("║   Queued Tasks               : %-27ld ║\n",
           get_queue_tasks_count(event_loop_context->threadpool->queue));

    // Calculate utilization if you have active/idle worker counts
    // printf("║   Active Workers             : %-27ld ║\n", active_workers);
    // printf("║   Idle Workers               : %-27ld ║\n", idle_workers);
    // printf("║   Thread Utilization         : %-24.1f %% ║\n",
    //        (active_workers * 100.0) / number_of_workers);
    // printf("║   Queue Capacity             : %-27ld ║\n", queue_capacity);
    // printf("║   Tasks Completed (total)    : %-27ld ║\n", completed_tasks);
    // printf("║   Tasks Rejected (total)     : %-27ld ║\n", rejected_tasks);

    // Event Loop Metrics
    printf("║                                                            ║\n");
    printf("║ Event Loop                                                 ║\n");
    // printf("║   Pending Events             : %-27d ║\n", pending_events);
    // printf("║   Events Processed (total)   : %-27ld ║\n", total_events);
    // printf("║   Avg Event Latency          : %-24.2f ms ║\n", avg_latency);

    // Daemon Metrics
    printf("║                                                            ║\n");
    printf("║ Daemon                                                     ║\n");
    printf("║   Registered Tasks           : %-27d ║\n",
           event_loop_context->daemon->number_of_tasks);
    // printf("║   Uptime                     : %-27ld s ║\n", uptime_seconds);

    // Memory Metrics
    printf("║                                                            ║\n");
    printf("║ Memory                                                     ║\n");
    // printf("║   RSS (Resident Set Size)    : %-24ld KB ║\n", rss_kb);
    // printf("║   Virtual Memory             : %-24ld KB ║\n", vm_kb);

    printf("╚════════════════════════════════════════════════════════════╝\n");
    printf("\n");
}