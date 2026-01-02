#pragma once

#include <http/ctx.h>

/*
 * frees all heap-allocated fields of the context.
 */
void free_ctx(ctx_s *ctx);
