#ifndef IO_H
#define IO_H
#include "connections/structs.h"
#include <stdio.h>
#include <errno.h>
int io_receive(Connection *connection);
int io_send(Connection *connection);
#endif