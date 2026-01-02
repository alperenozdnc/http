#include <http/ctx.h>
#include <stdlib.h>

void free_ctx(ctx_s *ctx) {
    free(ctx->pub_dir);
}
