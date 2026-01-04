#include <http/ctx.h>
#include <http/def.h>
#include <http/mime.h>
#include <http/misc.h>
#include <http/utils/build_default_not_found_path.h>
#include <http/utils/build_path.h>
#include <http/utils/print_http_header.h>
#include <http/utils/respond.h>

#include <assert.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

/*
 * gets the extension name of a file not including the prefixed '.'. returns
 * `NULL` if no extensions exist.
 */
char *get_file_extension(char *file_pathname) {
    char *stripped = strrchr(file_pathname, '.');

    if (!stripped)
        return NULL;

    return strdup(stripped + 1); // +1 to exclude the dot
}

/*
 * finds the current extension's index in the MIME type table.
 */
int resolve_extension_idx(char *ext) {
    if (!ext)
        return -1;

    size_t i = 0;

    while (KNOWN_EXTENSIONS[i]) {
        if (strcmp(ext, KNOWN_EXTENSIONS[i]) == 0)
            return i;

        i++;
    }

    return -1;
}

/*
 * finds the current request's MIME type using the index taken from
 * `resolve_extension_idx()`.
 */
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

/*
 * checks if a path is accessable to the server. returns a boolean value based
 * on the result.
 */
bool path_exists(char *pathname) {
    struct stat info = {0};

    return stat(pathname, &info) == 0;
}

/*
 * find the pathname and the status code of the response and its given
 * explanation. defaults to the user-defined 404 page if the pathname doesn't
 * exist on the server, if the user-defined 404 page doesn't exist either, this
 * function falls back to the default 404 page given in ~/.local/http/404.html.
 */
void resolve_pathname_and_status(ctx_s *ctx, pathname_and_status_s *info,
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

/*
 * decides if the respond's content body should be interpreted as text or
 * binary.
 */
bool is_body_text(char *content_type) {
    if (strstr(content_type, "Content-Type: text/") == NULL)
        return false;

    return true;
}

/*
 * finds and reads the requested files on the server either as text or binary
 * depending on the content type.
 */
void resolve_content(content_s *content, char *content_type, char *pathname) {
    content->is_body_text = is_body_text(content_type);

    FILE *f = fopen(pathname, "rb");

    if (!f) {
        perror("fopen()");

        goto empty_file;
    }

    fseek(f, 0, SEEK_END);
    ssize_t size = ftell(f);
    rewind(f);

    if (size == 0) {
        fclose(f);

        goto empty_file;
    }

    uint8_t *buf = malloc(size);
    size_t n = fread(buf, 1, size, f);

    if (n != (size_t)size)
        perror("fread()");

    content->len = size;

    if (content->is_body_text) {
        content->as.text = (char *)buf;
    } else {
        content->as.binary = buf;
    }

    fclose(f);

    return;

empty_file:
    content->len = 0;

    if (content->is_body_text) {
        content->as.text = NULL;
    } else {
        content->as.binary = NULL;
    }

    return;
}

void write_all(int fd, const void *content_data, const size_t len) {
    ssize_t size_written = 0;
    const uint8_t *p = (const uint8_t *)content_data;

    while (true) {
        if ((size_t)size_written == len)
            break;

        ssize_t written_tmp = write(fd, p + size_written, len - size_written);

        if (written_tmp <= 0) {
            // errno is only meaningful when write() returns -1
            if (written_tmp == -1 && errno == EINTR)
                continue;

            perror("write()");
            break;
        }

        size_written += written_tmp;
    }
}

/*
 * compiles all generated information and builds a response that is then sent to
 * the client file descriptor. the only place that the behaviour varies is
 * writing as text/binary.
 */
char *write_res(int fd, pathname_and_status_s *info, char *content_type,
                content_s *content) {
    size_t content_len_digit_count = 0;

    if (content->len != 0) {
        content_len_digit_count = log10(content->len) + 1;
    } else {
        // if n is 0, log10(n) is undefined
        content_len_digit_count = 1;
    }

    size_t header_size = strlen(info->status) + CRLF_LEN +
                         strlen(content_type) + CRLF_LEN +
                         strlen(CONTENT_LENGTH_PREFIX) +
                         content_len_digit_count + (2 * CRLF_LEN) + 1;

    char header[header_size];
    memset(header, 0, header_size);

    snprintf(header, header_size,
             "%s" CRLF "%s" CRLF "%s"
             "%zu" CRLF CRLF,
             info->status, content_type, CONTENT_LENGTH_PREFIX, content->len);

    write_all(fd, header, strlen(header));

    if (content->is_body_text) {
        write_all(fd, content->as.text, content->len);
    } else {
        write_all(fd, content->as.binary, content->len);
    }

    return strdup(header);
}

/*
 * builds and sends a response to the client file descriptor based on the given
 * request pathname.
 */
void respond(ctx_s *ctx, int fd, char *req_pathname) {
    pathname_and_status_s info = {0};
    resolve_pathname_and_status(ctx, &info, req_pathname);

    content_s content = {0};
    char *content_type = resolve_content_type(info.pathname);
    resolve_content(&content, content_type, info.pathname);

    char *res_info = write_res(fd, &info, content_type, &content);
    PRINT_ACTION_INFO(HTTP_RESPONSE_PREFIX, res_info);

    free(info.pathname);
    free(info.status);
    free(res_info);

    if (content.is_body_text) {
        free(content.as.text);
    } else {
        free(content.as.binary);
    }
}
