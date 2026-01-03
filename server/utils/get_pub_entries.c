#include <http/ctx.h>
#include <http/utils/get_pub_entries.h>

#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PUB_ENTRIES_SIZE(n) sizeof(char *) * (n + 1)
#define BLACKLIST                                                              \
    (char *[]) {                                                               \
        ".", "..", ".git", NULL                                                \
    }

void push_entry(ctx_s *ctx, char *entry, size_t *size) {
    (*size)++;

    ctx->pub_entries = realloc(ctx->pub_entries, PUB_ENTRIES_SIZE(*size));
    ctx->pub_entries[*size - 1] = strdup(entry);
    ctx->pub_entries[*size] = NULL;
}

bool array_contains(char *arr[], char *str) {
    size_t i = 0;

    while (arr[i]) {
        if (strcmp(arr[i], str) == 0)
            return true;

        i++;
    }

    return false;
}

size_t size = 0;

bool get_pub_entries(ctx_s *ctx, char *parent_dir) {
    bool is_root = (parent_dir == NULL);
    char *dir = is_root ? ctx->pub_dir : parent_dir;

    if (is_root)
        ctx->pub_entries = malloc(PUB_ENTRIES_SIZE(0));

    // pd stands for 'public directory'
    struct dirent *pde;
    DIR *pd = opendir(dir);

    if (pd == NULL) {
        perror("opendir()");

        return false;
    }

    while ((pde = readdir(pd)) != NULL) {
        if (array_contains(BLACKLIST, pde->d_name))
            continue;

        // +1 for '/' and +1 for '\0'
        size_t path_size = strlen(dir) + strlen(pde->d_name) + 2;
        char path[path_size];
        memset(path, 0, path_size);

        strcat(path, dir);

        if (dir[strlen(dir) - 1] != '/')
            strcat(path, "/");

        strcat(path, pde->d_name);

        if (pde->d_type == DT_DIR) {
            bool ret = get_pub_entries(ctx, path);

            if (!ret)
                return false;

            continue;
        }

        push_entry(ctx, path, &size);
    }

    closedir(pd);

    if (is_root)
        ctx->pub_entries[size] = NULL;

    return true;
}
