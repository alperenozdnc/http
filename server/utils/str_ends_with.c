#include <http/utils/str_ends_with.h>

#include <stdbool.h>
#include <string.h>

bool str_ends_with(char *str, char c) {
    return str[strlen(str) - 1] == c;
}
