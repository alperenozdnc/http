#include <http/utils/print_http_header.h>

#include <stdio.h>
#include <string.h>

size_t print_http_header(char *req) {
    size_t header_len = strcspn(req, "\n");

    for (size_t i = 0; i < header_len; i++) {
        printf("%c", req[i]);
    }

    fflush(stdout);

    return header_len;
}
