#pragma once

#include <ezcli.h>

#include <http/ctx.h>

ret_e opt_strip_body(void *_ctx, CLI_IGNORE_TOK);

/*
 * the option used to tell the server to strip .html extensions from files.
 * i.e. /home.html/ -> /home/
 */
void init_opt_strip(cli_s *cli, ctx_s *ctx);
