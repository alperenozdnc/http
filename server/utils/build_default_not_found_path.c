#include <http/def.h>
#include <http/utils/build_default_not_found_path.h>

#include <stdlib.h>
#include <string.h>

char *build_default_not_found_path() {
    const char *HOME = getenv("HOME");

    // +1 for NULL terminator
    size_t path_size = strlen(HOME) + strlen(NOT_FOUND_DEFAULT_PAGE) + 1;
    char path[path_size];
    memset(path, 0, path_size);

    strcat(path, HOME);
    strcat(path, NOT_FOUND_DEFAULT_PAGE);

    return strdup(path);
}
