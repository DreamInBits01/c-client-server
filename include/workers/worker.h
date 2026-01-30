#ifndef WORKER_H
#define WORKER_H
#include <stdio.h>
#include "threadpool.h"
#include "structs.h"
#include "net/io.h"
#include "connections/manager.h"
void *mock_routine(void *args);
#endif