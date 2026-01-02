#pragma once

#include <http/ctx.h>

#include <ezcli.h>
#include <stdbool.h>

/*
 * checks if the current token has no more options after it. this is used for
 * setting modes.
 */
bool is_last_opt(ctx_s *ctx, bool want_input);
