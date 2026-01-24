#ifndef NET_IO_H
#define NET_IO_H
#include <stdio.h>
#include <errno.h>
#include "utils.h"
#include "connections/structs.h"
int io_receive(Connection *connection);
int io_send(Connection *connection);
#endif