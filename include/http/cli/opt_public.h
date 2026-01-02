#pragma once

#include <ezcli.h>

#include <http/ctx.h>

ret_e opt_public_body(void *_ctx, char *dir);

/*
 * the option used to tell the server the public directory to search files from.
 */
void init_opt_public(cli_s *cli, ctx_s *ctx);
