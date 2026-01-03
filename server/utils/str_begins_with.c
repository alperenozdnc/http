#include <http/utils/str_begins_with.h>

#include <stdbool.h>

bool str_begins_with(char *str, char c) {
    return str[0] == c;
}
