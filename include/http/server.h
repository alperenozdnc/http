#pragma once

#include <netinet/in.h>
#include <stdbool.h>

/*
 * starts serving to localhost using port `port`.
 * return type indicates an error.
 */
bool server(in_port_t port);
