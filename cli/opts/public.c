#include <http/cli/is_last_opt.h>
#include <http/cli/opt_public.h>
#include <http/ctx.h>
#include <http/def.h>
#include <http/misc.h>
#include <http/server.h>

#include <netinet/in.h>

#include <ezcli.h>
#include <string.h>
#include <unistd.h>

#define DOES_OPTION_WANT_INPUT true

ret_e opt_public_body(void *_ctx, char *dir) {
    ctx_s *ctx = _ctx;

    ASSERT_SERVE_MODE_ENABLED(ctx, DOES_OPTION_WANT_INPUT);

    if (ctx->pub_dir_set) {
        cliprint(CLI_ERROR, HTTP_PREFIX,
                 "can't set the public directory twice.");

        return RET_FAIL;
    }

    if (access(dir, F_OK) < 0) {
        cliprint(
            CLI_WARN, HTTP_PREFIX,
            "can't set '%s' as public, falling back to the current directory.",
            dir);

        if (is_last_opt(ctx, DOES_OPTION_WANT_INPUT) && ctx->serve_mode)
            SERVE_HTTP(ctx->port);

        ctx->pub_dir_set = true;
        ctx->pub_dir = strdup(".");

        return RET_WARN;
    }

    ctx->pub_dir_set = true;
    ctx->pub_dir = strdup(dir);

    if (is_last_opt(ctx, DOES_OPTION_WANT_INPUT) && ctx->serve_mode)
        SERVE_HTTP(ctx->port);

    return RET_NORMAL;
}

void init_opt_public(cli_s *cli, ctx_s *ctx) {
    opt_s opt_strip = {
        .aliases = CLI_ALIASES("directory", "--public", "--setpublicdir"),
        .body = opt_public_body,
        .desc =
            "sets the public directory that the server searches files from.",
        .want_input = true,
        .ctx = ctx};

    allocopt(cli, &opt_strip);
}
