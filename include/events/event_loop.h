#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H
#include "net/io.h"
#include "structs.h"
void event_loop_run(EventLoopContext *ctx);
// EventLoopContext *initialize_event_loop_context();
// int destroy_event_loop_context(EventLoopContext *ctx);

#endif