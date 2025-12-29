#include <http/misc.h>

#include <http/cli/cfg.h>
#include <http/cli/opt_serve.h>

#include <ezcli.h>

int main(int argc, char *argv[]) {
    cli_s cli = {0};

    initcli(&cli, CMD_NAME, CMD_DESC, CMD_USAGE, CMD_FOOTER, EMPTY_OPTS,
            CMD_HELP_ALIASES);

    init_opt_serve(&cli);

    runcli(&cli, argc, argv);

    freecli(&cli);

    return 0;
}
