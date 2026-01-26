#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include "structs.h"

ThreadPool *init_threadpool();
int destroy_threadpool();
int submit_to_threadpool();
#endif