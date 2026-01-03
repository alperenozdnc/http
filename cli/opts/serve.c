#include <http/cli/is_last_opt.h>
#include <http/cli/opt_serve.h>

#include <http/ctx.h>
#include <http/def.h>
#include <http/misc.h>
#include <http/server.h>

#include <netinet/in.h>

#include <ezcli.h>

#include <stdlib.h>

#define DOES_OPTION_WANT_INPUT true

ret_e opt_serve_body(void *_ctx, char *tok) {
    ctx_s *ctx = _ctx;

    in_port_t port = atoi(tok);

    if (!is_last_opt(ctx, DOES_OPTION_WANT_INPUT)) {
        ctx->serve_mode = true;
        ctx->port = port;

        return RET_NORMAL;
    }

    CLEAR_CONSOLE();
    SERVE_HTTP(port, ctx);

    return RET_NORMAL;
}

void init_opt_serve(cli_s *cli, ctx_s *ctx) {
    opt_s opt_default = {.aliases = CLI_ALIASES("serve", "server", "--serve"),
                         .body = opt_serve_body,
                         .desc = "starts an http server on the provided port.",
                         .want_input = true,
                         .ctx = ctx};

    allocopt(cli, &opt_default);
}
