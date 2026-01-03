#pragma once

#include <http/ctx.h>

/*
 * takes a request path which assumes the root directory is the current working
 * directory and prepends the actual public directory.
 */
char *build_path(ctx_s *ctx, char *pathname);
