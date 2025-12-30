#include <http/utils/get_req_pathname.h>

#include <string.h>

/* here, NPHP just stands for 'non pathname head part'.
 i just mean all of the content in the first line of an http request that is
 specifically not the requested pathname. */
#define NPHP_LHS "GET "
#define NPHP_RHS " HTTP/1.1"

char *get_req_pathname(char *req) {
    size_t req_head_len = strcspn(req, "\n");
    char buf[req_head_len];
    memset(buf, 0, sizeof(buf));
    size_t buf_idx = 0;

    size_t lhs_len = strlen(NPHP_LHS);
    size_t rhs_len = strlen(NPHP_RHS);

    // -1 here because we want to end ON the pathname, not right after it.
    for (size_t i = lhs_len; i < req_head_len - rhs_len - 1; i++) {
        buf[buf_idx++] = req[i];
    }

    buf[buf_idx] = '\0';

    return strdup(buf);
}
