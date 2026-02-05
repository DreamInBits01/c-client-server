#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H
#include "net/io.h"
#include "structs.h"
void event_loop_run(EventLoopContext *ctx);
#endif