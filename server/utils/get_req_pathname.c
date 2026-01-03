#include <http/utils/get_req_pathname.h>

#include <stdio.h>
#include <string.h>

/* here, NPHP just stands for 'non pathname head part'.
 i just mean all of the content in the first line of an http request that is
 specifically not the requested pathname. */
#define NPHP_LHS "GET "
#define NPHP_RHS " HTTP/1.1"

char *get_req_pathname(char *req) {
    size_t lhs_len = strlen(NPHP_LHS);
    size_t rhs_len = strlen(NPHP_RHS);
    size_t req_head_len = strcspn(req, "\n");
    size_t buf_len = req_head_len - lhs_len - rhs_len;

    char buf[buf_len];
    memset(buf, 0, buf_len);
    size_t buf_idx = 0;

    for (size_t i = lhs_len; i < (buf_len + lhs_len) - 1; i++) {
        buf[buf_idx++] = req[i];
    }

    buf[buf_idx] = '\0';

    return strdup(buf);
}
