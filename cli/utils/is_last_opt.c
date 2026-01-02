#include <http/cli/is_last_opt.h>
#include <http/ctx.h>

#include <ezcli.h>
#include <stdbool.h>

bool is_last_opt(ctx_s *ctx, bool want_input) {
    int offset;

    if (want_input) {
        offset = 2;
    } else {
        offset = 1;
    }

    char *tok_next = gettok_offset(ctx->cli, offset);

    return tok_next == NULL;
}
