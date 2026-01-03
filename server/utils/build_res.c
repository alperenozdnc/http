#include <http/ctx.h>
#include <http/def.h>
#include <http/mime.h>
#include <http/utils/build_default_not_found_path.h>
#include <http/utils/build_path.h>
#include <http/utils/build_res.h>

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_file_extension(char *file_pathname) {
    char *stripped = strrchr(file_pathname, '.');

    // for enforcing no un-extensioned files are in use at all
    assert(stripped && "a request path must include at least one dot");

    return strdup(stripped + 1); // +1 to exclude the dot
}

int resolve_extension_idx(char *ext) {
    size_t i = 0;

    while (KNOWN_EXTENSIONS[i]) {
        if (strcmp(ext, KNOWN_EXTENSIONS[i]) == 0)
            return i;

        i++;
    }

    return -1;
}

char *resolve_content_type(char *req_pathname) {
    char *file_ext = get_file_extension(req_pathname);
    int ext_idx = resolve_extension_idx(file_ext);
    char *_content_type =
        ext_idx != -1 ? KNOWN_CONTENT_TYPES[ext_idx] : FALLBACK_CONTENT_TYPE;

    free(file_ext);

    size_t content_type_size =
        strlen(CONTENT_TYPE_PREFIX) + strlen(_content_type) + 1;

    char content_type[content_type_size];
    memset(content_type, 0, content_type_size);

    strcat(content_type, CONTENT_TYPE_PREFIX);
    strcat(content_type, _content_type);

    return strdup(content_type);
}

bool path_exists(char *pathname) {
    FILE *tmp = fopen(pathname, "r");

    if (!tmp)
        return false;

    fclose(tmp);

    return true;
}

typedef struct {
    char *pathname;
    char *status;
} pname_status_s;

void resolve_pathname_and_status(ctx_s *ctx, pname_status_s *info,
                                 char *req_pathname) {
    if (path_exists(req_pathname)) {
        info->status = strdup(OK_STATUS);
        info->pathname = strdup(req_pathname);

        return;
    }

    if (ctx->does_strip_paths) {
        size_t expanded_path_size =
            strlen(req_pathname) + strlen(HTML_EXTENSION) + 1;
        char expanded_path[expanded_path_size];
        memset(expanded_path, 0, expanded_path_size);

        strcat(expanded_path, req_pathname);
        strcat(expanded_path, HTML_EXTENSION);

        if (!path_exists(expanded_path))
            goto not_found;

        info->status = strdup(OK_STATUS);
        info->pathname = strdup(expanded_path);

        return;
    }

not_found:
    info->status = strdup(NOT_FOUND_STATUS);

    char *usergen_not_found_page = build_path(ctx, NOT_FOUND_PAGE);

    if (path_exists(usergen_not_found_page)) {
        info->pathname = usergen_not_found_page;

        return;
    }

    free(usergen_not_found_page);

    char *not_found_default_page = build_default_not_found_path();

    if (!path_exists(not_found_default_page)) {
        cliprint(CLI_ERROR, HTTP_PREFIX,
                 "can't find the default 404 page in '.local'. you "
                 "should run 'make install' in the project root. aborting.");

        exit(EXIT_FAILURE);
    }

    info->pathname = not_found_default_page;
}

typedef struct {
    char *body;
    size_t length;
} content_s;

void resolve_content(content_s *content, char *pathname) {
    size_t content_size = 0;
    FILE *content_file = fopen(pathname, "r");

    char *content_buf = malloc(1);

    while (true) {
        int c = fgetc(content_file);

        if (c == EOF) {
            content_buf[content_size] = '\0';

            break;
        }

        content_size++;
        content_buf = realloc(content_buf, content_size + 1);
        content_buf[content_size - 1] = c;
    }

    fclose(content_file);

    content->length = content_size;
    content->body = content_buf;
}

char *build_res_str(pname_status_s *info, char *content_type,
                    content_s *content) {
    size_t res_size = strlen(info->status) + CRLF_LEN + strlen(content_type) +
                      CRLF_LEN + strlen(CONTENT_LENGTH_PREFIX) +
                      (log10(content->length) + 1) + (2 * CRLF_LEN) +
                      content->length + 1;

    char res[res_size];
    memset(res, 0, res_size);

    snprintf(res, res_size - 1,
             "%s" CRLF "%s" CRLF "%s"
             "%zu" CRLF CRLF "%s",
             info->status, content_type, CONTENT_LENGTH_PREFIX, content->length,
             content->body);

    return strdup(res);
}

char *build_res(ctx_s *ctx, char *req_pathname) {
    pname_status_s info = {0};
    content_s content = {0};
    resolve_pathname_and_status(ctx, &info, req_pathname);
    resolve_content(&content, info.pathname);

    char *content_type = resolve_content_type(info.pathname);
    char *res = build_res_str(&info, content_type, &content);

    free(content_type);
    free(info.pathname);
    free(info.status);
    free(content.body);

    return res;
}
