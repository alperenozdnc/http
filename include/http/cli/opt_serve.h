#pragma once

#include <http/ctx.h>

#include <ezcli.h>

ret_e opt_serve_body(CLI_IGNORE_CTX, char *str);

/*
 * the main option used to start a local server on a specified port.
 */
void init_opt_serve(cli_s *cli, ctx_s *ctx);
