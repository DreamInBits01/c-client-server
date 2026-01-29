#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include "structs.h"
#include "queue.h"

ThreadPool *initialize_threadpool();
int destroy_threadpool(ThreadPool *threadpool);
int submit_to_threadpool(ThreadPool *threadpool, Connection *connection);
#endif