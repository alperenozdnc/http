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
