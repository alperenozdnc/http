#include <http/cli/is_last_opt.h>
#include <http/cli/opt_strip.h>
#include <http/ctx.h>
#include <http/def.h>
#include <http/misc.h>
#include <http/server.h>

#include <netinet/in.h>

#include <ezcli.h>

#define DOES_OPTION_WANT_INPUT false

ret_e opt_strip_body(void *_ctx, CLI_IGNORE_TOK) {
    ctx_s *ctx = _ctx;

    ASSERT_SERVE_MODE_ENABLED(ctx, DOES_OPTION_WANT_INPUT);

    if (ctx->does_strip_paths) {
        cliprint(CLI_ERROR, HTTP_PREFIX,
                 "you can't use %s more than once since its a boolean flag "
                 "that defaults to false.",
                 gettok_offset(ctx->cli, 0));

        return RET_FAIL;
    }

    ctx->does_strip_paths = true;

    if (is_last_opt(ctx, DOES_OPTION_WANT_INPUT) && ctx->serve_mode) {
        SERVE_HTTP(ctx->port);
    }

    return RET_NORMAL;
}

void init_opt_strip(cli_s *cli, ctx_s *ctx) {
    opt_s opt_strip = {
        .aliases = CLI_ALIASES("--strip", "--strip-paths", "--barepaths"),
        .body = opt_strip_body,
        .desc = "strips all .html extensions when resolving requests.",
        .want_input = false,
        .ctx = ctx};

    allocopt(cli, &opt_strip);
}
