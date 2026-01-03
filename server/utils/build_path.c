#include <http/ctx.h>
#include <http/def.h>
#include <http/utils/build_path.h>
#include <http/utils/str_begins_with.h>
#include <http/utils/str_ends_with.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *strip_pathname(char *pathname) {
    // no -1 or +1 because the slash does a -1 and \0 does a +1
    size_t stripped_size = strlen(pathname);
    char stripped[stripped_size];
    memset(stripped, 0, stripped_size);

    size_t i = 1;

    while (pathname[i]) {
        stripped[i - 1] = pathname[i];

        i++;
    }

    return strdup(stripped);
}

char *build_path(ctx_s *ctx, char *pathname) {
    size_t pub_dir_len = strlen(ctx->pub_dir);
    size_t pathname_len = strlen(pathname);

    // +1 for the NULL terminator
    // not adding +1 for the inserted slash because it's taken from the pathname
    size_t path_size = pub_dir_len + pathname_len + strlen(INDEX_PAGE) + 1;
    char path[path_size];
    memset(path, 0, path_size);

    strcat(path, ctx->pub_dir);

    if (!str_ends_with(ctx->pub_dir, '/'))
        strcat(path, "/");

    if (str_begins_with(pathname, '/')) {
        if (pathname_len > 1) {
            char *stripped_pathname = strip_pathname(pathname);
            strcat(path, stripped_pathname);
            free(stripped_pathname);
        }
    } else {
        strcat(path, pathname);
    }

    if (pathname_len == 1) {
        strcat(path, INDEX_PAGE);
    }

    return strdup(path);
}
