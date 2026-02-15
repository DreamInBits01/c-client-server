#ifndef WORKER_H
#define WORKER_H
#include <stdio.h>
#include "ds/queue.h"
#include "net/io.h"
#include "threadpool.h"
#include "structs.h"
#include "connections/manager.h"
#include "http/parser.h"
void *mock_routine(void *args);
#endif