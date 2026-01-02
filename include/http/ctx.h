#pragma once

#include <ezcli.h>
#include <netinet/in.h>
#include <stdbool.h>

/*
 * this is the context information passed around in the program.
 *
 * `cli` is just the cli instance.
 * `port` is the port that the http server will be hosted to.
 * `pub_dir` is the name of the public directory. if not set, defaults to the
 * current directory.
 * `pub_entries` are just all filenames inside the public directory.
 * `serve_mode` is used to track whether the appropriate configurations were set
 * before starting to serve.
 * `does_strip_paths` is a boolean value that is truthified using the `--strip`
 * option, it turns paths ending with `.html` to bare paths.
 */
typedef struct {
    cli_s *cli;

    in_port_t port;

    char *pub_dir;
    char **pub_entries;

    bool pub_dir_set;
    bool serve_mode;
    bool does_strip_paths;
} ctx_s;
