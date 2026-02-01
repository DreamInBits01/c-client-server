#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include "structs.h"
#include "ds/queue.h"
#include "worker.h"
#include "connections/structs.h"
ThreadPool *initialize_threadpool();
int destroy_threadpool(ThreadPool *threadpool);
int submit_to_threadpool(ThreadPool *threadpool, Connection *connection);
#endif