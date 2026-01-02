/*
 * miscellaneous helpers that are too big to not be listed as a helper, but too
 * small to deserve its own source file.
 */

#pragma once

/*
 * helper for declaring that the command line interface will be instantiated
 * with no options.
 */
#define EMPTY_OPTS                                                             \
    (opt_s *[]) {                                                              \
        NULL                                                                   \
    }

/*
 * helper for clearing the terminal/console window.
 */
#define CLEAR_CONSOLE() system("clear")

/*
 * helper for printing the appropriate prefix and header for an http action.
 */
#define PRINT_ACTION_INFO(prefix, buf)                                         \
    do {                                                                       \
        printf(ANSI_BLUE prefix ANSI_RESET);                                   \
        print_http_header(buf);                                                \
        printf("\n");                                                          \
    } while (0);

/*
 * helper for asserting that the 'serve' option must be called at least once
 * when running the program.
 */
#define ASSERT_SERVE_MODE_ENABLED(ctx, want_input)                             \
    do {                                                                       \
        if (is_last_opt(ctx, want_input) && !ctx->serve_mode) {                \
            cliprint(CLI_ERROR, HTTP_PREFIX,                                   \
                     "you need to include 'serve [port]' when you run this "   \
                     "program.");                                              \
                                                                               \
            return RET_FAIL;                                                   \
        }                                                                      \
    } while (0);

/*
 * helper for serving http on a specific port. this macro exists because the
 * 'serve' option isn't the only entrypoint. */
#define SERVE_HTTP(port)                                                       \
    do {                                                                       \
        cliprint(CLI_HINT, HTTP_PREFIX,                                        \
                 "serving on port %d | <http://localhost:%d>\n", port, port);  \
                                                                               \
        if (!server(htons(port))) {                                            \
            cliprint(CLI_ERROR, HTTP_PREFIX, "error starting server.");        \
                                                                               \
            return RET_FAIL;                                                   \
        }                                                                      \
    } while (0);
