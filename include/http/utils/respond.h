#pragma once

#include <http/ctx.h>

/*
 * the struct for holding the requested pathname and the respond's status code
 * and explanation.
 */
typedef struct {
    char *pathname;
    char *status;
} pathname_and_status_s;

/*
 * the struct for holding length, body, and type information about the
 * response's content.
 */
typedef struct {
    bool is_body_text;

    union {
        uint8_t *binary;
        char *text;
    } as;

    size_t len;
} content_s;

/*
 * builds a response using the current contextual information.
 */
void respond(ctx_s *ctx, int fd, char *req_pathname);
