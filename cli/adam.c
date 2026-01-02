#include <http/cli/free_ctx.h>
#include <http/misc.h>

#include <http/cli/cfg.h>
#include <http/cli/opt_public.h>
#include <http/cli/opt_serve.h>
#include <http/cli/opt_strip.h>

#include <http/ctx.h>

#include <ezcli.h>
#include <string.h>

int main(int argc, char *argv[]) {
    cli_s cli = {0};

    ctx_s ctx = {
        .cli = &cli,
        .does_strip_paths = false,
        .pub_dir = strdup(
            "."), // heap-allocated because free_ctx assumes all values are
        .pub_dir_set = false,
        .serve_mode = false};

    initcli(&cli, CMD_NAME, CMD_DESC, CMD_USAGE, CMD_FOOTER, EMPTY_OPTS,
            CMD_HELP_ALIASES);

    init_opt_serve(&cli, &ctx);
    init_opt_strip(&cli, &ctx);
    init_opt_public(&cli, &ctx);

    runcli(&cli, argc, argv);

    free_ctx(&ctx);
    freecli(&cli);

    return 0;
}
