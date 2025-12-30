#pragma once

#include <stddef.h>

/*
 * prints the first line of an http request/response.
 */
size_t print_http_header(char *req);
