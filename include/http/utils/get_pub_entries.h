#pragma once

#include <http/ctx.h>

/*
 * recursively gets all files that currently live in the public directory.
 * returns a boolean value based on success.
 */
bool get_pub_entries(ctx_s *ctx, char *parent_dir);
