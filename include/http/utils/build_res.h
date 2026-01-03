#pragma once

#include <http/ctx.h>

/*
 * builds a response using the current contextual information.
 */
char *build_res(ctx_s *ctx, char *req_pathname);
