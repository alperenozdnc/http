#include <http/cli/opt_serve.h>
#include <http/def.h>
#include <http/misc.h>
#include <http/server.h>

#include <netinet/in.h>

#include <ezcli.h>
#include <stdlib.h>

ret_e opt_serve_body(CLI_IGNORE_CTX, char *tok) {
    CLEAR_CONSOLE();

    in_port_t port = atoi(tok);

    cliprint(CLI_HINT, HTTP_PREFIX,
             "serving on port %d | <http://localhost:%d>\n", port, port);

    if (!server(htons(port))) {
        cliprint(CLI_ERROR, HTTP_PREFIX, "error starting server.");

        return RET_FAIL;
    }

    return RET_NORMAL;
}

void init_opt_serve(cli_s *cli) {
    opt_s opt_default = {.aliases = CLI_ALIASES("serve", "server", "--serve"),
                         .body = opt_serve_body,
                         .desc = "starts an http server on the provided port.",
                         .want_input = true};

    allocopt(cli, &opt_default);
}
